#pragma once
#ifndef _TEST_SET_
#define _TEST_SET_

#include "test.h"
#include "set_qmj.h"
#include <set>

void test_set() {
	vector<int>data_size{ 0,1,2,10,100,1000,10000};

	random_data_product(data_size, vt_data, insert_data)

		time_counter(
			"set insert",
			std::set<int>set_(vt_data.begin(), vt_data.end());
	for (auto i : set_)
		;
	for (auto i : vt_data)
		set_.find(i);
	for (auto i : vt_data)
		set_.erase(i);
	,
			qmj::set<int>qmj_(vt_data.begin(), vt_data.end());
	for (auto i : qmj_)
		;
	for (auto i : vt_data)
		qmj_.find(i);
	for (auto i : vt_data)
		qmj_.erase(i);
	,
			i
		)
		
		print_time("set", "std", "qmj")
}


#endif
