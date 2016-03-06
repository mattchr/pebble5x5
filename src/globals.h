#pragma once

#include <pebble.h>

extern Window *window;
extern TextLayer *center_tl;
extern TextLayer *time_tl;
extern TextLayer *down_tl;

Layer *get_window_layer(void);
GRect get_window_bounds(void);
void set_center_tl(const char *label, const char *font);
void set_down_tl(const char *text, const char *font);
void init_globals(void);