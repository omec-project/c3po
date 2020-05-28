/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#ifndef HSS_SRC_HOOKEVENT_H_
#define HSS_SRC_HOOKEVENT_H_

#include "sstats.h"
#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdproto.h"
#include "freeDiameter/libfdcore.h"

#include "rf_impl.h"


class HookEvent {
public:
   static void init(SStats* stat, rf::Application *rf);
   static void md_hook_cb_error(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata);
   static void md_hook_cb_ok(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata);

private:

   static struct fd_hook_hdl *m_hdl[2];
   static SStats *m_stat;

   static rf::Application *m_rf;
};

#endif /* HSS_SRC_HOOKEVENT_H_ */
