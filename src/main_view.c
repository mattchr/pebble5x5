//======================================================================
// Includes
//======================================================================
#include <pebble.h>
#include "main_view.h"
#include "view.h"
#include "workout.h"
#include "globals.h"
#include "new_workout_view.h"

//======================================================================
// Static Variables
//======================================================================
static TextLayer *last_workout_tl;
static TextLayer *next_workout_tl;

typedef struct {
    char prev_workout_string[WORKOUT_DISPLAY_LEN];
    char next_workout_string[WORKOUT_DISPLAY_LEN];
} MainWindow;

MainWindow main_window;

//======================================================================
// Private Functions
//======================================================================
static void update_previous_workout(Workout *w) {
    last_workout_tl = create_workout_tl(w, (GPoint){get_window_bounds().size.w / 3 - 35, get_window_bounds().size.h / 2 - 50});
    build_workout_string(main_window.prev_workout_string, w, WORKOUT_PREVIOUS);
    text_layer_set_text(last_workout_tl, main_window.prev_workout_string);
}

static void update_next_workout(Workout *w) {
    next_workout_tl = create_workout_tl(w, (GPoint){(get_window_bounds().size.w * 2) / 3 - 35, get_window_bounds().size.h / 2 - 50});
    build_workout_string(main_window.next_workout_string, w, WORKOUT_NEXT);
    text_layer_set_text(next_workout_tl, main_window.next_workout_string);
}

void main_view_constructor(void) {
    Workout workout;
//    workout.type = WORKOUT_A;
//    workout.squat = 135;
//    workout.bench = 120;
//    workout.row = 100;
//    workout.timestamp = time(0);
    if (load_previous_workout(&workout)) {
        update_previous_workout(&workout);
    }
        //   text_layer_set_font(last_workout_tl, fonts_get_system_font(FONT_KEY_GOTHIC_28 ));
    load_next_workout(&workout);
    update_next_workout(&workout);
//    workout.squat = 140;
//    workout.overhead = 100;
//    workout.deadlift = 225;
//    workout.timestamp = time(0);
//    update_next_workout(&workout);
    set_center_tl("+", FONT_KEY_GOTHIC_28);
}

void main_view_destructor(void) {
    text_layer_destroy(last_workout_tl);
    text_layer_destroy(next_workout_tl);
    text_layer_set_text(center_tl, "");
}

const View main_view = {
    .constructor = main_view_constructor,
    .destructor = main_view_destructor,
    .next_top = NULL,
    .next_select = &new_workout_view,
    .next_bottom = NULL,
    .top_pressed = NULL,
    .bottom_pressed = NULL,
    .select_pressed = NULL,
};

