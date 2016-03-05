#pragma once
#include <pebble.h>

typedef enum {
  BUTTON_TOP,
  BUTTON_CENTER,
  BUTTON_BOTTOM,
  BUTTON_BACK,
} Button;

typedef struct View {
  void (*constructor)(void);
  void (*destructor)(void);
  struct View *next_top;
  struct View *next_center;
  struct View *next_bottom;
  struct View *back;
} View;

void open_view(const View *view);
const View* ViewButtonPress(const View *view, Button button) ;