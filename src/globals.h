#pragma once

#include <pebble.h>

//======================================================================
// Public Definitions
//======================================================================
typedef enum {
    STORAGE_PREV_WORKOUT,
    STORAGE_NEXT_WORKOUT,
} StorageKey;

#define DEFAULT_BUTTON_FONT FONT_KEY_GOTHIC_18_BOLD

#define TIME_BUFFER_MAX_LEN  40
#define TIME_TL_WIDTH 60
#define TIME_TL_HEIGHT 40

//======================================================================
// Externs
//======================================================================
extern Window *window;
extern TextLayer *center_tl;
extern TextLayer *time_tl;
extern TextLayer *down_tl;
extern TextLayer *up_tl;
extern const struct View *current_view;

//======================================================================
// Public Functions
//======================================================================
Layer *get_window_layer(void);
GRect get_window_bounds(void);
GRect editable_window_bounds(void); 
void set_center_tl(const char *label, const char *font);
void set_down_tl(const char *text, const char *font);
void set_up_tl(const char *text, const char *font);
void init_globals(void);
void clear_button_tls(void);
void clear_storage(void);
void align_text_layer_vertically(TextLayer *tl);