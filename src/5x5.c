#include <pebble.h>
#include <string.h>
#include <pebble_fonts.h>
#include "globals.h"
#include "workout.h"
#include "main_view.h"
#include "view.h"

#define TIME_BUFFER_MAX_LEN  40
#define TIME_TL_WIDTH 60
#define TIME_TL_HEIGHT 40

typedef struct {
    char prev_workout_string[WORKOUT_DISPLAY_LEN];
    char next_workout_string[WORKOUT_DISPLAY_LEN];
} MainWindow;

MainWindow main_window;

char time_string[TIME_BUFFER_MAX_LEN];

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    ViewButtonPress(BUTTON_CENTER);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    ViewButtonPress(BUTTON_TOP);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    ViewButtonPress(BUTTON_BOTTOM);
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

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    init_globals();


    time_tl = text_layer_create((GRect) {
        .origin =
        {bounds.size.w / 2 - TIME_TL_WIDTH / 2, 0}, .size =
        {TIME_TL_WIDTH, TIME_TL_HEIGHT}});
    //   text_layer_set_background_color(time_tl, GColorRed);
    text_layer_set_text(time_tl, time_string);
    text_layer_set_text_alignment(time_tl, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(time_tl));
    text_layer_set_font(time_tl, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    current_view = &main_view;
    open_view(current_view);
}

static void window_unload(Window *window) {
    text_layer_destroy(time_tl);
}

static void init(void) {
//    clear_storage();
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