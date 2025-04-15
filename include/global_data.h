/*
 * DPVS is a software load balancer (Virtual Server) based on DPDK.
 *
 * Copyright (C) 2021 iQIYI (www.iqiyi.com).
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H__

#include <rte_per_lcore.h>
#include "conf/common.h"

RTE_DECLARE_PER_LCORE(uint32_t, g_dpvs_poll_tick);
#define this_poll_tick (RTE_PER_LCORE(g_dpvs_poll_tick))

typedef enum dpvs_lcore_role_type {
    LCORE_ROLE_IDLE,//空闲状态：表示线程未分配任何角色，未被分配任何角色
    LCORE_ROLE_MASTER,//主控线程：负责管理整个应用程序的运行，负责配置更新、状态监控、控制面处理等
    LCORE_ROLE_FWD_WORKER,//转发工作线程：负责处理数据包的转发，负责数据包接收、路由查找、转发等
    LCORE_ROLE_ISOLRX_WORKER,//隔离接收工作线程：负责接收隔离数据包，专门负责数据包的接收，与处理逻辑分离，提升接收效率
    LCORE_ROLE_KNI_WORKER,//KNI工作线程：负责处理KNI相关的任务，处理与linux内核的交互
    LCORE_ROLE_MAX//角色类型的最大值，用于边界检查
} dpvs_lcore_role_t;

extern uint64_t g_cycles_per_sec;
extern dpvs_lcore_role_t g_lcore_role[DPVS_MAX_LCORE];

/*
 *  Lcore fast search table:  g_lcore_index2id[index]-->cid
 *
 *  cid                 index
 *  ---------------------------
 *  master              0
 *  fwd_worker1         1
 *  fwd_worker2         2
 *  ...                 ...
 *  fwd_worker_n        n
 *  ioslrx_worker1      n+1
 *  isolrx_worker2      n+2
 *  ...                 ...
 *  isolrx_worker_m     n+m
 *
 *  anything else       -1
 *
 *  The inverse table:  g_lcore_index2id[cid] --> index
 * */
extern int g_lcore_index2id[DPVS_MAX_LCORE];
extern int g_lcore_id2index[DPVS_MAX_LCORE];
extern int g_lcore_num;
extern lcoreid_t g_master_lcore_id;
extern lcoreid_t g_kni_lcore_id;
extern uint8_t g_slave_lcore_num;
extern uint8_t g_isol_rx_lcore_num;
extern uint64_t g_slave_lcore_mask;
extern uint64_t g_isol_rx_lcore_mask;

extern char *dpvs_conf_file;
extern char *dpvs_pid_file;
extern char *dpvs_ipc_file;

extern unsigned int g_version;
extern bool g_kni_enabled;

#ifdef CONFIG_DPVS_PDUMP
extern bool g_dpvs_pdump;
#endif

int version_parse(const char *strver);

int global_data_init(void);
int global_data_term(void);

#endif
