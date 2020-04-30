#!/bin/bash

#  Copyright 2019-present Open Networking Foundation
#
#  SPDX-License-Identifier: Apache-2.0

set -o nounset

# Import common variables.
cd "$(dirname "${BASH_SOURCE[0]}")" || exit
source "${INSTALL_CI_PATH}"/c3po/.ci/install/install_config.sh

HSS_APP="hss"
export LOG_DIR=${BASE_LOG_DIR}/${HSS_APP}


# Log file names.

export HSS_INSTALL_STDOUT_LOG="${LOG_DIR}/hss${ACTION_INST}${STDOUT_EXT}"
export HSS_INSTALL_STDERR_LOG="${LOG_DIR}/hss${ACTION_INST}${STDERR_EXT}"

export HSSSEC_STDOUT_LOG="${LOG_DIR}/hsssec${ACTION_MAKE}${STDOUT_EXT}"
export HSSSEC_STDERR_LOG="${LOG_DIR}/hsssec${ACTION_MAKE}${STDERR_EXT}"

export HSS_STDOUT_LOG="${LOG_DIR}/hss${ACTION_MAKE}${STDOUT_EXT}"
export HSS_STDERR_LOG="${LOG_DIR}/hss${ACTION_MAKE}${STDERR_EXT}"
