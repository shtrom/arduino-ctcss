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

byte ctcss_code = tone_ctcss_V;
byte last_code = ctcss_code;
Tone_Id current_tone;
volatile byte state = LOW;

long loop_count = 0;

Tone_Id tone_map[] = {
    tone_no_tone,  /*  0 -> None  */
    tone_ctcss_AK, /*  1 -> AK/241.8 */
    tone_ctcss_AJ, /*  2 -> AJ/233.6 */
    tone_ctcss_AI, /*  3 -> AI/225.7 */
    tone_ctcss_AH, /*  4 -> AH/218.1 */
    tone_ctcss_AG, /*  5 -> AG/210.7 */
    tone_ctcss_AF, /*  6 -> AF/203.5 */
    tone_ctcss_AE, /*  7 -> AE/192.8 (maybe) */
    tone_ctcss_AD, /*  8 -> AD/186.2 */
    tone_ctcss_AC, /*  9 -> AC/179.9 */
    tone_ctcss_AB, /* 10 -> AB/173.8 */
    tone_ctcss_AA, /* 11 -> AA/167.9 */
    tone_ctcss_Z,  /* 12 -> Z/162.2 */
    tone_ctcss_Y,  /* 13 -> Y/156.7 */
    tone_ctcss_X,  /* 14 -> X/151.4 */
    tone_ctcss_W,  /* 15 -> W/146.2 [VK linked repeaters] */
    tone_ctcss_V,  /* 16 -> V/141.3 [VK linked repeaters]*/
    tone_ctcss_U,  /* 17 -> U/136.5 (maybe)*/
    tone_ctcss_T,  /* 18 -> T/131.8 */
    tone_ctcss_S,  /* 19 -> S/127.3 */
    tone_ctcss_R,  /* 20 -> R/123.0 [VK X-band repeaters, obsolete ]*/
    tone_ctcss_Q,  /* 21 -> Q/118.8 */
    tone_ctcss_P,  /* 22 -> P/114.8 */
    tone_ctcss_O,  /* 23 -> O/110.9 */
    tone_ctcss_N,  /* 24 -> N/107.2 */
    tone_ctcss_M,  /* 25 -> M/103.5 */
    tone_ctcss_L,  /* 26 -> L/100.0 */
    tone_ctcss_J,  /* 27 -> J/94.8 */
    tone_ctcss_I,  /* 28 -> I/91.5 [VK repeaters] (orig H/88.5) */
    tone_ctcss_G,  /* 29 -> G/85.4 [VK X-band repeaters] (orig F/82.5) */
    tone_ctcss_D,  /* 30 -> D/77.0 */
    tone_ctcss_B,  /* 31 -> B/71.9 */
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
  attachInterrupt(digitalPinToInterrupt(strobe), int_strobe, RISING); // HD14175BP strobes on rising clock signal

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
