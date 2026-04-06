#include <Wire.h>
#include "Tone.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include "font5x7.h"
#include <avr/pgmspace.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define OLED_ADDR 0x3C  // 一般的なSSD1306のアドレス

#define SOUND_PIN1 8
#define SOUND_PIN2 9
Tone tone1;
Tone tone2;

//const int BUZZER_PIN = 9;
int sound_flag=1;

const int LEFT_BUTTON_PIN = A1;
const int UP_BUTTON_PIN = A3;
const int DOWN_BUTTON_PIN = A0;
const int RIGHT_BUTTON_PIN = A2;
const int SHOOT_BUTTON_PIN = 2;

bool leftPressed; 
bool rightPressed;
bool oPressed;
bool dPressed;
bool sPressed;

bool leftPressed_f; 
bool rightPressed_f;
bool oPressed_f;
bool dPressed_f;
bool sPressed_f;

const uint16_t noteFrequencies[] PROGMEM = {
0,        // REST
NOTE_C1,NOTE_CS1,NOTE_D1,NOTE_DS1,NOTE_E1,NOTE_F1,NOTE_FS1,NOTE_G1,NOTE_GS1,NOTE_A1,NOTE_AS1,NOTE_B1,
NOTE_C2,NOTE_CS2,NOTE_D2,NOTE_DS2,NOTE_E2,NOTE_F2,NOTE_FS2,NOTE_G2,NOTE_GS2,NOTE_A2,NOTE_AS2,NOTE_B2,
NOTE_C3,NOTE_CS3,NOTE_D3,NOTE_DS3,NOTE_E3,NOTE_F3,NOTE_FS3,NOTE_G3,NOTE_GS3,NOTE_A3,NOTE_AS3,NOTE_B3,
NOTE_C4,NOTE_CS4,NOTE_D4,NOTE_DS4,NOTE_E4,NOTE_F4,NOTE_FS4,NOTE_G4,NOTE_GS4,NOTE_A4,NOTE_AS4,NOTE_B4,
NOTE_C5,NOTE_CS5,NOTE_D5,NOTE_DS5,NOTE_E5,NOTE_F5,NOTE_FS5,NOTE_G5,NOTE_GS5,NOTE_A5,NOTE_AS5,NOTE_B5,
NOTE_C6,NOTE_CS6,NOTE_D6,NOTE_DS6,NOTE_E6,NOTE_F6,NOTE_FS6,NOTE_G6,NOTE_GS6,NOTE_A6,NOTE_AS6,NOTE_B6,
NOTE_C7,NOTE_CS7,NOTE_D7,NOTE_DS7,NOTE_E7,NOTE_F7,NOTE_FS7,NOTE_G7,NOTE_GS7,NOTE_A7,NOTE_AS7,NOTE_B7,
NOTE_C8,NOTE_CS8,NOTE_D8,NOTE_DS8,
NOTE_B0
};

enum NoteName : byte {
N_R = 0, // 休符
N_C1,N_CS1,N_D1,N_DS1,N_E1,N_F1,N_FS1,N_G1,N_GS1,N_A1,N_AS1,N_B1,
N_C2,N_CS2,N_D2,N_DS2,N_E2,N_F2,N_FS2,N_G2,N_GS2,N_A2,N_AS2,N_B2,
N_C3,N_CS3,N_D3,N_DS3,N_E3,N_F3,N_FS3,N_G3,N_GS3,N_A3,N_AS3,N_B3,
N_C4,N_CS4,N_D4,N_DS4,N_E4,N_F4,N_FS4,N_G4,N_GS4,N_A4,N_AS4,N_B4,
N_C5,N_CS5,N_D5,N_DS5,N_E5,N_F5,N_FS5,N_G5,N_GS5,N_A5,N_AS5,N_B5,
N_C6,N_CS6,N_D6,N_DS6,N_E6,N_F6,N_FS6,N_G6,N_GS6,N_A6,N_AS6,N_B6,
N_C7,N_CS7,N_D7,N_DS7,N_E7,N_F7,N_FS7,N_G7,N_GS7,N_A7,N_AS7,N_B7,
N_C8,N_CS8,N_D8,N_DS8,
NOTE_COUNT // 全体の音数を確認するための番兵（自動的に最大数+1になる）
};

