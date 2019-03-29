// ---- CONFIGURATION ----
// Options de compilation selon les besoins de l'horloge

//#define alternee // Pour une horloge qui a besoin d'impulsions de polarité altérnées
                 // Sinon on crée des impulsions de même polarité
                 
//#define pas30 // Pour une horloge dont le pas correspondant à une impulsion est de 30s
              // Sinon on considère que c'est 60s

#ifdef pas30
  #define doublepas // Pour une horloge dont le pas est 30s, génère plutôt 2 impulsions rapprochées toutes les 60s 
                      // Sinon comportement par défaut (1 impulsion toutes les 30s)
#endif

#ifdef doublepas
  #define d_pause 100 // Durée pause entre 2 impulsions alternées en millisecondes
#endif

#ifdef alternee
  #define d_pas_1 8  // Durée impulsion sur broche 1 pour les impulsions alternées
  #define d_pas_2 10  // Durée impulsion sur broche 2 pour les impulsions alternées
#else
  #define d_pas 200  // Durée impulsion sur broche 1 pour les impulsions de même polarité
#endif
// ----      FIN      ----

#define reveil 1 // PB1 = PCINT1 = Attiny85 pin physique 6 
                 // Reliée à la pin de réveil du module RTC et au bouton poussoir
                 // ACTIVE LOW: mise à la masse pour déclencher
                 // Ne pas utiliser sur les version de platine < V5
                 
#define broche1 3 // PB2 = Attiny85 pin physique 2
#define broche2 4 // PB4 = Attiny85 pin physique 3
// SDA et SDK non utilisées (initialisées par TinyWireM). Pour mémoire.
#define SDK    2   // PB2 = Attiny85 pin physique 7
#define SDA    0   // PB0 = Attiny85 pin physique 5

#ifndef alternee   // Dans le cas d'impulsions monopolaire, on choisit la broche 2
  #define broche broche2
#endif

#ifdef pas30
  #ifndef doublepas
    #define periode30  // C'est le seul cas où on veut une action toutes les 30s
  #endif
#endif

// Communication I2C
#include <TinyWireM.h>
// Adresse I2C real time clock (RTC) du module DS1307 ou compatible
#define DS3231_ADDR   0x68              

// Gestion des interruptions
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptSettings.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>

#define F_CPU 1000000UL  
// _delay_loop_2 = (F_CPU / 4000) * ms
#define nb_loop_2_ms 250

#include <avr/sleep.h>

 
// Routines to set and clear bits (used in the sleep code)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


#ifdef alternee
bool b2=false;
#endif

void RaZRTC() {
  // réinitialise l'heure du module RTC DS3231 
  TinyWireM.beginTransmission(DS3231_ADDR); 
  // écrit une heure & date fantaisiste (mardi 23 décembre 2003, 21h53m00s)
  // cela démarre le module automatiquement s'il était bloqué
  TinyWireM.send(0x00); // Pointeur addresse HEURE
  TinyWireM.send(0x00); // 00s
  TinyWireM.send(0x53); // 53m
  TinyWireM.send(0x21); // 21h / mode 24h
  TinyWireM.send(0x03); // mardi (semaine anglaise commence un dimanche)
  TinyWireM.send(0x23); // 23
  TinyWireM.send(0x12); // décembre
  TinyWireM.send(0x03); // (20)03
  TinyWireM.endTransmission(); 
}

void RaZAlarmes() {
  TinyWireM.beginTransmission(DS3231_ADDR); 
  TinyWireM.send(0x0f); // Pointeur addresse status alarme
  TinyWireM.send(0x00); // efface toutes les alarmes (entre autres)
  TinyWireM.endTransmission();   
}

void setup() {
  cbi(ADCSRA,ADEN);                    // permute Analog to Digital converter OFF
//sbi(ADCSRA,ADEN);                    // permute Analog to Digital converter ON

  // Quelque soit le mode (alterné ou non), on s'assure de commencer avec les 2 sortie basses 
  pinMode(broche1, OUTPUT);
  digitalWrite(broche1, LOW);
  pinMode(broche2, OUTPUT);
  digitalWrite(broche2, LOW);

  pinMode(reveil, INPUT);  // SQW (interruption de réveil) est active-LOW, donc HIGH en général
                           // Idem pour le bouton poussoir

  // initialise I2C
  TinyWireM.begin();
  attend_ms(100);  // On patiente

  RaZRTC();
    // crée la(les) alarmes
  TinyWireM.beginTransmission(DS3231_ADDR); 
#ifdef periode30
  // Alarme toutes les minutes, à xx:xx:30 A1M1=0 A1M2=1 A1M3=1 A1M4=1
  TinyWireM.send(0x07); // Pointeur addresse alarme 1
  TinyWireM.send(0x30); // A1M1=0 xx:xx:30
  TinyWireM.send(0x80); // A1M2=1 xx:00:xx (valeur des minutes, ici 00, sans importance)
  TinyWireM.send(0x80); // A1M3=1 00:xx:xx (valeur des heures, ici 00, sans importance)
  TinyWireM.send(0x81); // A1M4=1 1=dimanche (valeur de jour, ici 1, sans importance)
#else
  TinyWireM.send(0x0B); // Pointeur addresse alarme 2
#endif
  // Alarme toutes les minutes, à xx:xx:00 A2M2=1 A2M3=1 A2M4=1
  TinyWireM.send(0x80); // A2M2=1 xx:00 (valeur des minutes, ici 00, sans importance)
  TinyWireM.send(0x80); // A2M3=1 00:xx (valeur des heures, ici 00, sans importance)
  TinyWireM.send(0x81); // A2M4=1 1=dimanche (valeur de jour, ici 1, sans importance)
  // Enclenchement des alarmes (addresse 0x0E)
  // EOSC=0 BBSQW=0 CONV=0 RS2=0 RS1=0 INTCN=1 (alarmes surveillées) A2IE=1 (alarme à xx:xx:00 enclenchée) A1IE=? (alarme à xx:xx:30 enclenchée): 0000011?
#ifdef periode30
  TinyWireM.send(0x07);
#else
  TinyWireM.send(0x06);
#endif
  // On arrête le 32kHz et on efface les alarmes (addresse 0x0F)
  TinyWireM.write(0x00);
  TinyWireM.endTransmission(); 

  interruptionsON();
}

