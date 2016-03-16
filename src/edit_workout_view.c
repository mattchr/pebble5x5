//======================================================================
// Includes
//======================================================================
#include "edit_workout_view.h"
#include "globals.h"
#include "workout.h"
#include "new_workout_view.h"

//======================================================================
// Private Definitions
//======================================================================
#define SELECTION_STRING_LEN 17

typedef enum {
    SELECTION_CANCEL,
    SELECTION_LIFT1,
    SELECTION_LIFT2,
    SELECTION_LIFT3,
    SELECTION_SAVE,
} SelectionIndex;

//======================================================================
// Local Prototyes
//======================================================================
void edit_workout_view_destructor(void);
void edit_workout_view_constructor(void);
void edit_workout_view_down_pressed(void);
void edit_workout_view_up_pressed(void);
void edit_workout_view_select_pressed(void);

//======================================================================
// Static Variables
//======================================================================
const View edit_workout_view = {
    .constructor = edit_workout_view_constructor,
    .destructor = edit_workout_view_destructor,
    .next_top = NULL,
    .next_select = NULL,
    .next_bottom = NULL,
    .top_pressed = edit_workout_view_up_pressed,
    .bottom_pressed = edit_workout_view_down_pressed,
    .select_pressed = edit_workout_view_select_pressed,
};

typedef struct {
    TextLayer *above_selection;
    TextLayer *selection;
    TextLayer *below_selection;

    char selection_string[SELECTION_STRING_LEN];
    char above_selection_string[SELECTION_STRING_LEN];
    char below_selection_string[SELECTION_STRING_LEN];

    SelectionIndex selection_index;
    Workout workout;    
    bool selected;
} EditWorkoutViewData;

EditWorkoutViewData *loc;


//======================================================================
// Private Functions
//======================================================================
void make_selection(void) {
    loc->selected = true;
    text_layer_set_background_color(loc->selection, GColorRed);
}

void unselect(void) {
    loc->selected = false;
    text_layer_set_background_color(loc->selection, GColorBlue);
}

const char *get_selection_text(SelectionIndex index) {
    switch (index) {
        case SELECTION_CANCEL:
            return "CANCEL";
        case SELECTION_LIFT1:
            return "SQUAT";
        case SELECTION_LIFT2:                          
            if (loc->workout.type == WORKOUT_A) {
                return "BENCH";
            } else {
                return "OVERHEAD";
            }
        case SELECTION_LIFT3:                         
            if (loc->workout.type == WORKOUT_A) {
                return "ROW";
            } else {
                return "DEADLIFT";
            }
        case SELECTION_SAVE:
            return "SAVE";
        default: return "";
    }
}
int get_selection_weight(SelectionIndex index) {
    switch (index) {
        case SELECTION_LIFT1:
            return loc->workout.squat;
        case SELECTION_LIFT2:                          
            if (loc->workout.type == WORKOUT_A) {
                return loc->workout.bench;
            } else {
                return loc->workout.overhead;
            }
        case SELECTION_LIFT3:                         
            if (loc->workout.type == WORKOUT_A) {
                return loc->workout.row;
            } else {
                return loc->workout.deadlift;
            }
        case SELECTION_CANCEL:
        case SELECTION_SAVE:
            return -1;
        default: return -1;
    }
}

void load_selection_string(char* output, SelectionIndex index) {
    const char* text = get_selection_text(index);
    int weight = get_selection_weight(index);
    if (weight >= 0) {
        snprintf(output, SELECTION_STRING_LEN, "%s: %d", text, weight);    
    } else {
        snprintf(output, SELECTION_STRING_LEN, "%s", text);
    }
}

void load_selection_tls(void) {
    load_selection_string(loc->selection_string, loc->selection_index);
    text_layer_set_text(loc->selection, loc->selection_string);
    if (loc->selection_index == 0) {
        text_layer_set_text(loc->above_selection, "");
    } else {
        load_selection_string(loc->above_selection_string, loc->selection_index-1);
        text_layer_set_text(loc->above_selection, loc->above_selection_string);
    }
    if (loc->selection_index == SELECTION_SAVE) {
        text_layer_set_text(loc->below_selection, "");
    } else {
        load_selection_string(loc->below_selection_string, loc->selection_index+1);
        text_layer_set_text(loc->below_selection, loc->below_selection_string);
    }
}