const byte melody1[] PROGMEM = {

N_D4,2,N_R,2,N_D5,2,N_R,2,N_D4,2,N_R,2,N_D5,2,N_R,2,N_F4,2,N_R,2,N_F5,2,N_R,2,
N_F4,2,N_R,2,N_F5,2,N_R,2,N_AS3,2,N_R,2,N_AS4,2,N_R,2,N_AS3,2,N_R,2,N_AS4,2,N_R,2,N_C4,2,
N_R,2,N_C5,2,N_R,2,N_C4,2,N_R,2,N_C5,2,N_R,2,N_D4,2,N_R,2,N_D5,2,N_R,2,N_D4,2,N_R,2,
N_D5,2,N_R,2,N_F4,2,N_R,2,N_F5,2,N_R,2,N_F4,2,N_R,2,N_F5,2,N_R,2,N_AS3,2,N_R,2,N_AS4,2,
N_R,2,N_AS3,2,N_R,2,N_AS4,2,N_R,2,N_C4,2,N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,N_F5,2,N_R,2,
N_E5,2,N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,N_A4,2,N_R,2,N_C5,2,N_R,2,N_A4,2,N_R,2,N_D5,2,
N_R,2,N_A4,2,N_R,2,N_E5,2,N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,N_A4,4,N_R,4,N_A4,2,N_R,2,
N_D5,2,N_R,2,N_F5,2,N_R,2,N_E5,2,N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,N_A4,2,N_R,2,N_C5,2,
N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,N_A4,2,N_R,2,N_E5,2,N_R,2,N_A4,2,N_R,2,N_F5,2,N_R,2,
N_G5,4,N_R,4,N_A4,2,N_R,2,N_D5,2,N_R,2,N_F5,2,N_R,2,N_E5,2,N_R,2,N_A4,2,N_R,2,N_D5,2,
N_R,2,N_A4,2,N_R,2,N_C5,2,N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,N_A4,2,N_R,2,N_E5,2,N_R,2,
N_A4,2,N_R,2,N_D5,2,N_R,2,N_A4,4,N_R,4,N_A4,2,N_R,2,N_D5,2,N_R,2,N_F5,2,N_R,2,N_E5,2,
N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,N_A4,2,N_R,2,N_C5,2,N_R,2,N_A4,2,N_R,2,N_D5,2,N_R,2,
N_A4,2,N_R,2,N_E5,2,N_R,2,N_A4,2,N_R,2,N_F5,2,N_R,2,N_G5,4,N_R,4,N_A4,2,N_R,2,N_D5,2,
N_R,2,N_F5,2,N_R,2,N_A4,23,N_R,1,N_G4,2,N_A4,2,N_AS4,2,N_C5,2,N_D5,15,N_R,1,N_C5,8,
N_AS4,8,N_C5,4,N_A4,4,N_F4,11,N_R,1,N_F4,2,N_R,2,N_G4,2,N_R,2,N_A4,2,
N_R,2,N_AS4,8,N_G4,15,N_R,1,N_E4,8,N_A4,23,N_R,1,N_E5,2,N_F5,2,N_G5,2,N_A5,2,
N_AS5,15,N_R,1,N_A5,8,N_G5,8,N_A5,4,N_F5,4,N_C6,11,N_R,1,N_C6,4,
N_E6,4,N_F6,4,N_G6,8,N_E6,8,N_E6,8,N_C6,8,
N_AS4,8,N_F5,53,N_R,7,N_C5,2,N_R,6,N_DS5,2,N_R,6,N_AS4,2,N_R,6,N_C5,2,N_R,2,N_C5,2,
N_R,2,N_GS4,2,N_R,2,N_AS4,2,N_R,2,N_C5,2,N_R,2,N_DS5,2,N_R,6,N_F5,4,N_DS5,2,N_R,2,
N_F4,2,N_G4,2,N_GS4,2,N_G4,2,N_F4,4,N_R,4,N_F4,2,N_G4,2,N_GS4,2,N_AS4,2,N_C5,4,N_R,4,N_AS4,2,
N_C5,2,N_CS5,2,N_DS5,2,N_F5,4,N_R,4,N_F5,2,N_G5,2,N_GS5,2,N_AS5,2,N_C6,4,N_R,4,N_C6,4,
N_C6,2,N_R,2,N_C6,2,N_R,2,N_CS6,2,N_R,2,N_AS5,8,N_AS5,11,N_R,1,N_DS6,4,N_AS5,4,
N_C6,4,N_GS5,8,N_C6,8,N_F4,2,N_G4,2,N_GS4,2,N_G4,2,N_F4,4,N_R,4,
N_F4,2,N_G4,2,N_GS4,2,N_AS4,2,N_C5,4,N_R,4,N_AS4,2,N_C5,2,N_CS5,2,N_DS5,2,N_F5,4,N_R,4,N_F5,2,
N_G5,2,N_GS5,2,N_AS5,2,N_C6,11,N_R,1,N_C6,2,N_R,2,N_C6,4,N_CS6,4,N_AS5,8,
N_C6,8,N_R,4,N_CS5,4,N_DS5,4,N_FS5,4,N_GS5,4,N_A5,4,N_AS5,8,
N_B5,11,N_R,5,N_A5,4,N_GS5,4,N_FS5,8,N_FS5,15,N_R,1,N_B5,4,
N_C6,4,N_CS6,8,N_D6,19,N_R,1,N_C6,4,N_D6,4,N_E6,8,N_D6,2,
N_R,2,N_E6,2,N_R,2,N_FS6,8,N_R,16

};


