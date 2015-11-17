/*
 * impl_test.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include "../../../include/cwf.h"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * OBDH/Queue impl test.
 */
int impl_test_on_cmd_a_0(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_log_i("cmd->A0->%s", cmd->hb + cmd->off);
	return 0;
}
int impl_test_on_cmd_a_1(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_log_i("cmd->A1->%s", cmd->hb + cmd->off);
	return 0;
}
int impl_test_on_cmd_b_0(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_log_i("cmd->B0->%s", cmd->hb + cmd->off);
	return 0;
}
int impl_test_on_cmd_b_1(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_log_i("cmd->B1->%s", cmd->hb + cmd->off);
	return 0;
}
int impl_test_on_cmd_c_0(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_log_i("cmd->C0->%s", cmd->hb + cmd->off);
	return 0;
}
int impl_test_on_cmd_c_1(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_log_i("cmd->C1->%s", cmd->hb + cmd->off);
	return 0;
}

void impl_obdh_queue_test() {
	printf("impl_obdh_queue_test start...\n");
	char keys[3];
	v_cwf_netw_hset* hs[3];
	v_cwf_netw_hset* hs_[2];
	//
	keys[0] = 'A';
	hs_[0] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_0);
	hs_[1] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_1);
	hs[0] = v_cwf_netw_hset_queue_n(hs_, 2);
	//
	keys[1] = 'B';
	hs_[0] = v_cwf_netw_hset_n_n(impl_test_on_cmd_b_0);
	hs_[1] = v_cwf_netw_hset_n_n(impl_test_on_cmd_b_1);
	hs[1] = v_cwf_netw_hset_queue_n(hs_, 2);
	//
	keys[2] = 'C';
	hs_[0] = v_cwf_netw_hset_n_n(impl_test_on_cmd_c_0);
	hs_[1] = v_cwf_netw_hset_n_n(impl_test_on_cmd_c_1);
	hs[2] = v_cwf_netw_hset_queue_n(hs_, 2);
	//
	v_cwf_netw_hset* root;
	root = v_cwf_netw_hset_obdh_n(keys, hs, 3);
	const char* tdata = "X data";
	v_cwf_netw_cmd* cmd;
	cmd = v_cwf_netw_cmd_n(strlen(tdata) + 1);
	strcpy(cmd->hb+1, tdata);
	printf("impl_test run...\n");
	for (int i = 0; i < 3; i++) {
		cmd->off = 0;
		cmd->hb[0] = 'A' + i;
		int code = v_cwf_netw_hset_r(root, cmd);
		if (code != 0) {
			printf("exit code:%d\n", code);
			exit(code);
		}
	}
	cmd->off = 0;
	cmd->hb[0] = 'A' + 3;
	int code = v_cwf_netw_hset_r(root, cmd);
	if (code == 0) {
		printf("exit code is:%d\n", code);
		exit(1);
	}
	v_cwf_netw_hset_f(&root); //will free all hset.
	v_cwf_netw_cmd_f(&cmd);
	printf("impl_obdh_queue_test done...\n");
}

/**
 * RC impl test.
 */
void *impl_obdh_rc_test_r(void *args) {
	void **args_ = args;
	v_cwf_netw_cmd* tcmd = (v_cwf_netw_cmd*) args_[0];
	v_cwf_netw_hset* hs = (v_cwf_netw_hset*) args_[1];
	v_cwf_log_i("impl_obdh_rc_test_r start->%p,%p->%s...", tcmd, hs, tcmd->hb);
	int code = v_cwf_netw_hset_r(hs, tcmd);
	v_cwf_log_i( "impl_obdh_rc_test_r code:%d...", code);
	v_cwf_netw_cmd_f(&tcmd);
	free(args_);
	v_cwf_log_i("%s", "impl_obdh_rc_test_r done...");
	return 0;
}

