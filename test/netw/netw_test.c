/*
 * netw_test.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include "../../include/cwf.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

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
v_cwf_proc_lck lck__;
void* test_v_cwf_netw_sck_s_h(void* args) {
	int* ind = (int*) args;
	char buf[1024];
	int rlen = 0;
	while (1) {
		rlen = read(*ind, buf, 1024);
		if (rlen < 1) {
			printf("read error on fd(%d) with code(%d)", *ind, rlen);
			break;
		}
		write(*ind, buf, rlen);
	}
	close(*ind);
	return 0;
}
int test_v_cwf_netw_sck_sfd = 0;
void* test_v_cwf_netw_sck_s(void* args) {
	int sfd, ind;
	struct sockaddr_in addr;
	struct sockaddr_in clent;
	char * myaddr = "127.0.0.1";
	socklen_t lent;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(15050);
	addr.sin_addr.s_addr = inet_addr(myaddr);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		printf("socket error \n");
		return 0;
	}
	test_v_cwf_netw_sck_sfd = sfd;
	printf("bind start \n");
	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) < 0) {
		printf("bind error \n");
		return 0;
	}
	printf("listen start \n");
	if (listen(sfd, 1024) < 0) {
		printf("listen error \n");
		return 0;
	}
	char buf[1024];
	while (1) {
		printf("accept start \n");
		memset(&clent, 0, sizeof(clent));
		lent = sizeof(clent);
		ind = accept(sfd, (struct sockaddr *) &clent, &lent);
		if (ind < 0) {
			printf("accept error %d \n", ind);
			return 0;
		}
		printf("accept client %s:%d->%d\n",
				inet_ntop(AF_INET, &clent.sin_addr, buf, sizeof(buf)),
				ntohs(clent.sin_port), ind);
		pthread_t t;
		int* ind_ = malloc(sizeof(int));
		*ind_ = ind;
		pthread_create(&t, 0, test_v_cwf_netw_sck_s_h, ind_);
		pthread_detach(t);
	}
	return 0;
}
void* test_v_cwf_netw_sck_c_r(void* args) {
	v_cwf_netw_sck_c* sck = (v_cwf_netw_sck_c*) args;
	v_cwf_netw_sck_c_run(sck, 1);
	return 0;
}
void v_cwf_netw_sck_c_test() {
	pthread_t t;
	pthread_create(&t, 0, test_v_cwf_netw_sck_s, 0);
	pthread_detach(t);
	sleep(1);
	printf("xxx->1\n");
	v_cwf_netw_hset *hs = v_cwf_netw_hset_n_n(v_cwf_netw_hset_n_n_test_ch);
	v_cwf_netw_sck_c* sck = v_cwf_netw_sck_c_n("127.0.0.1", 15050, hs);
	pthread_create(&t, 0, test_v_cwf_netw_sck_c_r, sck);
	pthread_detach(t);
	sleep(1);
	v_cwf_netw_cmd* cmd = v_cwf_netw_cmd_n2("Vssx", 4);
	for (int i = 0; i < 50; i++) {
		cmd->hb[3] = 'A' + (i % 26);
		v_cwf_netw_sck_c_w(sck, &cmd, 1);
	}
	sleep(1);
	close(test_v_cwf_netw_sck_sfd);
	v_cwf_netw_sck_c_close(sck);
	sleep(1);
	printf("v_cwf_netw_sck_c_test done...\n");
}

void netw_test() {
//	while (1) {
//	v_cwf_netw_cmd_test();
//	v_cwf_netw_hset_n_n_test();
//	v_cwf_netw_sck_c_test();
	impl_test();
//	}
	printf("netw_test done...\n");
}
