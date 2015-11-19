/*
 * im.h
 *
 *  Created on: Nov 18, 2015
 *      Author: vty
 */

#ifndef V_CWF_IM_H_
#define V_CWF_IM_H_
#include "../cwf.h"
#ifdef __cplusplus
extern "C" {
#endif
//
#define MK_NIM 0
#define MK_NMR 2
#define MK_NRC 4
#define MK_NRC_LI 10
#define MK_NRC_LO 20
#define MK_NRC_UR 30

typedef struct v_cwf_im v_cwf_im;

typedef int (*v_cwf_im_on_nim)(v_cwf_im*, v_cwf_netw_cmd*);

struct v_cwf_im {
	void* info;
	//
	v_cwf_netw_sck_c* sck;
	v_cwf_im_on_nim on_nim;
	//
	v_cwf_netw_hset* hs;
	v_cwf_netw_cmd* nim_m;
	v_cwf_netw_hset* nim; //MK_NIM
	v_cwf_netw_cmd* nmr_m;
	v_cwf_netw_hset* nmr; //MK_NMR
	v_cwf_netw_cmd* nrc_m;
	v_cwf_netw_hset* nrc; //MK_NRC
};

v_cwf_im* v_cwf_im_n(const char* addr, short port, v_cwf_im_on_nim nim);
void v_cwf_im_f(v_cwf_im** im);
int v_cwf_im_sms(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len);
int v_cwf_im_mr(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len);
int v_cwf_im_li(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len,
		v_cwf_netw_cmd ** out);
int v_cwf_im_lo(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len,
		v_cwf_netw_cmd ** out);
int v_cwf_im_ur(v_cwf_im* im, v_cwf_netw_cmd ** cmds, int len,
		v_cwf_netw_cmd ** out);
//
int v_cwf_im_run(v_cwf_im* im, int erc);
void v_cwf_im_close(v_cwf_im* im);
//

#ifdef __cplusplus
}
#endif
#endif /* V_CWF_IM_H_ */
