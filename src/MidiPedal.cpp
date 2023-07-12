
#include <Arduino.h>

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

//Settings mySettings;
//Settings * mySettingsInstance;
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

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);

// ST7262 IPS LCD 800x480
 Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
   bus,
   800 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
   480 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
   1 /* pclk_active_neg */, 14000000 /* prefer_speed */, true /* auto_flush */);

// ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil

/* Setting for new Arduino_GFX library that need to be fixed further */
// Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
//     40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
//     45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
//     5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
//     8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
//     0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
//     0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
//     1 /* pclk_active_neg */, 14000000 /* prefer_speed */);
// Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//     800 /* width */, 480 /* height */, bus, false /* rotation */, true /* auto_flush */);


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

/****************************/
/* configure pedal hardware */
/****************************/

//initialize GPIO multiplexer on address 0x20 on another wire - first wire is used by touch screen driver
TwoWire pcf8575I2C = TwoWire(1);
PCF8575 PCF(0x20, &pcf8575I2C);

const int n_cols = 2; // number of rows in the diode matrix (bass pedals)
const int n_rows = 8; // number of columns in the diode matrix (bass pedals)

//define pedal matrix pins
const int rows[n_rows] = {4,5,6,7,11,10,9,8};
const int cols[n_cols] = {3,12};

// contact states
bool ped_state[16] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
bool ped_prev_state[16] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };
bool state_changed = false;

unsigned long lastDebounceTime[n_cols * n_rows] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

/*******************************/
/* end of pedal hardware config*/
/*******************************/

void play() {

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
                  midiPlayer.pedalOff(index);
              }
              else {
                  Log.trace(F("PEDAL ON: %d"CR), index);
                  midiPlayer.pedalOn(index);
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

void setup()
{
  Serial.begin( 115200 ); /* prepare for possible serial debug */

  while(!Serial && !Serial.available()){}

  //Log.begin(LOG_LEVEL_TRACE, &Serial);
  Log.begin(LOG_LEVEL_ERROR, &Serial);


  //set default settings
  mp_Settings->init();

  midiPlayer.init();

  pcf8575I2C.begin(SDApin,SCLpin,400000); // SDA pin 5, SCL pin 4 builtin OLED

  //Init Pedal hardware
    if (!PCF.begin())
    {
      Log.error(F("could not initialize..."CR));
    }
    if (!PCF.isConnected())
    {
      Log.error(F("=> not connected"CR));
    }
    else
    {
      Log.info(F("=> connected!!"CR));
    }

    PCF.read16();

  // Init Display
  gfx->begin();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  lv_init();
  delay(10);
  touch_init();
  screenWidth = gfx->width();
  screenHeight = gfx->height();
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight/4 , MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);


  if (!disp_draw_buf)
  {
    Log.error(F("LVGL disp_draw_buf allocate failed!"CR));
  }
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight/4);

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

     screen_init();

    Log.info(F("LVGL Display Setup done"CR));
  }
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  play();
}
