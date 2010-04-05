/*
 *  ax25.h
 *  trackuino
 *
 *  Created by javi on 30/01/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

struct s_address {
	char callsign[7];
	unsigned char ssid;
};

void ax25_send_header(const struct s_address *addresses, int num_addresses);
void ax25_send_string(const char *string);
void ax25_send_footer();
void ax25_flush_frame();
