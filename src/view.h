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
  const struct View *next_center;
  const struct View *next_bottom;
} View;

void open_view(const View *view);
const View* ViewButtonPress(const View *view, Button button) ;