#include <pebble.h>
#include "workout.h"

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