const byte melody2[] PROGMEM = {

N_D3,2,N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,
N_F3,2,N_R,2,N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_C3,2,
N_R,2,N_C4,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,
N_D4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,
N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,
N_D3,2,N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_F3,2,
N_R,2,N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_C3,2,N_R,2,
N_C4,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,
N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,
N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_D3,2,
N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_F3,2,N_R,2,
N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_C3,2,N_R,2,N_C4,2,
N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,
N_F3,2,N_R,2,N_F4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,
N_R,2,N_AS3,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_D3,2,N_R,2,
N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,
N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,
N_C3,2,N_R,2,N_C4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_F3,2,
N_R,2,N_F4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,N_R,2,
N_AS3,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,
N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,
N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_C3,2,
N_R,2,N_C4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_D3,2,N_R,2,N_D4,2,N_R,2,N_F3,2,N_R,2,
N_F4,2,N_R,2,N_F3,2,N_R,2,N_F4,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,N_R,2,N_AS2,2,N_R,2,N_AS3,2,
N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_C3,2,N_R,2,N_C4,2,N_R,2,N_F3,2,N_R,2,N_F3,2,N_DS3,2,
N_C3,2,N_R,2,N_DS3,2,N_R,2,N_F3,2,N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,N_DS3,2,N_R,2,N_F3,2,
N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,N_DS3,2,N_R,2,N_F3,2,N_R,2,N_FS3,2,N_R,2,N_G3,2,N_R,2,
N_GS3,6,N_R,2,N_GS3,2,N_FS3,2,N_DS3,2,N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,N_GS3,2,N_FS3,2,N_DS3,2,
N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,N_GS3,2,N_FS3,2,N_DS3,2,N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,
N_G3,2,N_R,2,N_FS3,2,N_R,2,N_F3,6,N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,N_DS3,2,N_R,2,N_F3,2,
N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,N_DS3,2,N_R,2,N_F3,2,N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,
N_DS3,2,N_R,2,N_F3,2,N_R,2,N_FS3,2,N_R,2,N_G3,2,N_R,2,N_GS3,6,N_R,2,N_GS3,2,N_FS3,2,N_DS3,2,
N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,N_GS3,2,N_FS3,2,N_DS3,2,N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,
N_GS3,2,N_FS3,2,N_DS3,2,N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,N_G3,2,N_R,2,N_FS3,2,N_R,2,N_F3,6,
N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,N_DS3,2,N_R,2,N_F3,2,N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,
N_DS3,2,N_R,2,N_F3,2,N_R,2,N_F3,2,N_DS3,2,N_C3,2,N_R,2,N_DS3,2,N_R,2,N_F3,2,N_R,2,N_FS3,2,
N_R,2,N_G3,2,N_R,2,N_GS3,6,N_R,2,N_GS3,2,N_FS3,2,N_DS3,2,N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,
N_GS3,2,N_FS3,2,N_DS3,2,N_R,2,N_FS3,2,N_R,2,N_GS3,2,N_R,2,N_GS3,2,N_FS3,2,N_DS3,2,N_R,2,N_FS3,2,
N_R,2,N_GS3,2,N_R,2,N_A3,2,N_R,2,N_AS3,2,N_R,2,N_B3,6,N_R,2,N_B3,2,N_A3,2,N_FS3,4,
N_A3,4,N_B3,4,N_A3,4,N_FS3,4,N_A3,4,N_B3,2,N_R,2,N_B3,2,
N_A3,2,N_FS3,4,N_A3,4,N_B3,2,N_R,2,N_C4,2,N_R,2,N_CS4,4,N_R,4,N_D4,6,
N_C4,2,N_A3,4,N_C4,4,N_D4,2,N_R,2,N_D4,2,N_C4,2,N_A3,4,N_C4,4,
N_E4,2,N_R,2,N_E4,2,N_D4,2,N_B3,4,N_D4,4,N_FS4,2,N_R,16


};







