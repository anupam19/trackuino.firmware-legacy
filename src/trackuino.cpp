/*
 *  trackuino.cpp
 *  trackuino
 *
 *  Created by javi on 30/01/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "modem.h"
#include "gps.h"
#include "aprs.h"
#include "trackuino.h"
#include "radio_mx146.h"
#include "WProgram.h"
#include <SoftwareSerial.h>
#include <avr/power.h>
#include <avr/sleep.h>

Gps gps;
unsigned long next_tx_millis;


/* APRS broadcast periods: Hopefully CSMA adds enough randomness 
 * already, but odd periods (ie 61 secs) might avoid collision 
 * with other stations transmitting at fixed intervals like us.
 *
 * Low-power transmissions on occasional events (such as a balloon
 * launch) might be okay at lower-than-standard APRS periods (< 10m).
 * Maybe check with/ask permision to local digipeaters beforehand?
 */
const unsigned long APRS_PERIOD_DEBUG     = 5000UL;   // Debug: 5 secs
const unsigned long APRS_PERIOD_BALLOON   = 61000UL;  // Balloon tracking: 1m 1s
const unsigned long APRS_PERIOD_STD       = 601000UL; // Standard APRS: 10m (+ 1s)
const unsigned long APRS_PERIOD           = APRS_PERIOD_BALLOON;

/* An initial delay will allow the gps acquire a fix */
const unsigned long APRS_DELAY            = 30000UL;  // 30 secs

void disable_bod_and_sleep()
{
  /* This will turn off brown-out detection while
   * sleeping. Unfortunately this won't work in IDLE mode.
   * Relevant info about BOD disabling: datasheet p.44
   *
   * Procedure to disable the BOD:
   *
   * 1. BODSE and BODS must be set to 1
   * 2. Turn BODSE to 0
   * 3. BODS will automatically turn 0 after 4 cycles
   *
   * The catch is that we *must* go to sleep between 2
   * and 3, ie. just before BODS turns 0.
   */
  unsigned char mcucr;

  cli();
  mcucr = MCUCR | (_BV(BODS) | _BV(BODSE));
  MCUCR = mcucr;
  MCUCR = mcucr & (~_BV(BODSE));
  sei();
  sleep_mode();    // Go to sleep
}

void power_save()
{
  /* Enter power saving mode. SLEEP_MODE_IDLE is the least saving
   * mode, but it's the only one that will keep the UART running.
   * In addition, we need timer0 to keep track of time and timer2
   * to keep pwm output at its rest voltage.
   */

  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  power_adc_disable();
  power_spi_disable();
  power_twi_disable();
  if (! modem_busy()) {  // Don't let timer 1 sleep if we're txing.
    power_timer1_disable();
  }

  sleep_mode();    // Go to sleep
  
  sleep_disable();  // Resume after wake up
  power_all_enable();
}


void setup()
{
  Serial.begin(9600);
  radio_mx146_setup();
  modem_setup();
  next_tx_millis = millis(); // + APRS_DELAY;
}

void loop()
{
  int c;
  bool valid_gps_data;

  if (millis() >= next_tx_millis) {
    aprs_send(gps);
    next_tx_millis = millis() + APRS_PERIOD;
  }
  
  if (Serial.available()) {
    c = Serial.read();
    valid_gps_data = gps.encode(c);
  } else {
    power_save();
  }
}
