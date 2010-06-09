/* trackuino copyright (C) 2010  EA5HAV Javi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__


// NOTE: all pins are Arduino based, not the Atmega chip. Mapping:
// http://www.arduino.cc/en/Hacking/PinMapping

// --------------------------------------------------------------------------
// APRS config (aprs.c)

/* Some considerations on generic digipeating:
 * (summarized from: http://wa8lmf.net/DigiPaths/ )
 *
 * - Never use RELAY in the digi path. It will probably be ignored anyway.
 * - Use WIDE1-1, WIDE2-1 if you want the aid of home fill-in digis to reach
 *   a 'hilltop' digipeater. With the new WIDEN-n paradigm, most programs are
 *   configured to digipeat packets with "WIDE1-1" in their path.
 * - If a hilltop digi is at reach, WIDE2-2 is the shortest, most optimum
 *   setting.
 * - TTLs > 2 in busy areas are very likely unwelcome and maybe even ignored.
 *
 * And also some considerations on the source SSID 
 * (summarized from: http://zlhams.wikidot.com/aprs-ssidguide )
 *
 * - There is no strict guideline about the source SSID, but one common use
 *   seems to be to redundantly indicate the symbol (already indicated with
 *   the latXlon syntax). This is useful if we are transmitting from two 
 *   stations, to help the network tell them apart. Ie: when chasing a 
 *   balloon (-11) from a car (-9).
 */
#define S_CALLSIGN      "EA5HAV"    // your own callsign
#define S_CALLSIGN_ID   11          // ... and SSID (11 = balloons, 9 = cars, etc.)
#define D_CALLSIGN      "APRS"      // destination callsign
#define D_CALLSIGN_ID   0           // ... and SSID
#define DIGI_PATH1      "WIDE1"     // first digipeater in path (leave undefined if none)
#define DIGI_PATH1_TTL  1           // ... and time-to-live
#define DIGI_PATH2      "WIDE2"     // second digipeater (leave undefined if none)
#define DIGI_PATH2_TTL  1           // ... and time-to-live
#define APRS_COMMENT    "Prueba TRACKUINO"    // this goes in the APRS comment field

// --------------------------------------------------------------------------
// AX.25 config (ax25.cpp)
#define TX_DELAY      300         // TX delay in milliseconds

// --------------------------------------------------------------------------
// Tracker config (trackuino.cpp)

/* APRS broadcast periods: Hopefully CSMA adds enough randomness 
 * already, but odd periods (ie 61 secs) might avoid collision 
 * with other stations transmitting at fixed intervals like us.
 *
 * Low-power transmissions on occasional events (such as a balloon
 * launch) might be okay at lower-than-standard APRS periods (< 10m).
 * Check with/ask permision to local digipeaters beforehand.
 */
#define APRS_PERIOD   61000UL     // APRS period between messages (ms)
#define APRS_DELAY    0UL         // Time (ms) before first APRS message

// --------------------------------------------------------------------------
// Modem config (modem.cpp)
#define PWM_PIN       3           // (out) 3 or 11. 11 conflicts w/ SPI's MOSI

/* Radio: I've tested trackuino with two different radios:
 * Radiometrix HX1 and SRB MX146. The interface with these devices
 * is implemented in their respective radio_*.cpp files, and here
 * you can choose which one will be hooked up to the tracker.
 * The tracker will behave differently depending on the radio used:
 *
 * RadioHx1 (Radiometrix HX1):
 * - Time from PTT-on to transmit: 5ms (per datasheet)
 * - PTT is TTL-level driven (on=high) and audio input is 5v pkpk
 *   filtered and internally DC-coupled by the HX1, so both PTT
 *   and audio can be wired directly. Very few external components
 *   are needed for this radio, indeed.
 *
 * RadioMx146 (SRB MX146):
 * - Time from PTT-on to transmit: signaled by MX146 (pin RDY)
 * - Uses I2C to set freq (144.8 MHz) on start
 * - I2C requires wiring analog pins 4/5 (SDA/SCL) via two level
 *   converters (one for each, SDA and SCL). DO NOT WIRE A 5V ARDUINO
 *   DIRECTLY TO THE 3.3V MX146, YOU WILL DESTROY IT!!!
 *
 *   I2C 5-3.3v LEVEL TRANSLATOR:
 *
 *    +3.3v o--------+-----+      +---------o +5v
 *                   \     |      \
 *               3k3 / R   |    R / 4k7
 *                   \    G|      \
 *                   /   _ L _    /
 *                   |   T T T    |
 *   3.3v device o---+--+|_| |+---+---o 5v device
 *                     S|     |D
 *                      +-|>|-+
 *                             N-channel MOSFET
 *                           (BS170, for instance)
 *
 *   (Explanation of the lever translator:
 *   http://www.neoteo.com/adaptador-de-niveles-para-bus-i2c-3-3v-5v.neo)
 *
 * - Audio needs a low-pass filter (R=8k2 C=0.1u) plus DC coupling
 *   (Cc=10u). This also lowers audio to 500mV peak-peak required by
 *   the MX146.
 *
 *                   8k2        10uF
 *   Arduino out o--/\/\/\---+---||---o
 *                     R     |     Cc
 *                          ===
 *                     0.1uF | C
 *                           v
 *
 * - PTT is pulled internally to 3.3v (off) or shorted (on). Use
 *   an open-collector BJT to drive it:
 *        
 *                             o MX146 PTT
 *                             |
 *                    4k7    b |c
 *   Arduino PTT o--/\/\/\----K  (Any NPN will do)
 *                     R       |e
 *                             |
 *                             v GND
 * 
 * - 
 *
 * - Beware of keying the MX146 for too long, you will BURN it.
 *
 * So, summing up. Options:
 *
 * - RadioMx146
 * - RadioHx1
 */
#define RADIO_CLASS   RadioHx1

// --------------------------------------------------------------------------
// Radio config (radio_*.cpp)
#define PTT_PIN           4           // (out) PTT pin
#define MX146_READY_PIN   2           // (in)  High when the MX146 goes full RF

// --------------------------------------------------------------------------
// Sensors config (modem.cpp)

/* The Arduino reference docs:
 * http://arduino.cc/en/Reference/AnalogReference?from=Reference.AREF
 *
 * External voltages should be connected to AREF through a 5K pull-up.
 * Since there is already a 32K resistor at the ADC pin, the actual
 * voltage read will be VREF * 32 / (32 + AREF_PULLUP)
 *
 * Note that hooking up external voltages to AREF is incompatible with
 * the internal references (1.1v and Vcc), and can even damage the AVR
 * chip if AREF is not pulled through a resistor. 
 */

#define AREF_PULLUP           4700  // AREF pull-up (or 0 if no pull-up)
#define USE_AREF                    // Comment this out if not using AREF
#define INTERNAL_LM60_VS_PIN     6  // (dig out) LM60's Vs
#define INTERNAL_LM60_VOUT_PIN   0  // (ana in)  LM60's output

#endif
