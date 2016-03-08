#pragma once

#include <pebble.h>

//======================================================================
// Public Definitions
//======================================================================
typedef enum {
    STORAGE_PREV_WORKOUT,
    STORAGE_NEXT_WORKOUT,
} StorageKey;

//======================================================================
// Externs
//======================================================================
extern Window *window;
extern TextLayer *center_tl;
extern TextLayer *time_tl;
extern TextLayer *down_tl;

//======================================================================
// Public Functions
//======================================================================
Layer *get_window_layer(void);
GRect get_window_bounds(void);
void set_center_tl(const char *label, const char *font);
void set_down_tl(const char *text, const char *font);
void set_up_tl(const char *text, const char *font);
void init_globals(void);
void clear_button_tls(void);