void attend_ms(uint32_t n) {
  // _delay_loop_2 = 4 cycles horloge à attendre
  // s / cycle = 1 / F_CPU
  // ms / cycle = 1000 / F_CPU
  // ms / 4 cycles = 4000 / F_CPU
  // 4 cycles / ms = F_CPU / 4000
  // _delay_loop_2 = (F_CPU / 4000) * ms 
  // Attention _delay_loop_2 accepte un uint16_t (max 2^16=65536) correspondant à 262.1 ms à 1Mhz
  // C'est pour ca qu'on utilise le multiplicateur nb_loop_2_ms, défini dans les déclarations
  n=n*nb_loop_2_ms;
  _delay_loop_2( (uint16_t) n);
  n=n>>16;
  for(uint16_t i=(uint16_t) n; i>0; i--) { _delay_loop_2( 0 ); }  // _delay_looop_2( 0 ) correspond en fait à 65536
}

void identifieInterruption() {
  interruptionsOFF();
  // Lit le registre des alarmes
  TinyWireM.beginTransmission(DS3231_ADDR); 
  TinyWireM.send(0x0F); // Pointeur addresse status alarme
  TinyWireM.endTransmission();   
  TinyWireM.requestFrom(DS3231_ADDR,1);
  byte A = 0b00000011;  // Les drapeaux alarmes sont les 2 derniers bits
  A = A & TinyWireM.read();
  if (A) { // S'il n'est pas nul, c'est que c'est une alarme qui a déclenché
    avance();
    RaZAlarmes();
  } else { //sinon c'est un appui poussoir
    regle();
  }
  interruptionsON();
}

void interruptionsON() {
  // On détecte les alarmes: SQW (interruption de réveil) est active-LOW, donc HIGH en général
  attachPinChangeInterrupt(reveil, identifieInterruption, FALLING);
}

void interruptionsOFF() {
  detachPinChangeInterrupt(reveil);
}

void loop() {  // Remet le système en sommeil
    system_sleep();
}

void avance() {
  // Avance différente selon les choix à la compilation
  #ifdef doublepas
    digitalWrite(broche1, HIGH);
    attend_ms(d_pas_1);
    digitalWrite(broche1, LOW);
    attend_ms(d_pause);
    digitalWrite(broche2, HIGH);
    attend_ms(d_pas_2);
    digitalWrite(broche2, LOW);
  #else
    #ifdef alternee
    if (b2) {
      digitalWrite(broche1, HIGH);
      attend_ms(d_pas_1);
      digitalWrite(broche1, LOW);
    } else {
      digitalWrite(broche2, HIGH);
      attend_ms(d_pas_2);
      digitalWrite(broche2, LOW);
    }
    b2=!b2;
  #else
    digitalWrite(broche, HIGH);
    attend_ms(d_pas);
    digitalWrite(broche, LOW);
  #endif
#endif
  interruptionsON();
}

void regle() {
  uint32_t d = 1500;  // Temps de latence en millisecondes entre deux (groupes d')impulsion
  boolean a=true;
  #ifdef periode30
    b2=false;
  #endif
  while (a) {
    avance();
    #ifdef periode30()
      attend_ms(200);
      avance(); 
    #endif
    //delay(d);
    RaZAlarmes();
    attend_ms(d);
    if (d>300) { d *= 95;  d/= 100; }  // Si on laisse appuyé longtemps, on réduit le temps de latence

    a=!digitalRead(reveil); // Vérifie si l'utilisateur appuie encore sur le poussoir
  }
  RaZRTC();
  RaZAlarmes();
}

void system_sleep() {  // Mise en veille, en attente d'une interruption
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sommeil le plus profond
  sleep_enable();
  sei();                               // Assure que les interruptions sortent l'ATTiny de la veille
  sleep_mode();                        // Mise en veille
  // zzz ...
  sleep_disable();                     // Sortie de la veille
  cli();                               // Réactive les interruptions
}
 


