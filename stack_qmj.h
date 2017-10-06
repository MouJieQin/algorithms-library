#ifndef _STACK_QMJ_
#define _STACK_QMJ_
#include<vector>

namespace qmj
{
	template<typename value_type,
		typename Container=std::vector<value_type>>
		class stack
	{
	public:
		typedef typename Container container_type;
		typedef typename container_type::iterator iterator;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

		stack() = default;
		stack(iterator first,iterator last):c(first,last){}
		stack(const stack&rhs):c(rhs){}

		bool empty() { return c.empty(); }
		value_type top() { return c.back(); }
		void push(const value_type &value) { c.push_back(value); }
		size_t size() { return c.size(); }
		value_type pop_top()
		{
			auto top = c.back();
			c.pop_back();
			return top;
		}
	private:
		Container c;
	};
}
#endif
