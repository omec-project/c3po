#!/bin/bash

#  Copyright 2019-present Open Networking Foundation
#
#  SPDX-License-Identifier: Apache-2.0

set -o nounset
set -o errexit

INSTALL_CI_PATH=$1
APP=$2

cd "$(dirname "${BASH_SOURCE[0]}")"
source "${INSTALL_CI_PATH}"/c3po/.ci/install/"${APP}"/install_"${APP}"_config.sh

echo "${LOG_DIR}"
