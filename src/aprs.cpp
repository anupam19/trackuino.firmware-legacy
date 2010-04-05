/*
 *  aprs.cpp
 *  trackuino
 *
 *  Created by javi on 19/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ax25.h"
#include "gps.h"
#include "aprs.h"

void aprs_send(Gps &gps)
{
  /* Some considerations on generic digipeating:
   * (summarized from: http://wa8lmf.net/DigiPaths/ )
   *
   * - RELAY digis cause too much QRM and may be ignored by wide area (ie. hilltop) digis, if
   *   not anyone nowadays.
   * - A digi callsign, if known, can be used instead of WIDEn-N to avoid double-digipeating
   *   when two digis are at reach.
   * - Home user digis use the alias WIDE1-1 (not RELAY anymore), so if we need the aid of a home
   *   fill-in digi, WIDE1-1,WIDE2-1 is the way to go.
   * - If a hilltop digi is at reach, WIDE2-2 is the shortest, most optimum setting.
   *
   * And also some considerations on the source SSID 
   * (summarized from: http://zlhams.wikidot.com/aprs-ssidguide )
   *
   * - There is no strict guideline about the source SSID, but one common use is to redundantly
   *   indicate the symbol (already indicated with the latXlon syntax). This is useful if we are
   *   transmitting from two stations, to help the network tell them apart. Ie: when chasing
   *   a balloon (-11) from a car (-9).
   */
  const struct s_address addresses[] = { 
    {"APRS",    0},  // Destination callsign
    {"EA5HAV", 11},  // Source callsign (-11 = balloon, -9 = car)
    {"WIDE2",   2}  // Digi1 (first digi in the chain)
  };

  ax25_send_header(addresses, 3);
  ax25_send_string("/");              // Report w/ timestamp, no APRS messaging. $ = NMEA raw data
  // ax25_send_string("021709z");     // 021709z = 2nd day of the month, 17:09 zulu (UTC/GMT)
  ax25_send_string(gps.get_time());   // 170915h = 17h:09m:15s zulu (not allowed in Status Reports)
  ax25_send_string(gps.get_lat());    // Lat: 38deg and 22.20 min (.20 are NOT seconds, but 1/100th of minutes)
  ax25_send_string("/");              // Symbol table
  ax25_send_string(gps.get_lon());    // Lon: 000deg and 25.80 min
  ax25_send_string("O");              // Symbol: O=balloon, -=QTH
  ax25_send_string(gps.get_course()); // Course (degrees)
  ax25_send_string("/");              // and
  ax25_send_string(gps.get_speed());  // speed (knots)
  ax25_send_string("/A=");            // Altitude (feet). Goes anywhere in the comment area
  ax25_send_string(gps.get_altitude());
  ax25_send_string(                   // Comment
           " - Prueba TRACKUINO");  
  ax25_send_footer();
  ax25_flush_frame();                 // Tell the modem to go
}
