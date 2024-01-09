#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <Arduino.h>
#endif

#ifndef ARDUINOLOG_H_
#define ARDUINOLOG_H_
#include "ArduinoLog.h"
#endif

#ifndef SETTINGS_H_
#include "Settings.h"
#endif

#ifndef MIDIPLAYER_H_
#include "MidiPlayer.h"
#endif


#ifndef LVGL_H_
#include <lvgl.h>
#endif

#include <Arduino_GFX_Library.h>
#include <Screen.h>

#include "PCF8575.h"

//define i2C pins
# define SDApin 13
# define SCLpin 12

bool device_init=false;

Settings * mp_Settings = Settings::getInstance();
 
MidiPlayer midiPlayer;

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#define TFT_BL 2
#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

// create a display bus object
Arduino_ESP32RGBPanel* bus = new Arduino_ESP32RGBPanel(
    40, 41, 39, 42,     /* DIS_DE, DIS_VSYNC, DIS_HSYNC, DIS_PCLK, */
    45, 48, 47, 21, 14, /* DIS_R_BUS, */
    5, 6, 7, 15, 16, 4, /* DIS_G_BUS, */
    8, 3, 46, 9, 1,     /* DIS_B_BUS, */
    0, 8, 4, 8,        /* DIS_HS_POL, DIS_HS_FP, DIS_HS_PW, DIS_HS_BP, */
  //  0, 1, 1, 43,
    0, 8, 4, 8,        /* DIS_VS_POL, DIS_VS_FP, DIS_VS_PW, DIS_VS_BP, */
  //  0, 3, 1, 12, 
    1, 14000000 );        /* DIS_PC_A_N, DIS_SPEED ); */

// create a display driver object
Arduino_RGB_Display* gfx = new Arduino_RGB_Display( 800, 480, bus );
// ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil


#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#include "touch.h"

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

/* controller does somehting when pedal or switch is pressed
*  pedal index are in range of 0 - 12
*  switch index are in range of 13-16 where 
*        13 is settings switch
*        14 is a note playing switch
*        15 is a chord playing switch
*        16 is a program/command playing switch
*/ 
void controller(int index, bool status) {
  Log.trace(F("controller() function called. The state is: %d"CR), mp_Settings->getState());

  if(index <13) {
      Log.trace(F("controller() - pedal is pressed. Pedal is: %d"CR), index);

    switch(mp_Settings->getState()) {
      case 1: //Note playing
        if (status) { midiPlayer.pedalOn(index); } else { midiPlayer.pedalOff(index); }
        break;
      case 11: //Note settings
        if (index==0 && status) channel_note_down_switch() ;
        if (index==1 && status) channel_note_up_switch() ;
        if (index==2 && status) octave_note_down_switch() ;
        if (index==3 && status) octave_note_up_switch() ;
        if (index==4 && status) note_off_toggle() ;
        if (index==5 && status) double_note_toggle() ;
        break;
      case 2: //Chord playing
        if (status) { midiPlayer.pedalOn(index); } else { midiPlayer.pedalOff(index); }
        break;
      case 21: //Chord patch settings
        if (index==0 && status) channel_chord_down_switch() ;
        if (index==1 && status) channel_chord_up_switch() ;
        if (index==2 && status) octave_chord_down_switch() ;
        if (index==3 && status) octave_chord_up_switch() ;
        if (index==4 && status) note_off_toggle() ;
        if (index==5 && status) root_note_down_switch();
        if (index==6 && status) root_note_up_switch();
        if (index==7 && status) harmony_mode_down_switch();
        if (index==8 && status) harmony_mode_up_switch();
        break;
      case 22: //Chord patch save (used to set name of the patch)
        pedal_selected(index);
        break;
      case 3: //Program/Command playing
        break;
      case 31: //P/C settings
        break;
      case 32: //P/C patch save
        break;
      case 33: // P/C patch exit without save
        break;
    }
  } else {
    Log.trace(F("controller() - switch is pressed. Switch is: %d"CR), index);

    // do only  something on pressing the switch (ignore releasing the switch)
    if (status) {
      Log.trace(F("controller() - switch is on."));    
      switch(mp_Settings->getState()) {
        case 1: //Note playing
          switch(index) {
            case 13:
              note_settings_open();
              break;
            case 14:
              // do nothing - we already play notes
              break;
            case 15:
              // open chord playing screen
              changeTabChord();
              break;
            case 16:
              // open program command playing screen
              changeTabProgCom();
              break;
          }
          break;
        case 11: //Note settings
          switch(index) {
            case 13:
              // do nothing
              break;
            case 14:
              changeTabNote();
              break;
            case 15:
              // open chord playing screen
              changeTabChord();
              break;
            case 16:
              // open program command playing screen
              changeTabProgCom();
              break;
          }
          break;
        case 2: //Chord playing
          switch(index) {
            case 13:
              // open chord settings screen 
              chord_settings_open();
              break;
            case 14:
              // open note playing
              changeTabNote();
              break;
            case 15:
              // loop through chord sets
              change_chord_set_clicked();
              break;
            case 16:
              // open program command playing screen
              changeTabProgCom();
              break;
          }
          break;
        case 21: //Chord settings
          switch(index) {
            case 13:
              // close chord settings screen 
              chord_settings_advanced_open();
              break;
            case 14:
              // open note playing
              changeTabNote();
              break;
            case 15:
              changeTabChord();
              break;
            case 16:
              // open program command playing screen
              changeTabProgCom();
              break;
          }
        case 22: //Chord advanced settings
          switch(index) {
            case 13:
              // do nothing
              break;
            case 14:
              // move down selected chord type
              chord_type_down_switch();
              break;
            case 15:
              changeTabChord();
              break;
            case 16:
              // move done selecetd chord variant
              chord_variant_down_switch();
              break;
          }
          break;
        case 3: //Program/Command playing
          switch(index) {
            case 13:
              // open command / program, settings screen 
              break;
            case 14:
              // open note playing
              changeTabNote();
              break;
            case 15:
              // open chord playing screen
              changeTabChord();
              break;
            case 16:
              // open proram command playing screen
              changeTabProgCom();
              break;
          }
          break;
      }
    }
  }
}


