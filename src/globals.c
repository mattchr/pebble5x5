//======================================================================
// Includes
//======================================================================
#include "globals.h"
#include "view.h"

//======================================================================
// Private Definitions
//======================================================================

//======================================================================
// Static Variables
//======================================================================
Window *window;
TextLayer *center_tl;
TextLayer *down_tl;
TextLayer *up_tl;
TextLayer *time_tl;
const View *current_view;

//======================================================================
// Private Functions
//======================================================================

//======================================================================
// Public Functions
//======================================================================
void align_text_layer_vertically(TextLayer *tl) {
    GRect frame = layer_get_frame(text_layer_get_layer(tl));
    GSize content_size = text_layer_get_content_size(tl);
    layer_set_frame(text_layer_get_layer(tl), GRect(frame.origin.x, frame.origin.y + (frame.size.h - content_size.h) / 2, frame.size.w, content_size.h));
}

void set_tl(TextLayer *tl, const char *text, const char *font) {
    text_layer_set_text(tl, text);
    text_layer_set_font(tl, fonts_get_system_font(font));
    
      // Vertical alignment
    align_text_layer_vertically(tl);
    
    // Color
    text_layer_set_text_color(tl, GColorBlack);    
}

void set_center_tl(const char *text, const char *font) {    
    set_tl(center_tl, text, font);
}

void set_down_tl(const char *text, const char *font) {    
    set_tl(down_tl, text, font);
}

void set_up_tl(const char *text, const char *font) {    
    set_tl(up_tl, text, font);
}

Layer *get_window_layer(void) {
    return window_get_root_layer(window);
}

GRect get_window_bounds(void) {
    return  layer_get_bounds(get_window_layer());
}

GRect editable_window_bounds(void) {
    return GRect(
        0,
        TIME_TL_HEIGHT,
        get_window_bounds().size.w,
        get_window_bounds().size.h - TIME_TL_HEIGHT
    );
}




void init_globals(void) {
    GRect bounds = layer_get_bounds(get_window_layer());
    center_tl = text_layer_create((GRect) {.origin = {0, bounds.size.h / 2 - 20}, .size = { bounds.size.w - 10, 40}});    
    down_tl = text_layer_create((GRect) {.origin = {0, (3*bounds.size.h) / 4 - 20}, .size = { bounds.size.w - 30, 40}});    
    up_tl = text_layer_create((GRect) {.origin = {0, bounds.size.h / 4 - 20}, .size = { bounds.size.w - 30, 40}}); 
    
    text_layer_set_text_alignment(center_tl, GTextAlignmentRight);
    layer_add_child(get_window_layer(), text_layer_get_layer(center_tl));
    text_layer_set_text_alignment(down_tl, GTextAlignmentRight);
    layer_add_child(get_window_layer(), text_layer_get_layer(down_tl));
    text_layer_set_text_alignment(up_tl, GTextAlignmentRight);
    layer_add_child(get_window_layer(), text_layer_get_layer(up_tl));
}

void clear_button_tls(void) {
    text_layer_set_text(center_tl, "");
    text_layer_set_text(down_tl, "");
    text_layer_set_text(up_tl, "");
}

void clear_storage(void) {
    persist_delete(STORAGE_NEXT_WORKOUT);
    persist_delete(STORAGE_PREV_WORKOUT);
}