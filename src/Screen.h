#ifndef SCREEN_H
#define SCREEN_H

#ifndef LVGL_H_
#include <lvgl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void screen_init();

void renderNote(char *);
void renderChord(char *);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // SCREEN_H
