//======================================================================
// Includes
//======================================================================
#include "workout.h"
#include "globals.h"

//======================================================================
// Private Functions
//======================================================================

//======================================================================
// Public Functions
//======================================================================
TextLayer *create_workout_tl(Workout *w, GPoint location) {
    Layer *window_layer = window_get_root_layer(window);
    TextLayer *tl = text_layer_create((GRect) { .origin=location, .size ={ 70, 80}});
    text_layer_set_text_alignment(tl, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(tl));
    return tl;
}
void workout_day(char* output, size_t maxsize, Workout *w) {
  strftime(output, maxsize, "%m/%d", localtime(&w->timestamp));
}

void build_workout_string(char* output, Workout *w, WorkoutTime time) {
  output[0] = '\0';
  char workout_day_string[10];
  workout_day(workout_day_string, 10, w);
  if (time == WORKOUT_PREVIOUS) {
    snprintf(output, WORKOUT_DISPLAY_LEN, "%s\nPrev ", workout_day_string);
  } else if (time == WORKOUT_NEXT) {
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
    next->squat = current.squat + 5;
    if (current.type == WORKOUT_A) {
        next->type = WORKOUT_B;
        if (previous_exists) {
            next->overhead = previous.overhead + 5;
            next->deadlift = previous.deadlift + 10;
        } else {
            next->overhead = previous.overhead;
            next->deadlift = previous.deadlift;            
        }
    } else {
        next->type = WORKOUT_A;
        if (previous_exists) {
            next->bench = previous.bench + 5;
            next->row = previous.row + 5;
        } else {
            next->bench = previous.bench;
            next->row = previous.row;
        }
    }
    next->timestamp = time(0);
}