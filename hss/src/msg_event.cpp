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


#include "msg_event.h"

#include <iostream>
#include <functional>

struct fd_hook_hdl *HookEvent::m_hdl[2] = {NULL, NULL};
SStats *HookEvent::m_stat = NULL;

s6t::Application *HookEvent::m_s6t;
s6as6d::Application *HookEvent::m_s6as6d;
s6c::Application *HookEvent::m_s6c;

void HookEvent::init(SStats* stat, s6t::Application *s6t, s6as6d::Application *s6as6d, s6c::Application *s6c){
   m_stat = stat;
   m_s6t = s6t;
   m_s6as6d = s6as6d;
   m_s6c = s6c;

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

         if( (hdr->msg_appl == m_s6as6d->getDict().app().getId()) ){
            if( hdr->msg_code == m_s6as6d->getDict().cmdAUIR().getCommandCode() && isRequest ){
               m_stat->registerStatAttemp(stat_hss_air, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_s6as6d->getDict().cmdAUIA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_air, stat_sent_ko);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdUPLR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_ulr, stat_received_ko);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdUPLA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_ulr, stat_sent_ko);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdPUUR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_pur, stat_received_ko);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdPUUA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_pur, stat_sent_ko);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdINSDR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_idr, stat_sent_ko);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdINSDA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_idr, stat_received_ko);
            }
         }
         else if ( hdr->msg_appl == m_s6t->getDict().app().getId() ){
            if( (hdr->msg_code == m_s6t->getDict().cmdCOIR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_cir, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdCOIA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_hss_cir, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdREIR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_rir, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdREIA().getCommandCode()) && !isRequest){
               m_stat->registerStatAttemp(stat_hss_rir, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdNIIR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_nir, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdNIIA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_nir, stat_sent_ko);
            }
         }
         else if (hdr->msg_appl == m_s6c->getDict().app().getId()){
            if( (hdr->msg_code == m_s6c->getDict().cmdSERIFSR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_srr, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_s6c->getDict().cmdSERIFSA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_hss_srr, stat_sent_ko);
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

         if( (hdr->msg_appl == m_s6as6d->getDict().app().getId()) ){
            if( hdr->msg_code == m_s6as6d->getDict().cmdAUIR().getCommandCode() && isRequest ){
               m_stat->registerStatAttemp(stat_hss_air, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_s6as6d->getDict().cmdAUIA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_air, stat_attemp_sent);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdUPLR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_ulr, stat_attemp_received);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdUPLA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_ulr, stat_attemp_sent);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdPUUR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_pur, stat_attemp_received);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdPUUA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_pur, stat_attemp_sent);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdINSDR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_idr, stat_attemp_received);
            }
            else if( (hdr->msg_code == m_s6as6d->getDict().cmdINSDA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_idr, stat_attemp_sent);
            }
         }
         else if ( hdr->msg_appl == m_s6t->getDict().app().getId() ){
            if( (hdr->msg_code == m_s6t->getDict().cmdCOIR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_cir, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdCOIA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_hss_cir, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdREIR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_rir, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdREIA().getCommandCode()) && !isRequest){
               m_stat->registerStatAttemp(stat_hss_rir, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdNIIR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_nir, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_s6t->getDict().cmdNIIA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_hss_nir, stat_attemp_sent);
            }
         }
         else if (hdr->msg_appl == m_s6c->getDict().app().getId()){
            if( (hdr->msg_code == m_s6c->getDict().cmdSERIFSR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_hss_srr, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_s6c->getDict().cmdSERIFSA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_hss_srr, stat_sent_ko);
            }
         }
      }
   }
}