int impl_obdh_rc_test_w(v_cwf_netw_hset* hs, void* info, v_cwf_netw_cmd** cmds,
		int len) {
	v_cwf_log_i("%s", "impl_obdh_rc_test_w start...");
	int dlen = 0;
	for (int i = 0; i < len; i++) {
		dlen += cmds[i]->len;
	}
	v_cwf_log_i("impl_obdh_rc_test_w length->%d...", dlen);
	v_cwf_netw_cmd* tcmd = v_cwf_netw_cmd_n(dlen);
	dlen = 0;
	for (int i = 0; i < len; i++) {
		memcpy(tcmd->hb+dlen, cmds[i]->hb+cmds[i]->off, cmds[i]->len);
		dlen += cmds[i]->len;
	}
	v_cwf_log_i("impl_obdh_rc_test_w data->%s", cmds[len-1]->hb);
	void** args = malloc(sizeof(void*) * 2);
	args[0] = tcmd;
	args[1] = hs->parent->parent;
	pthread_t t;
	pthread_create(&t, 0, impl_obdh_rc_test_r, args);
	pthread_detach(t);
	v_cwf_log_i("%s", "impl_obdh_rc_test_w done...");
	return 0;
}

void impl_obdh_rc_test() {
	printf("%s", "impl_obdh_rc_test start...\n");
	int code;
	char keys[3];
	char keys_[2];
	v_cwf_netw_hset* hs[3];
	v_cwf_netw_hset* hs_[2];
	//
	keys[0] = 'A';
	keys_[0] = '1';
	hs_[0] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_0);
	keys_[1] = '2';
	hs_[1] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_1);
	hs[0] = v_cwf_netw_hset_obdh_n(keys_, hs_, 2);
	//
	keys[1] = 'B';
	v_cwf_netw_cmd *pb;
	v_cwf_netw_hset *hsb1, *hsb2;
	pb = v_cwf_netw_cmd_n(2);
	pb->hb[0] = 'B';
	pb->hb[1] = '1';
	keys_[0] = '1';
	hsb1 = v_cwf_netw_hset_rc_n(pb, impl_obdh_rc_test_w, 5);
	v_cwf_netw_cmd_f(&pb);
	hs_[0] = hsb1;
	pb = v_cwf_netw_cmd_n(2);
	pb->hb[0] = 'B';
	pb->hb[1] = '2';
	keys_[1] = '2';
	hsb2 = v_cwf_netw_hset_rc_n(pb, impl_obdh_rc_test_w, 5);
	v_cwf_netw_cmd_f(&pb);
	hs_[1] = hsb2;
	hs[1] = v_cwf_netw_hset_obdh_n(keys_, hs_, 2);
	//
	//
	v_cwf_netw_hset* root;
	root = v_cwf_netw_hset_obdh_n(keys, hs, 2);
	const char* tdata = "X data";
	v_cwf_netw_cmd* cmd;
	cmd = v_cwf_netw_cmd_n(strlen(tdata) + 2);
	strcpy(cmd->hb+2, tdata);
	v_cwf_log_i("%s", "impl_obdh_rc_test run...");
	for (int i = 0; i < 2; i++) {
		cmd->off = 0;
		cmd->hb[0] = 'A';
		cmd->hb[1] = '1' + i;
		code = v_cwf_netw_hset_r(root, cmd);
		if (code != 0) {
			v_cwf_log_i("exit code:%d\n", code);
			exit(code);
		}
	}
	v_cwf_netw_cmd_f(&cmd);
	cmd = v_cwf_netw_cmd_n2("Zxx", 3);
	v_cwf_netw_cmd* out = 0;

	for (int i = 0; i < 26; i++) {
		printf("\n\n\n");
		//
		//1
		v_cwf_log_i("XX->impl_obdh_rc_test hsb1--->%p-->root->%p",
				hsb1->parent->parent, root);
		cmd->hb[1] = 'A';
		cmd->hb[2] = 'A' + i;
		code = v_cwf_netw_hset_rc_exec(hsb1, 0, cmd, &out);
		if (code != 0) {
			v_cwf_log_i("exit code:%d\n", code);
			exit(code);
		}
		code = strncmp(cmd->hb, out->hb, out->len);
		if (code != 0) {
			v_cwf_log_i("cmp(%s,%s,%d) exit code:%d\n",
					cmd->hb, out->hb, out->len, code);
			exit(code);
		}
		v_cwf_netw_cmd_f(&out);
		//
		//2
		v_cwf_log_i("XX->impl_obdh_rc_test hsb2--->%p-->root->%p",
				hsb2->parent->parent, root);
		cmd->hb[1] = 'B';
		cmd->hb[2] = 'A' + i;
		code = v_cwf_netw_hset_rc_exec(hsb2, 0, cmd, &out);
		if (code != 0) {
			v_cwf_log_i("exit code:%d\n", code);
			exit(code);
		}
		code = strncmp(cmd->hb, out->hb, out->len);
		if (code != 0) {
			v_cwf_log_i("cmp(%s,%s,%d) exit code:%d\n",
					cmd->hb, out->hb, out->len, code);
			exit(code);
		}
		v_cwf_netw_cmd_f(&out);
	}
	//
	//
	v_cwf_netw_hset_f(&root); //will free all hset.
	printf("%s", "impl_obdh_rc_test done...\n");
}

