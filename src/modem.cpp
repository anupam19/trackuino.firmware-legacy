/*
 *  modem.cpp
 *  trackuino
 *
 *  Created by javi on 29/01/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "modem.h"

#include "WProgram.h"


// Credit to: http://www.arduino.cc/playground/Code/PCMAudio
// Credit to: http://arcfn.com/2009/07/secrets-of-arduino-pwm.html


#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>


const unsigned char sine_table[] = {
  127, 129, 130, 132, 133, 135, 136, 138, 139, 141, 143, 144, 146, 147, 149, 150, 152, 153, 155, 156, 158, 
  159, 161, 163, 164, 166, 167, 168, 170, 171, 173, 174, 176, 177, 179, 180, 182, 183, 184, 186, 187, 188, 
  190, 191, 193, 194, 195, 197, 198, 199, 200, 202, 203, 204, 205, 207, 208, 209, 210, 211, 213, 214, 215, 
  216, 217, 218, 219, 220, 221, 223, 224, 225, 226, 227, 228, 228, 229, 230, 231, 232, 233, 234, 235, 236, 
  236, 237, 238, 239, 239, 240, 241, 242, 242, 243, 244, 244, 245, 245, 246, 247, 247, 248, 248, 249, 249, 
  249, 250, 250, 251, 251, 251, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 
  254, 254, 255, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 252, 252, 252, 251, 
  251, 251, 250, 250, 249, 249, 249, 248, 248, 247, 247, 246, 245, 245, 244, 244, 243, 242, 242, 241, 240, 
  239, 239, 238, 237, 236, 236, 235, 234, 233, 232, 231, 230, 229, 228, 228, 227, 226, 225, 224, 223, 221, 
  220, 219, 218, 217, 216, 215, 214, 213, 211, 210, 209, 208, 207, 205, 204, 203, 202, 200, 199, 198, 197, 
  195, 194, 193, 191, 190, 188, 187, 186, 184, 183, 182, 180, 179, 177, 176, 174, 173, 171, 170, 168, 167, 
  166, 164, 163, 161, 159, 158, 156, 155, 153, 152, 150, 149, 147, 146, 144, 143, 141, 139, 138, 136, 135, 
  133, 132, 130, 129, 127, 125, 124, 122, 121, 119, 118, 116, 115, 113, 111, 110, 108, 107, 105, 104, 102, 
  101,  99,  98,  96,  95,  93,  91,  90,  88,  87,  86,  84,  83,  81,  80,  78,  77,  75,  74,  72,  71, 
   70,  68,  67,  66,  64,  63,  61,  60,  59,  57,  56,  55,  54,  52,  51,  50,  49,  47,  46,  45,  44, 
   43,  41,  40,  39,  38,  37,  36,  35,  34,  33,  31,  30,  29,  28,  27,  26,  26,  25,  24,  23,  22, 
   21,  20,  19,  18,  18,  17,  16,  15,  15,  14,  13,  12,  12,  11,  10,  10,   9,   9,   8,   7,   7, 
    6,   6,   5,   5,   5,   4,   4,   3,   3,   3,   2,   2,   2,   1,   1,   1,   1,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1, 
    2,   2,   2,   3,   3,   3,   4,   4,   5,   5,   5,   6,   6,   7,   7,   8,   9,   9,  10,  10,  11, 
   12,  12,  13,  14,  15,  15,  16,  17,  18,  18,  19,  20,  21,  22,  23,  24,  25,  26,  26,  27,  28, 
   29,  30,  31,  33,  34,  35,  36,  37,  38,  39,  40,  41,  43,  44,  45,  46,  47,  49,  50,  51,  52, 
   54,  55,  56,  57,  59,  60,  61,  63,  64,  66,  67,  68,  70,  71,  72,  74,  75,  77,  78,  80,  81, 
   83,  84,  86,  87,  88,  90,  91,  93,  95,  96,  98,  99, 101, 102, 104, 105, 107, 108, 110, 111, 113, 
  115, 116, 118, 119, 121, 122, 124, 125  
/*
  // Sine wave 0.6 .. 5 volts
  142, 143, 145, 146, 147, 149, 150, 152, 153, 154, 156, 157, 158, 160, 161, 162, 
  164, 165, 166, 168, 169, 170, 172, 173, 174, 176, 177, 178, 180, 181, 182, 183, 
  185, 186, 187, 188, 190, 191, 192, 193, 195, 196, 197, 198, 199, 200, 202, 203, 
  204, 205, 206, 207, 208, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 
  221, 222, 223, 224, 225, 226, 226, 227, 228, 229, 230, 231, 232, 232, 233, 234, 
  235, 235, 236, 237, 238, 238, 239, 240, 240, 241, 242, 242, 243, 243, 244, 244, 
  245, 246, 246, 247, 247, 247, 248, 248, 249, 249, 249, 250, 250, 250, 251, 251, 
  251, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 
  254, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 252, 252, 252, 252, 252, 
  251, 251, 251, 250, 250, 250, 249, 249, 249, 248, 248, 247, 247, 247, 246, 246, 
  245, 244, 244, 243, 243, 242, 242, 241, 240, 240, 239, 238, 238, 237, 236, 235, 
  235, 234, 233, 232, 232, 231, 230, 229, 228, 227, 226, 226, 225, 224, 223, 222, 
  221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 208, 207, 206, 205, 
  204, 203, 202, 200, 199, 198, 197, 196, 195, 193, 192, 191, 190, 188, 187, 186, 
  185, 183, 182, 181, 180, 178, 177, 176, 174, 173, 172, 170, 169, 168, 166, 165, 
  164, 162, 161, 160, 158, 157, 156, 154, 153, 152, 150, 149, 147, 146, 145, 143, 
  142, 141, 139, 138, 137, 135, 134, 132, 131, 130, 128, 127, 126, 124, 123, 122, 
  120, 119, 118, 116, 115, 114, 112, 111, 110, 108, 107, 106, 104, 103, 102, 101, 
   99,  98,  97,  96,  94,  93,  92,  91,  89,  88,  87,  86,  85,  84,  82,  81, 
   80,  79,  78,  77,  76,  74,  73,  72,  71,  70,  69,  68,  67,  66,  65,  64, 
   63,  62,  61,  60,  59,  58,  58,  57,  56,  55,  54,  53,  52,  52,  51,  50, 
   49,  49,  48,  47,  46,  46,  45,  44,  44,  43,  42,  42,  41,  41,  40,  40, 
   39,  38,  38,  37,  37,  37,  36,  36,  35,  35,  35,  34,  34,  34,  33,  33, 
   33,  32,  32,  32,  32,  32,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31, 
   31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  31,  32,  32,  32,  32,  32, 
   33,  33,  33,  34,  34,  34,  35,  35,  35,  36,  36,  37,  37,  37,  38,  38, 
   39,  40,  40,  41,  41,  42,  42,  43,  44,  44,  45,  46,  46,  47,  48,  49, 
   49,  50,  51,  52,  52,  53,  54,  55,  56,  57,  58,  58,  59,  60,  61,  62, 
   63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  76,  77,  78,  79, 
   80,  81,  82,  84,  85,  86,  87,  88,  89,  91,  92,  93,  94,  96,  97,  98, 
   99, 101, 102, 103, 104, 106, 107, 108, 110, 111, 112, 114, 115, 116, 118, 119, 
  120, 122, 123, 124, 126, 127, 128, 130, 131, 132, 134, 135, 137, 138, 139, 141
*/
};

