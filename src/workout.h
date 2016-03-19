#pragma once
#include <pebble.h>

#define WORKOUT_DISPLAY_LEN 100

#define EXERCISES_PER_WORKOUT   3
#define SETS_PER_EXERCISE       5
#define REPS_PER_EXERCISE       5

#define EXERCISE_STRING_SQUAT       "Squat"
#define EXERCISE_STRING_BENCH       "Bench"
#define EXERCISE_STRING_ROW         "Row"
#define EXERCISE_STRING_OVERHEAD    "Overhead Press"
#define EXERCISE_STRING_DEADLIFT    "Deadlift"

typedef enum {
  WORKOUT_A,
  WORKOUT_B,
} WorkoutType;

typedef enum {
    EXERCISE_SQUAT,
    EXERCISE_BENCH,
    EXERCISE_ROW,
    EXERCISE_OVERHEAD,
    EXERCISE_DEADLIFT,
} Exercise;

typedef enum {
  WORKOUT_PREVIOUS,
  WORKOUT_NEXT,
  WORKOUT_TODAY,
} WorkoutSequence;

typedef struct {
    WorkoutType type;  
    time_t timestamp;
    union {
        int lifts[3];
        struct {
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
        };
    };
    union {
        uint8_t reps[5][3];
        struct {
            uint8_t squat_reps[5];
            union {
                struct {
                    uint8_t bench_reps[5];
                    uint8_t row_reps[5];
                };
                struct {
                    uint8_t overhead_reps[5];
                    uint8_t deadlift_reps[5];
                };
            };
        };
    };
} Workout;

void workout_day(char* output, size_t maxsize, Workout *w);
TextLayer *create_workout_tl(Workout *w, GPoint location);
bool load_previous_workout(Workout *output);
void load_next_workout(Workout *output);
void save_previous_workout(Workout *workout);
void save_next_workout(Workout *workout);
void calculate_next_workout(Workout *next);
void build_workout_string(char* output, Workout *w, WorkoutSequence sequence);
const char *exercise_name_natural(Exercise exercise);
const char *exercise_name_upper_short(Exercise exercise);
const char *exercise_name_lower(Exercise exercise);
Exercise exercise_from_workout_index(Workout *workout, uint8_t index);
uint8_t *reps_for_exercise(Workout *workout, Exercise exercise);
int weight_for_exercise(Workout *workout, Exercise exercise);
void increment_exercise_rep(Workout *workout, Exercise exercise, uint8_t index);
bool is_exercise_finished(Workout *workout, Exercise exercise);