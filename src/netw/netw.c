/*
 * netw.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#include "../../include/cwf.h"
#include <stdlib.h>
#include <string.h>
//sck
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

long v_cwf_netw_read_w(int fd, void* buf, size_t len) {
	size_t dlen = 0;
	long res = 0;
	while (dlen < len) {
		res = read(fd, buf + dlen, len - dlen);
		if (res < 1) {
			return res;
		} else {
			dlen += res;
		}
	}
	return len;
}

v_cwf_netw_cmd* v_cwf_netw_cmd_n(unsigned int len) {
	v_cwf_netw_cmd* t = malloc(sizeof(v_cwf_netw_cmd));
	t->hb = v_cwf_alloc_c(len);
	t->len = len;
	t->off = 0;
	return t;
}
v_cwf_netw_cmd* v_cwf_netw_cmd_n2(char* data, unsigned int len) {
	v_cwf_netw_cmd* c = v_cwf_netw_cmd_n(len);
	memcpy(c->hb, data, len);
	return c;
}
v_cwf_netw_cmd* v_cwf_netw_cmd_cp(v_cwf_netw_cmd* cmd) {
	v_cwf_netw_cmd* t = malloc(sizeof(v_cwf_netw_cmd));
	t->hb = v_cwf_alloc_c(cmd->len);
	t->len = cmd->len;
	t->off = 0;
	memcpy(t->hb, cmd->hb+cmd->off, cmd->len);
	return t;
}
v_cwf_netw_cmd* v_cwf_netw_cmd_cp2(v_cwf_netw_cmd* cmd, unsigned int len) {
	v_cwf_netw_cmd* t = malloc(sizeof(v_cwf_netw_cmd));
	t->hb = v_cwf_alloc_c(cmd->len + len);
	t->len = cmd->len + len;
	t->off = 0;
	memcpy(t->hb, cmd->hb+cmd->off, cmd->len);
	return t;
}
v_cwf_netw_cmd* v_cwf_netw_cmd_join_v(v_cwf_netw_cmd* cmd,
		v_cwf_netw_cmd** cmds, int len) {
	int dlen = 0;
	if (cmd) {
		dlen += cmd->len;
	}
	for (int i = 0; i < len; i++) {
		dlen += cmds[i]->len;
	}
	v_cwf_netw_cmd* tcmd = v_cwf_netw_cmd_n(dlen);
	dlen = 0;
	if (cmd) {
		memcpy(tcmd->hb+dlen, cmd->hb+cmd->off, cmd->len);
		dlen += cmd->len;
	}
	for (int i = 0; i < len; i++) {
		memcpy(tcmd->hb+dlen, cmds[i]->hb+cmds[i]->off, cmds[i]->len);
		dlen += cmds[i]->len;
	}
	return tcmd;
}
v_cwf_netw_cmd* v_cwf_netw_cmd_join(v_cwf_netw_cmd** cmds, int len) {
	return v_cwf_netw_cmd_join_v(0, cmds, len);
}

void v_cwf_netw_cmd_f(v_cwf_netw_cmd** v) {
	v_cwf_free_c(&((*v)->hb));
	free(*v);
	*v = 0;
}

void v_cwf_netw_cmd_print(v_cwf_netw_cmd* v) {
	for (int i = 0; i < v->len; i++) {
		printf("%d ", (unsigned int) v->hb[v->off + i]);
	}
	printf("\n");
}

int v_cwf_netw_hset_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd) {
	return hs->ch(hs, cmd);
}

void v_cwf_netw_hset_f(v_cwf_netw_hset** hs) {
	v_cwf_netw_hset *t;
	for (int i = 0; i < (*hs)->childs_l; i++) {
		t = (*hs)->childs[i];
		if (t) {
			v_cwf_netw_hset_f(&t);
		}
	}
	(*hs)->free(hs);
}

v_cwf_netw_hset* v_cwf_netw_hset_n_n(v_cwf_netw_ch ch) {
	v_cwf_netw_hset* t;
	t = malloc(sizeof(v_cwf_netw_hset));
	t->parent = t->root = 0;
	t->childs = 0;
	t->childs_l = 0;
	t->type = V_CWF_NETW_HSET_N;
	t->ch = ch;
	t->free = v_cwf_netw_hset_n_f;
	v_cwf_log_d("create normal v_cwf_netw_hset with v_cwf_netw_ch(%p)->%p",
			ch, t);
	return t;
}

void v_cwf_netw_hset_n_f(v_cwf_netw_hset** hs) {
	v_cwf_log_d("free normal v_cwf_netw_hset->%p", *hs);
	free(*hs);
	*hs = 0;
}

/**
 * SCK impl
 */
