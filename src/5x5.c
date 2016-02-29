#include <pebble.h>
#include <string.h>
#include <pebble_fonts.h>

#define TIME_BUFFER_MAX_LEN  40
#define TIME_TL_WIDTH 60
#define TIME_TL_HEIGHT 40
#define WORKOUT_DISPLAY_LEN 100

static Window *window;
static TextLayer *center_tl;
static TextLayer *time_tl;
static TextLayer *last_workout_tl;
static TextLayer *next_workout_tl;
char prev_workout_string[WORKOUT_DISPLAY_LEN];
char next_workout_string[WORKOUT_DISPLAY_LEN];
char time_string[TIME_BUFFER_MAX_LEN];

typedef struct {
  const char* center;
} screen_view;

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

static void load_screen(screen_view *view) {
  text_layer_set_text(center_tl, view->center);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  screen_view view = {.center = "Select"};
  load_screen(&view);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  screen_view view = {.center = "Up"};
  load_screen(&view);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  screen_view view = {.center = "Down"};
  load_screen(&view);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void updateClock(void) {
  time_t currentTime = time(NULL);
  if (clock_is_24h_style()) {
    strftime(time_string, TIME_BUFFER_MAX_LEN, "%H:%M", localtime(&currentTime));
  } else {
      strftime(time_string, TIME_BUFFER_MAX_LEN, "%I:%M", localtime(&currentTime));
  }
}

void clockTickHandler(struct tm *tick_time, TimeUnits units_changed) {
  updateClock();
}

static void initClock() {
  updateClock();
  tick_timer_service_subscribe(MINUTE_UNIT, &clockTickHandler);
}

static void build_workout_string(char* output, Workout *w, WorkoutTime time) {
  output[0] = '\0';
  if (time == WORKOUT_PREVIOUS) {
    strcat(output, "Prev ");
  } else if (time == WORKOUT_NEXT) {
    strcat(output, "Next ");
  }
  if (w->type == WORKOUT_A) {
    snprintf(&output[strlen(output)], WORKOUT_DISPLAY_LEN, "A:\nSQ: %d\nBP: %d\nBR: %d", w->squat, w->bench, w->row);
  } else if (w->type == WORKOUT_B) {
    snprintf(&output[strlen(output)], WORKOUT_DISPLAY_LEN, "B:\nSQ: %d\nOP: %d\nDL: %d", w->squat, w->overhead, w->deadlift); 
  }
}

static void update_previous_workout(Window *window, Workout *w) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  last_workout_tl = text_layer_create((GRect) { .origin = {bounds.size.w/4-35, bounds.size.h/2-30}, .size = { 70, 60 } });
  build_workout_string(prev_workout_string, w, WORKOUT_PREVIOUS);
  text_layer_set_text(last_workout_tl, prev_workout_string);
  text_layer_set_background_color(last_workout_tl, GColorRed);
  text_layer_set_text_alignment(last_workout_tl, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(last_workout_tl));  
}

static void update_next_workout(Window *window, Workout *w) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  next_workout_tl = text_layer_create((GRect) { .origin = {(bounds.size.w*2)/3-35, bounds.size.h/2-30}, .size = { 70, 60 } });
  build_workout_string(next_workout_string, w, WORKOUT_NEXT);
  text_layer_set_text(next_workout_tl, next_workout_string);
  text_layer_set_background_color(next_workout_tl, GColorYellow);
  text_layer_set_text_alignment(next_workout_tl, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(next_workout_tl));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  center_tl = text_layer_create((GRect) { .origin = {0, bounds.size.h/2-20}, .size = { bounds.size.w-10, 40 } });
  text_layer_set_text(center_tl, "+");
  text_layer_set_text_alignment(center_tl, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(center_tl));
  text_layer_set_font(center_tl, fonts_get_system_font(FONT_KEY_GOTHIC_28 ));
  
  time_tl = text_layer_create((GRect) {.origin = {bounds.size.w/2 - TIME_TL_WIDTH/2, 0}, .size = {TIME_TL_WIDTH, TIME_TL_HEIGHT}});
//   text_layer_set_background_color(time_tl, GColorRed);
  text_layer_set_text(time_tl, time_string);
  text_layer_set_text_alignment(time_tl, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(time_tl));
  text_layer_set_font(time_tl, fonts_get_system_font(FONT_KEY_GOTHIC_28 ));
  
  Workout workout;
  workout.type = WORKOUT_A;
  workout.squat = 135;
  workout.bench = 120;
  workout.row = 100;
  update_previous_workout(window, &workout);
//   text_layer_set_font(last_workout_tl, fonts_get_system_font(FONT_KEY_GOTHIC_28 ));
  
  workout.type = WORKOUT_B;
  workout.squat = 140;
  workout.overhead = 100;
  workout.deadlift = 225;
  update_next_workout(window, &workout);
  
  
  
}

static void window_unload(Window *window) {
  text_layer_destroy(center_tl);
  text_layer_destroy(time_tl);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  initClock();
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}