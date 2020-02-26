#include "Arduino.h"

///////////////////////////////////////////////////////////////////////////////
//                          Constantes                               //
/////////////////////////////////////////////////////////////////////////////

#define LIMIAR_CONDUZ 40

#define STEINHART_A 0.001129148
#define STEINHART_B 0.000234125
#define STEINHART_C 0.0000000876741

#define PH_A -0.0232f
#define PH_B 25.985f

#define getPH(x) PH_A*x+PH_B

///////////////////////////////////////////////////////////////////////////////
//                          Modos de operação                               //
/////////////////////////////////////////////////////////////////////////////


#define MODO_PH 0
#define MODO_TEMP 1
#define MODO_IDSOL 2
#define MODO_PLAY 3

///////////////////////////////////////////////////////////////////////////////
//                   Endereçamento dos Áudios                               //
/////////////////////////////////////////////////////////////////////////////

#define AUDIO_E 200
#define AUDIO_PONTO 201

#define AUDIO_CONDUZ 202
#define AUDIO_NAO_CONDUZ 203

#define AUDIO_PH_SELECT 204
#define AUDIO_TEMP_SELECT 205
#define AUDIO_IDSOL_SELECT 206

#define AUDIO_PH_INIT 207
#define AUDIO_TEMP_INIT 208
#define AUDIO_TEMP_FIM 209
#define AUDIO_INIT_OK 210

#define AUDIO_ZERO 211

///////////////////////////////////////////////////////////////////////////////
//                        Definição dos pinos                               //
/////////////////////////////////////////////////////////////////////////////

//Sensores
//#define PH_PIN A1    //Analog IN
//#define TERM_PIN A0  //Analog IN
//#define IDSOL_PIN A4 //Digital IN

#define PH_PIN A4   //Analog IN
#define TERM_PIN A1  //Analog IN
#define IDSOL_PIN A7 //Digital IN

//Botões
#define BT_PH_PIN 10
#define BT_TEMP_PIN 11
#define BT_IDSOL_PIN 12
#define BT_PLAY_PIN 13

//LCD
#define LCD_RS 8
#define LCD_E 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

//DFPlayer
#define DFPLAYER_RX 3
#define DFPLAYER_TX A3
#define DFPLAYER_BUSY_PIN 2

//LED indicador
#define LED_PIN A5
