//======================================================================
// Includes
//======================================================================
#include <pebble.h>
#include "active_workout_view.h"
#include "globals.h"
#include "workout.h"
#include "main_view.h"
#include "new_workout_view.h"

//======================================================================
// Private Definitions
//======================================================================
void active_workout_view_constructor(void);  
void active_workout_view_destructor(void);

typedef enum {
    SELECTION_BACK,
    SELECTION_LIFT1,
    SELECTION_LIFT2,
    SELECTION_LIFT3,
    SELECTION_DONE,
    SELECTION_CNT,
} Selection;

//======================================================================
// Static Variables
//======================================================================
const View active_workout_view = {
    .constructor = active_workout_view_constructor,
    .destructor = active_workout_view_destructor,
    .next_top = NULL,
    .next_select = NULL,
    .next_bottom = NULL,
    .top_pressed = NULL,
    .bottom_pressed = NULL,
    .select_pressed = NULL,
};

MenuLayer *active_workout_ml;
Workout *active_workout;

//======================================================================
// Static Functions
//======================================================================
static Exercise exercise_from_selection(Selection index) {
    switch (index) {
        case SELECTION_LIFT1: return exercise_from_workout_index(active_workout, 0);
        case SELECTION_LIFT2: return exercise_from_workout_index(active_workout, 1);
        case SELECTION_LIFT3: return exercise_from_workout_index(active_workout, 2);   
        default: return -1;
    }
}

void update_next_previous_workouts(void) {
    Workout next;
    calculate_next_workout(&next);
    save_next_workout(&next);
    save_previous_workout(active_workout);
}

void cancel_active_workout(void) {
    close_view(&active_workout_view);
    open_view(&new_workout_view);
}

void complete_active_workout(void) {
    save_next_workout(active_workout);
    update_next_previous_workouts();
    close_view(&active_workout_view);
    open_view(&main_view);
    
}

//======================================================================
// Menulayer callbacks
//======================================================================
static uint16_t menu_get_num_rows_callback(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_contex) {
    switch (section_index) {
        case SELECTION_BACK:
        case SELECTION_DONE:
            return 1;
        case SELECTION_LIFT1:
        case SELECTION_LIFT2:
        case SELECTION_LIFT3:
            if (exercise_from_selection(section_index) == EXERCISE_DEADLIFT) return 1;
            return 5;
        default:
            return 0;
    }
}

static uint16_t menu_get_num_sections_callback(struct MenuLayer *menu_layer, void *callback_context) {
    return SELECTION_CNT;
}

static void menu_draw_section_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *callback_context) {
    char temp_string[20];
    const char *text;
    switch(section_index) {
        case SELECTION_BACK:
            text = "BACK"; break;
        case SELECTION_DONE:
            text = "DONE"; break;
        default: {
            const char *exercise_name = exercise_name_upper_short(exercise_from_selection(section_index));
            snprintf(temp_string, 20, "%s: %d", exercise_name, weight_for_exercise(active_workout, exercise_from_selection(section_index)));
            text = temp_string;
            break; }
    }
            
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), layer_get_bounds(cell_layer), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

}


static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    if (cell_index->section == SELECTION_BACK) {
        menu_cell_basic_draw(ctx, cell_layer, "<-", NULL, NULL);
        return;
    }
    if (cell_index->section == SELECTION_DONE) {
        menu_cell_basic_draw(ctx, cell_layer, "->", NULL, NULL);
        return;
    }
    char text[10];
    snprintf(text, 10, "%d", reps_for_exercise(active_workout, exercise_from_selection(cell_index->section))[cell_index->row]);
    graphics_context_set_text_color(ctx, menu_layer_is_index_selected(active_workout_ml, cell_index) ? GColorWhite : GColorBlack);
    graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), layer_get_bounds(cell_layer), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data){
    if (cell_index->section == SELECTION_BACK) return cancel_active_workout();
    if (cell_index->section == SELECTION_DONE) return complete_active_workout();
    decrement_exercise_rep(active_workout, exercise_from_selection(cell_index->section), cell_index->row);
    menu_layer_reload_data(active_workout_ml);
}

 int16_t menu_get_header_height_callback (MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
     return 30;
 }
 
 int16_t menu_get_cell_height_callback (struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
    switch(cell_index->section) {
        case SELECTION_BACK:
        case SELECTION_DONE:
            return 30;
        default:
            return 18;
    }
 }

//======================================================================
// Private Functions
//======================================================================

void active_workout_view_constructor(void) {
    active_workout = malloc(sizeof(Workout));
    load_next_workout(active_workout);
    
    active_workout_ml = menu_layer_create(editable_window_bounds());
    menu_layer_set_callbacks(active_workout_ml, NULL, (MenuLayerCallbacks){
        .get_num_sections = menu_get_num_sections_callback,
        .get_num_rows = menu_get_num_rows_callback,
        .get_header_height = menu_get_header_height_callback,
        .get_cell_height = menu_get_cell_height_callback,
        .draw_header = menu_draw_section_callback,
        .draw_row = menu_draw_row_callback,
        .select_click = menu_select_callback,
    });
    layer_add_child(get_window_layer(), menu_layer_get_layer(active_workout_ml));
    menu_layer_set_click_config_onto_window(active_workout_ml, window);
}
    

void active_workout_view_destructor(void) {
    menu_layer_destroy(active_workout_ml);
    free(active_workout);
}

//======================================================================
// Public Functions
//======================================================================