/*
 * impl_test.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include "../../../include/cwf.h"
#include <string.h>
#include <stdlib.h>

int impl_test_on_cmd_a_0(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	printf("cmd->A0->%s\n", cmd->hb);
	return 0;
}
int impl_test_on_cmd_a_1(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	printf("cmd->A1->%s\n", cmd->hb);
	return 0;
}
int impl_test_on_cmd_b_0(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	printf("cmd->B0->%s\n", cmd->hb);
	return 0;
}
int impl_test_on_cmd_b_1(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	printf("cmd->B1->%s\n", cmd->hb);
	return 0;
}
int impl_test_on_cmd_c_0(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	printf("cmd->C0->%s\n", cmd->hb);
	return 0;
}
int impl_test_on_cmd_c_1(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	printf("cmd->C1->%s\n", cmd->hb);
	return 0;
}

void impl_test() {
	printf("impl_test start...\n");
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
	cmd = v_cwf_netw_new_c(strlen(tdata) + 1);
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
	v_cwf_netw_hset_f(&root);
//	v_cwf_netw_free_c(&cmd);
	printf("impl_test done...\n");
}