const unsigned char font_7seg[10][5] = {
  {0x36, 0x41, 0x41, 0x41, 0x36}, // 0
  {0x00, 0x00, 0x00, 0x00, 0x36}, // 1
  {0x30, 0x49, 0x49, 0x49, 0x06}, // 2
  {0x00, 0x49, 0x49, 0x49, 0x36}, // 3
  {0x06, 0x08, 0x08, 0x08, 0x36}, // 4
  {0x06, 0x49, 0x49, 0x49, 0x30}, // 5
  {0x36, 0x49, 0x49, 0x49, 0x30}, // 6
  {0x00, 0x01, 0x01, 0x01, 0x36}, // 7
  {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
  {0x06, 0x49, 0x49, 0x49, 0x36}  // 9
};

const uint8_t icon2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t icon[] = { 0x00, 0x3F, 0x21, 0x21, 0x21, 0x21, 0x3F, 0x00 };

// ゲームフィールド設定
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 20;
const int BLOCK_SIZE = 6;
const int FIELD_OFFSET_X = 2;
const int FIELD_OFFSET_Y = 2;

// フィールド (0=空, 1=ブロック)
byte field[FIELD_HEIGHT][FIELD_WIDTH];

// テトロミノの形状 (4x4の配列)
const byte TETROMINOS[7][4][4] = {
  // I
  {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
  },
  // O
  {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
  },
  // T
  {
    {0,0,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0}
  },
  // S
  {
    {0,0,0,0},
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0}
  },
  // Z
  {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
  },
  // J
  {
    {0,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0}
  },
  // L
  {
    {0,0,0,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0}
  }
};

// 現在のテトロミノ
int currentType = 0;
int currentX = 3;
int currentY = 0;
int currentRotation = 0;
byte currentShape[4][4];

// 次のテトロミノ
int nextType = 0;

// ゲーム状態
bool gameStarted = false;
bool gameOver = false;
bool soundPlayed = false;
int score = 0;
unsigned long lastFallTime = 0;
int fallDelay = 500;
unsigned long lastButtonTime = 0;
const int buttonDelay = 150;
int ky=0;
int chg_flg=0;


