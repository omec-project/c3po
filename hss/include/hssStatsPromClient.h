/*
 * Copyright 2020-present Open Networking Foundation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _INCLUDE_hssStats_H__
#define _INCLUDE_hssStats_H__

#include <prometheus/gauge.h>
#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>


using namespace prometheus;
extern std::shared_ptr<Registry> registry;


void hssStatsSetupPrometheusThread(uint16_t port);

enum class hssStatsCounter {
	MME_MSG_TX_S6A_AUTH_INFO_ANSWER_SUCCESS,
	MME_MSG_TX_S6A_AUTH_INFO_ANSWER_FAILURE,
	MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_SUCCESS,
	MME_MSG_TX_S6A_UPDATE_LOCATION_ANSWER_FAILURE,
	MME_MSG_TX_S6A_PURGE_ANSWER_SUCCESS,
	MME_MSG_TX_S6A_PURGE_ANSWER_FAILURE,
	MME_MSG_TX_S6A_CANCEL_LOCATION_REQUEST,
	MME_MSG_RX_S6A_AUTH_INFO_REQUEST,
	MME_MSG_RX_S6A_UPDATE_LOCATION_REQUEST,
	MME_MSG_RX_S6A_PURGE_REQUEST
};

struct Node 
{
   hssStatsCounter id;
	std::string label_k;
	std::string label_v;

	Node(hssStatsCounter id, std::string label_k, std::string label_v)
	{
		this->id = id;
		this->label_k = label_k;
		this->label_v = label_v;
	}

	// operator== is required to compare keys in case of hash collision
	bool operator==(const Node &p) const
	{
		return label_k == p.label_k && label_v == p.label_v && id == p.id;
	}
};

struct hash_fn
{
	std::size_t operator() (const Node &node) const
	{
		std::size_t h1 = std::hash<std::string>()(node.label_k);
		std::size_t h2 = std::hash<std::string>()(node.label_v);
		std::size_t h3 = std::size_t(node.id);
		return h1 ^ h2 ^ h3;
	}
};


class DynamicMetricObject {
	public:
};


class mme_msg_tx_DynamicMetricObject1 : public DynamicMetricObject {
	public:
		mme_msg_tx_DynamicMetricObject1(Family<Counter> &mme_msg_tx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string label_k2,std::string label_v2,std::string dlabel_k0, std::string dlabel_v0):
		 DynamicMetricObject(),
		 counter(mme_msg_tx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{label_k2, label_v2},{dlabel_k0, dlabel_v0}}))
		{
		}
		mme_msg_tx_DynamicMetricObject1(Family<Counter> &mme_msg_tx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0):
		 DynamicMetricObject(),
		 counter(mme_msg_tx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{dlabel_k0, dlabel_v0}}))
		{
		}
		~mme_msg_tx_DynamicMetricObject1()
		{
		}
		Counter &counter;
};


class mme_msg_tx_DynamicMetricObject2 : public DynamicMetricObject {
	public:
		mme_msg_tx_DynamicMetricObject2(Family<Counter> &mme_msg_tx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string label_k2,std::string label_v2,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1):
		 DynamicMetricObject(),
		 counter(mme_msg_tx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{label_k2, label_v2},{dlabel_k0, dlabel_v0}, {dlabel_k1, dlabel_v1}}))
		{
		}
		mme_msg_tx_DynamicMetricObject2(Family<Counter> &mme_msg_tx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1):
		 DynamicMetricObject(),
		 counter(mme_msg_tx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{dlabel_k0, dlabel_v0}, {dlabel_k1, dlabel_v1}}))
		{
		}
		~mme_msg_tx_DynamicMetricObject2()
		{
		}
		Counter &counter;
};


class mme_msg_tx_DynamicMetricObject3 : public DynamicMetricObject {
	public:
		mme_msg_tx_DynamicMetricObject3(Family<Counter> &mme_msg_tx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string label_k2,std::string label_v2,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1,std::string dlabel_k2, std::string dlabel_v2):
		 DynamicMetricObject(),
		 counter(mme_msg_tx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{label_k2, label_v2},{dlabel_k0, dlabel_v0},{dlabel_k1, dlabel_v1},{dlabel_k2, dlabel_v2}}))
		{
		}
		mme_msg_tx_DynamicMetricObject3(Family<Counter> &mme_msg_tx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1,std::string dlabel_k2, std::string dlabel_v2):
		 DynamicMetricObject(),
		 counter(mme_msg_tx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{dlabel_k0, dlabel_v0},{dlabel_k1, dlabel_v1},{dlabel_k2, dlabel_v2}}))
		{
		}
		~mme_msg_tx_DynamicMetricObject3()
		{
		}
		Counter &counter;
};
class mme_msg_tx_counters {
	public:
	mme_msg_tx_counters();
	~mme_msg_tx_counters();
	Family<Counter> &mme_msg_tx_family;
	Counter &mme_msg_tx_s6a_auth_info_answer_success;
	Counter &mme_msg_tx_s6a_auth_info_answer_failure;
	Counter &mme_msg_tx_s6a_update_location_answer_success;
	Counter &mme_msg_tx_s6a_update_location_answer_failure;
	Counter &mme_msg_tx_s6a_purge_answer_success;
	Counter &mme_msg_tx_s6a_purge_answer_failure;
	Counter &mme_msg_tx_s6a_cancel_location_request;

	mme_msg_tx_DynamicMetricObject1* add_dynamic1(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string label_k2,std::string label_v2,std::string dlabel_k0, std::string dlabel_v0) {
		return new mme_msg_tx_DynamicMetricObject1(mme_msg_tx_family,label_k0,label_v0,label_k1,label_v1,label_k2,label_v2,dlabel_k0, dlabel_v0);
 	}

	mme_msg_tx_DynamicMetricObject1* add_dynamic1(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0) {
		return new mme_msg_tx_DynamicMetricObject1(mme_msg_tx_family,label_k0,label_v0,label_k1,label_v1,dlabel_k0, dlabel_v0);
 	}

	mme_msg_tx_DynamicMetricObject2* add_dynamic2(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string label_k2,std::string label_v2,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1) {
		return new mme_msg_tx_DynamicMetricObject2(mme_msg_tx_family,label_k0,label_v0,label_k1,label_v1,label_k2,label_v2,dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1);
 	}

	mme_msg_tx_DynamicMetricObject2* add_dynamic2(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1) {
		return new mme_msg_tx_DynamicMetricObject2(mme_msg_tx_family,label_k0,label_v0,label_k1,label_v1,dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1);
 	}

	mme_msg_tx_DynamicMetricObject3* add_dynamic3(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string label_k2,std::string label_v2,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1,std::string dlabel_k2, std::string dlabel_v2) {
		return new mme_msg_tx_DynamicMetricObject3(mme_msg_tx_family,label_k0,label_v0,label_k1,label_v1,label_k2,label_v2,dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1,dlabel_k2, dlabel_v2);
 	}

	mme_msg_tx_DynamicMetricObject3* add_dynamic3(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1,std::string dlabel_k2, std::string dlabel_v2) {
		return new mme_msg_tx_DynamicMetricObject3(mme_msg_tx_family,label_k0,label_v0,label_k1,label_v1,dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1,dlabel_k2, dlabel_v2);
 	}
};



class mme_msg_rx_DynamicMetricObject1 : public DynamicMetricObject {
	public:
		mme_msg_rx_DynamicMetricObject1(Family<Counter> &mme_msg_rx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0):
		 DynamicMetricObject(),
		 counter(mme_msg_rx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{dlabel_k0, dlabel_v0}}))
		{
		}
		~mme_msg_rx_DynamicMetricObject1()
		{
		}
		Counter &counter;
};


class mme_msg_rx_DynamicMetricObject2 : public DynamicMetricObject {
	public:
		mme_msg_rx_DynamicMetricObject2(Family<Counter> &mme_msg_rx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1):
		 DynamicMetricObject(),
		 counter(mme_msg_rx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{dlabel_k0, dlabel_v0}, {dlabel_k1, dlabel_v1}}))
		{
		}
		~mme_msg_rx_DynamicMetricObject2()
		{
		}
		Counter &counter;
};


class mme_msg_rx_DynamicMetricObject3 : public DynamicMetricObject {
	public:
		mme_msg_rx_DynamicMetricObject3(Family<Counter> &mme_msg_rx_family,std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1,std::string dlabel_k2, std::string dlabel_v2):
		 DynamicMetricObject(),
		 counter(mme_msg_rx_family.Add({{label_k0, label_v0},{label_k1, label_v1},{dlabel_k0, dlabel_v0},{dlabel_k1, dlabel_v1},{dlabel_k2, dlabel_v2}}))
		{
		}
		~mme_msg_rx_DynamicMetricObject3()
		{
		}
		Counter &counter;
};
class mme_msg_rx_counters {
	public:
	mme_msg_rx_counters();
	~mme_msg_rx_counters();
	Family<Counter> &mme_msg_rx_family;
	Counter &mme_msg_rx_s6a_auth_info_request;
	Counter &mme_msg_rx_s6a_update_location_request;
	Counter &mme_msg_rx_s6a_purge_request;

	mme_msg_rx_DynamicMetricObject1* add_dynamic1(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0) {
		return new mme_msg_rx_DynamicMetricObject1(mme_msg_rx_family,label_k0,label_v0,label_k1,label_v1,dlabel_k0, dlabel_v0);
 	}

	mme_msg_rx_DynamicMetricObject2* add_dynamic2(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1) {
		return new mme_msg_rx_DynamicMetricObject2(mme_msg_rx_family,label_k0,label_v0,label_k1,label_v1,dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1);
 	}

	mme_msg_rx_DynamicMetricObject3* add_dynamic3(std::string label_k0,std::string label_v0,std::string label_k1,std::string label_v1,std::string dlabel_k0, std::string dlabel_v0, std::string dlabel_k1, std::string dlabel_v1,std::string dlabel_k2, std::string dlabel_v2) {
		return new mme_msg_rx_DynamicMetricObject3(mme_msg_rx_family,label_k0,label_v0,label_k1,label_v1,dlabel_k0, dlabel_v0, dlabel_k1, dlabel_v1,dlabel_k2, dlabel_v2);
 	}
};

class hssStats {
	 public:
		hssStats();
		~hssStats() {}
		static hssStats* Instance(); 
		void hssStatspromThreadSetup(void);
		void increment(hssStatsCounter name, std::map<std::string, std::string> labels={}); 
		void decrement(hssStatsCounter name, std::map<std::string, std::string> labels={}); 
	 public:
		mme_msg_tx_counters *mme_msg_tx_m;
		mme_msg_rx_counters *mme_msg_rx_m;
		std::unordered_map<struct Node, DynamicMetricObject*, hash_fn> metrics_map;
};

#endif /* _INCLUDE_hssStats_H__ */
