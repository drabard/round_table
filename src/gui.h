#pragma once

struct window;

void gui_init(struct window* window);
void gui_shutdown();
void gui_prepare(struct window* window);
void gui_draw();