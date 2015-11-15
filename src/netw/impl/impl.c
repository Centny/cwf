/*
 * impl.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include "../../../include/cwf.h"
#include <stdlib.h>
#include <string.h>

int v_cwf_netw_hset_obdh_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	unsigned int key = (unsigned int) cmd->hb[cmd->off];
	v_cwf_netw_hset* t = hs->childs[key];
	if (t) {
		cmd->hb += 1;
		cmd->off++;
		cmd->len--;
		return t->ch(t, cmd);
	} else {
		v_cwf_log_w("OBDH receive not registered key(%d) data(%d)",
				key, cmd->len);
		return -1;
	}
}

v_cwf_netw_hset* v_cwf_netw_hset_obdh_n(char* keys, v_cwf_netw_hset** hs,
		int len) {
	v_cwf_netw_hset* t;
	t = malloc(sizeof(v_cwf_netw_hset));
	t->parent = t->root = 0;
	t->childs = malloc(sizeof(v_cwf_netw_hset*) * 256);
	memset((void*) t->childs, 0, (size_t) sizeof(v_cwf_netw_hset*) * 256);
	for (int i = 0; i < len; ++i) {
		t->childs[(unsigned int) keys[i]] = hs[i];
		hs[i]->parent = t;
	}
	t->childs_l = 256;
	t->type = V_CWF_NETW_HSET_OBDH;
	t->ch = v_cwf_netw_hset_obdh_r;
	t->free = v_cwf_netw_hset_obdh_f;
	return t;
}

void v_cwf_netw_hset_obdh_f(v_cwf_netw_hset** hs) {
	free((*hs)->childs);
	(*hs)->childs = 0;
	(*hs)->childs_l = 0;
	free(*hs);
	*hs = 0;
}

int v_cwf_netw_hset_queue_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	int code = 0;
	v_cwf_netw_hset* t;
	for (int i = 0; i < hs->childs_l; i++) {
		t = hs->childs[i];
		code = t->ch(t, cmd);
		if (code == 0) {
			continue;
		}
		return code;
	}
	return 0;
}

v_cwf_netw_hset* v_cwf_netw_hset_queue_n(v_cwf_netw_hset** hs, int len) {
	v_cwf_netw_hset* t;
	t = malloc(sizeof(v_cwf_netw_hset));
	t->parent = t->root = 0;
	t->childs = malloc(sizeof(v_cwf_netw_hset*) * len);
	for (int i = 0; i < len; ++i) {
		t->childs[i] = hs[i];
		hs[i]->parent = t;
	}
	t->childs_l = len;
	t->type = V_CWF_NETW_HSET_QUEUE;
	t->ch = v_cwf_netw_hset_queue_r;
	t->free = v_cwf_netw_hset_queue_f;
	return t;
}

void v_cwf_netw_hset_queue_f(v_cwf_netw_hset** hs) {
	free((*hs)->childs);
	(*hs)->childs = 0;
	(*hs)->childs_l = 0;
	free(*hs);
	*hs = 0;
}
