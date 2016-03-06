//======================================================================
// Includes
//======================================================================
#include "workout.h"
#include "globals.h"

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