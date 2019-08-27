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

uint8_t modo;
#define MODO_PH 0
#define MODO_TEMP 1
#define MODO_IDSOL 1

///////////////////////////////////////////////////////////////////////////////
//                   Endereçamento dos Áudios                               //
/////////////////////////////////////////////////////////////////////////////


#define AUDIO_E 1000
#define AUDIO_PONTO 1001

#define AUDIO_CONDUZ 
#define AUDIO_NAO_CONDUZ

#define AUDIO_PH_INIT 1002

#define AUDIO_TEMP_INIT 1003
#define AUDIO_TEMP_FIM 1004

#define AUDIO_INIT_OK 1006

#define AUDIO_PH_SELECT
#define AUDIO_TEMP_SELECT
#define AUDIO_IDSOL_SELECT


///////////////////////////////////////////////////////////////////////////////
//                        Definição dos pinos                               //
/////////////////////////////////////////////////////////////////////////////

//Sensores
#define PH_PIN A1 //Analog IN
#define TERM_PIN A0 //Analog IN
#define IDSOL_PIN 12 //Digital IN

//Botões
#define BT_PH_PIN
#define BT_TEMP_PIN
#define BT_IDSOL_PIN
#define BT_PLAY_PIN

//LCD
#define LCD_RS 8
#define LCD_RW 9
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D3 7

//DFPlayer
#define DFPLAYER_RX 3
#define DFPLAYER_TX 2
#define DFPLAYER_BUSY

//LED indicador de funcionamento
#define LED_PIN

///////////////////////////////////////////////////////////////////////////////
//                          Variáveis Globais                               //
/////////////////////////////////////////////////////////////////////////////

float temp;
float pH;
bool conduz;

///////////////////////////////////////////////////////////////////////////////
//                    Instâncias de objetos auxiliares                      //
/////////////////////////////////////////////////////////////////////////////

Thermistor temp(TERM_PIN);
LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

SoftwareSerial softwareSerial(DFPLAYER_RX,DFPLAYER_TX);
DFRobotDFPlayerMini player;

///////////////////////////////////////////////////////////////////////////////
//                          Funções de Leitura                              //
/////////////////////////////////////////////////////////////////////////////


bool estaConduzindo(){
    return digitalRead(ID_SOL_PIN);
}

float lerTemperatura(){
    return temp.getTemp();
}

float lerPH(){}

///////////////////////////////////////////////////////////////////////////////
//                         Funções de exibição                              //
/////////////////////////////////////////////////////////////////////////////

//Espera indefinidamente até que o DFPlayer esteja initializado
void inicializaAudio(){
    Serial.print("Incializando");
    softwareSerial.begin(9600);
    while(!player.begin(softwareSerial)){
        Serial.print(".");
        digitalWrite(LED_PIN,HIGH);
        delay(250)
        digitalWrite(LED_PIN,LOW);
        delay(250)
    }
    Serial.println();
    digitalWrite(LED_PIN,LOW);
    

    player.setTimeOut(500);
    player.volume(30);
    player.EQ(0);

    Serial.println(player.readFileCounts(DFPLAYER_DEVICE_SD));
}

//Atualiza os valores do display
void atualizaLCD(){
    lcd.setCursor(0,0);
    lcd.print("pH: ");
    lcd.setCursor(5,0);
    lcd.print(pH);

    lcd.setCursor(0,1);
    lcd.print("Temp.: ");
    lcd.setCursor(7,1);
    lcd.print(temperatura);
}

//Reproduz um áudio e espera seu término (Sincronizado)
void falarSync(int audioID){
    player.play(audioID);
    while(digitalRead(DFPLAYER_BUSY_PIN)){}
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

    falarInteiro(inteiro);
    player.play(AUDIO_PONTO);
    falarInteiro(decimal);
}

void falarInteiro(String inteiro){
    if(inteiro.toInt() <= 20)
        falarSync(inteiro.toInt());
    else if(inteiro.length() == 3){
        if(inteiro = "100")
            falarSync(AUDIO_CEM);
        else{
            int centena = (inteiro[0] - '0');
            int dezena  = (inteiro[1] - '0');
            int unidade = (inteiro[2] - '0');

            falarSync(centena*100);
            if(dezena != 0)
                falarSync(dezena*10);
            if(unidade != 0)
                falarSync(unidade);
        }
    }
    else if(inteiro.length() == 2){
        falarSync((inteiro[0] - '0')*10);
        falarSync(inteiro[1] - '0');
    }
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
    pinMode(ID_SOL_PIN,INPUT_PULLUP);
    pinMode(BT_PH_PIN,INPUT_PULLDOWN);
    pinMode(BT_TEMP_PIN,INPUT_PULLDOWN);
    pinMode(BT_IDSOL_PIN,INPUT_PULLDOWN);
    pinMode(BT_PLAY_PIN,INPUT_PULLDOWN);
    pinMode(LED_PIN,OUTPUT);

    Serial.begin(115200);

    lcd.begin(16,2);

    inicializaAudio();
        
    delay(1000);
    digitalWrite(LED_PIN,HIGH);
}

///////////////////////////////////////////////////////////////////////////////
//                          Rotina principal                                //
/////////////////////////////////////////////////////////////////////////////


void loop() { 
    pH = lerPH();
    temp = lerTemperatura();
    conduz = estaConduzindo();

    atualizaLCD();

    if(digitalRead(BT_PH_PIN)){
        modo = MODO_PH;
        falarSync(AUDIO_PH_SELECT);
    }
    else if(digitalRead(BT_TEMP_PIN)){
        modo = MODO_TEMP;
        falarSync(AUDIO_TEMP_SELECT);
    }
    else if(digitalRead(BT_IDSOL_PIN)){
        modo = MODO_IDSOL;
        falarSync(AUDIO_IDSOL_SELECT);
    }
    else if(digitalRead(BT_PLAY_PIN)){
        digitalWrite(LED_PIN,LOW);

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

        digitalWrite(LED_PIN,HIGH);
    }
    
    
}
