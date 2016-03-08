#pragma once
#include <pebble.h>

#define WORKOUT_DISPLAY_LEN 100

typedef enum {
  WORKOUT_A,
  WORKOUT_B,
} WorkoutType;

typedef enum {
  WORKOUT_PREVIOUS,
  WORKOUT_NEXT,
  WORKOUT_TODAY,
} WorkoutTime;

typedef struct {
  WorkoutType type;  
  int squat;
  time_t timestamp;
  union {
    struct {
      int bench;
      int row;
    };
    struct {
      int overhead;
      int deadlift;
    };
  };
} Workout;

void workout_day(char* output, size_t maxsize, Workout *w);
void build_workout_string(char* output, Workout *w, WorkoutTime time);
TextLayer *create_workout_tl(Workout *w, GPoint location);
bool loadPreviousWorkout(Workout *output);
void loadNextWorkout(Workout *output);
