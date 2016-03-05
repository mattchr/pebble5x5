#include <pebble.h>
#include "main_view.h"
#include "view.h"
#include "workout.h"

static TextLayer *center_tl;
static TextLayer *time_tl;
static TextLayer *last_workout_tl;
static TextLayer *next_workout_tl;

static void update_previous_workout(Workout *w) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  last_workout_tl = text_layer_create((GRect) { .origin = {bounds.size.w/4-35, bounds.size.h/2-40}, .size = { 70, 80 } });
  build_workout_string(main_window.prev_workout_string, w, WORKOUT_PREVIOUS);
  text_layer_set_text(last_workout_tl, main_window.prev_workout_string);
//   text_layer_set_background_color(last_workout_tl, GColorRed);
  text_layer_set_text_alignment(last_workout_tl, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(last_workout_tl));  
}

static void update_next_workout(Workout *w) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  next_workout_tl = text_layer_create((GRect) { .origin = {(bounds.size.w*2)/3-35, bounds.size.h/2-40}, .size = { 70, 80 } });
  build_workout_string(main_window.next_workout_string, w, WORKOUT_NEXT);
  text_layer_set_text(next_workout_tl, main_window.next_workout_string);
//   text_layer_set_background_color(next_workout_tl, GColorYellow);
  text_layer_set_text_alignment(next_workout_tl, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(next_workout_tl));
}

void load_main_window() {  
  Workout workout;
  workout.type = WORKOUT_A;
  workout.squat = 135;
  workout.bench = 120;
  workout.row = 100;
  workout.timestamp = time(0);
  update_previous_workout(window, &workout);
//   text_layer_set_font(last_workout_tl, fonts_get_system_font(FONT_KEY_GOTHIC_28 ));
  
  workout.type = WORKOUT_B;
  workout.squat = 140;
  workout.overhead = 100;
  workout.deadlift = 225;
  workout.timestamp = time(0);
  update_next_workout(window, &workout);
}

void main_view_constructor(void) {
  load_main_window();
}

const View main_view = {
  .constructor = main_view_constructor,
  .destructor = NULL,
  .next_top = NULL,
  .next_center = NULL,
  .next_bottom = NULL,
  .back = NULL,
};

