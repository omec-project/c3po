#!/bin/bash

#  Copyright 2019-present Open Networking Foundation
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http:# www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

set -o nounset

# Import common variables.
cd "$(dirname "${BASH_SOURCE[0]}")" || exit
source "${INSTALL_CI_PATH}"/c3po/.ci/install/install_config.sh

SGX_APP="sgx"
export LOG_DIR=${BASE_LOG_DIR}/${SGX_APP}


# Log file names.

export DEALER_INSTALL_STDOUT_LOG="${LOG_DIR}/dealer${ACTION_INST}${STDOUT_EXT}"
export DEALER_INSTALL_STDERR_LOG="${LOG_DIR}/dealer${ACTION_INST}${STDERR_EXT}"

export DEALER_STDOUT_LOG="${LOG_DIR}/dealer${ACTION_MAKE}${STDOUT_EXT}"
export DEALER_STDERR_LOG="${LOG_DIR}/dealer${ACTION_MAKE}${STDERR_EXT}"

export KMS_INSTALL_STDOUT_LOG="${LOG_DIR}/kms${ACTION_INST}${STDOUT_EXT}"
export KMS_INSTALL_STDERR_LOG="${LOG_DIR}/kms${ACTION_INST}${STDERR_EXT}"

export KMS_STDOUT_LOG="${LOG_DIR}/kms${ACTION_MAKE}${STDOUT_EXT}"
export KMS_STDERR_LOG="${LOG_DIR}/kms${ACTION_MAKE}${STDERR_EXT}"

export ROUTER_INSTALL_STDOUT_LOG="${LOG_DIR}/router${ACTION_INST}${STDOUT_EXT}"
export ROUTER_INSTALL_STDERR_LOG="${LOG_DIR}/router${ACTION_INST}${STDERR_EXT}"

export UTIL_STDOUT_LOG="${LOG_DIR}/util${ACTION_MAKE}${STDOUT_EXT}"
export UTIL_STDERR_LOG="${LOG_DIR}/util${ACTION_MAKE}${STDERR_EXT}"

export CTF_STDOUT_LOG="${LOG_DIR}/ctf${ACTION_MAKE}${STDOUT_EXT}"
export CTF_STDERR_LOG="${LOG_DIR}/ctf${ACTION_MAKE}${STDERR_EXT}"

export CDF_STDOUT_LOG="${LOG_DIR}/cdf${ACTION_MAKE}${STDOUT_EXT}"
export CDF_STDERR_LOG="${LOG_DIR}/cdf${ACTION_MAKE}${STDERR_EXT}"
