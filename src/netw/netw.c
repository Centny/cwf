/*
 * netw.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#include "../../include/cwf.h"
#include <stdlib.h>

v_cwf_netw_cmd* v_cwf_netw_new_c(unsigned int len) {
	v_cwf_netw_cmd* t = malloc(sizeof(v_cwf_netw_cmd));
	t->hb = v_cwf_alloc_c(len);
	t->len = len;
	t->off = 0;
	return t;
}

void v_cwf_netw_free_c(v_cwf_netw_cmd** v) {
	v_cwf_free_c(&(*v)->hb);
	free(*v);
	*v = 0;
}
