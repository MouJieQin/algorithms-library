#pragma once
#ifndef _TEST_HASH_SET_
#define _TEST_HASH_SET_
#include "test.h"
#include<unordered_set>
#include "hash_set_qmj.h"

void test_hash_set()
{
	vector<int>data_size{ 0,1,2,5,10,100,1000,10000,100000 };//测试数据大小

	random_data_product(data_size, vt_data, insert_data)
		vector<int>vt(insert_data);
	time_counter(
		"hash_set",
		std::unordered_set<int>std_(insert_data.begin(), insert_data.end());
	for (auto i : insert_data)
		std_.find(i);
	for (auto i : insert_data)
		std_.erase(i);
	,
		qmj::hash_set<int>qmj_(vt.begin(), vt.end());
	for (auto i : vt)
		qmj_.find(i);
	for(auto i:vt)
		qmj_.erase(i);
	,
		i
		)

	print_time("hash_set", "std", "qmj")

}


#endif
