#include <Screen.h>

#include <SettingsFacade.h>

#include <inttypes.h>

int (*p)[22];
int chordConfigPedalIndex=13;

static lv_obj_t * mb_playContainer;

static lv_obj_t * mb_tabView;
static lv_obj_t * mb_tabNote;
static lv_obj_t * mb_tabChord;
static lv_obj_t * mb_tabProgCom;

static lv_obj_t * mb_velocitySlider;

static lv_style_t note_style;
static lv_style_t chord_style;
static lv_style_t harmony_info_style;

static lv_obj_t * mb_noteLabel;
static lv_obj_t * mb_chordNoteLabel;
static lv_obj_t * mb_chordTypeLabel;
static lv_obj_t * mb_progComLabel;
static lv_obj_t * note_tab_conf_container;
static lv_obj_t * chord_tab_conf_container;
static lv_obj_t * chord_tab_advanced_conf_container;
static lv_obj_t * chord_type_container;
static lv_obj_t * chord_variant_container;
static lv_obj_t * screen_boot_container;

static lv_obj_t * mb_noteSettingsText;
static lv_obj_t * note_settings_button;

static lv_obj_t * mb_chordSettingsText;
static lv_obj_t * chord_settings_button;
static lv_obj_t * chord_settings_advanced_button;

static lv_obj_t * mb_chordsLabel;
static lv_obj_t * mb_chordsInSettingsLabel;
static lv_obj_t * chord_set_btn1;
static lv_obj_t * chord_set_btn2;
static lv_obj_t * chord_set_btn3;
static lv_obj_t * chord_set_btn1_label;
static lv_obj_t * chord_set_btn2_label;
static lv_obj_t * chord_set_btn3_label;

static lv_obj_t * channel_note_up_button;
static lv_obj_t * channel_note_down_button;
static lv_obj_t * channel_chord_up_button;
static lv_obj_t * channel_chord_down_button;

static lv_obj_t * chord_type_down_button;
static lv_obj_t * chord_variant_down_button;

static lv_obj_t * octave_note_up_button;
static lv_obj_t * octave_note_down_button;

static lv_obj_t * octave_chord_up_button;
static lv_obj_t * octave_chord_down_button;

static lv_obj_t * root_note_chord_value_label;
static lv_obj_t * root_note_up_button;
static lv_obj_t * root_note_down_button;

static lv_obj_t * chord_type_label;
static lv_obj_t * chord_variant_label;

static lv_obj_t * chordAdvancedConfigLabel;

static lv_obj_t * harmony_mode_value_label;
static lv_obj_t * harmony_mode_up_button;
static lv_obj_t * harmony_mode_down_button;

static lv_obj_t * mb_noteOffButton;
static lv_obj_t * mb_doubleNoteButton;

static lv_obj_t * pedal1_label;
static lv_obj_t * pedal2_label;
static lv_obj_t * pedal3_label;
static lv_obj_t * pedal4_label;
static lv_obj_t * pedal5_label;
static lv_obj_t * pedal6_label;
static lv_obj_t * pedal7_label;
static lv_obj_t * pedal8_label;
static lv_obj_t * pedal9_label;
static lv_obj_t * pedal10_label;
static lv_obj_t * pedal11_label;
static lv_obj_t * pedal12_label;
static lv_obj_t * pedal13_label;

static lv_obj_t * set_pedal1_label;
static lv_obj_t * set_pedal2_label;
static lv_obj_t * set_pedal3_label;
static lv_obj_t * set_pedal4_label;
static lv_obj_t * set_pedal5_label;
static lv_obj_t * set_pedal6_label;
static lv_obj_t * set_pedal7_label;
static lv_obj_t * set_pedal8_label;
static lv_obj_t * set_pedal9_label;
static lv_obj_t * set_pedal10_label;
static lv_obj_t * set_pedal11_label;
static lv_obj_t * set_pedal12_label;
static lv_obj_t * set_pedal13_label;

static lv_obj_t * set_pedal1_btn;
static lv_obj_t * set_pedal2_btn;
static lv_obj_t * set_pedal3_btn;
static lv_obj_t * set_pedal4_btn;
static lv_obj_t * set_pedal5_btn;
static lv_obj_t * set_pedal6_btn;
static lv_obj_t * set_pedal7_btn;
static lv_obj_t * set_pedal8_btn;
static lv_obj_t * set_pedal9_btn;
static lv_obj_t * set_pedal10_btn;
static lv_obj_t * set_pedal11_btn;
static lv_obj_t * set_pedal12_btn;
static lv_obj_t * set_pedal13_btn;

lv_obj_t * channel_note_value_label;
lv_obj_t * channel_chord_value_label;


lv_obj_t * octave_note_value_label;
lv_obj_t * octave_chord_value_label;

lv_obj_t * mb_noteOffLabel;
lv_obj_t * mb_doubleNoteLabel;

static lv_style_t chord_set_button_style_selected;
static lv_style_t chord_set_button_style;

static lv_style_t brown_pedal_button_style;
static lv_style_t black_pedal_button_style;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void note_screen_create(lv_obj_t * parent);
static void chord_screen_create(lv_obj_t * parent);
static void progcom_screen_create(lv_obj_t * parent);

const char *screenChordTypeNames[7][6] = {
    {" ", " ", " ", " ", " ", " "},
    {"Major", "Maj6", "Maj7", "Maj9", "Maj11", "Maj13"},
    {"Minor", "Min6", "Min7", "MinMaj7", "Min9", "Min11"},
    {"Dom7", "Dom9", "Dom11", "Dom13", "Dom7Sus4", " "},
    {"Dim", "Dim7", "HalfDim7", " ", " ", " "},
    {"Sus2", "Sus4", " ", " ", " ", " "},
    {"Aug", "Aug7", " ", " ", " ", " "}
};