// Globals
int current_sample_in_baud;
int go = 0;
int test = 0;
unsigned int phase_delta;     // 1200/2200 for standard AX.25
unsigned int phase;           // Fixed point 9.7 (2PI = TABLE_SIZE)
int packet_pos;
int test_amplitude;
int test_frequency;
int packet_size = 0;
unsigned char packet[512];

/* TODO: Since we're so close to the pwm frequency (F_CPU/256 = 62500 Hz),
 * why not using timer2's interrupt to feed samples to OCR2x at the exact
 * PWM rate and do without timer1 whatsoever? HINT: pre-calculate the next
 * value at the previous interrupt so that the new value is quickly fed
 * at the beginning of the ISR. Otherwise we might miss the counter.
 */

// Constants
const int REST_DUTY                 = sine_table[0];
const int TABLE_SIZE                = sizeof(sine_table);
const unsigned long PLAYBACK_RATE   = 48000;
const int TIMER1_DIVIDER            = F_CPU / PLAYBACK_RATE;
const int LED_PIN                   = 13;
const int SPEAKER_PIN               = 3;
const int PTT_PIN                   = 4;
const int BAUD_RATE                 = 1200;
const int SAMPLES_PER_BAUD          = (PLAYBACK_RATE / BAUD_RATE);
const unsigned int PHASE_DELTA_1200 = (((TABLE_SIZE * 1200L) << 7) / PLAYBACK_RATE); // Fixed point 9.7
const unsigned int PHASE_DELTA_2200 = (((TABLE_SIZE * 2200L) << 7) / PLAYBACK_RATE);


