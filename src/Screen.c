#include <Screen.h>

#include <SettingsFacade.h>

#include <inttypes.h>


static lv_obj_t * mb_playContainer;

static lv_obj_t * mb_tabView;
static lv_obj_t * mb_tabNote;
static lv_obj_t * mb_tabChord;
static lv_obj_t * mb_velocitySlider;

static lv_style_t note_style;

static lv_obj_t * mb_noteLabel;
static lv_obj_t * mb_chordLabel;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void note_screen_create(lv_obj_t * parent);
static void chord_screen_create(lv_obj_t * parent);

LV_FONT_DECLARE(Monserrat_200);

static void velocity_slider_handler(lv_event_t * event)
{

    lv_obj_t * slider = lv_event_get_target(event);
    setVelocity((int)lv_slider_get_value(slider));
}

static void midi_channel_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    setChannel(lv_dropdown_get_selected(obj)+1);
}

static void octave_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    setOctave(lv_dropdown_get_selected(obj)-2);
}

static void note_off_handler(lv_event_t * event)
{

    toggleNoteOff();
}

static void double_note_handler(lv_event_t * event)
{

    toggleDoubleNote();
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

void renderChord(char * chordName) {
    lv_label_set_text(mb_chordLabel, chordName);
}

static void tabview_change_event(lv_event_t * e)
{
    lv_obj_t * objTabView = lv_event_get_current_target(e);

    if ( lv_tabview_get_tab_act(objTabView) == 0 ) setChordPlaying(false);
    if ( lv_tabview_get_tab_act(objTabView) == 1 ) setChordPlaying(true);

}


void screen_init() {


    /*Create MIDI note velocity slider*/
    mb_velocitySlider = lv_slider_create(lv_scr_act());


    lv_obj_set_width(mb_velocitySlider, 760);
    lv_obj_align(mb_velocitySlider, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_slider_set_range(mb_velocitySlider, 0, 127);
    lv_slider_set_value(mb_velocitySlider, getVelocity(), LV_ANIM_OFF);
    lv_obj_add_event_cb(mb_velocitySlider, velocity_slider_handler, LV_EVENT_VALUE_CHANGED, NULL);

    /* create screen tab view */
    mb_tabView = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 50);
    lv_obj_set_height(mb_tabView, 440);


    // define style for tab labels
//    static lv_style_t mb_tabs_style;
//    lv_style_init(&mb_tabs_style);
//    lv_style_set_text_font(&mb_tabs_style, LV_FONT_MONTSERRAT_20);

    /* create Note tab */
    mb_tabNote = lv_tabview_add_tab(mb_tabView, "NOTE");

    /* create Chord tab */
    mb_tabChord = lv_tabview_add_tab(mb_tabView, "CHORD");

    /* set style of screen Tabs */
    lv_obj_t * tab_btns = lv_tabview_get_tab_btns(mb_tabView);
    lv_obj_set_style_pad_right(tab_btns, LV_HOR_RES / 2, 0);

    /* set logo image */
    lv_obj_t * logo_img = lv_img_create(tab_btns);
    LV_IMG_DECLARE(Logo);
    lv_img_set_src(logo_img, &Logo);
    lv_obj_align(logo_img, LV_ALIGN_RIGHT_MID, LV_HOR_RES / 2 - 25, 4);

    /* attach the event */
    lv_obj_add_event_cb(mb_tabView, tabview_change_event, LV_EVENT_VALUE_CHANGED, NULL);

    note_screen_create(mb_tabNote);
    chord_screen_create(mb_tabChord);

    // disable tab sliding and animation
    lv_obj_clear_flag(lv_tabview_get_content(mb_tabView), LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(lv_tabview_get_content(mb_tabView), scroll_begin_event, LV_EVENT_SCROLL_BEGIN, NULL);

}

static void note_screen_create(lv_obj_t * parent) {

    // define note tab left container
    lv_obj_t * note_tab_left_container = lv_obj_create(mb_tabNote);
    lv_obj_set_size(note_tab_left_container, lv_pct(62), lv_pct(100));

    // define note tab right container
    lv_obj_t * note_tab_right_container = lv_obj_create(mb_tabNote);
    lv_obj_set_size(note_tab_right_container, lv_pct(35), lv_pct(100));
    lv_obj_align(note_tab_right_container, LV_ALIGN_TOP_RIGHT, 0, 0);

    // define note label and its style
    lv_style_init(&note_style);
    lv_style_set_text_font(&note_style, &Monserrat_200);
    mb_noteLabel = lv_label_create(note_tab_left_container);
    lv_obj_align(mb_noteLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(mb_noteLabel, "");
    lv_obj_add_style(mb_noteLabel, &note_style, _LV_STYLE_STATE_CMP_SAME);


    // define grid for settings form
    static lv_coord_t note_settings_column_dsc[] = {140, 60, LV_GRID_TEMPLATE_LAST};   /*2 columns with 180 and 60 ps width*/
    static lv_coord_t note_settings_row_dsc[] = {65, 65, 65, 65, LV_GRID_TEMPLATE_LAST}; /*5 70 px tall rows*/

    //set right container to be a grid
    lv_obj_set_style_grid_column_dsc_array(note_tab_right_container, note_settings_column_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(note_tab_right_container, note_settings_row_dsc, 0);
    lv_obj_set_layout(note_tab_right_container, LV_LAYOUT_GRID);

    //midi channel label
    lv_obj_t * mb_midiChannelLabel = lv_label_create(note_tab_right_container);
    lv_label_set_text(mb_midiChannelLabel, "MIDI Channel:");
    lv_obj_set_grid_cell(mb_midiChannelLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    //midi channel drop down
    lv_obj_t * mb_midiChannelDropDown = lv_dropdown_create(note_tab_right_container);
    lv_dropdown_set_options(mb_midiChannelDropDown, "1\n"
                            "2\n"
                            "3\n"
                            "4\n"
                            "5\n"
                            "6\n"
                            "7\n"
                            "8\n"
                            "9\n"
                            "10\n"
                            "11\n"
                            "12\n"
                            "13\n"
                            "14\n"
                            "15\n"
                            "16");
    lv_dropdown_set_selected(mb_midiChannelDropDown, getChannel()-1);
    lv_obj_set_width(mb_midiChannelDropDown, 70);
    lv_obj_set_grid_cell(mb_midiChannelDropDown, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_add_event_cb(mb_midiChannelDropDown, midi_channel_handler, LV_EVENT_VALUE_CHANGED, NULL);

    //Octave label
    lv_obj_t * mb_octaveLabel = lv_label_create(note_tab_right_container);
    lv_label_set_text(mb_octaveLabel, "Octave:");
    lv_obj_set_grid_cell(mb_octaveLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

    //octave drop down
    lv_obj_t * mb_octaveDropDown = lv_dropdown_create(note_tab_right_container);
    lv_dropdown_set_options(mb_octaveDropDown, "-2\n"
                            "-1\n"
                            "0\n"
                            "1\n"
                            "2\n"
                            "3\n"
                            "4\n"
                            "5\n"
                            "6\n"
                            "7");
    lv_dropdown_set_selected(mb_octaveDropDown, getOctave()+2);
    lv_obj_set_width(mb_octaveDropDown, 70);
    lv_obj_set_grid_cell(mb_octaveDropDown, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_add_event_cb(mb_octaveDropDown, octave_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // note off label
    lv_obj_t * mb_noteOffLabel = lv_label_create(note_tab_right_container);
    lv_label_set_text(mb_noteOffLabel, "Play Note Off:");
    lv_obj_set_grid_cell(mb_noteOffLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 2, 1);

    // note off switch
    lv_obj_t * mb_noteOffSwitch = lv_switch_create(note_tab_right_container);
    if (getNoteOff()) {
            lv_obj_add_state(mb_noteOffSwitch, LV_STATE_CHECKED);
    }
    lv_obj_set_grid_cell(mb_noteOffSwitch, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_add_event_cb(mb_noteOffSwitch, note_off_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // double note label
    lv_obj_t * mb_doubleNoteLabel = lv_label_create(note_tab_right_container);
    lv_label_set_text(mb_doubleNoteLabel, "Play Double Note:");
    lv_obj_set_grid_cell(mb_doubleNoteLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 3, 1);

    // double note switch
    lv_obj_t * mb_doubleNoteSwitch = lv_switch_create(note_tab_right_container);
    if (getDoubleNote()) {
            lv_obj_add_state(mb_doubleNoteSwitch, LV_STATE_CHECKED);
    }
    lv_obj_set_grid_cell(mb_doubleNoteSwitch, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_add_event_cb(mb_doubleNoteSwitch, double_note_handler, LV_EVENT_VALUE_CHANGED, NULL);

}

static  void chord_screen_create(lv_obj_t * parent) {

    // define chord tab left container
    lv_obj_t * chord_tab_left_container = lv_obj_create(mb_tabChord);
    lv_obj_set_size(chord_tab_left_container, lv_pct(62), lv_pct(100));

    // define chord tab right container
    lv_obj_t * chord_tab_right_container = lv_obj_create(mb_tabChord);
    lv_obj_set_size(chord_tab_right_container, lv_pct(35), lv_pct(100));
    lv_obj_align(chord_tab_right_container, LV_ALIGN_TOP_RIGHT, 0, 0);

    // define chord label and its style
    mb_chordLabel = lv_label_create(chord_tab_left_container);
    lv_obj_align(mb_chordLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(mb_chordLabel, "");
    lv_obj_add_style(mb_chordLabel, &note_style, _LV_STYLE_STATE_CMP_SAME);

}






