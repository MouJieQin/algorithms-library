#ifndef _QUEUE_QMJ_
#define _QUEUE_QMJ_
#include<deque>

namespace qmj
{
	template<typename value_type,
		typename Container = std::deque<value_type>>
		class queue
	{
	public:
		typedef typename Container container_type;
		typedef typename container_type::iterator iterator;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

		queue() = default;
		queue(iterator first, iterator last) :c(first, last) {}
		queue(const queue&rhs) :c(rhs) {}

		bool empty()const { return c.empty(); }
		size_t size()const { return c.size(); }
		value_type dequeue()
		{
			auto first = c.front();
			c.pop_front();
			return first;
		}
		void enqueue(const value_type &value) { c.push_back(value); }

	private:
		Container c;
	};
}

#endif