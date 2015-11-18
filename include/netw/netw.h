/*
 * netw.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef V_CWF_NETW_H_
#define V_CWF_NETW_H_
#ifdef __cplusplus
extern "C" {
#endif

ssize_t v_cwf_netw_read_w(int fd, void* buf, size_t len);
/**
 * CMD
 */
//
typedef struct v_cwf_netw_cmd {
	//
	void* info;
	//
	char* hb;
	unsigned int off;
	unsigned int len;
} v_cwf_netw_cmd;

//new command by char* length.
v_cwf_netw_cmd* v_cwf_netw_cmd_n(unsigned int len);
v_cwf_netw_cmd* v_cwf_netw_cmd_n2(char* data, unsigned int len);
v_cwf_netw_cmd* v_cwf_netw_cmd_cp(v_cwf_netw_cmd* cmd);
v_cwf_netw_cmd* v_cwf_netw_cmd_cp2(v_cwf_netw_cmd* cmd, unsigned int len);
v_cwf_netw_cmd* v_cwf_netw_cmd_join_v(v_cwf_netw_cmd* cmd,
		v_cwf_netw_cmd** cmds, int len);
v_cwf_netw_cmd* v_cwf_netw_cmd_join(v_cwf_netw_cmd** cmds, int len);
//free the command.
void v_cwf_netw_cmd_f(v_cwf_netw_cmd** v);
void v_cwf_netw_cmd_print(v_cwf_netw_cmd* v);

/**
 * HSET
 */
typedef struct v_cwf_netw_hset v_cwf_netw_hset;
typedef int (*v_cwf_netw_ch)(v_cwf_netw_hset*, v_cwf_netw_cmd*);
typedef void (*v_cwf_netw_free)(v_cwf_netw_hset**);
typedef int (*v_cwf_netw_writer)(v_cwf_netw_hset*, void*, v_cwf_netw_cmd**, int);
typedef struct v_cwf_netw_hset {
	//
	void* info;
	//the set type.
	int type;
	//the handler function.
	v_cwf_netw_ch ch;
	//function to free self.
	v_cwf_netw_free free;
	//
	//linker
	v_cwf_netw_hset* root;
	v_cwf_netw_hset* parent;
	unsigned int childs_l;
	v_cwf_netw_hset** childs;
} v_cwf_netw_hset;
int v_cwf_netw_hset_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd);
void v_cwf_netw_hset_f(v_cwf_netw_hset** hs);

/**
 * normal HSET.
 */
#define V_CWF_NETW_HSET_N 10
v_cwf_netw_hset* v_cwf_netw_hset_n_n(v_cwf_netw_ch ch);
void v_cwf_netw_hset_n_f(v_cwf_netw_hset** hs);

/**
 * SCK
 */
#define V_CWF_NETW_SCK_H_MOD "^~^"

typedef struct v_cwf_netw_sck_c {
	short port;
	char* addr;
	v_cwf_netw_hset* hs;
	//
	int fd;
	v_cwf_netw_cmd* mod;
} v_cwf_netw_sck_c;
v_cwf_netw_sck_c* v_cwf_netw_sck_c_n(const char* addr, short port,
		v_cwf_netw_hset* hs);
//run v_cwf_netw_sck_c. if erc is 0, it will ignore the v_cwf_netw_hset_r result.
int v_cwf_netw_sck_c_run(v_cwf_netw_sck_c *sck, int erc);
void v_cwf_netw_sck_c_close(v_cwf_netw_sck_c* sck);
void v_cwf_netw_sck_c_f(v_cwf_netw_sck_c** sck);
int v_cwf_netw_sck_c_w(v_cwf_netw_sck_c* sck, v_cwf_netw_cmd* pref,
		v_cwf_netw_cmd** cmds, int len);
int v_cwf_netw_sck_c_writer(v_cwf_netw_hset* hs, void* info,
		v_cwf_netw_cmd** cmds, int len);
//

#ifdef __cplusplus
}
#endif
#endif /* V_CWF_NETW_H_ */
