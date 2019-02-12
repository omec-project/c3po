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

gx::Application *HookEvent::m_gx;
rx::Application *HookEvent::m_rx;
sd::Application *HookEvent::m_sd;
st::Application *HookEvent::m_st;


void HookEvent::init(SStats* stat, gx::Application *gx, rx::Application *rx, sd::Application *sd, st::Application *st){
   m_stat = stat;
   m_gx = gx;
   m_rx = rx;
   m_sd = sd;
   m_st = st;

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

         if( (hdr->msg_appl == m_gx->getDict().app().getId()) ){
            if( hdr->msg_code == m_gx->getDict().cmdCRCR().getCommandCode() ){
               m_stat->registerStatAttemp(stat_pcrf_gx_ccr, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_gx->getDict().cmdCRCA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_gx_ccr, stat_sent_ko);
            }
            else if( (hdr->msg_code == m_gx->getDict().cmdREAR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_gx_rar, stat_sent_ko);
            }
            else if( (hdr->msg_code == m_gx->getDict().cmdREAA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_gx_rar, stat_received_ko);
            }
         }
         else if ( hdr->msg_appl == m_sd->getDict().app().getId() ){
            if( (hdr->msg_code == m_sd->getDict().cmdTSR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_sd_tsr, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdTSA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_pcrf_sd_tsr, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdCRCR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_sd_ccr, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdCRCA().getCommandCode()) && !isRequest){
               m_stat->registerStatAttemp(stat_pcrf_sd_ccr, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdREAR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_sd_rar, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdREAA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_sd_rar, stat_received_ko);
            }
         }
         else if (hdr->msg_appl == m_st->getDict().app().getId()){
            if( (hdr->msg_code == m_sd->getDict().cmdTSR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_st_tsr, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_st->getDict().cmdTSA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_pcrf_st_tsr, stat_received_ko);
            }
            else if ( (hdr->msg_code == m_st->getDict().cmdSETR().getCommandCode()) && isRequest ) {
               m_stat->registerStatAttemp(stat_pcrf_st_str, stat_sent_ko);
            }
            else if ( (hdr->msg_code == m_st->getDict().cmdSETA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_pcrf_st_str, stat_received_ko);
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

         if( hdr->msg_appl == m_gx->getDict().app().getId() ){
            if( (hdr->msg_code == m_gx->getDict().cmdCRCR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_gx_ccr, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_gx->getDict().cmdCRCA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_gx_ccr, stat_attemp_sent);
            }
            else if( (hdr->msg_code == m_gx->getDict().cmdREAR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_gx_rar, stat_attemp_sent);
            }
            else if( (hdr->msg_code == m_gx->getDict().cmdREAA().getCommandCode()) && !isRequest){
               m_stat->registerStatAttemp(stat_pcrf_gx_rar, stat_attemp_received);
            }
         }
         else if ( hdr->msg_appl == m_sd->getDict().app().getId() ){
            if( (hdr->msg_code == m_sd->getDict().cmdTSR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_sd_tsr, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdTSA().getCommandCode()) && !isRequest) {
               m_stat->registerStatAttemp(stat_pcrf_sd_tsr, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdCRCR().getCommandCode()) && isRequest){
               m_stat->registerStatAttemp(stat_pcrf_sd_ccr, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdCRCA().getCommandCode()) && !isRequest){
               m_stat->registerStatAttemp(stat_pcrf_sd_ccr, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdREAR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_sd_rar, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_sd->getDict().cmdREAA().getCommandCode()) && !isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_sd_rar, stat_attemp_received);
            }
         }
         else if (hdr->msg_appl == m_st->getDict().app().getId()){
            if( (hdr->msg_code == m_sd->getDict().cmdTSR().getCommandCode()) && isRequest ){
               m_stat->registerStatAttemp(stat_pcrf_st_tsr, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_st->getDict().cmdTSA().getCommandCode()) && !isRequest ) {
               m_stat->registerStatAttemp(stat_pcrf_st_tsr, stat_attemp_received);
            }
            else if ( (hdr->msg_code == m_st->getDict().cmdSETR().getCommandCode()) && isRequest ) {
               m_stat->registerStatAttemp(stat_pcrf_st_str, stat_attemp_sent);
            }
            else if ( (hdr->msg_code == m_st->getDict().cmdSETA().getCommandCode()) && !isRequest ) {
               m_stat->registerStatAttemp(stat_pcrf_st_str, stat_attemp_received);
            }
         }
      }
   }
}

