// -------------------------------------------------------
// CTCSS Encoder by Henk Hamoen, PA3GUO, The Netherlands.
// March 2016
//
// Updated by Olivier Mehani VK7SHM for use as a replacement
// encoder circuit for the Z281 module in AWA RT85s.
// May 2025
//
// Credits CtcssTone libraries: Jan van Lienden PE1CID.
// -------------------------------------------------------

#include "CtcssToneId.h"
#include "CtcssTone.h"

// variables that define the inputs & outputs of the Arduino
// -------------------------------------------------------
const int tone_out =  12; // hadcoded in CtcssTone.cpp
const int strobe = 2;
const int D0 = 3;
const int D1 = 4;
const int D2 = 5;
const int D3 = 6;
const int D4 = 7;
const int LED = LED_BUILTIN; //13

byte ctcss_code = 22;
byte last_code = 0;
Tone_Id current_tone;
volatile byte state = LOW;

long loop_count = 0;

Tone_Id tone_map[] = {
    tone_no_tone,
    tone_ctcss_A,
    tone_ctcss_B,
    tone_ctcss_C,
    tone_ctcss_D,
    tone_ctcss_E,
    tone_ctcss_F,
    tone_ctcss_G,
    tone_ctcss_H,
    tone_ctcss_I,
    tone_ctcss_J,
    tone_ctcss_K,
    tone_ctcss_L,
    tone_ctcss_M,
    tone_ctcss_N,
    tone_ctcss_O,
    tone_ctcss_P,
    tone_ctcss_Q,
    tone_ctcss_R,
    tone_ctcss_S,
    tone_ctcss_T,
    tone_ctcss_U,
    tone_ctcss_V,
    tone_ctcss_W,
    tone_ctcss_X,
    tone_ctcss_Y,
    tone_ctcss_Z,
    tone_ctcss_AA,
    tone_ctcss_AB,
    tone_ctcss_AC,
    tone_ctcss_AD,
    tone_ctcss_AE,
    tone_ctcss_AF,
    tone_ctcss_AG,
    tone_ctcss_AH,
    tone_ctcss_AI,
    tone_ctcss_AJ,
    tone_ctcss_AK,
    tone_ctcss_AL
};

// INT0 handler
void int_strobe()
//ISR(INT0_vect)
{
  int tmp_code = 0;

  tmp_code = (
    digitalRead(D0)
    + digitalRead(D1) << 1
    + digitalRead(D2) << 2
    + digitalRead(D3) << 3
    + digitalRead(D4) << 4
  );

  ctcss_code = tmp_code;
  state = !state;
}

void setup() 
{                
  // configure the inputs and outputs
  // -------------------------------------
  //  pinMode(tone_out, OUTPUT);  // Configured by CtcssTone.cpp
  pinMode(LED, OUTPUT); 

  pinMode(strobe, INPUT);
  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);

  // configure interrupt
  attachInterrupt(digitalPinToInterrupt(strobe), int_strobe, RISING);

  Serial.begin(9600);
  
  // initialise the CTCSS tone generation 
  // -------------------------------------
  CtcssTone.init();

  Serial.println("CTCSS Encoder ready.");
}

// main program loop
// -------------------------------------

void loop() 
{
   digitalWrite (LED, state ) ;

   if(++loop_count > 1024000) {
     Serial.print("Looping with ");    
     Serial.println(ctcss_code);
     loop_count = 0;
   }
   
   if (ctcss_code == last_code) {
      return;
   }

   last_code = ctcss_code;
   
   Serial.print("New code received: ");
   Serial.println(ctcss_code);
   
   current_tone = tone_map[ctcss_code];
   CtcssTone.tone_on(current_tone);

   return;
}