v_cwf_netw_sck_c* v_cwf_netw_sck_c_n(const char* addr, const char* port,
		v_cwf_netw_hset* hs) {
	v_cwf_netw_sck_c* c = malloc(sizeof(v_cwf_netw_sck_c));
    //
    c->addr = malloc(sizeof(char) * strlen(addr) + 1);
    memcpy(c->addr, addr, strlen(addr));
    c->addr[strlen(addr)] = 0;
    //
    c->port = malloc(sizeof(char) * strlen(port) + 1);
    memcpy(c->port, port, strlen(port));
    c->port[strlen(port)] = 0;
    //
	c->hs = hs;
	c->fd = 0;
	c->mod = v_cwf_netw_cmd_n2(V_CWF_NETW_SCK_H_MOD,
			strlen(V_CWF_NETW_SCK_H_MOD));
	c->evnh = 0;
	return c;
}

int v_cwf_netw_sck_c_run(v_cwf_netw_sck_c *sck, int erc) {
	if (sck->evnh) {
		sck->evnh(sck, V_CWF_NETW_SCK_EVN_RUN, &erc, 0);
	}
    int nsd = 0;
    int code = 0;
    char buf[64];
    struct addrinfo hints = {0};
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC; // IPv4 and IPv6 allowed
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    struct addrinfo* addr;
    code = getaddrinfo(sck->addr, sck->port, &hints, &addr );
    if (sck->evnh) {
        sck->evnh(sck, V_CWF_NETW_SCK_EVN_DNS_D, &erc, &code);
    }
    if (code!=0) {
        v_cwf_log_e("<v_cwf_netw_sck_c_r>get addr info by addr(%s),port(%s) fail with code(%d)", sck->addr, sck->port, code);
        return (int)code;
    }
    const char* taddr = inet_ntop(addr->ai_family, &addr->ai_addr, buf, 64);
    v_cwf_log_i("<v_cwf_netw_sck_c_r>start connect to %s-%s", taddr, sck->port);
    nsd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (sck->evnh) {
		sck->evnh(sck, V_CWF_NETW_SCK_EVN_CON_S, &erc, 0);
	}
    sck->fd = nsd;
	code = connect(nsd, addr->ai_addr, addr->ai_addrlen);
	if (sck->evnh) {
		sck->evnh(sck, V_CWF_NETW_SCK_EVN_CON_D, &erc, &code);
	}
	if (code < 0) {
		v_cwf_log_e("<v_cwf_netw_sck_c_r>connect to %s-%s error with code(%d)",
                    taddr, sck->port, code);
		return (int) code;
	}
	v_cwf_log_i("<v_cwf_netw_sck_c_r>connect to %s-%s success with fd(%d)",
			taddr, sck->port, nsd);
	int dlen = 0;
	char head[5];
	v_cwf_netw_cmd* cmd;
	if (sck->evnh) {
		sck->evnh(sck, V_CWF_NETW_SCK_EVN_LR_S, &erc, 0);
	}
	while (1) {
		dlen = 0;
		long rlen = v_cwf_netw_read_w(nsd, head, 5);
		if (rlen != 5) {
            code=(int)rlen;
			v_cwf_log_e(
					"<v_cwf_netw_sck_c_r>read data from fd(%d) error with code(%d)",
					nsd, code);
			break;
		}
		if (strncmp(head, V_CWF_NETW_SCK_H_MOD, 3) != 0) {
			head[3] = 0;
			v_cwf_log_e(
					"<v_cwf_netw_sck_c_r>read data from fd(%d) error->expect head(%s), but(%s)",
					nsd, V_CWF_NETW_SCK_H_MOD, head);
            break;
		}
		dlen = (unsigned char) head[3];
		dlen = dlen << 8;
		dlen += (unsigned char) head[4];
		if (dlen < 1) {
			v_cwf_log_e(
					"<v_cwf_netw_sck_c_r>read data from fd(%d) error->the data length is %d",
					nsd, dlen);
			continue;
		}
		cmd = v_cwf_netw_cmd_n(dlen);
		cmd->info = &nsd;
		rlen = v_cwf_netw_read_w(nsd, cmd->hb, dlen);
		if (rlen < 1) {
            code=(int)rlen;
			v_cwf_netw_cmd_f(&cmd);
			v_cwf_log_e(
					"<v_cwf_netw_sck_c_r>read data from fd(%d) error with code(%d)",
					nsd, code);
			break;
		}
		code = v_cwf_netw_hset_r(sck->hs, cmd);
		v_cwf_netw_cmd_f(&cmd);
		if (code == 0) {
			continue;
		}
		if (erc) {
			v_cwf_log_e(
					"<v_cwf_netw_sck_c_r>do v_cwf_netw_hset_r on fd(%d) error with code(%d)",
					nsd, code);
			break;
		}
	}
	if (sck->evnh) {
		sck->evnh(sck, V_CWF_NETW_SCK_EVN_LR_D, &erc, 0);
	}
	close(nsd);
    freeaddrinfo(addr);
	if (sck->evnh) {
		sck->evnh(sck, V_CWF_NETW_SCK_EVN_CLOSED, &erc, 0);
	}
	v_cwf_log_i("<v_cwf_netw_sck_c_r>stop connect on fd(%d) for %s:%s",
			nsd, sck->addr, sck->port);
	return (int) code;
}