void updateSound1(int f) {
  static int i1;
  static int l1=0;
  int t1=0;
  int o1=0;

  static int i2;
  static int l2=0;
  int t2=0;
  int o2=0;
  if (f==-1){ i1-0; i2=0; i1=0;i2=0;}

  if (f==0){ return;}
  //if (millis()<ti){return;}
  if (l1>0){
    l1--;
  }else{
    t1=pgm_read_byte(melody1+i1*2);
    l1=pgm_read_byte(melody1+i1*2+1);
    o1=pgm_read_word(noteFrequencies+t1);
    tone1.play(o1,l1*30);
    l1--;
    i1++;
    if (i1>sizeof(melody1)/2){i1=0;}
  }

  if (l2>0){
    l2--;
  }else{
    t2=pgm_read_byte(melody2+i2*2);
    l2=pgm_read_byte(melody2+i2*2+1);
    o2=pgm_read_word(noteFrequencies+t2);
    tone2.play(o2,l2*30);
    l2--;
    i2++;
    if (i2>sizeof(melody2)/2){i2=0;}
  }
  if(i1==0  || i2==0){i1=0; i2=0; l1=0;l2=0;} 
}

void updateSound(int sound_flag) {
  static unsigned long ti=0;
  if (millis()<ti){return;}
  if (millis()>ti){ti=millis()+45;}else{return;} 
  updateSound1(sound_flag);
}

/*
Beep SND_JUMP     = {1200, 60};
Beep SND_AIRJUMP  = {1050, 60};
Beep SND_OK       = {1100, 60};
Beep SND_CANCEL   = { 400,120};
*/

void setup() {

  Wire.begin();	
  Wire.setClock(400000);	
  //tiny_init();
  //cls();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);  // 画面を90度回転（縦使用）


  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SHOOT_BUTTON_PIN, INPUT_PULLUP);

  //pinMode(BUZZER_PIN, OUTPUT);

  randomSeed(analogRead(A7));
  nextType = random(7);  // 最初の次のブロックを決定
  //Serial.begin(9600);
  displayTitle();
  //playBeep(SND_JUMP );
  sound_flag=1;
  tone1.begin(SOUND_PIN1);
  tone1.stop();
  tone2.begin(SOUND_PIN2);
  tone2.stop();

   tone1.play( NOTE_C4, 200);  
  //tone2.play( NOTE_C5, 200);  
  delay(300);
  tone1.play( NOTE_C4, 200);  
  //tone2.play( NOTE_C5, 200);  
  delay(200);

}

void displayTitle() {
  display.clearDisplay();
  //cls();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 15);
  display.print("Tetris");
  //disp_str(1,1,"TETRIS");
  display.setTextSize(1);
  display.setCursor(5, 40);
  display.print("Press L to start");
  //disp_str(0,4,"PRESS L TO START");
  display.display();

}

void initGame() {
  // フィールドをクリア
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      field[y][x] = 0;
    }
  }

  score = 0;
  gameOver = false;
  soundPlayed = false;
  spawnNewTetromino();
  ky=0;

  oPressed = false;
  dPressed = false;
  sPressed = false;

  updateSound1(-1);

}


void spawnNewTetromino() {
  currentType = nextType;  // 次のブロックを現在のブロックにする
  nextType = random(7);    // 新しい次のブロックを決定
  currentX = 3;
  currentY = 0;
  currentRotation = 0;

  // 形状をコピー
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      currentShape[y][x] = TETROMINOS[currentType][y][x];
    }
  }

  // スポーン位置に既にブロックがある場合はゲームオーバー
  if (checkCollision(currentX, currentY, currentShape)) {
    gameOver = true;
  }
}

void rotateTetromino() {
  byte newShape[4][4];

  // 90度回転
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      newShape[x][3-y] = currentShape[y][x];
    }
  }

  // 回転後の衝突チェック
  if (!checkCollision(currentX, currentY, newShape)) {
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        currentShape[y][x] = newShape[y][x];
      }
    }
  }
}

bool checkCollision(int x, int y, byte shape[4][4]) {
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (shape[py][px]) {
        int fieldX = x + px;
        int fieldY = y + py;

        // 壁のチェック
        if (fieldX < 0 || fieldX >= FIELD_WIDTH || fieldY >= FIELD_HEIGHT) {
          return true;
        }

        // 既存のブロックとの衝突チェック
        if (fieldY >= 0 && field[fieldY][fieldX]) {
          return true;
        }
      }
    }
  }
  return false;
}

