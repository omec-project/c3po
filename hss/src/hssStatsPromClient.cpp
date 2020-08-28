/*
 * Copyright 2020-present Open Networking Foundation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <iostream>
#include <chrono>
#include <map>
#include <memory>
#include <thread>
#include <sstream>
#include "hssStatsPromClient.h"

using namespace prometheus;
std::shared_ptr<Registry> registry;

void hssStatsSetupPrometheusThread(uint16_t port)
{
    std::stringstream ss;
    ss << "0.0.0.0"<<":"<<port;
    registry = std::make_shared<Registry>();
    /* Create single instance */ 
    hssStats::Instance(); 
    Exposer exposer{ss.str(), 1};
    std::string metrics("/metrics");
    exposer.RegisterCollectable(registry, metrics);
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
hssStats::hssStats()
{
	 mme_msg_tx_m = new mme_msg_tx_counters;
	 mme_msg_rx_m = new mme_msg_rx_counters;
}
hssStats* hssStats::Instance() 
{
	static hssStats object;
	return &object; 
}


mme_msg_tx_counters::mme_msg_tx_counters():
mme_msg_tx_family(BuildCounter().Name("number_of_messages_sent").Help("Number of messages sent by HSS").Labels({{"direction","outgoing"}}).Register(*registry)),
mme_msg_tx_s6a_auth_info_answer_success(mme_msg_tx_family.Add({{"interface","s6a"},{"msg_type","auth_info_answer"},{"result","success"}})),
mme_msg_tx_s6a_auth_info_answer_failure(mme_msg_tx_family.Add({{"interface","s6a"},{"msg_type","auth_info_answer"},{"result","failure"}})),
mme_msg_tx_s6a_update_location_answer_success(mme_msg_tx_family.Add({{"interface","s6a"},{"msg_type","update_location_answer"},{"result","success"}})),
mme_msg_tx_s6a_update_location_answer_failure(mme_msg_tx_family.Add({{"interface","s6a"},{"msg_type","update_location_answer"},{"result","failure"}})),
mme_msg_tx_s6a_purge_answer_success(mme_msg_tx_family.Add({{"interface","s6a"},{"msg_type","purge_answer"},{"result","success"}})),
mme_msg_tx_s6a_purge_answer_failure(mme_msg_tx_family.Add({{"interface","s6a"},{"msg_type","purge_answer"},{"result","failure"}})),
mme_msg_tx_s6a_cancel_location_request(mme_msg_tx_family.Add({{"interface","s6a"},{"msg_type","cancel_location_request"}}))
{
}


mme_msg_tx_counters::~mme_msg_tx_counters()
{
}




mme_msg_rx_counters::mme_msg_rx_counters():
mme_msg_rx_family(BuildCounter().Name("number_of_messages_received").Help("Number of messages received by HSS ").Labels({{"direction","incoming"}}).Register(*registry)),
mme_msg_rx_s6a_auth_info_request(mme_msg_rx_family.Add({{"interface","s6a"},{"msg_type","auth_info_request"}})),
mme_msg_rx_s6a_auth_info_request_resync(mme_msg_rx_family.Add({{"interface","s6a"},{"msg_type","auth_info_request_resync"}})),
mme_msg_rx_s6a_update_location_request(mme_msg_rx_family.Add({{"interface","s6a"},{"msg_type","update_location_request"}})),
mme_msg_rx_s6a_purge_request(mme_msg_rx_family.Add({{"interface","s6a"},{"msg_type","purge_request"}}))
{
}


mme_msg_rx_counters::~mme_msg_rx_counters()
{
}




