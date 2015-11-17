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
		cmd->off++;
		cmd->len--;
		v_cwf_log_d(
				"<v_cwf_netw_hset_obdh_r>run v_cwf_netw_ch with key(%d),data(%d)",
				key, cmd->len);
		return t->ch(t, cmd);
	} else {
		v_cwf_log_w(
				"<v_cwf_netw_hset_obdh_r>receive not registered key(%d) with data(%d)",
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
	v_cwf_log_d("create ODBH v_cwf_netw_hset with keys(%d)->%p", len, t);
	return t;
}

void v_cwf_netw_hset_obdh_f(v_cwf_netw_hset** hs) {
	v_cwf_log_d("free OBDH v_cwf_netw_hset->%p", *hs);
	free((*hs)->childs);
	(*hs)->childs = 0;
	(*hs)->childs_l = 0;
	free(*hs);
	*hs = 0;
}

int v_cwf_netw_hset_queue_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	int code = 0;
	v_cwf_netw_hset* t;
	v_cwf_log_d("<v_cwf_netw_hset_queue_r>run %d v_cwf_netw_ch with data(%d)",
			hs->childs_l, cmd->len);
	for (int i = 0; i < hs->childs_l; i++) {
		t = hs->childs[i];
		code = t->ch(t, cmd);
		if (code == 0) {
			continue;
		}
		v_cwf_log_w(
				"<v_cwf_netw_hset_queue_r>run v_cwf_netw_ch on %d with data(%d) return code(%d)",
				i, cmd->len, code);
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
	v_cwf_log_d("create Queue v_cwf_netw_hset with %d v_cwf_netw_hset->%p",
			len, t);
	return t;
}

void v_cwf_netw_hset_queue_f(v_cwf_netw_hset** hs) {
	v_cwf_log_d("free Queue v_cwf_netw_hset->%p", *hs);
	free((*hs)->childs);
	(*hs)->childs = 0;
	(*hs)->childs_l = 0;
	free(*hs);
	*hs = 0;
}

/**
 * RC impl
 */
int v_cwf_netw_hset_rc_lck_n(v_cwf_netw_hset_rc* rc) {
	v_cwf_proc_lck_lock(rc->lck);
	int idx = -1;
	for (int i = 0; i < rc->lcks_l; i++) {
		if (rc->lcks[i]) {
			continue;
		} else {
			idx = i;
			break;
		}
	}
	if (idx < 0) {
		v_cwf_log_e(
				"<v_cwf_netw_hset_rc_lck_n>not more executor, having %d executor is running",
				rc->lcks_l);
		v_cwf_proc_lck_unlock(rc->lck);
		return -500;
	}
	v_cwf_proc_lck lck = v_cwf_proc_lck_new(1);
	rc->lcks[idx] = lck;
	v_cwf_proc_lck_unlock(rc->lck);
	v_cwf_log_d("<v_cwf_netw_hset_rc_lck_n>create executor id(%d) success",
			idx);
	return idx;
}

int v_cwf_netw_hset_rc_lck_f(v_cwf_netw_hset_rc* rc, int idx) {
	v_cwf_proc_lck_lock(rc->lck);
	v_cwf_proc_lck lck = rc->lcks[idx];
	rc->lcks[idx] = 0;
	v_cwf_proc_lck_unlock(rc->lck);
	if (lck) {
		v_cwf_log_d("<v_cwf_netw_hset_rc_lck_f>free executor id(%d) success",
				idx);
		return v_cwf_proc_lck_free(lck);
	} else {
		v_cwf_log_w(
				"<v_cwf_netw_hset_rc_lck_f>free executor id(%d) fail with not found",
				idx);
		return -404;
	}
}

int v_cwf_netw_hset_rc_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	if (cmd->len < 2) {
		v_cwf_log_e(
				"<v_cwf_netw_hset_rc_r>receive invalid command for v_cwf_netw_hset_rc_r, the data length is %d",
				cmd->len);
		return -1;
	}
	v_cwf_netw_hset_rc *rc = (v_cwf_netw_hset_rc*) hs->info;
	if (rc == 0) {
		v_cwf_log_e("<v_cwf_netw_hset_rc_r>%s",
				"using not executable v_cwf_netw_hset, hset.info must be v_cwf_netw_hset_rc*");
		return -1;
	}
	unsigned int idx = 0;
	idx = (unsigned int) cmd->hb[cmd->off];
	idx = idx << 8;
	idx += (unsigned int) cmd->hb[cmd->off + 1];
	cmd->off += 2;
	cmd->len -= 2;
	v_cwf_proc_lck_lock(rc->lck);
	v_cwf_proc_lck lck = rc->lcks[idx];
	v_cwf_proc_lck_unlock(rc->lck);
	if (lck) {
		v_cwf_proc_lck_lock(lck);
		lck[2] = v_cwf_netw_cmd_cp(cmd);
		v_cwf_proc_lck_signal(lck);
		v_cwf_proc_lck_unlock(lck);
		v_cwf_log_d(
				"<v_cwf_netw_hset_rc_r>the command call back success with data(%d),id(%d)",
				cmd->len, idx);
	} else {
		v_cwf_log_e(
				"<v_cwf_netw_hset_rc_r>the command call back lock is not found by id(%d)",
				idx);
	}
	return 0;
}