void init_selection_tl(TextLayer *tl) {
    layer_add_child(get_window_layer(), text_layer_get_layer(tl));
    text_layer_set_text_alignment(tl, GTextAlignmentCenter);
    text_layer_set_font(tl, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    text_layer_set_text_color(tl, GColorWhite);
}
    
void edit_workout_view_destructor(void) {
    text_layer_destroy(loc->above_selection);
    text_layer_destroy(loc->selection);
    text_layer_destroy(loc->below_selection);
    free(loc);
}

void edit_workout_view_constructor(void) {
    loc = malloc(sizeof(EditWorkoutViewData));
    loc->selection_index = 0;
    load_next_workout(&loc->workout);
    
    int width = get_window_bounds().size.w;
    int height = get_window_bounds().size.h / 4;
    loc->above_selection = text_layer_create(GRect(0, height, width, height));
    loc->selection = text_layer_create(GRect(0, 2*height, width, height));
    loc->below_selection = text_layer_create(GRect(0, 3*height, width, height));
    text_layer_set_background_color(loc->above_selection, GColorBlack);
    text_layer_set_background_color(loc->below_selection, GColorBlack);
    init_selection_tl(loc->above_selection);
    init_selection_tl(loc->selection);
    init_selection_tl(loc->below_selection);
    
    load_selection_tls();
    unselect();
}

void increment_lift(int* to_increment) {
    *to_increment += 5;
    if (*to_increment < 0) *to_increment = 0;
}

void decrement_lift(int* to_decrement) {
    *to_decrement -= 5;
    if (*to_decrement < 0) *to_decrement = 0;
}

void increment_selection() {
    int *to_increment = NULL;
    if (loc->selection_index == SELECTION_LIFT1) to_increment = &loc->workout.squat;
    else if (loc->workout.type == WORKOUT_A) {
        if (loc->selection_index == SELECTION_LIFT2) to_increment = &loc->workout.bench;
        else to_increment = &loc->workout.row;
    } else {
        if (loc->selection_index == SELECTION_LIFT2) to_increment = &loc->workout.overhead;
        else to_increment = &loc->workout.deadlift;        
    }
    if (to_increment == NULL) return; // error
    increment_lift(to_increment);
    load_selection_tls();
}

void decrement_selection() {
    int *to_decrement = NULL;
    if (loc->selection_index == SELECTION_LIFT1) to_decrement = &loc->workout.squat;
    else if (loc->workout.type == WORKOUT_A) {
        if (loc->selection_index == SELECTION_LIFT2) to_decrement = &loc->workout.bench;
        else to_decrement = &loc->workout.row;
    } else {
        if (loc->selection_index == SELECTION_LIFT2) to_decrement = &loc->workout.overhead;
        else to_decrement = &loc->workout.deadlift;        
    }
    if (to_decrement == NULL) return; // error
    decrement_lift(to_decrement);
    load_selection_tls();
}

void edit_workout_view_up_pressed(void) {
    if (loc->selected) {
        increment_selection();
        return;
    }
    if (loc->selection_index > 0) {
        loc->selection_index--;
        load_selection_tls();
    }
}

void edit_workout_view_down_pressed(void) {
    if (loc->selected) {
        decrement_selection();
        return;
    }
    if (loc->selection_index < SELECTION_SAVE) {
        loc->selection_index++;
        load_selection_tls();
    }    
}

void return_to_previous_view(void) {
    close_view(&edit_workout_view);
    open_view(&new_workout_view);    
}

void edit_workout_view_select_pressed(void) {
    if (loc->selection_index == SELECTION_CANCEL) {
        return_to_previous_view();
        return;
    }    
    if (loc->selection_index == SELECTION_SAVE) {
        save_next_workout(&loc->workout);
        return_to_previous_view();
        return;        
    }
    if (loc->selected) unselect();
    else make_selection();
}
//======================================================================
// Public Functions
//======================================================================