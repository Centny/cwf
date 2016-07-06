/*
 * im.c
 *
 *  Created on: Nov 18, 2015
 *      Author: vty
 */
#include "../../include/cwf.h"
#include <stdlib.h>

int v_cwf_im_nim_(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	v_cwf_im* im = hs->info;
	return im->on_nim(im, cmd);
}
int v_cwf_im_nmr_(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	return 0;
}

v_cwf_im* v_cwf_im_n(const char* host, const char* addr, short port, v_cwf_im_on_nim nim) {
	v_cwf_im* im = malloc(sizeof(v_cwf_im));
	char keys[3];
	v_cwf_netw_hset* hss[3];
	im->on_nim = nim;
	//
	//
	keys[0] = MK_NIM;
	im->nim_m = v_cwf_netw_cmd_n2(keys, 1);
	im->nim = v_cwf_netw_hset_n_n(v_cwf_im_nim_);
	im->nim->info = im;
	hss[0] = im->nim;
	//
	//
	keys[1] = MK_NMR;
	im->nmr_m = v_cwf_netw_cmd_n2(keys + 1, 1);
	im->nmr = v_cwf_netw_hset_n_n(v_cwf_im_nmr_);
	im->nmr->info = im;
	hss[1] = im->nmr;
	//
	//
	keys[2] = MK_NRC;
	im->nrc_m = v_cwf_netw_cmd_n2(keys + 2, 1);
	im->nrc = v_cwf_netw_hset_rc_n(im->nrc_m, v_cwf_netw_sck_c_writer, 5);
	hss[2] = im->nrc;
	//
	//
	im->hs = v_cwf_netw_hset_obdh_n(keys, hss, 3);
	im->sck = v_cwf_netw_sck_c_n(host, addr, port, im->hs);
    im->sck->info=im;
	return im;
}
void v_cwf_im_f(v_cwf_im** im) {
    //
	v_cwf_netw_cmd_f(&(*im)->nim_m);
//    v_cwf_netw_hset_f(&(*im)->nim);
    //
	v_cwf_netw_cmd_f(&(*im)->nmr_m);
//    v_cwf_netw_hset_f(&(*im)->nmr);
    //
    v_cwf_netw_cmd_f(&(*im)->nrc_m);
//    v_cwf_netw_hset_f(&(*im)->nrc);
    //
	v_cwf_netw_hset_f(&(*im)->hs);
    //
	v_cwf_netw_sck_c_f(&(*im)->sck);
	free(*im);
	*im = 0;
}
int v_cwf_im_sms(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len) {
	return v_cwf_netw_sck_c_w(im->sck, im->nim_m, cmds, len);
}
int v_cwf_im_mr(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len) {
	return v_cwf_netw_sck_c_w(im->sck, im->nmr_m, cmds, len);
}
int v_cwf_im_li(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len,
		v_cwf_netw_cmd ** out) {
	return v_cwf_netw_hset_rc_exec(im->nrc, im->sck, MK_NRC_LI, cmds, len, out);
}
int v_cwf_im_lo(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len,
		v_cwf_netw_cmd ** out) {
	return v_cwf_netw_hset_rc_exec(im->nrc, im->sck, MK_NRC_LO, cmds, len, out);
}
int v_cwf_im_ur(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len,
		v_cwf_netw_cmd ** out) {
	return v_cwf_netw_hset_rc_exec(im->nrc, im->sck, MK_NRC_UR, cmds, len, out);
}
int v_cwf_im_run(v_cwf_im* im, int erc) {
	return v_cwf_netw_sck_c_run(im->sck, erc);
}
void v_cwf_im_close(v_cwf_im* im) {
	v_cwf_netw_sck_c_close(im->sck);
}
