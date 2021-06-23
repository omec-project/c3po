/*
* Copyright (c) 2021  Great Software Laboratory Pvt. Ltd
* Copyright 2019-present Open Networking Foundation
* Copyright (c) 2017 Sprint
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <iostream>
#include <inttypes.h>
#include <iomanip>

#include "dataaccess.h"
#include "sutility.h"
#include "serror.h"
#include "common_def.h"

#include "util.h"
#include "logger.h"
#include "options.h"
#include "redis_dataaccess.h"
#include "cassandra_dataaccess.h"


extern "C" {
    #include "auc.h"
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void convert_ascii_to_binary(unsigned char *dest, unsigned char *src, int length)
{
    int i;
    for (i = 0; i < length; i++)
    dest[i] = (ASCII_TO_BINARY(src[i << 1]) << 4) | ASCII_TO_BINARY(src[(i << 1) + 1]);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DAEventIdList::DAEventIdList()
{
}

DAEventIdList::~DAEventIdList()
{
    DAEventIdList::iterator it;

    while ( (it = begin()) != end()) {
        delete *it;
        pop_front();
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DAEventList::DAEventList()
{
}

DAEventList::~DAEventList()
{
    DAEventList::iterator it;

    while ( (it = begin()) != end() ) {
        delete *it;
        pop_front();
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DataAccess* DataAccess::createDataAccessObj(std::string dbms_type)
{
    DataAccess *p_db = nullptr;

    if(dbms_type == "redis") {
        p_db = new RedisDataAccess();
    }
    else if(dbms_type == "cassandra") {
        p_db = new CassandraDataAccess();
    }
    else {
        Logger::system().error( "DataAccess::%s - Invalid data base type configured", __func__);
    }

    return p_db;
}