void lockTetromino() {
  // テトロミノをフィールドに固定
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (currentShape[py][px]) {
        int fieldY = currentY + py;
        int fieldX = currentX + px;
        if (fieldY >= 0 && fieldY < FIELD_HEIGHT && fieldX >= 0 && fieldX < FIELD_WIDTH) {
          field[fieldY][fieldX] = 1;
        }
      }
    }
  }

  // ライン消去チェック
  clearLines();

  // 新しいテトロミノを生成
  spawnNewTetromino();
  dPressed=false;
}

void clearLines() {
  int linesCleared = 0;

  for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
    bool lineFull = true;
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (field[y][x] == 0) {
        lineFull = false;
        break;
      }
    }

    if (lineFull) {
      linesCleared++;

      // ラインを消して上のブロックを落とす
      for (int yy = y; yy > 0; yy--) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
          field[yy][x] = field[yy - 1][x];
        }
      }

      // 一番上の行をクリア
      for (int x = 0; x < FIELD_WIDTH; x++) {
        field[0][x] = 0;
        //playBeep(SND_AIRJUMP );
        //delay(10);
        //playBeep(SND_CANCEL);
        //playBeep(SND_OK);
        //delay(10);
        //playBeep(SND_CANCEL);

      }
      //tone(BUZZER_PIN, 1400, 80);
      
 
      y++; // 同じ行を再チェック
    }
  }

  if (linesCleared > 0) {
    score += linesCleared * 10;
  }
}

void drawField() {
  // 枠線を描画
  display.drawRect(FIELD_OFFSET_X - 1, FIELD_OFFSET_Y - 1,
                   FIELD_WIDTH * BLOCK_SIZE + 2, FIELD_HEIGHT * BLOCK_SIZE + 2,
                   SSD1306_WHITE);

  // フィールドのブロックを描画
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (field[y][x]) {
        display.fillRect(FIELD_OFFSET_X + x * BLOCK_SIZE,
                        FIELD_OFFSET_Y + y * BLOCK_SIZE,
                        BLOCK_SIZE - 1, BLOCK_SIZE - 1, SSD1306_WHITE);
      }
    }
  }

  // 現在のテトロミノを描画
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (currentShape[py][px]) {
        int drawY = currentY + py;
        if (drawY >= 0) {
          display.fillRect(FIELD_OFFSET_X + (currentX + px) * BLOCK_SIZE,
                          FIELD_OFFSET_Y + drawY * BLOCK_SIZE,
                          BLOCK_SIZE - 1, BLOCK_SIZE - 1, SSD1306_WHITE);
        }
      }
    }
  }
}

void drawNextTetromino() {
  // 右上に次のテトロミノを表示（回転後の座標系で）
  int previewX = 50;  // 少し右に移動
  int previewY = 6;   // 少し下に移動
  int previewSize = 2;

  // 次のテトロミノを描画
  for (int py = 0; py < 4; py++) {
    for (int px = 0; px < 4; px++) {
      if (TETROMINOS[nextType][py][px]) {
        display.fillRect(previewX + px * previewSize,
                        previewY + py * previewSize,
                        previewSize - 1, previewSize - 1, SSD1306_WHITE);
      }
    }
  }
}

void playGameOverSound() {
  // ゲームオーバー音を鳴らす（1回だけ）
  tone1.play( 200, 200);  // 低い音
  delay(250);
  tone1.play( 150, 200);  // さらに低い音
  delay(250);
  tone1.play( 100, 400);  // とても低い音
  delay(450);
  tone1.stop();
  //noTone(BUZZER_PIN);
}

void displayGameOver() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 15);
  display.print("GAME");
  display.setCursor(15, 35);
  display.print("OVER");
  display.setTextSize(1);
  display.setCursor(30, 55);
  display.print("Score:");
  display.print(score);
  display.display();
}




