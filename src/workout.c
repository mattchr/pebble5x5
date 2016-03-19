//======================================================================
// Includes
//======================================================================
#include "workout.h"
#include "globals.h"
#include "view.h"

//======================================================================
// Private Functions
//======================================================================

//======================================================================
// Public Functions
//======================================================================
TextLayer *create_workout_tl(Workout *w, GPoint location) {
    Layer *window_layer = window_get_root_layer(window);
    TextLayer *tl = text_layer_create((GRect) { .origin=location, .size ={ 70, 100}});
    text_layer_set_text_alignment(tl, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(tl));
    text_layer_set_font(tl, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
//    text_layer_set_background_color(tl, GColorRed);
    return tl;
}
void workout_day(char* output, size_t maxsize, Workout *w) {
  strftime(output, maxsize, "%m/%d", localtime(&w->timestamp));
}

void build_workout_string(char* output, Workout *w, WorkoutSequence sequence) {
  output[0] = '\0';
  char workout_day_string[10];
  workout_day(workout_day_string, 10, w);
  if (sequence == WORKOUT_PREVIOUS) {
    snprintf(output, WORKOUT_DISPLAY_LEN, "%s\nPrev ", workout_day_string);
  } else if (sequence == WORKOUT_NEXT) {
    strcat(output, "\nNext ");
  }
  if (w->type == WORKOUT_A) {
    snprintf(&output[strlen(output)], WORKOUT_DISPLAY_LEN, "A:\nSQ: %d\nBP: %d\nBR: %d", w->squat, w->bench, w->row);
  } else if (w->type == WORKOUT_B) {
    snprintf(&output[strlen(output)], WORKOUT_DISPLAY_LEN, "B: \nSQ: %d\nOP: %d\nDL: %d", w->squat, w->overhead, w->deadlift); 
  }
}

void load_default_workout(Workout *output) {
    output->type = WORKOUT_A;
    output->timestamp = time(0);
    output->bench = 45;
    output->squat = 45;
    output->row = 65;
    memset((void*)&output->reps, 0, sizeof(output->reps));
}

bool load_previous_workout(Workout *output) {
    if (!persist_exists(STORAGE_PREV_WORKOUT)) return false;
    if (persist_read_data(STORAGE_PREV_WORKOUT, (void*)output, sizeof(Workout)) != sizeof(Workout)) return false;
    return true;
}

void load_next_workout(Workout *output) {
    if (!persist_exists(STORAGE_NEXT_WORKOUT)) {
        load_default_workout(output);
        persist_write_data(STORAGE_NEXT_WORKOUT, (void*)output, sizeof(Workout));
        return;
    }
    persist_read_data(STORAGE_NEXT_WORKOUT, (void*)output, sizeof(Workout));
}

void save_previous_workout(Workout *workout) {
    persist_write_data(STORAGE_PREV_WORKOUT, (void*)workout, sizeof(Workout));
}

void save_next_workout(Workout *workout) {
    persist_write_data(STORAGE_NEXT_WORKOUT, (void*)workout, sizeof(Workout));
}

void calculate_next_workout(Workout *next) {
    Workout previous, current;
    bool previous_exists = load_previous_workout(&previous);
    if (!previous_exists) {
        load_default_workout(&previous);
    }
    load_next_workout(&current);
    next->squat = current.squat + (is_exercise_finished(&current, EXERCISE_SQUAT) ? 5 : 0);
    memset((void*)&next->reps, 0, sizeof(next->reps));
    if (current.type == WORKOUT_A) {
        next->type = WORKOUT_B;
        if (previous_exists) {
            next->overhead = previous.overhead + (is_exercise_finished(&previous, EXERCISE_OVERHEAD) ? 5 : 0);
            next->deadlift = previous.deadlift + (is_exercise_finished(&previous, EXERCISE_DEADLIFT) ? 10 : 0);
        } else {
            next->overhead = previous.overhead;
            next->deadlift = previous.deadlift;            
        }
    } else {
        next->type = WORKOUT_A;
        if (previous_exists) {
            next->bench = previous.bench + (is_exercise_finished(&previous, EXERCISE_BENCH) ? 5 : 0);
            next->row = previous.row + (is_exercise_finished(&previous, EXERCISE_ROW) ? 5 : 0);
        } else {
            next->bench = previous.bench;
            next->row = previous.row;
        }
    }
    next->timestamp = time(0);
}

//======================================================================
// Functions for exercise object
//======================================================================
const char *exercise_name_natural(Exercise exercise) {
    switch (exercise) {
        case EXERCISE_SQUAT: return "Squat";
        case EXERCISE_BENCH: return "Bench Press";
        case EXERCISE_ROW: return "Row";
        case EXERCISE_OVERHEAD: return "Overhead Press";
        case EXERCISE_DEADLIFT: return "Deadlift";
    }
    return "";
}

const char *exercise_name_upper_short(Exercise exercise) {
    switch (exercise) {
        case EXERCISE_SQUAT: return "SQUAT";
        case EXERCISE_BENCH: return "BENCH";
        case EXERCISE_ROW: return "ROW";
        case EXERCISE_OVERHEAD: return "OVERHEAD";
        case EXERCISE_DEADLIFT: return "DEADLIFT";
    }
    return "";
}

const char *exercise_name_lower(Exercise exercise) {
    return "";
} 

Exercise exercise_from_workout_index(Workout *workout, uint8_t index) {
    if (index == 0) return EXERCISE_SQUAT;
    if (workout->type == WORKOUT_A) {
        if (index == 1) return EXERCISE_BENCH;
        if (index == 2) return EXERCISE_ROW;
    } else {
        if (index == 1) return EXERCISE_OVERHEAD;
        if (index == 2) return EXERCISE_DEADLIFT;        
    }
    return -1;
}

int weight_for_exercise(Workout *workout, Exercise exercise) {
    switch (exercise) {
        case EXERCISE_SQUAT: return workout->squat;
        case EXERCISE_BENCH: return workout->bench;
        case EXERCISE_ROW: return workout->row;
        case EXERCISE_OVERHEAD: return workout->overhead;
        case EXERCISE_DEADLIFT: return workout->deadlift;
        default: return -1;
    }
}

uint8_t *reps_for_exercise(Workout *workout, Exercise exercise) {
    switch (exercise) {
        case EXERCISE_SQUAT: return workout->squat_reps;
        case EXERCISE_BENCH: return workout->bench_reps;
        case EXERCISE_ROW: return workout->row_reps;
        case EXERCISE_OVERHEAD: return workout->overhead_reps;
        case EXERCISE_DEADLIFT: return workout->deadlift_reps;
        default: return NULL;
    }
}

void increment_exercise_rep(Workout *workout, Exercise exercise, uint8_t index) {
    uint8_t *to_increment;
    to_increment = &(reps_for_exercise(workout, exercise)[index]);
    *to_increment += 1;
    if (*to_increment > 5) *to_increment = 0;
}


bool is_exercise_finished(Workout *workout, Exercise exercise) {
    if (exercise == EXERCISE_DEADLIFT) return (workout->deadlift_reps[0] >= REPS_PER_EXERCISE) ? true : false;
    uint8_t *reps = reps_for_exercise(workout, exercise);
    for (int i=0; i < SETS_PER_EXERCISE; i++) {
        if (reps[i] < REPS_PER_EXERCISE) return false;
    }
    return true;
}