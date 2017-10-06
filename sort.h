#ifndef _SORT_
#define _SORT_
#include<vector>
#include<algorithm>

namespace qmj
{
	const int threshold = 32;
	
	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>> inline
		void copy(RandomAccessIterator first, RandomAccessIterator last,
			RandomAccessIterator start)
	{
		while (first != last)*start++ = *first++;
	}
}

namespace qmj
{
	template<typename RandomAccessIterator,
		typename Compare = 
		std::less<typename RandomAccessIterator::value_type>>
	void insert_sort(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp=Compare())
	{
		typedef typename RandomAccessIterator::value_type value_type;
		if (last == first)return;
		for (RandomAccessIterator iter = first + 1; iter != last; ++iter)
			insert_sort_imple(first, iter,comp);
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void insert_sort_imple(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{
		auto val = *last;
		if (comp(val, *first))
		{
			copy_backward(first, last, last + 1);
			*first = val;
		}
		else
			insert_sort_case(last, val, comp);
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>inline
		void insert_sort_case(RandomAccessIterator last,const typename RandomAccessIterator::value_type val,
			Compare comp = Compare())
	{
		auto prev = last;
		--prev;
		while (comp(val, *prev))
		{
			*last = *prev;
			last = prev;
			--prev;
		}
		*last = val;
	}
}

namespace qmj
{
	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void merge_sort(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{
		if (last - first < threshold)
			insert_sort(first, last, comp);
		else
		{
			auto med = first + (last - first) / 2;
			merge_sort(first, med,comp);
			merge_sort(med, last,comp);
			if (comp(*med, *(med - 1)))
				merge(first, last, med, comp);
		}
	}
	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void merge(RandomAccessIterator first, RandomAccessIterator last,RandomAccessIterator med,
			Compare comp = Compare())
	{
		typedef typename RandomAccessIterator::value_type value_type;
		std::vector<value_type>left(first, med);
		std::vector<value_type>right(med, last);
		auto Liter = left.begin();
		auto Riter = right.begin();
		if (!comp(right.back(), left.front()))
			while (Liter != left.end() && Riter != right.end())
				*first++ = *Liter < *Riter ? *Liter++ : *Riter++;
		while (Riter != right.end())
			*first++ = *Riter++;
		while (Liter != left.end())
			*first++ = *Liter++;
	}
}


namespace qmj
{
	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>inline
		RandomAccessIterator 
		partition(RandomAccessIterator first, RandomAccessIterator last,
			typename RandomAccessIterator::value_type pivot,
			Compare comp = Compare())
	{
		auto cut = last;
		auto iter = last;
		do {
			--iter;
			if (!comp(*iter,pivot))
				std::swap(*(--cut), *iter);
		} while (iter != first);
		return cut;
	}
}

namespace qmj
{
	
	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void quicksort(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{

		if (last - first>1)
		{
			auto pivot = madian_partition(first, last, comp);
			quicksort(first, pivot);
			quicksort(pivot + 1, last);
		}
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>inline
		RandomAccessIterator
		madian_partition(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{
		//swap(*first,median)
		median(*first, *(first + (last - first) / 2), *(last - 1), comp);
		return
			qs_partition(first, last, comp);
	}

	template<typename value_type,
		typename Compare =
		std::less<value_type>>inline
		void median(value_type& first, value_type& med, value_type& last,
			Compare comp = Compare())
	{
		if (comp(first, med))
		{
			if (comp(med, last))
				std::swap(med, first);
			else if (comp(first, last))
				std::swap(last, first);
		}
		else if (comp(last, med))
			std::swap(med, first);
		else if (comp(first, last))
			std::swap(last, first);
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>inline
		RandomAccessIterator
		qs_partition(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{//part first to last with *first=pivot, return iterator of pivot after partition
		auto cut = last;
		auto iter = --last;
		for(;iter!=first;--iter)
			if (!comp(*iter, *first))
				std::swap(*(--cut), *iter);
		swap(*first, *(--cut));
		return cut;
	}
}


//namespace qmj
//{
//	template<typename RandomAccessIterator,
//		typename Compare =
//		std::less<typename RandomAccessIterator::value_type>>
//		void sort(RandomAccessIterator first, RandomAccessIterator last,
//			Compare comp = Compare())
//	{
//		if (last - first > threshold)
//			sort_qs(first, last, comp);
//		insert_sort(first, last, comp);
//	}
//
//	template<typename RandomAccessIterator,
//		typename Compare =
//		std::less<typename RandomAccessIterator::value_type>>
//		void sort_qs(RandomAccessIterator first, RandomAccessIterator last,
//			Compare comp = Compare())
//	{
//		while (last - first > threshold)
//		{
//			auto cut = madian_partition(first, last, comp);
//			//recursive less partition
//			if (cut - first > last - cut)
//			{
//				sort_qs(cut + 1, last);
//				last = cut;
//			}
//			else
//			{
//				sort_qs(first, cut);
//				first = cut + 1;
//			}
//		}
//	}
//}

namespace qmj
{
	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void sort(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{
		if (last - first > threshold)
			sort_qs(first, last,  static_cast<int>(log2l(last - first)*1.5),comp);
		final_insert_sort(first,last,comp);
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void final_insert_sort(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{
		if (last - first > threshold)
		{
			insert_sort(first, first + threshold, comp);
			unguarded_insert_sort(first + threshold, last,comp);
		}
		else
			insert_sort(first, last, comp);
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void unguarded_insert_sort(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{
		for (auto i = first; i != last; ++i)
			insert_sort_case(i, *i,comp);
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		void sort_qs(RandomAccessIterator first, RandomAccessIterator last, int depth_limit,
			Compare comp = Compare())
	{
		while (last - first > threshold&&depth_limit--)
		{

			auto cut = med_partition(first, last, comp);

			if (cut - first > last - cut)
			{
				sort_qs(cut, last,depth_limit,comp);
				last = cut;
			}
			else
			{
				sort_qs(first, cut,depth_limit,comp);
				first = cut;
			}
		}
		if (!depth_limit)
			std::partial_sort(first, last, last,comp);
	}

	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		RandomAccessIterator med_partition(RandomAccessIterator first, RandomAccessIterator last,
			Compare comp = Compare())
	{
		auto piovt = get_piovt(*first, *(first + ((last - first) >> 1)), *(last - 1));
		while (true)
		{
			while (comp(*first, piovt))++first;
			--last;
			while (comp(piovt,*last))--last;
			if (!(first < last))return first;
			iter_swap(first, last);
			++first;
		}
		return first;
	}

	template<typename value_type,
		typename Compare =std::less<value_type>>
		const value_type& get_piovt(const value_type&a ,const value_type&b,const value_type&c,
			Compare comp = Compare())
	{
		if (comp(a, b))
		{
			if (comp(b, c))
				return b;
			else if (comp(a, c))
				return c;
			else
				return a;
		}
		else if (comp(a, c))
			return a;
		else if (comp(b, c))
			return c;
		else
			return b;
	}
}

namespace qmj
{
	template<typename RandomAccessIterator>
		void counting_sort(RandomAccessIterator first, RandomAccessIterator last)
	{//改进的计数排序理论上能计算任意范围的数字，但如果数字之间相差较大而且较为稀疏
			//如只有两个数INT_MIN,INT_MAX也会耗费非常长的时间，
			//经测试在不知道最小值和最大值的情况下且序列相对‘好’的情况下也是std::sort()的两倍耗时
		typedef typename RandomAccessIterator::value_type value_type;
		if (first == last)return;
		value_type min_value = *first, max_value = *first;
		for (auto iter = first + 1; iter != last; ++iter)
			if (*iter < min_value)
				min_value = *iter;
			else if (*iter > max_value)
				max_value = *iter;
		counting_sort_imple(first, last, -static_cast<int>(min_value), max_value);
	}

	template<typename RandomAccessIterator>
	void counting_sort(RandomAccessIterator first, RandomAccessIterator last,
		typename RandomAccessIterator::value_type min_value,
		typename RandomAccessIterator::value_type max_value)
	{//konwed min_value && max_value
		counting_sort_imple(first, last, -static_cast<int>(min_value), max_value);
	}

	template<typename RandomAccessIterator>inline
	void counting_sort_imple(RandomAccessIterator first,RandomAccessIterator last,
		int differ_zero,
		typename RandomAccessIterator::value_type max_value)
	{
		typedef typename RandomAccessIterator::value_type value_type;
		std::vector<value_type>temp_copy(first, last);/////////
		std::vector<value_type>counter(max_value + differ_zero + 1, 0);
		for (auto iter = first; iter != last; ++iter)
			++counter[*iter + differ_zero];
		for (size_t i = 1; i != counter.size(); ++i)
			counter[i] += counter[i - 1];
		size_t index=temp_copy.size();
		do {
			--index;
			*(first + counter[temp_copy[index]+differ_zero]-1) = temp_copy[index];
			--counter[temp_copy[index]+differ_zero];
		} while (index != 0);
	}
}
namespace qmj
{
	template<typename RandomAccessIterator>
	void bucket_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		typedef typename RandomAccessIterator::value_type value_type;
		size_t n = last - first;
		vector<vector<value_type>>bucket(n, vector<value_type>());
		for (auto iter = first; iter != last; ++iter)
			bucket[static_cast<size_t>(*iter*n)].push_back(*iter);
		for (vector<value_type> &iter : bucket)
			insert_sort(iter.begin(), iter.end());
		for (vector<value_type> &i : bucket)
			for (auto j : i)
				*(first++) = j;
	}
}

namespace qmj
{
	template<typename RandomAccessIterator,
		typename Compare =
		std::less<typename RandomAccessIterator::value_type>>
		RandomAccessIterator
		randomized_select(RandomAccessIterator first, RandomAccessIterator last,
			size_t order_index,Compare comp = Compare())
	{
		auto beg = first;
		auto end = last;
		while (beg < end)
		{
			auto cut = madian_partition(beg, end, comp);
			if (cut == first + order_index - 1)
				return cut;
			else if (first + order_index-1 < cut)
				end = cut;
			else
				beg = cut + 1;
		}
		return end;
	}
}

#endif