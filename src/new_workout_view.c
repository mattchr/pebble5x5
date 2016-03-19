//======================================================================
// Includes
//======================================================================
#include "new_workout_view.h"
#include "main_view.h"
#include "globals.h"
#include "workout.h"
#include "edit_workout_view.h"
#include "active_workout_view.h"

//======================================================================
// Private Definitions
//======================================================================
void new_workout_view_constructor(void);
void new_workout_view_destructor(void);
void new_workout_view_select_handler(void);

const View new_workout_view = {
    .constructor = new_workout_view_constructor,
    .destructor = new_workout_view_destructor,
    .next_top = &edit_workout_view,
    .next_select = &active_workout_view,
    .next_bottom = &main_view,
    .top_pressed = NULL,
    .bottom_pressed = NULL,
    .select_pressed = NULL,
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
    new_workout_tl = create_workout_tl(w, (GPoint){get_window_bounds().size.w / 2 - 50, 30});
    build_workout_string(new_workout_string, w, WORKOUT_NEXT);
    text_layer_set_text(new_workout_tl, new_workout_string);
}

void new_workout_view_constructor(void) {
    set_center_tl("Start", FONT_KEY_GOTHIC_18_BOLD);
    set_down_tl("Back", FONT_KEY_GOTHIC_18_BOLD);
    set_up_tl("Edit", FONT_KEY_GOTHIC_18_BOLD);
    Workout workout;
    load_next_workout(&workout);
    workout.timestamp = time(0);
    update_new_workout(&workout);
}
    

void new_workout_view_destructor(void) {
    clear_button_tls();
    text_layer_destroy(new_workout_tl);
}

//======================================================================
// Public Functions
//======================================================================
