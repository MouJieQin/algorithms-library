#ifndef _HEAP_TEST_
#define _HEAP_TEST_
#include"heap.h"
#include<vector>
#include<queue>
#include"test.h"

using namespace std;
using namespace qmj;

void heap_test() {
	vector<int>data_size{0,1,2,10,100,1000,10000};

	random_data_product(data_size, vt_data, insert_data)

		time_counter(
			"build_heap",
			priority_queue<int>pq_std(vt_data.begin(), vt_data.end());,
			fib_heap<int>bh(vt_data.begin(), vt_data.end()); ,
			i
		)

		time_counter(
			"insert",
			for (auto i : insert_data)
				pq_std.push(i);,
			for (auto i : insert_data)
				bh.insert(i);,
			i
			)

		time_counter(
			"extract_top",
		while (!pq_std.empty())
			pq_std.pop();,	
		while (!bh.empty())
			bh.extract_top();,
		2*i
			)

	print_time("binary_heap","std","qmj::binary_heap")

			random_data_product(data_size, vt_data, insert_data)

			time_counter(
				"build_heap",
				priority_queue<int>pq_std(vt_data.begin(), vt_data.end()); ,
				fib_heap<int>fib(vt_data.begin(), vt_data.end());,
				i
			)

			time_counter(
				"insert",
				for (auto i : insert_data)
					pq_std.push(i); ,
					for (auto i : insert_data)
						fib.insert(i); ,
						i
						)

			time_counter(
				"extract_top",
				while (!pq_std.empty())
					pq_std.pop(); ,
					while (!fib.empty())
						fib.extract_top(); ,
						2 * i
						)

						print_time("fib_heap","std","qmj::fib_heap")

	random_data_product(data_size, vt_data, insert_data)

		time_counter(
			"build_heap",
			binary_heap<int>pq_std(vt_data.begin(), vt_data.end());,
			fib_heap<int>fib(vt_data.begin(), vt_data.end()); ,
			i
		)

		time_counter(
			"insert",
			for (auto i : insert_data)
				pq_std.insert(i);,
				for (auto i : insert_data)
					fib.insert(i);,
					i
					)

		time_counter(
			"extract_top",
			while (!pq_std.empty())
				pq_std.extract_top();,
				while (!fib.empty())
					fib.extract_top();,
				//	cout << fib.extract_top() << "  "; cout << endl;,
					2 * i
					)

					print_time("fib_heap","binary_heap","fib_heap")
}

#endif
