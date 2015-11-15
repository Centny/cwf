/*
 * netw_test.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include "../../include/cwf.h"

void netw_test() {
	v_cwf_netw_cmd* cmd = v_cwf_netw_new_c(100);
	v_cwf_netw_free_c(&cmd);
	printf("netw_test done...\n");
}
