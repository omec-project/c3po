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

#include "s6as6d_impl.h"
#include "s6c_impl.h"
#include "s6t_impl.h"


class HookEvent {
public:
   static void init(SStats* stat, s6t::Application *s6t, s6as6d::Application *s6as6d, s6c::Application *s6c);
   static void md_hook_cb_error(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata);
   static void md_hook_cb_ok(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata);

private:

   static struct fd_hook_hdl *m_hdl[2];
   static SStats *m_stat;
   static s6t::Application *m_s6t;
   static s6as6d::Application *m_s6as6d;
   static s6c::Application *m_s6c;
};

#endif /* HSS_SRC_HOOKEVENT_H_ */