int v_cwf_netw_hset_rc_n_test_w(v_cwf_netw_hset* hs, void* info,
		v_cwf_netw_cmd** cmds, int len) {
	v_cwf_log_i("v_cwf_netw_hset_rc_n_test_w->%s",
			cmds[len-1]->hb + cmds[len-1]->off);
	return 0;
}

void v_cwf_netw_hset_queue_n_test() {
	v_cwf_netw_hset* hs[2];
	for (int i = 0; i < 10; i++) {
		hs[0] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_0);
		hs[1] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_1);
		v_cwf_netw_hset* hs_ = v_cwf_netw_hset_queue_n(hs, 2);
		v_cwf_netw_hset_f(&hs_);
	}
	printf("v_cwf_netw_hset_queue_n_test done...\n");
}

void v_cwf_netw_hset_obdh_n_test() {
	char keys[2];
	v_cwf_netw_hset* hs[2];
	//v_cwf_netw_hset_queue_n
	keys[0] = 'A';
	keys[1] = 'B';
	for (int i = 0; i < 10; i++) {
		hs[0] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_0);
		hs[1] = v_cwf_netw_hset_n_n(impl_test_on_cmd_a_1);
		v_cwf_netw_hset* hs_ = v_cwf_netw_hset_obdh_n(keys, hs, 2);
		v_cwf_netw_hset_f(&hs_);
	}
	printf("v_cwf_netw_hset_obdh_n_test done...\n");
}

void v_cwf_netw_hset_rc_n_test() {
	v_cwf_netw_hset* hs = v_cwf_netw_hset_rc_n(0, v_cwf_netw_hset_rc_n_test_w,
			5);
	v_cwf_netw_hset_rc* rc = (v_cwf_netw_hset_rc*) hs->info;
	int idx1, idx2;
	for (int i = 0; i < 10; i++) {
		idx1 = v_cwf_netw_hset_rc_lck_n(rc);
		idx2 = v_cwf_netw_hset_rc_lck_n(rc);
		v_cwf_netw_hset_rc_lck_f(rc, idx1);
		v_cwf_netw_hset_rc_lck_f(rc, idx2);
	}
	hs->free(&hs);
	printf("v_cwf_netw_hset_rc_n_test done...\n");
}

void* impl_test_thr_r(void* args) {
	printf("impl_test_thr->running->\n");
	return 0;
}

void impl_test_thr() {
	for (int i = 0; 1; i++) {
		pthread_t t;
		pthread_create(&t, 0, impl_test_thr_r, 0);
		pthread_detach(t);
	}
}

void impl_test() {
//	while (1) {
	//	impl_test_thr();
	v_cwf_netw_hset_rc_n_test();
	v_cwf_netw_hset_queue_n_test();
	v_cwf_netw_hset_obdh_n_test();
	impl_obdh_queue_test();
	impl_obdh_rc_test();
//	}
}
