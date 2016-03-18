#include <pebble.h>
#include "view.h"
#include "globals.h"

//======================================================================
// Public Functions
//======================================================================
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    ViewButtonPress(BUTTON_CENTER);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    ViewButtonPress(BUTTON_TOP);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    ViewButtonPress(BUTTON_BOTTOM);
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void setup_click_config_for_view(void) {    
    window_set_click_config_provider(window, click_config_provider);
}

void open_view(const View *view) {
    setup_click_config_for_view();
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