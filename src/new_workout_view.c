//======================================================================
// Includes
//======================================================================
#include "new_workout_view.h"
#include "main_view.h"
#include "globals.h"
#include "workout.h"

//======================================================================
// Private Definitions
//======================================================================
void new_workout_view_constructor(void);
void new_workout_view_destructor(void);

const View new_workout_view = {
    .constructor = new_workout_view_constructor,
    .destructor = new_workout_view_destructor,
    .next_top = NULL,
    .next_center = NULL,
    .next_bottom = &main_view,
    .top_pressed = NULL,
    .bottom_pressed = NULL,
    .center_pressed = NULL,
};

//======================================================================
// Static Variables
//======================================================================
TextLayer *new_workout_tl;
char new_workout_string[WORKOUT_DISPLAY_LEN];

//======================================================================
// Private Functions
//======================================================================
static void update_new_workout(Workout *w) {
    new_workout_tl = create_workout_tl(w, (GPoint){get_window_bounds().size.w / 2 - 35, get_window_bounds().size.h / 2 - 40});
    build_workout_string(new_workout_string, w, WORKOUT_PREVIOUS);
    text_layer_set_text(new_workout_tl, new_workout_string);
}

void new_workout_view_constructor(void) {
    set_center_tl("Save", FONT_KEY_GOTHIC_18_BOLD);
    set_down_tl("Cancel", FONT_KEY_GOTHIC_18_BOLD);
    Workout workout;
    workout.type = WORKOUT_A;
    workout.squat = 135;
    workout.bench = 120;
    workout.row = 100;
    workout.timestamp = time(0);
    update_new_workout(&workout);
}
    

void new_workout_view_destructor(void) {
    text_layer_set_text(center_tl, "");
    text_layer_set_text(down_tl, "");
    
}

//======================================================================
// Public Functions
//======================================================================