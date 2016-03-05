#include <pebble.h>
#include "globals.h"

Window *window;
TextLayer *center_tl;
TextLayer *down_tl;
TextLayer *time_tl;

void set_tl(TextLayer *tl, const char *text, const char *font) {
    text_layer_set_text(tl, text);
    text_layer_set_font(tl, fonts_get_system_font(font));
    
      // Vertical alignment
    GRect frame = layer_get_frame(text_layer_get_layer(tl));
    GSize content_size = text_layer_get_content_size(tl);
    layer_set_frame(text_layer_get_layer(tl), GRect(frame.origin.x, frame.origin.y + (frame.size.h - content_size.h) / 2, frame.size.w, content_size.h));
    
}

void set_center_tl(const char *text, const char *font) {    
    set_tl(center_tl, text, font);
}

void set_down_tl(const char *text, const char *font) {    
    set_tl(down_tl, text, font);
}


void init_globals(void) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    center_tl = text_layer_create((GRect) {.origin = {0, bounds.size.h / 2 - 20}, .size = { bounds.size.w - 10, 40}});    
    down_tl = text_layer_create((GRect) {.origin = {0, (3*bounds.size.h) / 4 - 20}, .size = { bounds.size.w - 30, 40}});    
}