/* The sine_table is the carrier signal. To achieve phase continuity, each tone
 * starts at the index where the previous one left off. By changing the stride of
 * the index (phase_delta) we get 1200 or 2200 Hz. The PHASE_DELTA_XXXX values
 * can be calculated as:
 * 
 * Fg = frequency of the output tone (1200 or 2200)
 * Fm = sampling rate (PLAYBACK_RATE_HZ)
 * Tt = sine table size (TABLE_SIZE)
 * 
 * PHASE_DELTA_Fg = Tt*(Fg/Fm)
 */

void modem_setup()
{
  pinMode(PTT_PIN, OUTPUT);
  digitalWrite(PTT_PIN, LOW);
  pinMode(SPEAKER_PIN, OUTPUT);

  // Set up Timer 2 to do pulse width modulation on the speaker
  // pin.
  
  // Use internal clock (datasheet p.160)
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  
  // Set fast PWM mode  (p.157)
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);

#if 0  
  // Do non-inverting PWM on pin OC2A (arduino pin 11) (p.159)
  // OC2B (arduino pin 3) stays in normal port operation:
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~(_BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0));
#endif  

  // Do non-inverting PWM on pin OC2B (arduino pin 3) (p.159).
  // OC2A (arduino pin 11) stays in normal port operation: 
  // COM2B1=1, COM2B0=0, COM2A1=0, COM2A0=0
  TCCR2A = (TCCR2A | _BV(COM2B1)) & ~(_BV(COM2B0) | _BV(COM2A1) | _BV(COM2A0));
  
  // No prescaler (p.162)
  TCCR2B = (TCCR2B & ~(_BV(CS22) | _BV(CS21))) | _BV(CS20);

  // Set initial pulse width to the rest position (0v after DC decoupling)
  OCR2B = REST_DUTY;


  // Set up Timer 1 to send a sample every interrupt.

  // Set CTC mode (Clear Timer on Compare Match) (p.133)
  // Have to set OCR1A *after*, otherwise it gets reset to 0!
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

  // Set the compare register (OCR1A).
  // OCR1A is a 16-bit register, so we have to do this with
  // interrupts disabled to be safe.
  cli();
  OCR1A = TIMER1_DIVIDER;
  sei();
}

void modem_start()
{
  // Enable interrupt when TCNT1 == OCR1A (p.136)
  TIMSK1 |= _BV(OCIE1A);

  // Start the per-sample timer without prescaler (p.137)
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

  // Key the radio
  digitalWrite(PTT_PIN, HIGH);

  digitalWrite(LED_PIN, HIGH);
  delay(25); // TODO: move this to radio_mx146
}


void modem_stop()
{
  // Output 0v (after DC coupling)
  OCR2B = REST_DUTY;

  // Release PTT
  digitalWrite(PTT_PIN, LOW);
  
  // Disable playback per-sample interrupt.
  TIMSK1 &= ~_BV(OCIE1A);
  
  // Disable the per-sample timer completely.
  TCCR1B &= ~_BV(CS10);

  digitalWrite(LED_PIN, LOW);
}

int
modem_busy()
{
  return go || test;
}

void modem_flush_frame()
{
  phase_delta = PHASE_DELTA_1200;
  phase = 0;
  packet_pos = 0;
  current_sample_in_baud = 0;
  go = 1;
  
  modem_start();
}

void
modem_test()
{
  test_amplitude = 0;
  test_frequency = 1;
  test = 1;
  
  modem_start();
}


// This is called at PLAYBACK_RATE Hz to load the next sample.
ISR(TIMER1_COMPA_vect) {
  if (go) {
    if (packet_pos == packet_size) {
      go = 0;
      modem_stop();
      return;
    }
    // If sent SAMPLES_PER_BAUD already, go to the next bit
    if (current_sample_in_baud == 0) {
      if ((packet[packet_pos >> 3] & (1 << (packet_pos & 7))) == 0) {
        // Toggle tone (1200<>2200)
        phase_delta ^= (PHASE_DELTA_1200 ^ PHASE_DELTA_2200);
      }
    }
    
    phase += phase_delta;
    OCR2B = sine_table[(phase >> 7) & (TABLE_SIZE - 1)];
    
        if(++current_sample_in_baud == SAMPLES_PER_BAUD) {
      current_sample_in_baud = 0;
      packet_pos++;
    }
    return;
  }

  if (test) {
    // Output sawteeth signals of increasing frequency. Be done
    // after covering the freq range 0-255
    if (test_frequency > 255) {
      OCR2B = 127;
      modem_stop();
      test_amplitude = 0;
      return;
    }
    if (test_amplitude > 255) {
      test_amplitude = 0;
      test_frequency++;
    }
    OCR2B = test_amplitude;
    test_amplitude += test_frequency;
    return;
  }
}

