/*
 *  radio_mx146.cpp
 *  trackuino
 *
 *  Created by javi on 02/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "radio_mx146.h"

#include "Wire.h"
#include "WProgram.h"

const int MAX_RES = 16;
char res[MAX_RES];

void radio_mx146_cmd(const char *cmd, int cmd_len, char *res, int res_len)
{
  int i;
  Wire.beginTransmission(0x48);
  for (i = 0; i < cmd_len; i++) {
    Wire.send(cmd[i]);
  }
  Wire.endTransmission();
  delay(100);
  
  if (res_len > 0) {
    Wire.requestFrom(0x48, res_len);
    for (i = 0; i < res_len; i++) {
      while (Wire.available() == 0) ;
      res[i] = Wire.receive();
    }
  }
}

void radio_mx146_set_freq(unsigned long freq)
{
  char cmd[5];
  cmd[0] = 'B';
  *((unsigned long *)(cmd+1)) = freq;
  radio_mx146_cmd(cmd, 5, res, MAX_RES);
}

int mx146_query_temp()
{
  radio_mx146_cmd("QT", 2, res, 1);
  return res[0];
}

void radio_mx146_setup()
{
  Wire.begin();
  radio_mx146_set_freq(144800000UL);
}