void loop() {

  // ゲーム開始前
  if (!gameStarted) {
    if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
      delay(50);
      while (digitalRead(LEFT_BUTTON_PIN) == LOW) {
        delay(10);
      }
      gameStarted = true;
      initGame();
    }
    return;
  }

  // ゲームオーバー
  if (gameOver) {
    // BGMを止める
    //noTone(BUZZER_PIN);
    //updateSound(0);
    // 音を1回だけ鳴らす
    if (!soundPlayed) {
      playGameOverSound();
      soundPlayed = true;
      delay(1000);  // スコア表示時間
    }

    // リトライ待ち画面を表示
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(5, 20);
    display.print("Score:");
    display.print(score);
    display.setCursor(5, 35);
    display.print("Press L to retry");
    display.display();

    if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
      delay(50);
      while (digitalRead(LEFT_BUTTON_PIN) == LOW) {
        delay(10);
      }
      initGame();

    }
    return;
  }

  unsigned long currentTime = millis();

  //updateSound(sound_flag);
  // ボタン入力
  if (currentTime - lastButtonTime > buttonDelay) {

    leftPressed = digitalRead(LEFT_BUTTON_PIN) == LOW;
    rightPressed = digitalRead(RIGHT_BUTTON_PIN) == LOW;
    //dPressed = digitalRead(DOWN_BUTTON_PIN) == LOW;
  
    if (dPressed_f==false){dPressed = digitalRead(DOWN_BUTTON_PIN) == LOW;}
    if (dPressed){dPressed_f=true; }

    if (oPressed_f==false){oPressed = digitalRead(UP_BUTTON_PIN) == LOW;}
    if (oPressed){oPressed_f=true; }

    if (sPressed_f==false){sPressed = digitalRead(SHOOT_BUTTON_PIN) == LOW;}
    if (sPressed){sPressed_f=true; }

    if ( sPressed ) { if (sound_flag==0){sound_flag=1;}else{sound_flag=0;}  }
    if ( dPressed ) { fallDelay=40; }

    if ( digitalRead(DOWN_BUTTON_PIN) != LOW){dPressed_f=false; dPressed=false; }
    if ( digitalRead(UP_BUTTON_PIN) != LOW){oPressed_f=false;}
    if ( digitalRead(SHOOT_BUTTON_PIN) != LOW){sPressed_f=false;}

    if ( (leftPressed && rightPressed) || (oPressed)) {
      // 両方押されたら回転
      rotateTetromino();
      lastButtonTime = currentTime;
      ky=0;
      chg_flg=1;
      //tone(BUZZER_PIN, 1000, 20);
    } else if (leftPressed) {
      // 左移動
      if (!checkCollision(currentX - 1, currentY, currentShape)) {
        currentX--;
        //tone(BUZZER_PIN, 600, 20);
        //playBeep(SND_JUMP );
        ky=0;
        chg_flg=1;

      }
      lastButtonTime = currentTime;
    } else if (rightPressed) {
      // 右移動
      if (!checkCollision(currentX + 1, currentY, currentShape)) {
        currentX++;
        //tone(BUZZER_PIN, 800, 20);
        //playBeep(SND_AIRJUMP );
        ky=0;
        chg_flg=1;
      }
      lastButtonTime = currentTime;
    }
  }
  // 落下
  if (currentTime - lastFallTime > fallDelay) {
    if (!checkCollision(currentX, currentY + 1, currentShape)) {
      currentY++;
      ky++;
      chg_flg=1;
    } else {
      lockTetromino();
      fallDelay=500;
      ky=0;
    }
    //if (ky>3){fallDelay=80;}else{fallDelay=500;}
    fallDelay=500;
    lastFallTime = currentTime;
  }

  //leftPressed = 0;
  //rightPressed = 0;
  oPressed = false;
  //dPressed = false;
  sPressed = false;

  //updateSound(sound_flag);
  // 描画
  if (chg_flg==1){
    display.clearDisplay();
  }
  //updateSound(sound_flag);
  // 左上にスコアを表示（少し下、少し右に）
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 8);
  display.print(score);
  // updateSound(sound_flag);
  // フィールドとテトロミノを描画
  drawField();
  //updateSound(sound_flag);

  // 次のブロックを表示
  drawNextTetromino();
  //updateSound(sound_flag);
  //updateSound(sound_flag);
  if (chg_flg==1){
    display.display();
    chg_flg=0;
  }
  //playBeep(SND_JUMP );
  updateSound(sound_flag);
  //updateSound();
  //delay(10);
}



