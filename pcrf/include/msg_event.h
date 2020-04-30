/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#ifndef PCRF_SRC_HOOKEVENT_H_
#define PCRF_SRC_HOOKEVENT_H_

#include "sstats.h"
#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdproto.h"
#include "freeDiameter/libfdcore.h"

#include "gx_impl.h"
#include "rx_impl.h"
#include "sd_impl.h"
#include "st_impl.h"


class HookEvent {
public:
   static void init(SStats* stat, gx::Application *gx, rx::Application *rx, sd::Application *sd, st::Application *st);
   static void md_hook_cb_error(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata);
   static void md_hook_cb_ok(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata);

private:

   static struct fd_hook_hdl *m_hdl[2];
   static SStats *m_stat;
   static gx::Application *m_gx;
   static rx::Application *m_rx;
   static sd::Application *m_sd;
   static st::Application *m_st;

};

#endif /* PCRF_SRC_HOOKEVENT_H_ */
