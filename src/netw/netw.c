/*
 * netw.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#include "../../include/cwf.h"
#include <stdlib.h>
#include <string.h>

v_cwf_netw_cmd* v_cwf_netw_cmd_n(unsigned int len) {
	v_cwf_netw_cmd* t = malloc(sizeof(v_cwf_netw_cmd));
	t->hb = v_cwf_alloc_c(len);
	t->len = len;
	t->off = 0;
	return t;
}
v_cwf_netw_cmd* v_cwf_netw_cmd_n2(char* data, unsigned int len) {
	v_cwf_netw_cmd* c = v_cwf_netw_cmd_n(len);
	memcpy(c->hb, data, len);
	return c;
}
v_cwf_netw_cmd* v_cwf_netw_cmd_cp(v_cwf_netw_cmd* cmd) {
	v_cwf_netw_cmd* t = malloc(sizeof(v_cwf_netw_cmd));
	t->hb = v_cwf_alloc_c(cmd->len);
	t->len = cmd->len;
	t->off = 0;
	memcpy(t->hb, cmd->hb+cmd->off, cmd->len);
	return t;
}

void v_cwf_netw_cmd_f(v_cwf_netw_cmd** v) {
	v_cwf_free_c(&((*v)->hb));
	free(*v);
	*v = 0;
}

int v_cwf_netw_hset_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	return hs->ch(hs, cmd);
}

void v_cwf_netw_hset_f(v_cwf_netw_hset** hs) {
	v_cwf_netw_hset *t;
	for (int i = 0; i < (*hs)->childs_l; i++) {
		t = (*hs)->childs[i];
		if (t) {
			v_cwf_netw_hset_f(&t);
		}
	}
	(*hs)->free(hs);
}

v_cwf_netw_hset* v_cwf_netw_hset_n_n(v_cwf_netw_ch ch) {
	v_cwf_netw_hset* t;
	t = malloc(sizeof(v_cwf_netw_hset));
	t->parent = t->root = 0;
	t->childs = 0;
	t->childs_l = 0;
	t->type = V_CWF_NETW_HSET_N;
	t->ch = ch;
	t->free = v_cwf_netw_hset_n_f;
	v_cwf_log_d("create normal v_cwf_netw_hset with v_cwf_netw_ch(%p)->%p",
			ch, t);
	return t;
}

void v_cwf_netw_hset_n_f(v_cwf_netw_hset** hs) {
	v_cwf_log_d("free normal v_cwf_netw_hset->%p", *hs);
	free(*hs);
	*hs = 0;
}

