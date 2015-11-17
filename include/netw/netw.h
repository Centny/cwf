/*
 * netw.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef NETW_H_
#define NETW_H_

typedef struct v_cwf_netw_hset v_cwf_netw_hset;

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
//free the command.
void v_cwf_netw_cmd_f(v_cwf_netw_cmd** v);

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

#define V_CWF_NETW_HSET_N 10
v_cwf_netw_hset* v_cwf_netw_hset_n_n(v_cwf_netw_ch ch);
void v_cwf_netw_hset_n_f(v_cwf_netw_hset** hs);

#endif /* NETW_H_ */
