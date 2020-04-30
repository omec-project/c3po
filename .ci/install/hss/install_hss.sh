#!/bin/bash

#  Copyright 2019-present Open Networking Foundation
#
#  SPDX-License-Identifier: Apache-2.0

set -o nounset
set -o errexit

export INSTALL_CI_PATH=$1

cd "$(dirname "${BASH_SOURCE[0]}")"
# shellcheck disable=SC1091
source "${INSTALL_CI_PATH}"/c3po/.ci/install/hss/install_hss_config.sh
# shellcheck disable=SC1091
source "${INSTALL_CI_PATH}"/c3po/.ci/common/utils.sh

check_process()
{
    # Kill any running process
    if pgrep -xl hss; then pkill -x hss; fi
}


# Main

# Clean log dir or create it, if needed
log_step "Clean log directory ${LOG_DIR} ..."
clean_all_logs "${LOG_DIR}"


log_step "Check/kill processes, if any ..."
check_process


log_step "Install hss ..."
cd "${INSTALL_CI_PATH}"/c3po && ./install.sh < "${INSTALL_CI_PATH}"/c3po/.ci/install/hss/hss-auto-install.txt 1>"${HSS_INSTALL_STDOUT_LOG}" 2>"${HSS_INSTALL_STDERR_LOG}"


log_step "Make util ..."
make clean WHAT=util && make WHAT=util 1>"${HSSSEC_STDOUT_LOG}" 2>"${HSSSEC_STDERR_LOG}"


log_step "Make hsssec ..."
make clean WHAT=hsssec && make WHAT=hsssec 1>>"${HSSSEC_STDOUT_LOG}" 2>>"${HSSSEC_STDERR_LOG}"


log_step "Copy config ..."
cp -f "${INSTALL_CI_PATH}"/c3po/.ci/config/hss.json "${INSTALL_CI_PATH}"/c3po/hss/conf/hss.json


log_step "Make Certificates ..."
cd "${INSTALL_CI_PATH}"/c3po/hss/conf && ../bin/make_certs.sh hss openair4G.eur 2>&1


log_step "Make hss ..."
cd "${INSTALL_CI_PATH}"/c3po
make clean WHAT=hss && make WHAT=hss 1>"${HSS_STDOUT_LOG}" 2>"${HSS_STDERR_LOG}"
