#ifndef _SORT_TEST_
#define _SORT_TEST_
#include"test.h"
#include"sort.h"
#include<algorithm>
#include<vector>
using namespace qmj;

void sort_test()
{
	vector<int>data_size{0,1,2,5,100,1000,10000,100000};//�������ݴ�С

	random_data_product(data_size, vt_data, insert_data)
		vector<int>vt(insert_data);
		time_counter(
			"sort",
			std::sort(insert_data.begin(), insert_data.end()); 
	,
			qmj::sort(vt.begin(), vt.end());
	,
		i
		)
			if (insert_data == vt)
				cout << "�ȶ���ȷ" << endl;
			else
				cout << "�ȶԴ���" << endl;
	print_time("sort","std","qmj")
}

#endif