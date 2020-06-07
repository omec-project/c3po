#!/bin/bash

#  Copyright 2019-present Open Networking Foundation
#
#  SPDX-License-Identifier: Apache-2.0

set -o nounset

export BASE_LOG_DIR="/var/log/cicd/install"

export ACTION_INST="_install"
export ACTION_MAKE="_make"

export STDOUT_EXT=".stdout.log"
export STDERR_EXT=".stderr.log"