void v_cwf_netw_sck_c_close(v_cwf_netw_sck_c* sck) {
	if (sck->fd) {
		close(sck->fd);
	}
}

void v_cwf_netw_sck_c_f(v_cwf_netw_sck_c** sck) {
    if ((*sck)->port) {
        free((*sck)->port);
    }
    if ((*sck)->addr) {
        free((*sck)->addr);
    }
	v_cwf_netw_cmd_f(&((*sck)->mod));
	free(*sck);
	*sck = 0;
}

v_cwf_netw_cmd* v_cwf_netw_sck_cmd(v_cwf_netw_cmd* mod, v_cwf_netw_cmd* pref,
		v_cwf_netw_cmd** cmds, int len) {
	int dlen = 0;
	if (pref) {
		dlen += pref->len;
	}
	for (int i = 0; i < len; i++) {
		dlen += cmds[i]->len;
	}
	v_cwf_netw_cmd* cmd = v_cwf_netw_cmd_n(mod->len + 2 + dlen);
	memcpy(cmd->hb, mod->hb+mod->off, mod->len);
	cmd->hb[mod->len + 1] = (char) dlen;
	cmd->hb[mod->len] = (char) (dlen >> 8);
	dlen = mod->len + 2;
	if (pref) {
		memcpy(cmd->hb+dlen, pref->hb+pref->off, pref->len);
		dlen += pref->len;
	}
	for (int i = 0; i < len; i++) {
		memcpy(cmd->hb+dlen, cmds[i]->hb+cmds[i]->off, cmds[i]->len);
		dlen += cmds[i]->len;
	}
	return cmd;
}
int v_cwf_netw_sck_c_w(v_cwf_netw_sck_c* sck, v_cwf_netw_cmd* pref,
		v_cwf_netw_cmd** cmds, int len) {
	if (sck->fd) {
		v_cwf_netw_cmd* cmd = v_cwf_netw_sck_cmd(sck->mod, pref, cmds, len);
//        v_cwf_netw_cmd_print(cmd);
		int code = (int) write(sck->fd, cmd->hb + cmd->off, cmd->len);
		v_cwf_netw_cmd_f(&cmd);
		return code;
	} else {
		return -1;
	}
}
int v_cwf_netw_sck_c_writer(v_cwf_netw_hset* hs, void* info,
		v_cwf_netw_cmd** cmds, int len) {
	return v_cwf_netw_sck_c_w((v_cwf_netw_sck_c*) info, 0, cmds, len);
}

