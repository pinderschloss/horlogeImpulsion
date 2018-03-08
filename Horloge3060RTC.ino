#define F_CPU 1000000  // This is used by delay.h library
 
#include <stdlib.h> 
#include <avr/io.h>        // Des constantes utiles
  
#include <avr/sleep.h>
#include <avr/interrupt.h>
 
// Routines to set and claer bits (used in the sleep code)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// COMMUNICATION I2C
#include <TinyWireM.h>

// SDA et SDK non utilisées (initialisées par TinyWireM). Pour mémoire.
#define SDK    2 // Attiny85 pin physique 7
#define SDA    0 // Attiny85 pin physique 5
//
#define mode30 1 // Attiny85 pin physique 6
#define step30 3 // Attiny85 pin physique 2
#define step60 4 // Attiny85 pin physique 3

#define d30 150   // Durée impulsion '30s' en millisecondes
#define d60 150   // Durée impulsion '60s' en millisecondes
#define d3060 250 // Durée pause entre impulsions '30s' et '60s' en millisecondes

// Adresse I2C real time clock (RTC) du module DS1307 ou compatible
#define DS1307_ADDR   0x68              

byte secondes,secRef,secAct,delta;

bool mode30actif;
bool dejafait30;
bool pre30;
byte eco;

volatile boolean f_wdt = 1;

byte lireSec(){
  byte wireRet = 0;

  TinyWireM.beginTransmission(DS1307_ADDR); // reset DS1307 register pointer 
  TinyWireM.send(0); 
  wireRet = TinyWireM.endTransmission(); 
  if (wireRet) { // Problème de communication avec le module RTC
    system_sleep();  // On patiente
  }

  wireRet = TinyWireM.requestFrom(DS1307_ADDR, 1);    // Lit 1 octet du DS1307 (les secondes)

  if (wireRet) { // Problème de communication avec le module RTC
    system_sleep();  // On patiente
  }
  wireRet = TinyWireM.receive();
  wireRet = bcdToDec(wireRet);

//  for (byte i=0;i<(wireRet);i++){digitalWrite(step30,HIGH);delay(100);digitalWrite(step30,LOW);delay(100);}
  return wireRet;  // handle the byte received
}
byte bcdToDec(byte val) {               // Convertir binaire codé décimal en nombre décimal
  return ((val / 16 * 10) + (val % 16));
}

void setup() {
  setup_watchdog(7); // environ 2 secondes de sommeil
  cbi(ADCSRA,ADEN);                    // permute Analog to Digital converter OFF
//sbi(ADCSRA,ADEN);                    // permute Analog to Digital converter ON
  
  pinMode(step30, OUTPUT);
  digitalWrite(step30, LOW);
  pinMode(step60, OUTPUT);
  digitalWrite(step60, LOW);

  pinMode(mode30, INPUT);
  // initialise I2C
  TinyWireM.begin();
  delay(1000);  // On patiente

  // reinitialise DS1307 
  TinyWireM.beginTransmission(DS1307_ADDR); 
  TinyWireM.send(0x00);
  // ecrit une heure/date fantaisiste
  // cela démarre le module automatiquement s'il était bloqué
  TinyWireM.send(0x02);
  TinyWireM.send(0x02);
  TinyWireM.send(0x02);
  TinyWireM.send(0x02);
  TinyWireM.send(0x02);
  TinyWireM.send(0x02);
  TinyWireM.send(0x02);
  TinyWireM.endTransmission(); 

  // Prend une référence de temps
  secRef = lireSec();
  delta = 60 - secRef;
  pre30 = true;
  dejafait30 = false;
  eco = 0;
  secondes = 0;

  system_sleep();  // On patiente
}

void liremode30() {
  mode30actif = digitalRead(mode30);
}

void lireAvance() {
  //lireREGLAGE
  //pinMode(SDK, INPUT_PULLUP); n'est pas utile car TinyWireM l'utilise déjà dans ce mode
  while (!digitalRead(SDK)) {
    avance3060();
    pre30 = true;
    dejafait30 = false;
    eco=0;
    secondes = 0;
    secRef=lireSec();
    delta = 60 - secRef;
    delay(500);
  }
}

// the loop function runs over and over again forever
void loop() {
  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag
    // Détermine si l'utilisateur appuie sur le bouton d'avance rapide
    lireAvance();

    if (eco>12) { // Attend environ (12+1)*2s=26 secondes
      // Se renseigne sur l'heure précise
      secAct=lireSec();
  
      // Determine le nombre de secondes écoulées depuis la référence de temps, ou la référence + 30s:
      secondes=(secAct+delta) % 60;
    }
    //
    if (secondes>30) {
      secondes = 0;
      eco=0;
      if (pre30) {
        delta = 90 - secRef;
        pre30 = false;
        liremode30();
        if (mode30actif) {
          avance30();
          dejafait30 = true;
        }
        
      } else {
        delta = 60 - secRef;
        pre30 = true;
        if (dejafait30) {
          avance60();
          dejafait30 = false;
        } else {
          avance3060();
        }
      }
    } 
    eco++;
    system_sleep();  // On patiente
  }
}

void avance30() {
  digitalWrite(step30, HIGH);
  delay(d30);
  digitalWrite(step30, LOW);
}
void avance60() {
  digitalWrite(step60, HIGH);
  delay(d60);
  digitalWrite(step60, LOW);
}

void avance3060() {
  avance30();
  delay(d3060);
  avance60();
}

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  
 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
 
  sleep_mode();                        // System actually sleeps here
 
  sleep_disable();                     // System continues execution here when watchdog timed out 
  
  
}
 
// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
 
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
 
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}
  
// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
}
