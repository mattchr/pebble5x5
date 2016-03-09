#include <pebble.h>
#include "view.h"
#include "globals.h"

void open_view(const View *view) {
    view->constructor();
    current_view = view;
}

void close_view(const View *view) {
    view->destructor();
}

void ViewButtonPress(Button button) {
  const View *next;
  void (*button_handler)(void) = NULL;
  switch(button) {
    case BUTTON_TOP:
      next = current_view->next_top;
      button_handler = current_view->top_pressed;
      break;
    case BUTTON_CENTER:
      next = current_view->next_select;
      button_handler = current_view->select_pressed;
      break;
    case BUTTON_BOTTOM:
      next = current_view->next_bottom;
      button_handler = current_view->bottom_pressed;
      break;
    default:
      next = NULL;
  }
  if (button_handler != NULL) {
      button_handler();
  }
  if (next == NULL) return;
  close_view(current_view);
  open_view(next);
  return;
}