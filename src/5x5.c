#include <pebble.h>
#include <string.h>
#include <pebble_fonts.h>
#include "globals.h"
#include "workout.h"
#include "main_view.h"
#include "view.h"

typedef struct {
    char prev_workout_string[WORKOUT_DISPLAY_LEN];
    char next_workout_string[WORKOUT_DISPLAY_LEN];
} MainWindow;

MainWindow main_window;

char time_string[TIME_BUFFER_MAX_LEN];

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
    layer_mark_dirty(text_layer_get_layer(time_tl));
}

static void initClock() {
    tick_timer_service_subscribe(MINUTE_UNIT, &clockTickHandler);
    text_layer_set_font(time_tl, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_text(time_tl, time_string);
    updateClock();
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
    text_layer_set_text_alignment(time_tl, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(time_tl));
    current_view = &main_view;
    open_view(current_view);
    initClock();
}

static void window_unload(Window *window) {
    text_layer_destroy(time_tl);
}

static void init(void) {
//    clear_storage();
    window = window_create();
    setup_click_config_for_view();

    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}