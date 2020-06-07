/*
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef __BEARER_H
#define __BEARER_H

#include <string>
#include <map>

#include "rule.h"

class Bearer
{
public:
   Bearer() {}
   ~Bearer() {}

   int32_t getEBI() { return m_ebi; }
   int32_t setEBI( int32_t v ) { m_ebi = v; return getEBI(); }

   RulesList &getComputedRules() { return m_computed_rules; }
   RulesList &getActivateNow() { return m_activate_now; }
   RulesList &getPcefInstalled() { return m_pcef_installed; }
   RulesList &getTdfInstalled() { return m_tdf_installed; }
   RulesList &getTscInstalled() { return m_tsc_installed; }

private:
   int32_t m_ebi;
   RulesList m_computed_rules;
   RulesList m_activate_now;
   RulesList m_pcef_installed;
   RulesList m_tdf_installed;
   RulesList m_tsc_installed;
};

class BearerMap : public std::map<int32_t,Bearer*>
{
public:
   BearerMap();
   ~BearerMap();
};

#endif // __BEARER_H
