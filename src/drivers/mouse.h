#ifndef MOUSE_H
#define MOUSE_H

#include "../kernel/types.h" // IWYU pragma: keep

uint32_t initMouse(void);
void mouse_restore_background();
void mouse_draw();

#endif