const char *modeNames[8] = { "", "Ionian", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Aeolian", "Locrian" };

LV_FONT_DECLARE(Monserrat_200);
LV_FONT_DECLARE(Monserrat_150);

static void velocity_slider_handler(lv_event_t * event)
{

    lv_obj_t * slider = lv_event_get_target(event);
    setVelocity((int)lv_slider_get_value(slider));
}

static void channel_note_up_event(lv_event_t * e)
{
    (getChannelNote()==16) ? setChannelNote(1) : setChannelNote(getChannelNote()+1);
    lv_label_set_text_fmt(channel_note_value_label, "Channel %d", getChannelNote());

}

void channel_note_up_switch() {
    lv_event_send(channel_note_up_button, LV_EVENT_CLICKED, NULL);
}

static void channel_note_down_event(lv_event_t * e)
{
    (getChannelNote()==1) ? setChannelNote(16) : setChannelNote(getChannelNote()-1);
    lv_label_set_text_fmt(channel_note_value_label, "Channel %d", getChannelNote());

}

void channel_note_down_switch() {
    lv_event_send(channel_note_down_button, LV_EVENT_CLICKED, NULL);
}

static void channel_chord_up_event(lv_event_t * e)
{
    (getChannelChord()==16) ? setChannelChord(1) : setChannelChord(getChannelChord()+1);
    lv_label_set_text_fmt(channel_chord_value_label, "Channel %d", getChannelChord());

}

void channel_chord_up_switch() {
    lv_event_send(channel_chord_up_button, LV_EVENT_CLICKED, NULL);
}

static void channel_chord_down_event(lv_event_t * e)
{
    (getChannelChord()==1) ? setChannelChord(16) : setChannelChord(getChannelChord()-1);
    lv_label_set_text_fmt(channel_chord_value_label, "Channel %d", getChannelChord());

}

void channel_chord_down_switch() {
    lv_event_send(channel_chord_down_button, LV_EVENT_CLICKED, NULL);
}

static void octave_note_up_event(lv_event_t * e)
{
    (getOctave()==5) ? setOctave(-4) : setOctave(getOctave()+1);
    lv_label_set_text_fmt(octave_note_value_label, "Octave %d", getOctave());

}

void octave_note_up_switch() {
    lv_event_send(octave_note_up_button, LV_EVENT_CLICKED, NULL);
}

static void octave_note_down_event(lv_event_t * e)
{
    (getOctave()==-4) ? setOctave(5) : setOctave(getOctave()-1);
    lv_label_set_text_fmt(octave_note_value_label, "Octave %d", getOctave());
}

void octave_note_down_switch() {
    lv_event_send(octave_note_down_button, LV_EVENT_CLICKED, NULL);
}

static void octave_chord_up_event(lv_event_t * e)
{
    (getChordsSetOctave(getChordSet())==5) ? setChordsSetOctave(getChordSet(), -4) : setChordsSetOctave(getChordSet(), getChordsSetOctave(getChordSet())+1);
    lv_label_set_text_fmt(octave_chord_value_label, "Octave %d", getChordsSetOctave(getChordSet()));

}

void octave_chord_up_switch() {
    lv_event_send(octave_chord_up_button, LV_EVENT_CLICKED, NULL);
}

static void octave_chord_down_event(lv_event_t * e)
{
    (getChordsSetOctave(getChordSet())==-4)? setChordsSetOctave(getChordSet(), 5) : setChordsSetOctave(getChordSet(), getChordsSetOctave(getChordSet())-1);
    lv_label_set_text_fmt(octave_chord_value_label, "Octave %d", getChordsSetOctave(getChordSet()));
}

void octave_chord_down_switch() {
    lv_event_send(octave_chord_down_button, LV_EVENT_CLICKED, NULL);
}

void change_chord_set_clicked() {
    switch (getChordSet()) {
        case 1:
            lv_event_send(chord_set_btn2, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(chord_set_btn3, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(chord_set_btn1, LV_EVENT_CLICKED, NULL);
            break;
    }
}

static void root_note_up_event(lv_event_t * e)
{
    (getChordsSetRootNote(getChordSet)==12) ? setChordsSetRootNote(getChordSet(), 0) : setChordsSetRootNote(getChordSet(), getChordsSetRootNote(getChordSet())+1) ;
    lv_label_set_text_fmt(root_note_chord_value_label, "Root Note %s", getChordNoteName(getChordsSetRootNote(getChordSet)));
    renderChords();
}

void root_note_up_switch() {
    lv_event_send(root_note_up_button, LV_EVENT_CLICKED, NULL);
}

static void root_note_down_event(lv_event_t * e)
{
    (getChordsSetRootNote(getChordSet)==0) ? setChordsSetRootNote(getChordSet(), 12) : setChordsSetRootNote(getChordSet(), getChordsSetRootNote(getChordSet())-1) ;
    lv_label_set_text_fmt(root_note_chord_value_label, "Root Note %s", getChordNoteName(getChordsSetRootNote(getChordSet)));
    renderChords();
}

void root_note_down_switch() {
    lv_event_send(root_note_down_button, LV_EVENT_CLICKED, NULL);
}


static void harmony_mode_up_event(lv_event_t * e)
{
    (getChordsSetMode(getChordSet)==7) ? setChordsSetMode(getChordSet(), 1) : setChordsSetMode(getChordSet(), getChordsSetMode(getChordSet())+1) ;
    lv_label_set_text(harmony_mode_value_label, modeNames[getChordsSetMode(getChordSet)]);
    renderChords();
}

void harmony_mode_up_switch() {
    lv_event_send(harmony_mode_up_button, LV_EVENT_CLICKED, NULL);
}

static void harmony_mode_down_event(lv_event_t * e)
{
    (getChordsSetMode(getChordSet)==1) ? setChordsSetMode(getChordSet(), 7) : setChordsSetMode(getChordSet(), getChordsSetMode(getChordSet())-1) ;
    lv_label_set_text(harmony_mode_value_label, modeNames[getChordsSetMode(getChordSet)]);
    renderChords();
}


static void chord_type_down_event(lv_event_t * e)
{
    // make sure that button is clicked and the pedal is already slected for which chord variant down need to perform
    if (lv_event_get_code(e) == LV_EVENT_CLICKED && chordConfigPedalIndex < 13) {
        // move up chord type
        p = getChordsSetPointer();
        if ((p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10)/10 != 6) {
            configChord(getChordSet(), chordConfigPedalIndex, p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10+11);
        } else {
            //it is moving to bottom (index 0)
            configChord(getChordSet(), chordConfigPedalIndex, 0);
        }

        // render the outcome of this action
        renderChords();
        renderChordGroupLabel(chordConfigPedalIndex);
        renderChordVariantLabel(chordConfigPedalIndex);
    }
}

void chord_type_down_switch() {
    lv_event_send(chord_type_down_button, LV_EVENT_CLICKED, NULL);
}

static void chord_type_up_event(lv_event_t * e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // move down chord type
        p = getChordsSetPointer();
        if ((p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10)/10 != 0) {
            //check that this is not moved to non defined chord - index 0
            if ((p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10)/10 != 1) {
                configChord(getChordSet(), chordConfigPedalIndex, p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10-9);
            } else {
                // it is moved to not defined chord so call chordConfig with 0 chord type
                configChord(getChordSet(), chordConfigPedalIndex, 0);
            }
        } else {
            //it is moving to top (index 6)
            configChord(getChordSet(), chordConfigPedalIndex, 61);
        }

        // render the outcome of this action
        renderChords();
        renderChordGroupLabel(chordConfigPedalIndex);
        renderChordVariantLabel(chordConfigPedalIndex);
    }
}

static void chord_variant_down_event(lv_event_t * e)
{
    // make sure that button is clicked and the pedal is already slected for which chord variant down need to perform
    if (lv_event_get_code(e) == LV_EVENT_CLICKED && chordConfigPedalIndex < 13) {
        //check if any action should be taken - if chord tyoe is "not defined" do not act on variant up event
        p = getChordsSetPointer();
        if (p[chordConfigPedalIndex][21] != 0) {
            //check what is the chord type so that you don't go more down then available variants
            //    {" ", " ", " ", " ", " ", " "},
            //    {"Major", "Maj 6", "Maj 7", "Maj 9", "Maj 11", "Maj 13"},
            //    {"Minor", "Min 6", "Min 7", "Min (Maj7)", "Min 9", "Min 11"},
            //    {"Dom 7", "Dom 9", "Dom 11", "Dom 13", "Dom 7 Sus 4", " "},
            //    {"Dim", "Dim 7", "Half Dim 7", " ", " ", " "},
            //    {"Sus 2", "Sus 4", " ", " ", " ", " "},
            //    {"Aug", "Aug 7", " ", " ", " ", " "}
            int variantNumber;
            switch ((p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10)/10) {
                case 1:
                    // chord type is major - there are maximum 6 variants
                    variantNumber=6;
                    break;
                case 2:
                    // chord type is Minor  - there are maximum 6 variants
                    variantNumber=6;
                    break;
                case 3:
                    // chord type is Dominant - there are maximum 5 variants
                    variantNumber=5;
                    break;
                case 4:
                    // chord type is Diminished - there are maximum 3 variants
                    variantNumber=3;
                    break;
                case 5:
                    // chord type is Suspended - there are maximum 2 variants
                    variantNumber=2;
                    break;
                case 6:
                    // chord type is Augmented - there are maximum 2 variants
                    variantNumber=2;
                    break;

            }

            // move down chord variant
            if (p[chordConfigPedalIndex][21]%10<variantNumber) {
                //there is one more to go down so simple increase the chord number
                configChord(getChordSet(), chordConfigPedalIndex, p[chordConfigPedalIndex][21]+1);
            } else {
                // there is no more variant so  go back to first one
                configChord(getChordSet(), chordConfigPedalIndex, p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10+1);
            }

            // render the outcome of this action
            renderChords();
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

void chord_variant_down_switch() {
    lv_event_send(chord_variant_down_button, LV_EVENT_CLICKED, NULL);
}

static void chord_variant_up_event(lv_event_t * e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        //check if any action should be taken - if chord tyoe is "not defined" do not act on variant up event
        p = getChordsSetPointer();
        if (p[chordConfigPedalIndex][21] != 0) {
           //check what that it is not first variant because we can not go more up if it is first
            if (p[chordConfigPedalIndex][21]%10<2 ) {
                // this is first variant so we need to discover what is the top variant inside this chord type
                //    {" ", " ", " ", " ", " ", " "},
                //    {"Major", "Maj 6", "Maj 7", "Maj 9", "Maj 11", "Maj 13"},
                //    {"Minor", "Min 6", "Min 7", "Min (Maj7)", "Min 9", "Min 11"},
                //    {"Dom 7", "Dom 9", "Dom 11", "Dom 13", "Dom 7 Sus 4", " "},
                //    {"Dim", "Dim 7", "Half Dim 7", " ", " ", " "},
                //    {"Sus 2", "Sus 4", " ", " ", " ", " "},
                //    {"Aug", "Aug 7", " ", " ", " ", " "}
                int variantNumber;
                switch ((p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10)/10) {
                    case 1:
                        // chord type is major - there are maximum 6 variants
                        variantNumber=6;
                        break;
                    case 2:
                        // chord type is Minor  - there are maximum 6 variants
                        variantNumber=6;
                        break;
                    case 3:
                        // chord type is Dominant - there are maximum 5 variants
                        variantNumber=5;
                        break;
                    case 4:
                        // chord type is Diminished - there are maximum 3 variants
                        variantNumber=3;
                        break;
                    case 5:
                        // chord type is Suspended - there are maximum 2 variants
                        variantNumber=2;
                        break;
                    case 6:
                        // chord type is Augmented - there are maximum 2 variants
                        variantNumber=2;
                        break;

                }
                configChord(getChordSet(), chordConfigPedalIndex, p[chordConfigPedalIndex][21]-p[chordConfigPedalIndex][21]%10 + variantNumber);
            } else {
                // we can safely go one variant up
                configChord(getChordSet(), chordConfigPedalIndex, p[chordConfigPedalIndex][21]-1);
            }

            // render the outcome of this action
            renderChords();
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

void harmony_mode_down_switch() {
    lv_event_send(harmony_mode_down_button, LV_EVENT_CLICKED, NULL);
}


static void note_off_event(lv_event_t * event)
{

    toggleNoteOff();
    lv_label_set_text_fmt(mb_noteOffLabel, "Note Off\n%s", (getNoteOff()) ? "Enabled" : "Disabled");

}

void note_off_toggle() {
    lv_event_send(mb_noteOffButton, LV_EVENT_CLICKED, NULL);
    if (!getNoteOff()) {
        lv_obj_add_state(mb_noteOffButton, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(mb_noteOffButton, LV_STATE_CHECKED);
    }
}


static void double_note_event(lv_event_t * event)
{

    toggleDoubleNote();
    lv_label_set_text_fmt(mb_doubleNoteLabel, "Double\nNote\n%s", (getDoubleNote()) ? "Enabled" : "Disabled");

}

void double_note_toggle() {
    lv_event_send(mb_doubleNoteButton, LV_EVENT_CLICKED, NULL);
    if (!getDoubleNote()) {
        lv_obj_add_state(mb_doubleNoteButton, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(mb_doubleNoteButton, LV_STATE_CHECKED);
    }
}

static void scroll_begin_event(lv_event_t * e)
{
    /*Disable the scroll animations. Triggered when a tab button is clicked */
    if (lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN) {
        lv_anim_t* a = lv_event_get_param(e);
        if (a)  a->time = 0;

    }
}

void renderNote(char * noteName) {
    lv_label_set_text(mb_noteLabel, noteName);
}

void renderChord(int pedalIndex) {
    if (pedalIndex >= 0 && pedalIndex < 13) {
        lv_label_set_text(mb_chordNoteLabel, getChordNoteName(pedalIndex));
        lv_label_set_text(mb_chordTypeLabel, getChordTypeName(pedalIndex));
    } else {
        lv_label_set_text(mb_chordNoteLabel, "");
        lv_label_set_text(mb_chordTypeLabel, "");
    }
}

void renderProgCom(char * progComName) {
    lv_label_set_text(mb_progComLabel, progComName);
}

void changeTabNote() {
    lv_tabview_set_act(mb_tabView, 0, LV_ANIM_OFF);
    lv_obj_move_background(note_tab_conf_container);
    lv_obj_move_background(chord_tab_conf_container);
    lv_obj_move_background(chord_tab_advanced_conf_container);
    renderNoteSettings();
    setState(1);
}

void changeTabChord() {
    lv_tabview_set_act(mb_tabView, 1, LV_ANIM_OFF);
    lv_obj_move_background(note_tab_conf_container);
    lv_obj_move_background(chord_tab_conf_container);
    lv_obj_move_background(chord_tab_advanced_conf_container);
    renderChordSettings();
    renderChords();
    setState(2);
}

void note_settings_open() {
    lv_event_send(note_settings_button, LV_EVENT_CLICKED, NULL);
}

void chord_settings_open() {
    lv_event_send(chord_settings_button, LV_EVENT_CLICKED, NULL);
}

void chord_settings_advanced_open() {
    lv_event_send(chord_settings_advanced_button, LV_EVENT_CLICKED, NULL);
}

void renderNoteSettings() {
    lv_label_set_text_fmt(mb_noteSettingsText, "Channel:\t$ff0000 %d$\nOctave:\t$ff0000 %d$\nNote Off:\t$ff0000 %s$\nDouble:\t$ff0000 %s$\n", getChannelNote(), getOctave(), (getNoteOff()) ? "ON" : "OFF", (getDoubleNote()) ? "ON" : "OFF");
}

void renderChordSettings() {
    lv_label_set_text_fmt(mb_chordSettingsText, "Root Note: \t$ff0000 %s$\nMode: \t$ff0000 %s$\nChannel:\t$ff0000 %d$\nOctave:\t$ff0000 %d$\nNote Off:\t$ff0000 %s$\n", getChordNoteName(getChordsSetRootNote(getChordSet())), modeNames[getChordsSetMode(getChordSet())], getChannelChord(), getChordsSetOctave(getChordSet()), (getNoteOff()) ? "ON" : "OFF");
}

void changeTabProgCom() {
    lv_tabview_set_act(mb_tabView, 2, LV_ANIM_OFF);
    setState(3);
}

static void tabview_change_event(lv_event_t * e)
{
    lv_obj_t * objTabView = lv_event_get_current_target(e);

    // put any configuration screens to background
    lv_obj_move_background(note_tab_conf_container);
    lv_obj_move_background(chord_tab_conf_container);
    lv_obj_move_background(chord_tab_advanced_conf_container);

    if ( lv_tabview_get_tab_act(objTabView) == 0 ) {
        setState(1);
        lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
        renderNoteSettings();

    }
    if ( lv_tabview_get_tab_act(objTabView) == 1 ) {
        setState(2);
        lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
        renderChords();
    }
    if ( lv_tabview_get_tab_act(objTabView) == 2 ) {
        setState(3);
        lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_ORANGE), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    }

}

static void note_settings_event(lv_event_t * e) {

    lv_obj_move_foreground(note_tab_conf_container);
    setState(11);
}


static void chord_settings_event(lv_event_t * e) {

    lv_obj_move_foreground(chord_tab_conf_container);
    setState(21);
    lv_label_set_text_fmt(channel_chord_value_label, "Channel %d", getChannelChord());
    lv_label_set_text_fmt(octave_chord_value_label, "Octave %d", getChordsSetOctave(getChordSet()));
    lv_label_set_text_fmt(root_note_chord_value_label, "Root Note %s", getChordNoteName(getChordsSetRootNote(getChordSet)));
    lv_label_set_text(harmony_mode_value_label, modeNames[getChordsSetMode(getChordSet)]);

}

static void chord_settings_advanced_event(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        lv_obj_move_foreground(chord_tab_advanced_conf_container);
        lv_label_set_text(chordAdvancedConfigLabel, "Click on pedal to set its \ncustom chord.");
        lv_obj_add_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
        //set chord pedal index to 13 which means that pedal is not selected
        chordConfigPedalIndex=13;
        setState(22);
    }
}


static void chord_set_btn1_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        setChordSet(1);
        renderChords();
        renderChordSettings();
        lv_obj_add_state(chord_set_btn1, LV_STATE_CHECKED);
        lv_obj_clear_state(chord_set_btn2, LV_STATE_CHECKED);
        lv_obj_clear_state(chord_set_btn3, LV_STATE_CHECKED);

    }
}

static void chord_set_btn2_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        setChordSet(2);
        renderChords();
        renderChordSettings();
        lv_obj_add_state(chord_set_btn2, LV_STATE_CHECKED);
        lv_obj_clear_state(chord_set_btn1, LV_STATE_CHECKED);
        lv_obj_clear_state(chord_set_btn3, LV_STATE_CHECKED);

    }
}

static void chord_set_btn3_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        setChordSet(3);
        renderChords();
        renderChordSettings();
        lv_obj_add_state(chord_set_btn3, LV_STATE_CHECKED);
        lv_obj_clear_state(chord_set_btn1, LV_STATE_CHECKED);
        lv_obj_clear_state(chord_set_btn2, LV_STATE_CHECKED);

    }
}

void renderChordGroupLabel(int pedalIndex) {
    p = getChordsSetPointer();
    switch ((p[pedalIndex][21]-p[pedalIndex][21]%10)/10) {
        case 0:
            lv_label_set_text_fmt(chord_type_label, "$ff0000 Not Defined$\nMajor\nMinor\nDominant\nDiminished\nSuspended\nAugmented");
            break;
        case 1:
            lv_label_set_text_fmt(chord_type_label, "Not Defined\n$ff0000 Major$\nMinor\nDominant\nDiminished\nSuspended\nAugmented");
            break;
        case 2:
            lv_label_set_text_fmt(chord_type_label, "Not Defined\nMajor\n$ff0000 Minor$\nDominant\nDiminished\nSuspended\nAugmented");
            break;
        case 3:
            lv_label_set_text_fmt(chord_type_label, "Not Defined\nMajor\nMinor\n$ff0000 Dominant$\nDiminished\nSuspended\nAugmented");
            break;
        case 4:
            lv_label_set_text_fmt(chord_type_label, "Not Defined\nMajor\nMinor\nDominant\n$ff0000 Diminished$\nSuspended\nAugmented");
            break;
        case 5:
            lv_label_set_text_fmt(chord_type_label, "Not Defined\nMajor\nMinor\nDominant\nDiminished\n$ff0000 Suspended$\nAugmented");
            break;
        case 6:
            lv_label_set_text_fmt(chord_type_label, "Not Defined\nMajor\nMinor\nDominant\nDiminished\nSuspended\n$ff0000 Augmented$");
            break;
    }
}

void renderChordVariantLabel(int pedalIndex) {
    p = getChordsSetPointer();
    lv_label_set_text(chord_variant_label, "");
    // check if this is a chord of any type - if not do not render variant
    if (p[pedalIndex][21] != 0) {
        for (int i = 0; i < 6; i++ ) {
            if (i!=0) { lv_label_ins_text(chord_variant_label, LV_LABEL_POS_LAST, "\n"); }
            if (p[pedalIndex][21]%10-1 == i) {
                    lv_label_ins_text(chord_variant_label, LV_LABEL_POS_LAST, "$ff0000 ");
                    lv_label_ins_text(chord_variant_label, LV_LABEL_POS_LAST, screenChordTypeNames[(p[pedalIndex][21]-p[pedalIndex][21]%10)/10][i]);
                    lv_label_ins_text(chord_variant_label, LV_LABEL_POS_LAST, "$");
            } else {
                lv_label_ins_text(chord_variant_label, LV_LABEL_POS_LAST, screenChordTypeNames[(p[pedalIndex][21]-p[pedalIndex][21]%10)/10][i]);
            }
        }
    }
}

void pedal_selected(int pedalIndex) {
    switch (pedalIndex) {
        case 0:
            lv_event_send(set_pedal1_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 1:
            lv_event_send(set_pedal2_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 2:
            lv_event_send(set_pedal3_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 3:
            lv_event_send(set_pedal4_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 4:
            lv_event_send(set_pedal5_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 5:
            lv_event_send(set_pedal6_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 6:
            lv_event_send(set_pedal7_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 7:
            lv_event_send(set_pedal8_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 8:
            lv_event_send(set_pedal9_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 9:
            lv_event_send(set_pedal10_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 10:
            lv_event_send(set_pedal11_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 11:
            lv_event_send(set_pedal12_btn, LV_EVENT_CLICKED, NULL);
            break;
        case 12:
            lv_event_send(set_pedal13_btn, LV_EVENT_CLICKED, NULL);
            break;
    }
}

static void  pedal1_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        // do something only if in state 22
        if (getState() == 22) {
            chordConfigPedalIndex=0;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal2_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=1;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal3_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=2;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal4_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=3;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal5_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=4;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }

    }
}

static void  pedal6_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=5;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal7_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=6;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal8_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=7;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal9_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=8;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal10_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=9;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal11_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=10;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal12_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=11;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}

static void  pedal13_btn_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (getState() == 22) {
            chordConfigPedalIndex=12;
            lv_label_set_text_fmt(chordAdvancedConfigLabel, "Setting a chord for pedal root note $ff0000 %s$", getChordNoteName(chordConfigPedalIndex));
            lv_obj_clear_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);
            renderChordGroupLabel(chordConfigPedalIndex);
            renderChordVariantLabel(chordConfigPedalIndex);
        }
    }
}


void renderChords() {

    lv_label_set_text(pedal1_label, getChordNoteName(0));
    lv_label_ins_text(pedal1_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal1_label, LV_LABEL_POS_LAST, getChordTypeName(0));

    lv_label_set_text(pedal2_label, getChordNoteName(1));
    lv_label_ins_text(pedal2_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal2_label, LV_LABEL_POS_LAST, getChordTypeName(1));

    lv_label_set_text(pedal3_label, getChordNoteName(2));
    lv_label_ins_text(pedal3_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal3_label, LV_LABEL_POS_LAST, getChordTypeName(2));

    lv_label_set_text(pedal4_label, getChordNoteName(3));
    lv_label_ins_text(pedal4_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal4_label, LV_LABEL_POS_LAST, getChordTypeName(3));

    lv_label_set_text(pedal5_label, getChordNoteName(4));
    lv_label_ins_text(pedal5_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal5_label, LV_LABEL_POS_LAST, getChordTypeName(4));


    lv_label_set_text(pedal6_label, getChordNoteName(5));
    lv_label_ins_text(pedal6_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal6_label, LV_LABEL_POS_LAST, getChordTypeName(5));


    lv_label_set_text(pedal7_label, getChordNoteName(6));
    lv_label_ins_text(pedal7_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal7_label, LV_LABEL_POS_LAST, getChordTypeName(6));


    lv_label_set_text(pedal8_label, getChordNoteName(7));
    lv_label_ins_text(pedal8_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal8_label, LV_LABEL_POS_LAST, getChordTypeName(7));


    lv_label_set_text(pedal9_label, getChordNoteName(8));
    lv_label_ins_text(pedal9_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal9_label, LV_LABEL_POS_LAST, getChordTypeName(8));


    lv_label_set_text(pedal10_label, getChordNoteName(9));
    lv_label_ins_text(pedal10_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal10_label, LV_LABEL_POS_LAST, getChordTypeName(9));


    lv_label_set_text(pedal11_label, getChordNoteName(10));
    lv_label_ins_text(pedal11_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal11_label, LV_LABEL_POS_LAST, getChordTypeName(10));


    lv_label_set_text(pedal12_label, getChordNoteName(11));
    lv_label_ins_text(pedal12_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal12_label, LV_LABEL_POS_LAST, getChordTypeName(11));


    lv_label_set_text(pedal13_label, getChordNoteName(12));
    lv_label_ins_text(pedal13_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(pedal13_label, LV_LABEL_POS_LAST, getChordTypeName(12));

    lv_label_set_text(set_pedal1_label, getChordNoteName(0));
    lv_label_ins_text(set_pedal1_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal1_label, LV_LABEL_POS_LAST, getChordTypeName(0));

    lv_label_set_text(set_pedal2_label, getChordNoteName(1));
    lv_label_ins_text(set_pedal2_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal2_label, LV_LABEL_POS_LAST, getChordTypeName(1));

    lv_label_set_text(set_pedal3_label, getChordNoteName(2));
    lv_label_ins_text(set_pedal3_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal3_label, LV_LABEL_POS_LAST, getChordTypeName(2));

    lv_label_set_text(set_pedal4_label, getChordNoteName(3));
    lv_label_ins_text(set_pedal4_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal4_label, LV_LABEL_POS_LAST, getChordTypeName(3));

    lv_label_set_text(set_pedal5_label, getChordNoteName(4));
    lv_label_ins_text(set_pedal5_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal5_label, LV_LABEL_POS_LAST, getChordTypeName(4));


    lv_label_set_text(set_pedal6_label, getChordNoteName(5));
    lv_label_ins_text(set_pedal6_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal6_label, LV_LABEL_POS_LAST, getChordTypeName(5));


    lv_label_set_text(set_pedal7_label, getChordNoteName(6));
    lv_label_ins_text(set_pedal7_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal7_label, LV_LABEL_POS_LAST, getChordTypeName(6));


    lv_label_set_text(set_pedal8_label, getChordNoteName(7));
    lv_label_ins_text(set_pedal8_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal8_label, LV_LABEL_POS_LAST, getChordTypeName(7));


    lv_label_set_text(set_pedal9_label, getChordNoteName(8));
    lv_label_ins_text(set_pedal9_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal9_label, LV_LABEL_POS_LAST, getChordTypeName(8));


    lv_label_set_text(set_pedal10_label, getChordNoteName(9));
    lv_label_ins_text(set_pedal10_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal10_label, LV_LABEL_POS_LAST, getChordTypeName(9));


    lv_label_set_text(set_pedal11_label, getChordNoteName(10));
    lv_label_ins_text(set_pedal11_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal11_label, LV_LABEL_POS_LAST, getChordTypeName(10));


    lv_label_set_text(set_pedal12_label, getChordNoteName(11));
    lv_label_ins_text(set_pedal12_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal12_label, LV_LABEL_POS_LAST, getChordTypeName(11));


    lv_label_set_text(set_pedal13_label, getChordNoteName(12));
    lv_label_ins_text(set_pedal13_label, LV_LABEL_POS_LAST, " ");
    lv_label_ins_text(set_pedal13_label, LV_LABEL_POS_LAST, getChordTypeName(12));

}


void screen_boot() {
    // define container for screen boot
    screen_boot_container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen_boot_container, lv_pct(100), lv_pct(100));
    lv_obj_align(screen_boot_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(screen_boot_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(screen_boot_container, 0, LV_PART_MAIN);

    //boot message
    lv_obj_t * boot_label_style;
    lv_style_init(&boot_label_style);
    lv_style_set_text_font(&boot_label_style, &lv_font_montserrat_48);

    lv_obj_t * mb_bootLabel = lv_label_create(screen_boot_container);
    lv_obj_align(mb_bootLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(mb_bootLabel, "Initializing Stojos MIDI player ...");
    lv_obj_add_style(mb_bootLabel, &boot_label_style, _LV_STYLE_STATE_CMP_SAME);
}

void screen_init() {

    lv_obj_move_background(screen_boot_container);

    /*Create MIDI note velocity slider*/
    mb_velocitySlider = lv_slider_create(lv_scr_act());


    lv_obj_set_width(mb_velocitySlider, 760);
    lv_obj_align(mb_velocitySlider, LV_ALIGN_TOP_MID, 0, 20);
    lv_slider_set_range(mb_velocitySlider, 0, 127);
    lv_slider_set_value(mb_velocitySlider, getVelocity(), LV_ANIM_OFF);
    lv_obj_add_event_cb(mb_velocitySlider, velocity_slider_handler, LV_EVENT_VALUE_CHANGED, NULL);

    /* create screen tab view */
    mb_tabView = lv_tabview_create(lv_scr_act(), LV_DIR_BOTTOM, 50);
    lv_obj_set_height(mb_tabView, 440);
    lv_obj_align(mb_tabView, LV_ALIGN_BOTTOM_MID, 0, 0);


    // define style for tab labels
//    static lv_style_t mb_tabs_style;
//    lv_style_init(&mb_tabs_style);
//    lv_style_set_text_font(&mb_tabs_style, LV_FONT_MONTSERRAT_20);

    /* create Note tab */
    mb_tabNote = lv_tabview_add_tab(mb_tabView, "NOTE");

    /* create Chord tab */
    mb_tabChord = lv_tabview_add_tab(mb_tabView, "CHORD");
    static lv_style_t chordTabStyle;
    lv_style_init(&chordTabStyle);
    lv_style_set_pad_bottom(&chordTabStyle, 0);
    lv_style_set_pad_top(&chordTabStyle, 0);
    lv_obj_add_style(mb_tabChord, &chordTabStyle, 0);


    /* create Program Command tab */
    mb_tabProgCom = lv_tabview_add_tab(mb_tabView, "COMMAND");


    /* set style of screen Tabs */

    // lv_obj_t * tab_btns = lv_tabview_get_tab_btns(mb_tabView);
    // lv_obj_set_style_pad_right(tab_btns, LV_HOR_RES / 2 , 0);

    // /* set logo image */
    // lv_obj_t * logo_img = lv_img_create(tab_btns);
    // LV_IMG_DECLARE(Logo);
    // lv_img_set_src(logo_img, &Logo);
    // lv_obj_align(logo_img, LV_ALIGN_RIGHT_MID, LV_HOR_RES / 2 - 25, 4);

    /* attach the event */
    lv_obj_add_event_cb(mb_tabView, tabview_change_event, LV_EVENT_VALUE_CHANGED, NULL);

    note_screen_create(mb_tabNote);
    chord_screen_create(mb_tabChord);
    progcom_screen_create(mb_tabProgCom);

    // disable tab sliding and animation
    lv_obj_clear_flag(lv_tabview_get_content(mb_tabView), LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(lv_tabview_get_content(mb_tabView), scroll_begin_event, LV_EVENT_SCROLL_BEGIN, NULL);

}

static void note_screen_create(lv_obj_t * parent) {

    // define note label and its style
    lv_style_init(&note_style);
    lv_style_set_text_font(&note_style, &Monserrat_200);
    mb_noteLabel = lv_label_create(mb_tabNote);
    lv_obj_align(mb_noteLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(mb_noteLabel, "");
    lv_obj_add_style(mb_noteLabel, &note_style, _LV_STYLE_STATE_CMP_SAME);

    // note settings text
    mb_noteSettingsText = lv_label_create(mb_tabNote);
    lv_label_set_recolor(mb_noteSettingsText, true);
    lv_obj_set_style_text_line_space(mb_noteSettingsText, 10, LV_PART_MAIN);
    lv_obj_align(mb_noteSettingsText, LV_ALIGN_RIGHT_MID, 0, -20);
    lv_obj_set_style_text_font(mb_noteSettingsText, &lv_font_montserrat_20, LV_PART_MAIN);
    renderNoteSettings();

    // note settings button
    note_settings_button = lv_btn_create(mb_tabNote);
    lv_obj_t * note_settings_btn_label = lv_label_create(note_settings_button);
    lv_label_set_text(note_settings_btn_label, "Settings");
    lv_obj_center(note_settings_btn_label);
    lv_obj_add_event_cb(note_settings_button, note_settings_event, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_text_font(note_settings_btn_label, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align_to(note_settings_button, mb_noteSettingsText, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    note_settings_screen();

}


static  void chord_screen_create(lv_obj_t * parent) {

   //int (*p)[22];
    p = getChordsSetPointer();
    int i = p[0][21];

    // define and render chord settings text
    mb_chordSettingsText = lv_label_create(mb_tabChord);
    lv_label_set_recolor(mb_chordSettingsText, true);
    lv_obj_set_style_text_line_space(mb_chordSettingsText, 10, LV_PART_MAIN);
    lv_obj_set_style_text_font(mb_chordSettingsText, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(mb_chordSettingsText, LV_ALIGN_RIGHT_MID, 0, -20);

    renderChordSettings();

    // chord settings button
    chord_settings_button = lv_btn_create(mb_tabChord);
    lv_obj_t * chord_settings_btn_label = lv_label_create(chord_settings_button);
    lv_label_set_text(chord_settings_btn_label, "Settings");
    lv_obj_center(chord_settings_btn_label);
    lv_obj_add_event_cb(chord_settings_button, chord_settings_event, LV_EVENT_CLICKED, NULL);
    lv_obj_align_to(chord_settings_button, mb_chordSettingsText, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_text_font(chord_settings_button, &lv_font_montserrat_20, LV_PART_MAIN);

    lv_style_init(&brown_pedal_button_style);

    lv_style_set_radius(&brown_pedal_button_style, 0);

    lv_style_set_bg_opa(&brown_pedal_button_style, LV_OPA_100);
    lv_style_set_bg_color(&brown_pedal_button_style, lv_palette_main(LV_PALETTE_BROWN));
    lv_style_set_bg_grad_color(&brown_pedal_button_style, lv_palette_darken(LV_PALETTE_BROWN, 2));
    lv_style_set_bg_grad_dir(&brown_pedal_button_style, LV_GRAD_DIR_VER);

    lv_style_set_border_opa(&brown_pedal_button_style, LV_OPA_40);
    lv_style_set_border_width(&brown_pedal_button_style, 1);
    lv_style_set_border_color(&brown_pedal_button_style, lv_palette_main(LV_PALETTE_GREY));

    lv_style_set_text_color(&brown_pedal_button_style, lv_color_white());
    lv_style_set_pad_all(&brown_pedal_button_style, 0);
    lv_style_set_text_font(&brown_pedal_button_style, &lv_font_montserrat_20);

    lv_style_init(&black_pedal_button_style);

    lv_style_set_radius(&black_pedal_button_style, 0);

    lv_style_set_bg_opa(&black_pedal_button_style, LV_OPA_100);
    lv_style_set_bg_color(&black_pedal_button_style, lv_palette_darken(LV_PALETTE_BROWN, 4));
    lv_style_set_bg_grad_color(&black_pedal_button_style, lv_palette_darken(LV_PALETTE_BROWN, 5));
    lv_style_set_bg_grad_dir(&black_pedal_button_style, LV_GRAD_DIR_VER);

    lv_style_set_border_opa(&black_pedal_button_style, LV_OPA_40);
    lv_style_set_border_width(&black_pedal_button_style, 1);
    lv_style_set_border_color(&black_pedal_button_style, lv_palette_main(LV_PALETTE_GREY));

    lv_style_set_text_color(&black_pedal_button_style, lv_color_white());
    lv_style_set_pad_all(&black_pedal_button_style, 0);
    lv_style_set_text_font(&black_pedal_button_style, &lv_font_montserrat_20);

    int y_pedal_offset = -165;

    // PEDAL 1
    lv_obj_t * pedal1_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal1_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal1_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal1_btn, 200, 30);
    lv_obj_align(pedal1_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset);

    pedal1_label = lv_label_create(pedal1_btn);
    lv_obj_align(pedal1_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 3
    lv_obj_t * pedal3_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal3_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal3_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal3_btn, 200, 30);
    lv_obj_align(pedal3_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+50);

    pedal3_label = lv_label_create(pedal3_btn);
    lv_obj_align(pedal3_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 5
    lv_obj_t * pedal5_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal5_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal5_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal5_btn, 200, 30);
    lv_obj_align(pedal5_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+100);

    pedal5_label = lv_label_create(pedal5_btn);
    lv_obj_align(pedal5_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 6
    lv_obj_t * pedal6_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal6_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal6_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal6_btn, 200, 30);
    lv_obj_align(pedal6_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+135);

    pedal6_label = lv_label_create(pedal6_btn);
    lv_obj_align(pedal6_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 8
    lv_obj_t * pedal8_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal8_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal8_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal8_btn, 200, 30);
    lv_obj_align(pedal8_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+185);

    pedal8_label = lv_label_create(pedal8_btn);
    lv_obj_align(pedal8_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 10
    lv_obj_t * pedal10_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal10_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal10_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal10_btn, 200, 30);
    lv_obj_align(pedal10_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+235);

    pedal10_label = lv_label_create(pedal10_btn);
    lv_obj_align(pedal10_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 12
    lv_obj_t * pedal12_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal12_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal12_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal12_btn, 200, 30);
    lv_obj_align(pedal12_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+285);

    pedal12_label = lv_label_create(pedal12_btn);
    lv_obj_align(pedal12_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 13
    lv_obj_t * pedal13_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal13_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal13_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(pedal13_btn, 200, 30);
    lv_obj_align(pedal13_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+320);

    pedal13_label = lv_label_create(pedal13_btn);
    lv_obj_align(pedal13_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 2
    lv_obj_t * pedal2_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal2_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal2_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(pedal2_btn, 150, 30);
    lv_obj_align(pedal2_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+25);

    pedal2_label = lv_label_create(pedal2_btn);
    lv_obj_align(pedal2_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 4
    lv_obj_t * pedal4_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal4_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal4_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(pedal4_btn, 150, 30);
    lv_obj_align(pedal4_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+75);

    pedal4_label = lv_label_create(pedal4_btn);
    lv_obj_align(pedal4_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 7
    lv_obj_t * pedal7_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal7_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal7_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(pedal7_btn, 150, 30);
    lv_obj_align(pedal7_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+160);

    pedal7_label = lv_label_create(pedal7_btn);
    lv_obj_align(pedal7_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 9
    lv_obj_t * pedal9_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal9_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal9_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(pedal9_btn, 150, 30);
    lv_obj_align(pedal9_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+210);

    pedal9_label = lv_label_create(pedal9_btn);
    lv_obj_align(pedal9_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 11
    lv_obj_t * pedal11_btn = lv_btn_create(mb_tabChord);
    lv_obj_remove_style_all(pedal11_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(pedal11_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(pedal11_btn, 150, 30);
    lv_obj_align(pedal11_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+260);

    pedal11_label = lv_label_create(pedal11_btn);
    lv_obj_align(pedal11_label, LV_ALIGN_LEFT_MID, 5, 0);

    chord_settings_screen();

    // render chord set buttons

    //define style for selected chord set button
    lv_style_init(&chord_set_button_style_selected);
    lv_style_set_radius(&chord_set_button_style_selected, 0);
    lv_style_set_bg_opa(&chord_set_button_style_selected, LV_OPA_100);
    lv_style_set_bg_color(&chord_set_button_style_selected, lv_color_white());
    lv_style_set_border_color(&chord_set_button_style_selected, lv_color_white());
    lv_style_set_border_width(&chord_set_button_style_selected, 1);

    lv_style_set_text_color(&chord_set_button_style_selected, lv_color_black());
    lv_style_set_pad_all(&chord_set_button_style_selected, 0);

    //define style for not selected chord set button
    lv_style_init(&chord_set_button_style);
    lv_style_set_radius(&chord_set_button_style, 0);
    lv_style_set_bg_opa(&chord_set_button_style, LV_OPA_100);
    lv_style_set_bg_color(&chord_set_button_style, lv_color_white());
    lv_style_set_border_color(&chord_set_button_style, lv_color_white());
    lv_style_set_border_width(&chord_set_button_style, 1);

    lv_style_set_text_color(&chord_set_button_style, lv_color_black());
    lv_style_set_pad_all(&chord_set_button_style, 0);


    //set button 1
    chord_set_btn1 = lv_btn_create(mb_tabChord);
    lv_obj_add_flag(chord_set_btn1, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_state(chord_set_btn1, LV_STATE_CHECKED);
    lv_obj_remove_style_all(chord_set_btn1);
    lv_obj_add_style(chord_set_btn1, &chord_set_button_style_selected, LV_STATE_CHECKED);
    //lv_obj_add_style(chord_set_btn1, &chord_set_button_style, LV_STATE_DISABLED);
    lv_obj_set_size(chord_set_btn1, 89, 50);
    lv_obj_add_event_cb(chord_set_btn1, chord_set_btn1_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(chord_set_btn1, LV_ALIGN_BOTTOM_MID, -89, 0);

    chord_set_btn1_label = lv_label_create(chord_set_btn1);
    lv_label_set_text(chord_set_btn1_label, "Set 1");
    lv_obj_center(chord_set_btn1_label);

    //set button 2
    chord_set_btn2 = lv_btn_create(mb_tabChord);
    lv_obj_add_flag(chord_set_btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_remove_style_all(chord_set_btn2);
    lv_obj_add_style(chord_set_btn2, &chord_set_button_style_selected, LV_STATE_CHECKED);
    //lv_obj_add_style(chord_set_btn2, &chord_set_button_style, LV_STATE_DISABLED);
    lv_obj_set_size(chord_set_btn2, 89, 50);
    lv_obj_add_event_cb(chord_set_btn2, chord_set_btn2_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(chord_set_btn2, LV_ALIGN_BOTTOM_MID, 0, 0);


    chord_set_btn2_label = lv_label_create(chord_set_btn2);
    lv_label_set_text(chord_set_btn2_label, "Set 2");
    lv_obj_center(chord_set_btn2_label);

    //set button 3
    chord_set_btn3 = lv_btn_create(mb_tabChord);
    lv_obj_add_flag(chord_set_btn3, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_remove_style_all(chord_set_btn3);
    lv_obj_add_style(chord_set_btn3, &chord_set_button_style_selected, LV_STATE_CHECKED);
    //lv_obj_add_style(chord_set_btn3, &chord_set_button_style, LV_STATE_DISABLED);
    lv_obj_set_size(chord_set_btn3, 89, 50);
    lv_obj_add_event_cb(chord_set_btn3, chord_set_btn3_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(chord_set_btn3, LV_ALIGN_BOTTOM_MID, 89, 0);

    chord_set_btn3_label = lv_label_create(chord_set_btn3);
    lv_label_set_text(chord_set_btn3_label, "Set 3");
    lv_obj_center(chord_set_btn3_label);


    // define labels for chord name that is played
    lv_style_init(&note_style);
    lv_style_set_text_font(&note_style, &Monserrat_150);
    mb_chordNoteLabel = lv_label_create(mb_tabChord);
    lv_obj_align(mb_chordNoteLabel, LV_ALIGN_CENTER, 0, -50);
    lv_label_set_text(mb_chordNoteLabel, "");
    lv_obj_add_style(mb_chordNoteLabel, &note_style, _LV_STYLE_STATE_CMP_SAME);

    lv_style_init(&chord_style);
    //lv_style_set_text_font(&chord_style, &Monserrat_100);
    lv_style_set_text_font(&chord_style, &lv_font_montserrat_48);
    mb_chordTypeLabel = lv_label_create(mb_tabChord);
    lv_obj_align(mb_chordTypeLabel, LV_ALIGN_CENTER, 0, 50);
    lv_label_set_text(mb_chordTypeLabel, "");
    lv_obj_add_style(mb_chordTypeLabel, &chord_style, _LV_STYLE_STATE_CMP_SAME);

}

static  void progcom_screen_create(lv_obj_t * parent) {

    // define program command tab left container
    lv_obj_t * prog_com_tab_left_container = lv_obj_create(mb_tabProgCom);
    lv_obj_set_size(prog_com_tab_left_container, lv_pct(62), lv_pct(100));

    // define program command tab right container
    lv_obj_t * prog_com_tab_right_container = lv_obj_create(mb_tabProgCom);
    lv_obj_set_size(prog_com_tab_right_container, lv_pct(35), lv_pct(100));
    lv_obj_align(prog_com_tab_right_container, LV_ALIGN_TOP_RIGHT, 0, 0);

    // define chord label and its style
    mb_progComLabel = lv_label_create(prog_com_tab_left_container);
    lv_obj_align(mb_progComLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(mb_progComLabel, "");
    lv_obj_add_style(mb_progComLabel, &note_style, _LV_STYLE_STATE_CMP_SAME);

}

void note_settings_screen() {
    // define container for note setting configuration
    note_tab_conf_container = lv_obj_create(lv_scr_act());
    lv_obj_move_background(note_tab_conf_container);
    lv_obj_set_size(note_tab_conf_container, lv_pct(100), lv_pct(80));
    lv_obj_align(note_tab_conf_container, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_style_bg_color(note_tab_conf_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_pad_top(note_tab_conf_container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(note_tab_conf_container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(note_tab_conf_container, 0, LV_PART_MAIN);

    //midi channel block
    lv_obj_t * channel_note_value_label_container = lv_obj_create(note_tab_conf_container);
    lv_obj_align(channel_note_value_label_container, LV_ALIGN_TOP_MID, -100, 25);
    //lv_obj_set_style_bg_color(channel_note_value_label_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_content_width(channel_note_value_label_container, 140);
    lv_obj_set_content_height(channel_note_value_label_container, 100);
    lv_obj_set_style_pad_all(channel_note_value_label_container, 0, LV_PART_MAIN);


    channel_note_value_label = lv_label_create(channel_note_value_label_container);
    lv_label_set_text_fmt(channel_note_value_label, "Channel %d", getChannelNote());
    lv_obj_align(channel_note_value_label, LV_ALIGN_CENTER, 0, 0);

    channel_note_up_button = lv_btn_create(channel_note_value_label_container);
    lv_obj_set_content_width(channel_note_up_button, 140);
    lv_obj_set_content_height(channel_note_up_button, 20);
    lv_obj_align(channel_note_up_button, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(channel_note_up_button, channel_note_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * channel_up_label = lv_label_create(channel_note_up_button);
    lv_label_set_text(channel_up_label, LV_SYMBOL_UP);
    lv_obj_align(channel_up_label, LV_ALIGN_CENTER, 0, 0);

    channel_note_down_button = lv_btn_create(channel_note_value_label_container);
    lv_obj_set_content_width(channel_note_down_button, 140);
    lv_obj_set_content_height(channel_note_down_button, 20);
    lv_obj_align(channel_note_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(channel_note_down_button, channel_note_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * channel_down_label = lv_label_create(channel_note_down_button);
    lv_label_set_text(channel_down_label, LV_SYMBOL_DOWN);
    lv_obj_align(channel_down_label, LV_ALIGN_CENTER, 0, 0);

    //octave block
    lv_obj_t * octave_value_label_container = lv_obj_create(note_tab_conf_container);
    lv_obj_align_to(octave_value_label_container, channel_note_value_label_container, LV_ALIGN_OUT_RIGHT_TOP, 40, 0);
    //lv_obj_set_style_bg_color(octave_value_label_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_content_width(octave_value_label_container, 140);
    lv_obj_set_content_height(octave_value_label_container, 100);
    lv_obj_set_style_pad_all(octave_value_label_container, 0, LV_PART_MAIN);

    octave_note_value_label = lv_label_create(octave_value_label_container);
    lv_label_set_text_fmt(octave_note_value_label, "Octave %d", getOctave());
    lv_obj_align(octave_note_value_label, LV_ALIGN_CENTER, 0, 0);

    octave_note_up_button = lv_btn_create(octave_value_label_container);
    lv_obj_set_content_width(octave_note_up_button, 140);
    lv_obj_set_content_height(octave_note_up_button, 20);
    lv_obj_align(octave_note_up_button, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(octave_note_up_button, octave_note_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * octave_up_label = lv_label_create(octave_note_up_button);
    lv_label_set_text(octave_up_label, LV_SYMBOL_UP);
    lv_obj_align(octave_up_label, LV_ALIGN_CENTER, 0, 0);

    octave_note_down_button = lv_btn_create(octave_value_label_container);
    lv_obj_set_content_width(octave_note_down_button, 140);
    lv_obj_set_content_height(octave_note_down_button, 20);
    lv_obj_align(octave_note_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(octave_note_down_button, octave_note_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * octave_down_label = lv_label_create(octave_note_down_button);
    lv_label_set_text(octave_down_label, LV_SYMBOL_DOWN);
    lv_obj_align(octave_down_label, LV_ALIGN_CENTER, 0, 0);

    // note off block
    mb_noteOffButton = lv_btn_create(note_tab_conf_container);
    lv_obj_align_to(mb_noteOffButton, channel_note_value_label_container, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 40);
    lv_obj_set_content_width(mb_noteOffButton, 140);
    lv_obj_set_content_height(mb_noteOffButton, 100);
    lv_obj_add_flag(mb_noteOffButton, LV_OBJ_FLAG_CHECKABLE);

    if (!getNoteOff()) lv_obj_add_state(mb_noteOffButton, LV_STATE_CHECKED);

    lv_obj_add_event_cb(mb_noteOffButton, note_off_event, LV_EVENT_CLICKED, NULL);

    mb_noteOffLabel = lv_label_create(mb_noteOffButton);
    lv_label_set_text_fmt(mb_noteOffLabel, "Note Off\n%s", (getNoteOff()) ? "Enabled" : "Disabled");
    lv_obj_align(mb_noteOffLabel, LV_ALIGN_CENTER, 0, 0);

    // double note block
    mb_doubleNoteButton = lv_btn_create(note_tab_conf_container);
    lv_obj_align_to(mb_doubleNoteButton, mb_noteOffButton, LV_ALIGN_OUT_RIGHT_TOP, 40, 0);
    lv_obj_set_content_width(mb_doubleNoteButton, 140);
    lv_obj_set_content_height(mb_doubleNoteButton, 100);
    lv_obj_add_flag(mb_doubleNoteButton, LV_OBJ_FLAG_CHECKABLE);

    if (!getDoubleNote()) lv_obj_add_state(mb_doubleNoteButton, LV_STATE_CHECKED);

    lv_obj_add_event_cb(mb_doubleNoteButton, double_note_event, LV_EVENT_CLICKED, NULL);

    mb_doubleNoteLabel = lv_label_create(mb_doubleNoteButton);
    lv_label_set_text_fmt(mb_doubleNoteLabel, "Double\nNote\n%s", (getDoubleNote()) ? "Enabled" : "Disabled");
    lv_obj_align(mb_doubleNoteLabel, LV_ALIGN_CENTER, 0, 0);

}

void chord_settings_screen() {

    // define container for chord setting configuration
    chord_tab_conf_container = lv_obj_create(lv_scr_act());
    lv_obj_move_background(chord_tab_conf_container);
    lv_obj_set_size(chord_tab_conf_container, lv_pct(70), lv_pct(80));
    lv_obj_align(chord_tab_conf_container, LV_ALIGN_RIGHT_MID, 0, -2);
    lv_obj_set_style_bg_color(chord_tab_conf_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_pad_top(chord_tab_conf_container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(chord_tab_conf_container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(chord_tab_conf_container, 0, LV_PART_MAIN);

    //midi channel block
    lv_obj_t * channel_chord_value_label_container = lv_obj_create(chord_tab_conf_container);
    lv_obj_align(channel_chord_value_label_container, LV_ALIGN_TOP_LEFT, 0, 25);
    lv_obj_set_content_width(channel_chord_value_label_container, 150);
    lv_obj_set_content_height(channel_chord_value_label_container, 100);
    lv_obj_set_style_pad_all(channel_chord_value_label_container, 0, LV_PART_MAIN);


    channel_chord_value_label = lv_label_create(channel_chord_value_label_container);
    lv_label_set_text_fmt(channel_chord_value_label, "Channel %d", getChannelChord());
    lv_obj_align(channel_chord_value_label, LV_ALIGN_CENTER, 0, 0);

    channel_chord_up_button = lv_btn_create(channel_chord_value_label_container);
    lv_obj_set_content_width(channel_chord_up_button, 150);
    lv_obj_set_content_height(channel_chord_up_button, 20);
    lv_obj_align(channel_chord_up_button, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(channel_chord_up_button, channel_chord_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * channel_chord_up_label = lv_label_create(channel_chord_up_button);
    lv_label_set_text(channel_chord_up_label, LV_SYMBOL_UP);
    lv_obj_align(channel_chord_up_label, LV_ALIGN_CENTER, 0, 0);

    channel_chord_down_button = lv_btn_create(channel_chord_value_label_container);
    lv_obj_set_content_width(channel_chord_down_button, 150);
    lv_obj_set_content_height(channel_chord_down_button, 20);
    lv_obj_align(channel_chord_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(channel_chord_down_button, channel_chord_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * channel_chord_down_label = lv_label_create(channel_chord_down_button);
    lv_label_set_text(channel_chord_down_label, LV_SYMBOL_DOWN);
    lv_obj_align(channel_chord_down_label, LV_ALIGN_CENTER, 0, 0);

    //octave block
    lv_obj_t * octave_chord_value_label_container = lv_obj_create(chord_tab_conf_container);
    lv_obj_align_to(octave_chord_value_label_container, channel_chord_value_label_container, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_content_width(octave_chord_value_label_container, 150);
    lv_obj_set_content_height(octave_chord_value_label_container, 100);
    lv_obj_set_style_pad_all(octave_chord_value_label_container, 0, LV_PART_MAIN);

    octave_chord_value_label = lv_label_create(octave_chord_value_label_container);
    lv_label_set_text_fmt(octave_chord_value_label, "Octave %d", getChordsSetOctave(getChordSet()));
    lv_obj_align(octave_chord_value_label, LV_ALIGN_CENTER, 0, 0);

    octave_chord_up_button = lv_btn_create(octave_chord_value_label_container);
    lv_obj_set_content_width(octave_chord_up_button, 150);
    lv_obj_set_content_height(octave_chord_up_button, 20);
    lv_obj_align(octave_chord_up_button, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(octave_chord_up_button, octave_chord_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * octave_chord_up_label = lv_label_create(octave_chord_up_button);
    lv_label_set_text(octave_chord_up_label, LV_SYMBOL_UP);
    lv_obj_align(octave_chord_up_label, LV_ALIGN_CENTER, 0, 0);

    octave_chord_down_button = lv_btn_create(octave_chord_value_label_container);
    lv_obj_set_content_width(octave_chord_down_button, 150);
    lv_obj_set_content_height(octave_chord_down_button, 20);
    lv_obj_align(octave_chord_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(octave_chord_down_button, octave_chord_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * octave_chord_down_label = lv_label_create(octave_chord_down_button);
    lv_label_set_text(octave_chord_down_label, LV_SYMBOL_DOWN);
    lv_obj_align(octave_chord_down_label, LV_ALIGN_CENTER, 0, 0);

    //root note block
    lv_obj_t * root_note_value_label_container = lv_obj_create(chord_tab_conf_container);
    lv_obj_align_to(root_note_value_label_container, channel_chord_value_label_container, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_obj_set_content_width(root_note_value_label_container, 150);
    lv_obj_set_content_height(root_note_value_label_container, 100);
    lv_obj_set_style_pad_all(root_note_value_label_container, 0, LV_PART_MAIN);

    root_note_chord_value_label = lv_label_create(root_note_value_label_container);
    lv_label_set_text_fmt(root_note_chord_value_label, "Root Note %s", getChordNoteName(getChordsSetRootNote(getChordSet)));
    lv_obj_align(root_note_chord_value_label, LV_ALIGN_CENTER, 0, 0);

    root_note_up_button = lv_btn_create(root_note_value_label_container);
    lv_obj_set_content_width(root_note_up_button, 150);
    lv_obj_set_content_height(root_note_up_button, 20);
    lv_obj_align(root_note_up_button, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(root_note_up_button, root_note_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * root_note_up_label = lv_label_create(root_note_up_button);
    lv_label_set_text(root_note_up_label, LV_SYMBOL_UP);
    lv_obj_align(root_note_up_label, LV_ALIGN_CENTER, 0, 0);

    root_note_down_button = lv_btn_create(root_note_value_label_container);
    lv_obj_set_content_width(root_note_down_button, 150);
    lv_obj_set_content_height(root_note_down_button, 20);
    lv_obj_align(root_note_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(root_note_down_button, root_note_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * root_note_down_label = lv_label_create(root_note_down_button);
    lv_label_set_text(root_note_down_label, LV_SYMBOL_DOWN);
    lv_obj_align(root_note_down_label, LV_ALIGN_CENTER, 0, 0);

    //harmony mode block
    lv_obj_t * harmony_mode_value_label_container = lv_obj_create(chord_tab_conf_container);
    lv_obj_align_to(harmony_mode_value_label_container, root_note_value_label_container, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_obj_set_content_width(harmony_mode_value_label_container, 150);
    lv_obj_set_content_height(harmony_mode_value_label_container, 100);
    lv_obj_set_style_pad_all(harmony_mode_value_label_container, 0, LV_PART_MAIN);

    harmony_mode_value_label = lv_label_create(harmony_mode_value_label_container);
    lv_label_set_text(harmony_mode_value_label, modeNames[getChordsSetMode(getChordSet)]);
    lv_obj_align(harmony_mode_value_label, LV_ALIGN_CENTER, 0, 0);

    harmony_mode_up_button = lv_btn_create(harmony_mode_value_label_container);
    lv_obj_set_content_width( harmony_mode_up_button, 150);
    lv_obj_set_content_height( harmony_mode_up_button, 20);
    lv_obj_align( harmony_mode_up_button, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb( harmony_mode_up_button, harmony_mode_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * harmony_mode_up_label = lv_label_create( harmony_mode_up_button);
    lv_label_set_text(harmony_mode_up_label, LV_SYMBOL_UP);
    lv_obj_align(harmony_mode_up_label, LV_ALIGN_CENTER, 0, 0);

    harmony_mode_down_button = lv_btn_create(harmony_mode_value_label_container);
    lv_obj_set_content_width(harmony_mode_down_button, 150);
    lv_obj_set_content_height(harmony_mode_down_button, 20);
    lv_obj_align(harmony_mode_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(harmony_mode_down_button, harmony_mode_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * harmony_mode_down_label = lv_label_create(harmony_mode_down_button);
    lv_label_set_text(harmony_mode_down_label , LV_SYMBOL_DOWN);
    lv_obj_align(harmony_mode_down_label , LV_ALIGN_CENTER, 0, 0);

    // advanced chord settings button
    chord_settings_advanced_button = lv_btn_create(chord_tab_conf_container);
    lv_obj_t * chord_settings_advanced_btn_label = lv_label_create(chord_settings_advanced_button);
    lv_label_set_text(chord_settings_advanced_btn_label, "More");
    lv_obj_center(chord_settings_advanced_btn_label);
    lv_obj_add_event_cb(chord_settings_advanced_button, chord_settings_advanced_event, LV_EVENT_CLICKED, NULL);
    lv_obj_align(chord_settings_advanced_button, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_font(chord_settings_advanced_button, &lv_font_montserrat_20, LV_PART_MAIN);

    chord_settings_advanced_screen();

}

void chord_settings_advanced_screen() {

    // define container for chord setting advanced configuration
    chord_tab_advanced_conf_container = lv_obj_create(lv_scr_act());
    lv_obj_move_background(chord_tab_advanced_conf_container);
    lv_obj_set_size(chord_tab_advanced_conf_container, lv_pct(100), lv_pct(80));
    lv_obj_align(chord_tab_advanced_conf_container, LV_ALIGN_RIGHT_MID, 0, -2);
    lv_obj_set_style_bg_color(chord_tab_advanced_conf_container, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_pad_top(chord_tab_advanced_conf_container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(chord_tab_advanced_conf_container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(chord_tab_advanced_conf_container, 0, LV_PART_MAIN);

    int y_pedal_offset = -165;

    // PEDAL 1
    set_pedal1_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal1_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal1_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal1_btn, 200, 30);
    lv_obj_align(set_pedal1_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset);
    lv_obj_add_event_cb(set_pedal1_btn, pedal1_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal1_label = lv_label_create(set_pedal1_btn);
    lv_obj_align(set_pedal1_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 3
    set_pedal3_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal3_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal3_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal3_btn, 200, 30);
    lv_obj_align(set_pedal3_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+50);
    lv_obj_add_event_cb(set_pedal3_btn, pedal3_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal3_label = lv_label_create(set_pedal3_btn);
    lv_obj_align(set_pedal3_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 5
    set_pedal5_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal5_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal5_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal5_btn, 200, 30);
    lv_obj_align(set_pedal5_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+100);
    lv_obj_add_event_cb(set_pedal5_btn, pedal5_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal5_label = lv_label_create(set_pedal5_btn);
    lv_obj_align(set_pedal5_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 6
    set_pedal6_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal6_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal6_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal6_btn, 200, 30);
    lv_obj_align(set_pedal6_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+135);
    lv_obj_add_event_cb(set_pedal6_btn, pedal6_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal6_label = lv_label_create(set_pedal6_btn);
    lv_obj_align(set_pedal6_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 8
    set_pedal8_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal8_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal8_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal8_btn, 200, 30);
    lv_obj_align(set_pedal8_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+185);
    lv_obj_add_event_cb(set_pedal8_btn, pedal8_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal8_label = lv_label_create(set_pedal8_btn);
    lv_obj_align(set_pedal8_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 10
    set_pedal10_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal10_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal10_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal10_btn, 200, 30);
    lv_obj_align(set_pedal10_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+235);
    lv_obj_add_event_cb(set_pedal10_btn, pedal10_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal10_label = lv_label_create(set_pedal10_btn);
    lv_obj_align(set_pedal10_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 12
    set_pedal12_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal12_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal12_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal12_btn, 200, 30);
    lv_obj_align(set_pedal12_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+285);
    lv_obj_add_event_cb(set_pedal12_btn, pedal12_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal12_label = lv_label_create(set_pedal12_btn);
    lv_obj_align(set_pedal12_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 13
    set_pedal13_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal13_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal13_btn, &brown_pedal_button_style, 0);
    lv_obj_set_size(set_pedal13_btn, 200, 30);
    lv_obj_align(set_pedal13_btn, LV_ALIGN_LEFT_MID, 0, y_pedal_offset+320);
    lv_obj_add_event_cb(set_pedal13_btn, pedal13_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal13_label = lv_label_create(set_pedal13_btn);
    lv_obj_align(set_pedal13_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 2
    set_pedal2_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal2_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal2_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(set_pedal2_btn, 150, 30);
    lv_obj_align(set_pedal2_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+25);
    lv_obj_add_event_cb(set_pedal2_btn, pedal2_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal2_label = lv_label_create(set_pedal2_btn);
    lv_obj_align(set_pedal2_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 4
    set_pedal4_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal4_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal4_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(set_pedal4_btn, 150, 30);
    lv_obj_align(set_pedal4_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+75);
    lv_obj_add_event_cb(set_pedal4_btn, pedal4_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal4_label = lv_label_create(set_pedal4_btn);
    lv_obj_align(set_pedal4_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 7
    set_pedal7_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal7_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal7_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(set_pedal7_btn, 150, 30);
    lv_obj_align(set_pedal7_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+160);
    lv_obj_add_event_cb(set_pedal7_btn, pedal7_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal7_label = lv_label_create(set_pedal7_btn);
    lv_obj_align(set_pedal7_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 9
    set_pedal9_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal9_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal9_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(set_pedal9_btn, 150, 30);
    lv_obj_align(set_pedal9_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+210);
    lv_obj_add_event_cb(set_pedal9_btn, pedal9_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal9_label = lv_label_create(set_pedal9_btn);
    lv_obj_align(pedal9_label, LV_ALIGN_LEFT_MID, 5, 0);

    // PEDAL 11
    set_pedal11_btn = lv_btn_create(chord_tab_advanced_conf_container);
    lv_obj_remove_style_all(set_pedal11_btn);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(set_pedal11_btn, &black_pedal_button_style, 0);
    lv_obj_set_size(set_pedal11_btn, 150, 30);
    lv_obj_align(set_pedal11_btn, LV_ALIGN_LEFT_MID, 50, y_pedal_offset+260);
    lv_obj_add_event_cb(set_pedal11_btn, pedal11_btn_event, LV_EVENT_CLICKED, NULL);

    set_pedal11_label = lv_label_create(set_pedal11_btn);
    lv_obj_align(set_pedal11_label, LV_ALIGN_LEFT_MID, 5, 0);

    renderChords();

    chordAdvancedConfigLabel = lv_label_create(chord_tab_advanced_conf_container);
    lv_label_set_recolor(chordAdvancedConfigLabel, true);
    lv_obj_align(chordAdvancedConfigLabel, LV_ALIGN_TOP_MID, 110, 0);
    lv_label_set_text(chordAdvancedConfigLabel, "Click on pedal to set its \ncustom chord.");
    lv_obj_set_style_text_align(chordAdvancedConfigLabel, LV_TEXT_ALIGN_CENTER, 0);

    //chord type block
    chord_type_container = lv_obj_create(chord_tab_advanced_conf_container);
    lv_obj_align(chord_type_container, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_content_width(chord_type_container, 150);
    lv_obj_set_content_height(chord_type_container, 290);
    lv_obj_set_style_pad_all(chord_type_container, 0, LV_PART_MAIN);

    lv_obj_t * chord_type_title_label = lv_label_create(chord_type_container);
    lv_label_set_recolor(chord_type_title_label, true);
    lv_label_set_text(chord_type_title_label, "$487f4a Chord Type$");
    lv_obj_align(chord_type_title_label, LV_ALIGN_TOP_MID, 0, 5);

    chord_type_label = lv_label_create(chord_type_container);
    lv_label_set_recolor(chord_type_label, true);
    lv_label_set_text(chord_type_label, "");
    lv_obj_align(chord_type_label, LV_ALIGN_CENTER, 0, 20);

    lv_obj_t * chord_type_up_button = lv_btn_create(chord_type_container);
    lv_obj_set_content_width(chord_type_up_button, 150);
    lv_obj_set_content_height(chord_type_up_button, 20);
    lv_obj_align(chord_type_up_button, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_add_event_cb(chord_type_up_button, chord_type_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * chord_type_up_label = lv_label_create(chord_type_up_button);
    lv_label_set_text(chord_type_up_label, LV_SYMBOL_UP);
    lv_obj_align(chord_type_up_label, LV_ALIGN_CENTER, 0, 0);

    chord_type_down_button = lv_btn_create(chord_type_container);
    lv_obj_set_content_width(chord_type_down_button, 150);
    lv_obj_set_content_height(chord_type_down_button, 20);
    lv_obj_align(chord_type_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(chord_type_down_button, chord_type_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * chord_type_down_label = lv_label_create(chord_type_down_button);
    lv_label_set_text(chord_type_down_label, LV_SYMBOL_DOWN);
    lv_obj_align(chord_type_down_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(chord_type_container, LV_OBJ_FLAG_HIDDEN);

    //chord variant block
    chord_variant_container = lv_obj_create(chord_tab_advanced_conf_container);
    lv_obj_align(chord_variant_container, LV_ALIGN_CENTER, 220, 10);
    lv_obj_set_content_width(chord_variant_container, 150);
    lv_obj_set_content_height(chord_variant_container, 290);
    lv_obj_set_style_pad_all(chord_variant_container, 0, LV_PART_MAIN);

    lv_obj_t * chord_variant_title_label = lv_label_create(chord_variant_container);
    lv_label_set_recolor(chord_variant_title_label, true);
    lv_label_set_recolor(chord_variant_title_label, true);
    lv_label_set_text(chord_variant_title_label, "$487f4a Chord Variant$");
    lv_obj_align(chord_variant_title_label, LV_ALIGN_TOP_MID, 0, 5);

    chord_variant_label = lv_label_create(chord_variant_container);
    lv_label_set_recolor(chord_variant_label, true);
    lv_label_set_text(chord_variant_label, "");
    lv_obj_align(chord_variant_label, LV_ALIGN_CENTER, 0, 20);

    lv_obj_t * chord_variant_up_button = lv_btn_create(chord_variant_container);
    lv_obj_set_content_width(chord_variant_up_button, 150);
    lv_obj_set_content_height(chord_variant_up_button, 20);
    lv_obj_align(chord_variant_up_button, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_add_event_cb(chord_variant_up_button, chord_variant_up_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * chord_variant_up_label = lv_label_create(chord_variant_up_button);
    lv_label_set_text(chord_variant_up_label, LV_SYMBOL_UP);
    lv_obj_align(chord_variant_up_label, LV_ALIGN_CENTER, 0, 0);

    chord_variant_down_button = lv_btn_create(chord_variant_container);
    lv_obj_set_content_width(chord_variant_down_button, 150);
    lv_obj_set_content_height(chord_variant_down_button, 20);
    lv_obj_align(chord_variant_down_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(chord_variant_down_button, chord_variant_down_event, LV_EVENT_CLICKED, NULL);

    lv_obj_t * chord_variant_down_label = lv_label_create(chord_variant_down_button);
    lv_label_set_text(chord_variant_down_label, LV_SYMBOL_DOWN);
    lv_obj_align(chord_variant_down_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(chord_variant_container, LV_OBJ_FLAG_HIDDEN);

}

char * getChordTypeName(int tmpIndex) {

    p = getChordsSetPointer();

    if (p[tmpIndex][21] == 0) {
        return "";
    } else {
        int charType = (p[tmpIndex][21]-p[tmpIndex][21]%10)/10;
        int charVariant = p[tmpIndex][21]%10-1;
        return screenChordTypeNames[charType][charVariant];
    }
}

char * getChordNoteName(int tmpIndex) {

    switch (tmpIndex) {
        case 0:
            return "C";
            break;
        case 1:
            return "C#";
            break;
        case 2:
            return "D";
            break;
        case 3:
            return "D#";
            break;
        case 4:
            return "E";
            break;
        case 5:
            return "F";
            break;
        case 6:
            return "F#";
            break;
        case 7:
            return "G";
            break;
        case 8:
            return "G#";
            break;
        case 9:
            return "A";
            break;
        case 10:
            return "A#";
            break;
        case 11:
            return "B";
            break;
        case 12:
            return "C";
            break;
        default :
            return "";
            break;

    }
}
