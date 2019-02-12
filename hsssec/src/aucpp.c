/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include "aucpp.h"
#include "auc.h"

uint8_t                                *
sqn_ms_derive_cpp (
  const uint8_t opc[16],
  uint8_t * key,
  uint8_t * auts,
  uint8_t * rand_p)
{
    return sqn_ms_derive(opc, key, auts, rand_p);
}

void
generate_random_cpp (
  uint8_t * random_p,
  ssize_t length)
{
    generate_random(random_p, length);
}

int generate_vector_cpp(const uint8_t opc[16], uint64_t imsi, uint8_t key[16], uint8_t plmn[3],
                    uint8_t sqn[6], auc_vector_t *vector){
    return generate_vector(opc, imsi, key, plmn, sqn, vector);
}
