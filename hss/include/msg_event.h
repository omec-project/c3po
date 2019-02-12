/*
* Copyright (c) 2017 Sprint
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
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