v_cwf_netw_hset* v_cwf_netw_hset_rc_n(v_cwf_netw_cmd* pref,
		v_cwf_netw_writer writer, unsigned int len) {
	v_cwf_netw_hset* t;
	t = malloc(sizeof(v_cwf_netw_hset));
	t->parent = t->root = 0;
	t->childs = 0;
	t->childs_l = 0;
	t->type = V_CWF_NETW_HSET_RC;
	t->ch = v_cwf_netw_hset_rc_r;
	t->free = v_cwf_netw_hset_rc_f;
	v_cwf_netw_hset_rc* rc;
	rc = malloc(sizeof(v_cwf_netw_hset_rc));
	rc->lck = v_cwf_proc_lck_new(0);
	if (pref) {
		rc->pref = v_cwf_netw_cmd_cp(pref);
	} else {
		rc->pref = 0;
	}
	rc->writer = writer;
	rc->lcks = malloc(sizeof(v_cwf_proc_lck) * len);
	memset(rc->lcks, 0, sizeof(v_cwf_proc_lck) * len);
	rc->lcks_l = len;
	t->info = rc;
	v_cwf_log_d("create RC v_cwf_netw_hset with executor(%d)->%p", len, t);
	return t;

}

void v_cwf_netw_hset_rc_f(v_cwf_netw_hset** hs) {
	v_cwf_log_d("free RC v_cwf_netw_hset->%p", *hs);
	v_cwf_netw_hset_rc* rc = (v_cwf_netw_hset_rc*) (*hs)->info;
	v_cwf_proc_lck_free(rc->lck);
	free(rc->lcks);
	free(rc);
	(*hs)->info = 0;
	free(*hs);
	*hs = 0;
}

int v_cwf_netw_hset_rc_exec(v_cwf_netw_hset* hs, void* info,
		v_cwf_netw_cmd* cmd, v_cwf_netw_cmd** out) {
	v_cwf_netw_hset_rc *rc = (v_cwf_netw_hset_rc*) hs->info;
	if (rc == 0) {
		v_cwf_log_e("<v_cwf_netw_hset_rc_exec>%s",
				"using not executable v_cwf_netw_hset, hset.info must be v_cwf_netw_hset_rc*");
		return -1;
	}
	int idx = v_cwf_netw_hset_rc_lck_n(rc);
	v_cwf_netw_cmd *eid = v_cwf_netw_cmd_n(2);
	eid->hb[1] = (char) idx;
	eid->hb[0] = (char) (idx >> 8);
	//
	int code;
	v_cwf_netw_cmd* cmds[3];
	v_cwf_proc_lck lck = rc->lcks[idx];
	v_cwf_proc_lck_lock(lck);
	if (rc->pref) {
		cmds[0] = rc->pref;
		cmds[1] = eid;
		cmds[2] = cmd;
		code = rc->writer(hs, info, cmds, 3);
	} else {
		cmds[0] = eid;
		cmds[1] = cmd;
		code = rc->writer(hs, info, cmds, 2);
	}
	if (code != 0) {
		v_cwf_proc_lck_unlock(lck);
		v_cwf_netw_cmd_f(&eid);
		v_cwf_netw_hset_rc_lck_f(rc, idx);
		v_cwf_log_w("<v_cwf_netw_hset_rc_exec>execute writer return code(%d)",
				code);
		return code;
	}
	code = v_cwf_proc_lck_timed_wait(lck, 10);
	v_cwf_proc_lck_unlock(lck);
	if (code == 0) {
		*out = (v_cwf_netw_cmd*) lck[2];
	}
	v_cwf_netw_cmd_f(&eid);
	v_cwf_netw_hset_rc_lck_f(rc, idx);
	v_cwf_log_d("<v_cwf_netw_hset_rc_exec>execute result code(%d)", code);
	return code;
}
