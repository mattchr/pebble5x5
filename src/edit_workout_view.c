//===========================================================ftex===========
// Includes
//======================================================================
#include "edit_workout_view.h"
#include "globals.h"
#include "workout.h"
#include "new_workout_view.h"
#include "workout.h"

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
    SELECTION_CHOICE_CNT,
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

Workout *workout_to_edit;
MenuLayer *edit_workout_ml;

//======================================================================
// Private Functions
//======================================================================
void redraw_menu(void) {
    layer_mark_dirty(menu_layer_get_layer(edit_workout_ml));    
}

void make_selection(void) {
    menu_layer_set_highlight_colors(edit_workout_ml, GColorRed, GColorWhite);
    setup_click_config_for_view();
    redraw_menu();
}

void unselect(void) {
    menu_layer_set_highlight_colors(edit_workout_ml, GColorBlue, GColorWhite);
    menu_layer_set_click_config_onto_window(edit_workout_ml, window);
    redraw_menu();
}

SelectionIndex get_selection_index(void) {
    return menu_layer_get_selected_index(edit_workout_ml).row;
}

const char *get_selection_text(SelectionIndex index) {
    switch (index) {
        case SELECTION_CANCEL:
            return "CANCEL";
        case SELECTION_LIFT1:
            return "SQUAT";
        case SELECTION_LIFT2:                          
            if (workout_to_edit->type == WORKOUT_A) {
                return "BENCH";
            } else {
                return "OVERHEAD";
            }
        case SELECTION_LIFT3:                         
            if (workout_to_edit->type == WORKOUT_A) {
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
            return workout_to_edit->squat;
        case SELECTION_LIFT2:                          
            if (workout_to_edit->type == WORKOUT_A) {
                return workout_to_edit->bench;
            } else {
                return workout_to_edit->overhead;
            }
        case SELECTION_LIFT3:                         
            if (workout_to_edit->type == WORKOUT_A) {
                return workout_to_edit->row;
            } else {
                return workout_to_edit->deadlift;
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

void init_selection_tl(TextLayer *tl) {
    layer_add_child(get_window_layer(), text_layer_get_layer(tl));
    text_layer_set_text_alignment(tl, GTextAlignmentCenter);
    text_layer_set_font(tl, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    text_layer_set_text_color(tl, GColorWhite);
}

void return_to_previous_view(void) {
    close_view(&edit_workout_view);
    open_view(&new_workout_view);    
}

//======================================================================
// MenuLayer Callbacks
//======================================================================
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return SELECTION_CHOICE_CNT;
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) { 
    char tempString[20];
    load_selection_string(tempString, cell_index->row);
    menu_cell_basic_draw(ctx, cell_layer, tempString, NULL, NULL);
}

void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    if (get_selection_index() == SELECTION_CANCEL) {
        return_to_previous_view();
        return;
    }    
    if (get_selection_index() == SELECTION_SAVE) {
        save_next_workout(workout_to_edit);
        return_to_previous_view();
        return;        
    }
    make_selection();
}

//======================================================================
// View Callbacks
//======================================================================
void edit_workout_view_select_pressed(void) {
    unselect();
}

void edit_workout_view_destructor(void) {
    menu_layer_destroy(edit_workout_ml);
    free(workout_to_edit);
}

void edit_workout_view_constructor(void) {    
    workout_to_edit = malloc(sizeof(Workout));
    load_next_workout(workout_to_edit);
    edit_workout_ml = menu_layer_create(editable_window_bounds());
    menu_layer_set_callbacks(edit_workout_ml, NULL, (MenuLayerCallbacks){
        .get_num_sections = NULL,
        .get_num_rows = menu_get_num_rows_callback,
        .get_header_height = NULL,
        .draw_header = NULL,
        .draw_row = menu_draw_row_callback,
        .select_click = menu_select_callback,
    });
    layer_add_child(get_window_layer(), menu_layer_get_layer(edit_workout_ml));
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
    if (get_selection_index() == SELECTION_LIFT1) to_increment = &workout_to_edit->squat;
    else if (workout_to_edit->type == WORKOUT_A) {
        if (get_selection_index() == SELECTION_LIFT2) to_increment = &workout_to_edit->bench;
        else to_increment = &workout_to_edit->row;
    } else {
        if (get_selection_index() == SELECTION_LIFT2) to_increment = &workout_to_edit->overhead;
        else to_increment = &workout_to_edit->deadlift;        
    }
    if (to_increment == NULL) return; // error
    increment_lift(to_increment);
    redraw_menu();
}

void decrement_selection() {
    int *to_decrement = NULL;
    if (get_selection_index() == SELECTION_LIFT1) to_decrement = &workout_to_edit->squat;
    else if (workout_to_edit->type == WORKOUT_A) {
        if (get_selection_index() == SELECTION_LIFT2) to_decrement = &workout_to_edit->bench;
        else to_decrement = &workout_to_edit->row;
    } else {
        if (get_selection_index() == SELECTION_LIFT2) to_decrement = &workout_to_edit->overhead;
        else to_decrement = &workout_to_edit->deadlift;        
    }
    if (to_decrement == NULL) return; // error
    decrement_lift(to_decrement);
    redraw_menu();
}

void edit_workout_view_up_pressed(void) {
    increment_selection();
}

void edit_workout_view_down_pressed(void) {
    decrement_selection();  
}

//======================================================================
// Public Functions
//======================================================================