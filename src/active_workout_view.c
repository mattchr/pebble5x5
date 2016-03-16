//======================================================================
// Includes
//======================================================================
#include "active_workout_view.h"
#include "pebble.h"

//======================================================================
// Private Definitions
//======================================================================
void active_workout_view_constructor(void);  
void active_workout_view_destructor(void);

typedef struct {
    uint8_t rep;
    Layer   *layer;
} RepLayer;

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

RepLayer *a;

//======================================================================
// Private Functions
//======================================================================
void rep_layer_update_proc(struct Layer *layer, GContext *ctx) {
    
}


RepLayer *rep_layer_create(GPoint origin) {
    RepLayer *rl = malloc(sizeof(RepLayer));
    rl->rep = 0;
    rl->layer = layer_create((GRect){.origin = origin, .size = {10, 10}});
    layer_set_update_proc(rl->layer, rep_layer_update_proc);
    return rl;
}

void rep_layer_destroy(RepLayer *rl) {
    layer_destroy(rl->layer);
    free(rl);
}


void active_workout_view_constructor(void) {
    a = rep_layer_create(GPoint(50, 50));
}
    

void active_workout_view_destructor(void) {
    rep_layer_destroy(a);
}

//======================================================================
// Public Functions
//======================================================================