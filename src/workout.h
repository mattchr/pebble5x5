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
    time_t timestamp;
    int squat;
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
bool load_previous_workout(Workout *output);
void load_next_workout(Workout *output);
void save_previous_workout(Workout *workout);
void save_next_workout(Workout *workout);
void calculate_next_workout(Workout *next);