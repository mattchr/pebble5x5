#include <pebble.h>
#include "view.h"

void open_view(const View *view) {
  view->constructor();
}

const View* ViewButtonPress(const View *view, Button button) {
  const View *next;
  switch(button) {
    case BUTTON_TOP:
      next = view->next_top;
      break;
    case BUTTON_CENTER:
      next = view->next_center;
      break;
    case BUTTON_BOTTOM:
      next = view->next_bottom;
      break;
    default:
      next = NULL;
  }
  if (next == NULL) return view;
  view->destructor();
  next->constructor();
  return next;
}