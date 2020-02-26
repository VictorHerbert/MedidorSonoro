///////////////////////////////////////////////////////////////////////////////
//                               Biliotecas                                 //
/////////////////////////////////////////////////////////////////////////////

#include <LiquidCrystal.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Definicoes.cpp";
#include "Thermistor.h";
#include "SMA.hpp";

///////////////////////////////////////////////////////////////////////////////
//                    Instâncias de objetos auxiliares                      //
/////////////////////////////////////////////////////////////////////////////

uint8_t modo = 1;


LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

SoftwareSerial softwareSerial(DFPLAYER_RX,DFPLAYER_TX);
DFRobotDFPlayerMini player;

///////////////////////////////////////////////////////////////////////////////
//                          Funções de Leitura                              //
/////////////////////////////////////////////////////////////////////////////

#define TEMP_MEDIA_MOVEL_COUNT 100
#define PH_MEDIA_MOVEL_COUNT 50

Thermistor term(TERM_PIN, STEINHART_A, STEINHART_B, STEINHART_C);
SMA filtroTemp(TEMP_MEDIA_MOVEL_COUNT);
SMA filtroPH(PH_MEDIA_MOVEL_COUNT);

float temp;
float ph;
bool conduz;
float condutancia;

bool estaConduzindo(){
    condutancia = analogRead(IDSOL_PIN);
    return  condutancia < LIMIAR_CONDUZ;
}

float lerTemperatura(){
    return filtroTemp.updateSamples(term.getTemp());
    //return filtroTemp.updateSamples(analogRead(TERM_PIN));
}

float lerPH(){
    return filtroPH.updateSamples( getPH( analogRead(PH_PIN) ) );
}

void atualizaSensores(){
    ph = lerPH();
    temp = lerTemperatura();
    conduz = estaConduzindo();

    digitalWrite(LED_PIN, conduz);
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

unsigned long previousLCDMillis = millis();
unsigned long currentLCDMillis = millis();

#define LCD_update_time 1000

//Atualiza os valores do display
void atualizaLCD(){
    currentLCDMillis = millis();
    if(currentLCDMillis - previousLCDMillis <= LCD_update_time) return;

    Serial.print(temp);
    Serial.print(" ");
    Serial.print(analogRead(TERM_PIN));
    Serial.print(" ");
    Serial.println(condutancia);
    
    previousLCDMillis = millis();
    
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
    
    falarSync(inteiro.toInt());
    falarSync(AUDIO_PONTO);
    falarSync(decimal.toInt());
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
    Serial.println("Incializando...");
    
    //pinMode(IDSOL_PIN,INPUT);
    pinMode(BT_PH_PIN,INPUT_PULLUP);
    pinMode(BT_TEMP_PIN,INPUT_PULLUP);
    pinMode(BT_IDSOL_PIN,INPUT_PULLUP);
    pinMode(LED_PIN,OUTPUT);

    lcd.begin(16,2);

    inicializaAudio();
    
        
    delay(1000);    
    Serial.println("Inicializado");
    
    
}



///////////////////////////////////////////////////////////////////////////////
//                          Rotina principal                                //
/////////////////////////////////////////////////////////////////////////////

void loop() { 
    
    atualizaSensores();
    atualizaLCD();
    
    if(!digitalRead(BT_PH_PIN)){
        Serial.println(modo);
        if(modo == MODO_PH){
            falarSync(AUDIO_PH_INIT);
            falarFloat(ph);
        }
        else{
            modo = MODO_PH;
            Serial.println("PH");
            falarSync(AUDIO_PH_SELECT);
        }
    }
    else if(!digitalRead(BT_TEMP_PIN)){
        Serial.println(modo);
        if(modo == MODO_TEMP){
            falarSync(AUDIO_TEMP_INIT);
            falarFloat(temp);
            falarSync(AUDIO_TEMP_FIM);
        }
        else{
            modo = MODO_TEMP;
            Serial.println("TEMP");
            falarSync(AUDIO_TEMP_SELECT);
        }
    }
    else if(!digitalRead(BT_IDSOL_PIN)){
        Serial.println(modo);
        if(modo == MODO_IDSOL){
          falarConduz();
        }
        else{
          modo = MODO_IDSOL;
          Serial.println("IDSOL");
          falarSync(AUDIO_IDSOL_SELECT);
        }
    }

}
