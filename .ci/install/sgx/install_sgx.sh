#!/bin/bash

#  Copyright 2019-present Open Networking Foundation
#
#  SPDX-License-Identifier: Apache-2.0

set -o nounset
set -o errexit

export INSTALL_CI_PATH=$1

cd "$(dirname "${BASH_SOURCE[0]}")"
# shellcheck disable=SC1091
source "${INSTALL_CI_PATH}"/c3po/.ci/install/sgx/install_sgx_config.sh
# shellcheck disable=SC1091
source "${INSTALL_CI_PATH}"/c3po/.ci/common/utils.sh


check_process()
{
    # Kill any running process
    if pgrep -l ctf; then pkill ctf; fi
    if pgrep -l cdf; then pkill cdf; fi
    if pgrep -l kms; then pkill kms; fi
    if pgrep -xl dealer; then pkill -x dealer; fi
    if pgrep -xl dealer-out; then pkill -x dealer-out; fi
}


# Main

# Clean log dir or create it, if needed
log_step "Clean log directory ${LOG_DIR} ..."
clean_all_logs "${LOG_DIR}"


log_step "Check/kill processes, if any ..."
check_process


log_step "Install dealer ..."
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr/dealer && ./install.sh < "${INSTALL_CI_PATH}"/c3po/.ci/install/sgx/sgx-auto-install.txt 1>"${DEALER_INSTALL_STDOUT_LOG}" 2>"${DEALER_INSTALL_STDERR_LOG}"
cp -f "${INSTALL_CI_PATH}"/c3po/.ci/config/dealer-in.json "${INSTALL_CI_PATH}"/c3po/sgxcdr/dealer/conf/dealer.json


log_step "Make dealer ..."
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr/dealer && make clean && make SGX_MODE=HW SGX_DEBUG=1 1>"${DEALER_STDOUT_LOG}" 2>"${DEALER_STDERR_LOG}"


# update ca_bundle.h with MRENCLAVE / MRSIGNER
log_step "update ca_bundle.h with MRENCLAVE / MRSIGNER"

mapfile -t SGX_DEALER_KEYS < <(get_sgx_dealer_keys "${INSTALL_CI_PATH}")

MRENCLAVE=${SGX_DEALER_KEYS[0]}
MRSIGNER=${SGX_DEALER_KEYS[1]}
echo "MRENCLAVE = ${MRENCLAVE}"
echo "MRSIGNER = ${MRSIGNER}"

update_ca_bundle_keys "${INSTALL_CI_PATH}" "${MRENCLAVE}" "${MRSIGNER}"


log_step "Install kms ..."
cp -f "${INSTALL_CI_PATH}"/c3po/.ci/config/kms.json "${INSTALL_CI_PATH}"/c3po/sgxcdr/kms/conf/kms.json
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr/kms && ./install.sh < "${INSTALL_CI_PATH}"/c3po/.ci/install/sgx/sgx-auto-install.txt 1>"${KMS_INSTALL_STDOUT_LOG}" 2>"${KMS_INSTALL_STDERR_LOG}"


log_step "Make kms ..."
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr/kms && make clean && make SGX_MODE=HW SGX_DEBUG=1 1>"${KMS_STDOUT_LOG}" 2>"${KMS_STDERR_LOG}"


log_step "Install dealer-out ..."
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr && cp -R dealer dealer-out
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr/dealer-out && mv dealer dealer-out
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr/dealer-out && mkdir cdr
cp -f "${INSTALL_CI_PATH}"/c3po/.ci/config/dealer-out.json "${INSTALL_CI_PATH}"/c3po/sgxcdr/dealer-out/conf/dealer.json


log_step "Install router ..."
cd "${INSTALL_CI_PATH}"/c3po/sgxcdr/router && ./install.sh 1>"${ROUTER_INSTALL_STDOUT_LOG}" 2>"${ROUTER_INSTALL_STDERR_LOG}"


log_step "Install c3po deps ..."
cd "${INSTALL_CI_PATH}"/c3po && touch .agree && ./install.sh < "${INSTALL_CI_PATH}"/c3po/.ci/install/sgx/c3po-submodule-auto-install.txt 1>"${C3PO_DEPS_INSTALL_STDOUT_LOG}" 2>"${C3PO_DEPS_INSTALL_STDERR_LOG}"


log_step "Make util/ctf ..."
cp -f "${INSTALL_CI_PATH}"/c3po/.ci/config/ctf.json "${INSTALL_CI_PATH}"/c3po/ctf/conf/ctf.json
cd "${INSTALL_CI_PATH}"/c3po
make clean WHAT=util && make WHAT=util 1>"${UTIL_STDOUT_LOG}" 2>"${UTIL_STDERR_LOG}"
mkdir -p "${INSTALL_CI_PATH}"/c3po/ctf/logs && make clean WHAT=ctf && make WHAT=ctf 1>"${CTF_STDOUT_LOG}" 2>"${CTF_STDERR_LOG}"


log_step "Make ctf certificates ..."
cd "${INSTALL_CI_PATH}"/c3po/ctf/conf && ../bin/make_certs.sh ctf test3gpp.net


log_step "Make cdf ..."
cp -f "${INSTALL_CI_PATH}"/c3po/.ci/config/cdf.conf "${INSTALL_CI_PATH}"/c3po/cdf/conf/cdf.conf
cd "${INSTALL_CI_PATH}"/c3po
make clean WHAT=cdf && make WHAT=cdf 1>"${CDF_STDOUT_LOG}" 2>"${CDF_STDERR_LOG}"


log_step "Make cdf certificates ..."
cd "${INSTALL_CI_PATH}"/c3po/cdf/conf && ../bin/make_certs.sh sgx-kms-cdr test3gpp.net
