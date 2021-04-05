# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2019 Intel Corporation

# Multi-stage Dockerfile

ARG BASE_OS=ubuntu:16.04
ARG RUN_BASE=runtime

## Stage build: kitchen sink stage for compiling dependencies and C3PO bins
FROM $BASE_OS as build
ARG CPUS

WORKDIR /c3po
COPY install_builddeps.sh .
COPY modules modules
COPY patches patches 

RUN ./install_builddeps.sh
RUN rm -rf /c3po-libs && mkdir -p /c3po-libs && \
    find modules -name "*.so" | xargs -I '{}' cp '{}' /c3po-libs

COPY . .
RUN make -j $CPUS

## Stage runtime: no utils
FROM $BASE_OS as runtime
COPY install_rundeps.sh .
COPY --from=build /c3po-libs /usr/local/lib
RUN ldconfig
RUN bash -c "source ./install_rundeps.sh && update_pkg_cache"

## Stage runtime-utils: install common production runtime utils
FROM runtime as runtime-utils
RUN bash -c "source ./install_rundeps.sh && install_run_utils"

## Stage cdf
FROM $RUN_BASE as cdf
RUN bash -c "source ./install_rundeps.sh && install_run_cdf_deps && cleanup_image"
COPY --from=build /c3po/cdf/bin /bin

## Stage ctf
FROM $RUN_BASE as ctf
RUN bash -c "source ./install_rundeps.sh && install_run_ctf_deps && cleanup_image"
COPY --from=build /c3po/ctf/bin /bin

## Stage hss
FROM $RUN_BASE as hss
RUN bash -c "source ./install_rundeps.sh && install_run_hss_deps && cleanup_image"
COPY --from=build /c3po/hss/bin /bin
COPY --from=build /usr/local/lib/freeDiameter /usr/local/lib/freeDiameter
COPY --from=build /c3po/modules/prometheus-cpp/_build/deploy/usr/local/lib /usr/local/lib

## Stage hssgtw
FROM $RUN_BASE as hssgtw
RUN bash -c "source ./install_rundeps.sh && install_run_hssgtw_deps && cleanup_image"
COPY --from=build /c3po/hssgtw/bin /bin

## Stage pcrf
FROM $RUN_BASE as pcrf
RUN bash -c "source ./install_rundeps.sh && install_run_pcrf_deps && cleanup_image"
COPY --from=build /c3po/pcrf/bin /bin

## Stage pcef
FROM $RUN_BASE as pcef
RUN bash -c "source ./install_rundeps.sh && install_run_pcef_deps && cleanup_image"
COPY --from=build /c3po/pcrf/pcef/bin /bin

## Stage tdf
FROM $RUN_BASE as tdf
RUN bash -c "source ./install_rundeps.sh && install_run_tdf_deps && cleanup_image"
COPY --from=build /c3po/pcrf/tdf/bin /bin

## Stage tssf
FROM $RUN_BASE as tssf
RUN bash -c "source ./install_rundeps.sh && install_run_tssf_deps && cleanup_image"
COPY --from=build /c3po/pcrf/tssf/bin /bin

## Stage hssdb
FROM cassandra:2.1 as hssdb
RUN apt-get update && apt-get -y install \
      dnsutils && \
    rm -rf /var/lib/apt/lists/*
COPY hss/db/oai_db.cql /opt/c3po/hssdb/oai_db.cql
COPY db_docs/data_provisioning_users.sh db_docs/data_provisioning_mme.sh /bin/
