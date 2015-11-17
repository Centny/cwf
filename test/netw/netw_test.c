/*
 * netw_test.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include "../../include/cwf.h"
void impl_test();

void v_cwf_netw_cmd_test() {
	v_cwf_netw_cmd* cmd1 = v_cwf_netw_cmd_n(100);
	v_cwf_netw_cmd* cmd2 = v_cwf_netw_cmd_cp(cmd1);
	v_cwf_netw_cmd_f(&cmd1);
	v_cwf_netw_cmd_f(&cmd2);
}

int v_cwf_netw_hset_n_n_test_ch(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_log_i("cmd->A0->%s", cmd->hb + cmd->off);
	return 0;
}

void v_cwf_netw_hset_n_n_test() {
	v_cwf_netw_hset *hs = v_cwf_netw_hset_n_n(v_cwf_netw_hset_n_n_test_ch);
	v_cwf_netw_cmd* cmd;
	for (int i = 0; i < 100; i++) {
		cmd = v_cwf_netw_cmd_n(3);
		cmd->hb[0] = 'Z';
		cmd->hb[1] = 'x';
		cmd->hb[2] = 'A' + (i % 26);
		v_cwf_netw_hset_r(hs, cmd);
		v_cwf_netw_cmd_f(&cmd);
	}
	v_cwf_netw_hset_f(&hs);
}

void netw_test() {
//	while (1) {
	v_cwf_netw_cmd_test();
	v_cwf_netw_hset_n_n_test();
	impl_test();
//	}
	printf("netw_test done...\n");
}
