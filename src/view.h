#pragma once
#include <pebble.h>

typedef enum {
  BUTTON_TOP,
  BUTTON_CENTER,
  BUTTON_BOTTOM,
} Button;

typedef struct View {
  void (*constructor)(void);
  void (*destructor)(void);
  const struct View *next_top;
  const struct View *next_select;
  const struct View *next_bottom;
  void (*top_pressed)(void);
  void (*select_pressed)(void);
  void (*bottom_pressed)(void);
} View;

void setup_click_config_for_view(void);
void open_view(const View *view);
void close_view(const View *view);
void ViewButtonPress(Button button) ;