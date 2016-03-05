//======================================================================
// Includes
//======================================================================
#include "new_workout_view.h"
#include "main_view.h"
#include "globals.h"

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
};

//======================================================================
// Static Variables
//======================================================================

//======================================================================
// Private Functions
//======================================================================
void new_workout_view_constructor(void) {
    set_center_tl("Save", FONT_KEY_GOTHIC_18_BOLD);
    set_down_tl("Cancel", FONT_KEY_GOTHIC_18_BOLD);
}
    

void new_workout_view_destructor(void) {
    text_layer_set_text(center_tl, "");
    text_layer_set_text(down_tl, "");
    
}

//======================================================================
// Public Functions
//======================================================================