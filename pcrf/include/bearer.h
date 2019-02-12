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
