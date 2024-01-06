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
void renderChord(int);
void renderProgCom(char *);

void changeTabNote();
void changeTabChord();
void changeTabProgCom();

void note_settings_open();
void note_settings_close();

void chord_settings_open();


void channel_note_up_switch();
void channel_note_down_switch();

void channel_chord_up_switch();
void channel_chord_down_switch();

void octave_note_up_switch();
void octave_note_down_switch();
void octave_chord_up_switch();
void octave_chord_down_switch();

void note_off_toggle();
void double_note_toggle();
char * getChordTypeName(int);
char * getChordNoteName(int);
void change_chord_set_clicked();
void root_note_up_switch();
void root_note_down_switch();

void harmony_mode_up_switch();
void harmony_mode_down_switch();

void chord_settings_advanced_open();

void pedal_selected(int);

void chord_type_down_switch();
void chord_variant_down_switch();


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // SCREEN_H
