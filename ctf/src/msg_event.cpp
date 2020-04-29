/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/


#include "msg_event.h"

#include <iostream>
#include <functional>

struct fd_hook_hdl *HookEvent::m_hdl[2] = {NULL, NULL};
SStats *HookEvent::m_stat = NULL;

rf::Application *HookEvent::m_rf;


void HookEvent::init(SStats* stat, rf::Application *rf){
   m_stat = stat;
   m_rf = rf;

   uint32_t mask_errors;
   mask_errors = HOOK_MASK( HOOK_MESSAGE_PARSING_ERROR, HOOK_MESSAGE_ROUTING_ERROR, HOOK_MESSAGE_DROPPED  );
   fd_hook_register( mask_errors, md_hook_cb_error, NULL, NULL, &m_hdl[0] );

   uint32_t mask_ok;
   mask_ok = HOOK_MASK( HOOK_MESSAGE_RECEIVED, HOOK_MESSAGE_SENDING );
   fd_hook_register( mask_ok, md_hook_cb_ok, NULL, NULL, &m_hdl[1] );
}


void HookEvent::md_hook_cb_error(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata)
{
   if(msg){
      struct msg_hdr* hdr = NULL;

      if( !fd_msg_hdr ( msg , &hdr ) ) {
         bool isRequest = (hdr->msg_flags & CMD_FLAG_REQUEST) == CMD_FLAG_REQUEST;

         if( (hdr->msg_appl == m_rf->getDict().app().getId()) ){
            if( hdr->msg_code == m_rf->getDict().cmdACR().getCommandCode() && isRequest ){
               m_stat->registerStatAttemp(stat_ctf_acr, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_rf->getDict().cmdACA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_ctf_acr, stat_received_ko);
            }
         }
      }
   }
}

void HookEvent::md_hook_cb_ok(enum fd_hook_type type, struct msg * msg, struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, void * regdata)
{
   if(msg){
      struct msg_hdr* hdr = NULL;

      if( !fd_msg_hdr ( msg , &hdr ) ) {
         bool isRequest = (hdr->msg_flags & CMD_FLAG_REQUEST) == CMD_FLAG_REQUEST;

         if( (hdr->msg_appl == m_rf->getDict().app().getId()) ){
            if( hdr->msg_code == m_rf->getDict().cmdACR().getCommandCode() && isRequest ){
               m_stat->registerStatAttemp(stat_ctf_acr, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_rf->getDict().cmdACA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_ctf_acr, stat_attemp_received);
            }
         }
      }
   }
}

