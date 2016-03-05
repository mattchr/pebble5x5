#pragma once

extern Window *window;
extern TextLayer *center_tl;
extern TextLayer *time_tl;
extern TextLayer *down_tl;

void set_center_tl(const char *label, const char *font);
void set_down_tl(const char *text, const char *font);
void init_globals(void);