/****************************/
/* configure pedal hardware */
/****************************/

//initialize GPIO multiplexer on address 0x20 on another wire - first wire is used by touch screen driver
TwoWire pcf8575I2C = TwoWire(1);
PCF8575 PCF(0x20, &pcf8575I2C);

const int n_cols = 2; // number of rows in the diode matrix (bass pedals)
const int n_rows = 8; // number of columns in the diode matrix (bass pedals)
const int n_sw = 4; // number of pedal switches

/* pedal matrix pins
   for prototype board */
//const int rows[n_rows] = {4,5,6,7,11,10,9,8};
//const int cols[n_cols] = {3,12};

/* pedal matrix pins
   for stojos esp32 s3 expansion board */
const int rows[n_rows] = {8,9,10,11,7,6,5,4};
const int cols[n_cols] = {13,2};
const int switches[n_sw] = {0,15,14,12};


// contact states
bool ped_state[16] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
bool ped_prev_state[16] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
bool state_changed = false;

// contact states
bool sw_state[4] = { true, true, true, true };
bool sw_prev_state[4] = { true, true, true, true};
bool sw_state_changed = false;

unsigned long lastDebounceTime[n_cols * n_rows] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

/*******************************/
/* end of pedal hardware config*/
/*******************************/

void readPedal() {

    // iterate the columns 
    for (int colIndex=0; colIndex < n_cols; colIndex++) {
        // col: set to output to low
        int curCol = cols[colIndex];

        PCF.write(curCol, LOW);
        //digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < n_rows; rowIndex++) {

          int index = rowIndex + colIndex * n_rows;

          int rowCol = rows[rowIndex];
          //pinMode(rowCol, INPUT);

          // scan normally open
          ped_state[index] = (PCF.read(rowCol) == HIGH);
          if (ped_state[index] != ped_prev_state[index]) {
            

            if ((millis() - lastDebounceTime[index]) > debounceDelay) {
              
              // reset the debouncing timer
              lastDebounceTime[index] = millis();

              if (ped_state[index]) {
                  Log.trace(F("PEDAL OFF: %d"CR), index);
                  //midiPlayer.pedalOff(index);
                  controller(index, false);
              }
              else {
                  Log.trace(F("PEDAL ON: %d"CR), index);
                  //midiPlayer.pedalOn(index);
                  controller(index, true);
              }
              ped_prev_state[index] = ped_state[index];
            }
          }

          //PCF.read(rowCol);
        }
        // disable the column
        PCF.write(curCol, HIGH);
    }
}


void readSwitches() {

    // iterate the switches 
    for (int index=0; index < n_sw; index++) {

      sw_state[index] = (PCF.read(switches[index]) == HIGH);

      if (sw_state[index] != sw_prev_state[index]) {

        if ((millis() - lastDebounceTime[index]) > debounceDelay) {
          
          // reset the debouncing timer
          lastDebounceTime[index] = millis();

          if (sw_state[index]) {
              Log.trace(F("SWITCH OFF: %d"CR), index);
              controller(13+index, false);
          }
          else {
              Log.trace(F("SWITCH ON: %d"CR), index);
              controller(13+index, true);
          }
          sw_prev_state[index] = sw_state[index];
        }
      }
  }
}

void setup()
{
  Serial.begin( 115200 ); /* prepare for possible serial debug */

  while(!Serial && !Serial.available()){}

  Log.begin(LOG_LEVEL_TRACE, &Serial);
  //Log.begin(LOG_LEVEL_ERROR, &Serial);
  //Log.begin(LOG_LEVEL_INFO, &Serial);

  pcf8575I2C.begin(SDApin,SCLpin,400000); // SDA pin 5, SCL pin 4 builtin OLED

  //Init Pedal hardware
  if (!PCF.begin())  {
    Log.error(F("could not initialize..."CR));
  }

  if (!PCF.isConnected())  {
    Log.error(F("=> not connected"CR));
  }  else  {
    Log.info(F("=> connected!!"CR));
  }

  PCF.read16();

  //mp_Settings->setFactoryReset(true);

  Log.trace(F("Initializing settings"CR));
  mp_Settings->init();

  Log.trace(F("Initializing Midi player"CR));
  midiPlayer.init();


  Log.info(F("LVGL Display Setup ..."CR));
  
  gfx->begin();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  lv_init();
  delay(10);
  touch_init();
  screenWidth = gfx->width();
  screenHeight = gfx->height();
   
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight/4 , MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

  if (!disp_draw_buf) {

    Log.error(F("LVGL disp_draw_buf allocate failed!"CR));

  } else {

    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight/4);
    //    lv_disp_draw_buf_init( &draw_buf, buf, NULL, 800 * 480 / 4 );
    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    Log.info(F("LVGL Display Setup done"CR));

    Log.trace(F("Initializing Screen"CR));
//    screen_boot();
    screen_init();

  }
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    readPedal();
    readSwitches();
}
