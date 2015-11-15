/*
 * netw.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef NETW_H_
#define NETW_H_

typedef struct v_cwf_netw_cmd {
	char* hb;
	unsigned int off;
	unsigned int len;
} v_cwf_netw_cmd;

//new command by char* length.
v_cwf_netw_cmd* v_cwf_netw_new_c(unsigned int len);
void v_cwf_netw_free_c(v_cwf_netw_cmd** v);

#endif /* NETW_H_ */