void hssStats::increment(hssStatsCounter name,std::map<std::string,std::string> labels)
{
	switch(name) {
	case hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_SUCCESS:
	{
		mme_msg_tx_m->mme_msg_tx_s6a_auth_info_answer_success.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject1 *obj = static_cast<mme_msg_tx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject1 *obj = mme_msg_tx_m->add_dynamic1("interface","s6a","msg_type","auth_info_answer","result","success",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject2 *obj = static_cast<mme_msg_tx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject2 *obj = mme_msg_tx_m->add_dynamic2("interface","s6a","msg_type","auth_info_answer","result","success",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject3 *obj = static_cast<mme_msg_tx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject3 *obj = mme_msg_tx_m->add_dynamic3("interface","s6a","msg_type","auth_info_answer","result","success",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE:
	{
		mme_msg_tx_m->mme_msg_tx_s6a_auth_info_answer_failure.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject1 *obj = static_cast<mme_msg_tx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject1 *obj = mme_msg_tx_m->add_dynamic1("interface","s6a","msg_type","auth_info_answer","result","failure",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject2 *obj = static_cast<mme_msg_tx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject2 *obj = mme_msg_tx_m->add_dynamic2("interface","s6a","msg_type","auth_info_answer","result","failure",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject3 *obj = static_cast<mme_msg_tx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject3 *obj = mme_msg_tx_m->add_dynamic3("interface","s6a","msg_type","auth_info_answer","result","failure",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_SUCCESS:
	{
		mme_msg_tx_m->mme_msg_tx_s6a_update_location_answer_success.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject1 *obj = static_cast<mme_msg_tx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject1 *obj = mme_msg_tx_m->add_dynamic1("interface","s6a","msg_type","update_location_answer","result","success",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject2 *obj = static_cast<mme_msg_tx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject2 *obj = mme_msg_tx_m->add_dynamic2("interface","s6a","msg_type","update_location_answer","result","success",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject3 *obj = static_cast<mme_msg_tx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject3 *obj = mme_msg_tx_m->add_dynamic3("interface","s6a","msg_type","update_location_answer","result","success",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE:
	{
		mme_msg_tx_m->mme_msg_tx_s6a_update_location_answer_failure.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject1 *obj = static_cast<mme_msg_tx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject1 *obj = mme_msg_tx_m->add_dynamic1("interface","s6a","msg_type","update_location_answer","result","failure",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject2 *obj = static_cast<mme_msg_tx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject2 *obj = mme_msg_tx_m->add_dynamic2("interface","s6a","msg_type","update_location_answer","result","failure",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject3 *obj = static_cast<mme_msg_tx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject3 *obj = mme_msg_tx_m->add_dynamic3("interface","s6a","msg_type","update_location_answer","result","failure",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_TX_S6A_PURGE_ANSWER_SUCCESS:
	{
		mme_msg_tx_m->mme_msg_tx_s6a_purge_answer_success.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject1 *obj = static_cast<mme_msg_tx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject1 *obj = mme_msg_tx_m->add_dynamic1("interface","s6a","msg_type","purge_answer","result","success",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject2 *obj = static_cast<mme_msg_tx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject2 *obj = mme_msg_tx_m->add_dynamic2("interface","s6a","msg_type","purge_answer","result","success",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject3 *obj = static_cast<mme_msg_tx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject3 *obj = mme_msg_tx_m->add_dynamic3("interface","s6a","msg_type","purge_answer","result","success",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_TX_S6A_PURGE_ANSWER_FAILURE:
	{
		mme_msg_tx_m->mme_msg_tx_s6a_purge_answer_failure.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject1 *obj = static_cast<mme_msg_tx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject1 *obj = mme_msg_tx_m->add_dynamic1("interface","s6a","msg_type","purge_answer","result","failure",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject2 *obj = static_cast<mme_msg_tx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject2 *obj = mme_msg_tx_m->add_dynamic2("interface","s6a","msg_type","purge_answer","result","failure",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject3 *obj = static_cast<mme_msg_tx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject3 *obj = mme_msg_tx_m->add_dynamic3("interface","s6a","msg_type","purge_answer","result","failure",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_TX_S6A_CANCEL_LOCATION_REQUEST:
	{
		mme_msg_tx_m->mme_msg_tx_s6a_cancel_location_request.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject1 *obj = static_cast<mme_msg_tx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject1 *obj = mme_msg_tx_m->add_dynamic1("interface","s6a","msg_type","cancel_location_request",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject2 *obj = static_cast<mme_msg_tx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject2 *obj = mme_msg_tx_m->add_dynamic2("interface","s6a","msg_type","cancel_location_request",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_tx_DynamicMetricObject3 *obj = static_cast<mme_msg_tx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_tx_DynamicMetricObject3 *obj = mme_msg_tx_m->add_dynamic3("interface","s6a","msg_type","cancel_location_request",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_RX_S6A_AUTH_INFO_REQUEST:
	{
		mme_msg_rx_m->mme_msg_rx_s6a_auth_info_request.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject1 *obj = static_cast<mme_msg_rx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject1 *obj = mme_msg_rx_m->add_dynamic1("interface","s6a","msg_type","auth_info_request",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject2 *obj = static_cast<mme_msg_rx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject2 *obj = mme_msg_rx_m->add_dynamic2("interface","s6a","msg_type","auth_info_request",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject3 *obj = static_cast<mme_msg_rx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject3 *obj = mme_msg_rx_m->add_dynamic3("interface","s6a","msg_type","auth_info_request",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_RX_S6A_AUTH_INFO_REQUEST_RESYNC:
	{
		mme_msg_rx_m->mme_msg_rx_s6a_auth_info_request_resync.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject1 *obj = static_cast<mme_msg_rx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject1 *obj = mme_msg_rx_m->add_dynamic1("interface","s6a","msg_type","auth_info_request_resync",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject2 *obj = static_cast<mme_msg_rx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject2 *obj = mme_msg_rx_m->add_dynamic2("interface","s6a","msg_type","auth_info_request_resync",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject3 *obj = static_cast<mme_msg_rx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject3 *obj = mme_msg_rx_m->add_dynamic3("interface","s6a","msg_type","auth_info_request_resync",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_RX_S6A_UPDATE_LOCATION_REQUEST:
	{
		mme_msg_rx_m->mme_msg_rx_s6a_update_location_request.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject1 *obj = static_cast<mme_msg_rx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject1 *obj = mme_msg_rx_m->add_dynamic1("interface","s6a","msg_type","update_location_request",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject2 *obj = static_cast<mme_msg_rx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject2 *obj = mme_msg_rx_m->add_dynamic2("interface","s6a","msg_type","update_location_request",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject3 *obj = static_cast<mme_msg_rx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject3 *obj = mme_msg_rx_m->add_dynamic3("interface","s6a","msg_type","update_location_request",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	case hssStatsCounter::MME_MSG_RX_S6A_PURGE_REQUEST:
	{
		mme_msg_rx_m->mme_msg_rx_s6a_purge_request.Increment();
		if(labels.size() == 0) {
		break;
		}
		if(labels.size() == 1) {
		auto it = labels. begin();
		struct Node s1 = {name, it->first, it->second};
		auto it1 = metrics_map.find(s1);
		if(it1 != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject1 *obj = static_cast<mme_msg_rx_DynamicMetricObject1 *>(it1->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject1 *obj = mme_msg_rx_m->add_dynamic1("interface","s6a","msg_type","purge_request",it->first, it->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		} else if (labels.size() == 2) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		struct Node s1 = {name, it1->first+it2->first, it2->second+it2->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject2 *obj = static_cast<mme_msg_rx_DynamicMetricObject2 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject2 *obj = mme_msg_rx_m->add_dynamic2("interface","s6a","msg_type","purge_request",it1->first, it1->second, it2->first, it2->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		} 
		} else if (labels.size() == 3) {
		auto it1 = labels. begin();
		auto it2 = it1++;
		auto it3 = it2++;
		struct Node s1 = {name, it1->first+it2->first+it3->first, it1->second+it2->second+it3->second};
		auto itf = metrics_map.find(s1);
		if(itf != metrics_map.end()) {
		    mme_msg_rx_DynamicMetricObject3 *obj = static_cast<mme_msg_rx_DynamicMetricObject3 *>(itf->second);
		    obj->counter.Increment();
		} else {
		    mme_msg_rx_DynamicMetricObject3 *obj = mme_msg_rx_m->add_dynamic3("interface","s6a","msg_type","purge_request",it1->first, it1->second, it2->first, it2->second, it3->first, it3->second);
		    auto p1 = std::make_pair(s1, obj);
		    metrics_map.insert(p1);
		    obj->counter.Increment();
		}
		}
		break;
	}
	default:
		break;
	}
}




void hssStats::decrement(hssStatsCounter name,std::map<std::string,std::string> labels)
{
	switch(name) {
	default:
		break;
	}
}


#ifdef TEST_PROMETHEUS 
#include <unistd.h>
int main() {
	std::thread prom(hssStatsSetupPrometheusThread, 3081);
	prom.detach();
	sleep(1);
	while(1) {
	spgwStats::Instance()->increment(spgwStatsCounter::NUM_UE_SPGW_ACTIVE_SUBSCRIBERS, {{"mme_addr","1.1.1.1"},{"spgwu_addr", "1.1.1.2"}});
	sleep(1);
	}
}
#endif
