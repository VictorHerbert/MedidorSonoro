///////////////////////////////////////////////////////////////////////////////
//                               Biliotecas                                 //
/////////////////////////////////////////////////////////////////////////////

#include <LiquidCrystal.h>
#include "Thermistor.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

///////////////////////////////////////////////////////////////////////////////
//                          Modos de operação                               //
/////////////////////////////////////////////////////////////////////////////

uint8_t modo = 1;
#define MODO_PH 0
#define MODO_TEMP 1
#define MODO_IDSOL 2

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
#define PH_PIN A1 //Analog IN
#define TERM_PIN A0 //Analog IN
#define IDSOL_PIN 12 //Digital IN

//Botões
#define BT_PH_PIN 10
#define BT_TEMP_PIN 11
#define BT_IDSOL_PIN 12
#define BT_PLAY_PIN 13

//LCD
#define LCD_RS 8
#define LCD_RW 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

//DFPlayer
#define DFPLAYER_RX 3
#define DFPLAYER_TX 2
#define DFPLAYER_BUSY_PIN A3

//LED indicador de funcionamento
#define LED_PIN LED_BUILTIN 

///////////////////////////////////////////////////////////////////////////////
//                          Variáveis Globais                               //
/////////////////////////////////////////////////////////////////////////////

float temp;
float ph;
bool conduz;

///////////////////////////////////////////////////////////////////////////////
//                    Instâncias de objetos auxiliares                      //
/////////////////////////////////////////////////////////////////////////////

Thermistor term(TERM_PIN);
LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

SoftwareSerial softwareSerial(DFPLAYER_RX,DFPLAYER_TX);
DFRobotDFPlayerMini player;

///////////////////////////////////////////////////////////////////////////////
//                          Funções de Leitura                              //
/////////////////////////////////////////////////////////////////////////////

float precisaoDecimal(float f, int p = 4){
  int esc = 100*p;
  return round(esc*f)/(esc);
}

bool estaConduzindo(){
    return digitalRead(IDSOL_PIN);
}

#define TEMP_MEDIA_MOVEL_COUNT 100

float temps[TEMP_MEDIA_MOVEL_COUNT];
int tempIndex = 0;
float medTemp=0;

float lerTemperatura(){
    float nTemp = term.getTemp();

    medTemp += (nTemp-temps[tempIndex])/TEMP_MEDIA_MOVEL_COUNT;
    temps[tempIndex] = nTemp;
    tempIndex = (tempIndex+1)%TEMP_MEDIA_MOVEL_COUNT;
    
    return medTemp;        
}

float lerPH(){}

void atualizaSensores(){
    ph = lerPH();
    temp = lerTemperatura();
    conduz = estaConduzindo();
}

///////////////////////////////////////////////////////////////////////////////
//                         Funções de exibição                              //
/////////////////////////////////////////////////////////////////////////////

//Espera indefinidamente até que o DFPlayer esteja initializado
void inicializaAudio(){
    softwareSerial.begin(9600);
    while(!player.begin(softwareSerial)){
        Serial.print(".");
        digitalWrite(LED_PIN,HIGH);
        delay(250);
        digitalWrite(LED_PIN,LOW);
        delay(250);

    }
    digitalWrite(LED_PIN,LOW);
    

    player.setTimeOut(500);
    player.volume(30);
    player.EQ(0);

}

//Atualiza os valores do display
void atualizaLCD(){
    lcd.setCursor(0,0);
    lcd.print("pH: ");
    lcd.setCursor(5,0);
    lcd.print(ph);

    lcd.setCursor(0,1);
    lcd.print("Temp.: ");
    lcd.setCursor(7,1);
    lcd.print(temp);
}

int correcaoIndice(int i){
  if (i>40)
    i++;
  
  return (171+i)%212;
}

//Reproduz um áudio e espera seu término (Sincronizado)
void falarSync(int audioID){
    audioID = correcaoIndice(audioID);
    
    player.play(audioID);
    while(!digitalRead(DFPLAYER_BUSY_PIN)){
      atualizaSensores();
      atualizaLCD();
      Serial.println(".");
    }
    
    unsigned long previousMillis = millis();
    unsigned long currentMillis = millis();
    while (currentMillis - previousMillis <= 2000) {
      currentMillis = millis();
      
      atualizaSensores();
      atualizaLCD();
    }

    return;
}

//
void falarFloat(float v){
    
    //   Divide o valor de float em duas strings
    //   contendo partes inteira e decimal
    String s = String(v,2);
    int posPonto = s.indexOf('.');
    String inteiro = s.substring(0,posPonto);
    String decimal = s.substring(posPonto+1);
    Serial.println(v);
    Serial.println(inteiro.toInt());
    Serial.println(decimal.toInt());
    
    //falarInteiro(inteiro);
    falarSync(inteiro.toInt());
    falarSync(AUDIO_PONTO);
    falarSync(decimal.toInt());
    //falarInteiro(decimal);
}

void falarConduz(){
    if(conduz)
        falarSync(AUDIO_CONDUZ);
    else
        falarSync(AUDIO_NAO_CONDUZ);
}

///////////////////////////////////////////////////////////////////////////////
//                             Inicialização                                //
/////////////////////////////////////////////////////////////////////////////


void setup() {
    
    Serial.begin(115200);
    Serial.print("Incializando...");
    
    pinMode(IDSOL_PIN,INPUT_PULLUP);
    pinMode(BT_PH_PIN,INPUT_PULLUP);
    pinMode(BT_TEMP_PIN,INPUT_PULLUP);
    pinMode(BT_IDSOL_PIN,INPUT_PULLUP);
    pinMode(BT_PLAY_PIN,INPUT_PULLUP);
    pinMode(LED_PIN,OUTPUT);

    lcd.begin(16,2);

    inicializaAudio();
    
        
    delay(1000);
    digitalWrite(LED_PIN,HIGH);
    
    falarSync(AUDIO_INIT_OK);
    Serial.println("Inicializado");
}

///////////////////////////////////////////////////////////////////////////////
//                          Rotina principal                                //
/////////////////////////////////////////////////////////////////////////////


void loop() { 
    atualizaSensores();
    atualizaLCD();

    if(!digitalRead(BT_PH_PIN)){
        modo = MODO_PH;
        Serial.println("PH");
        falarSync(AUDIO_PH_SELECT);
    }
    else if(!digitalRead(BT_TEMP_PIN)){
        modo = MODO_TEMP;
        Serial.println("TEMP");
        falarSync(AUDIO_TEMP_SELECT);
    }
    else if(!digitalRead(BT_IDSOL_PIN)){
        modo = MODO_IDSOL;
        Serial.println("IDSOL");
        falarSync(AUDIO_IDSOL_SELECT);
    }
    else if(!digitalRead(BT_PLAY_PIN)){
        digitalWrite(LED_PIN,LOW);

        Serial.println("PLAY");

        switch (modo){
        case MODO_PH:
            falarSync(AUDIO_PH_INIT);
            falarFloat(ph);
            break;
        case MODO_TEMP:
            falarSync(AUDIO_TEMP_INIT);
            falarFloat(temp);
            falarSync(AUDIO_TEMP_FIM);
            break;
        case MODO_IDSOL:
            falarConduz();
            break;
        default:
            break;
        }
        

        digitalWrite(LED_PIN, HIGH);
    }
    
}
