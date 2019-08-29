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
set -o errexit

INSTALL_CI_PATH=$1
APP=$2

cd "$(dirname "${BASH_SOURCE[0]}")"
source "${INSTALL_CI_PATH}"/c3po/.ci/install/"${APP}"/install_"${APP}"_config.sh

echo "${LOG_DIR}"
