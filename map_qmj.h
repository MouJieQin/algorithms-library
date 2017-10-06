#ifndef _MAP_QMJ_
#define _MAP_QMJ_

#include"rb_tree.h"

namespace qmj
{
	template<typename key_type,
		typename data_type,
		typename Compare = std::less<key_type>>
		class map
	{
	public:
		typedef key_type key_type;
		typedef data_type data_type;
		typedef std::pair<const key_type, data_type> value_type;
		typedef Compare key_Compare;
	private:
		class keyOfValue:
			public unary_function<value_type,key_type>
		{
		public:
			const key_type& operator()(const value_type&x)const
			{
				return x.first;
			}
		};
	public:

		typedef rb_tree<key_type,value_type,keyOfValue,Compare>
			rbt_type;
		typedef map<key_type, data_type, Compare> self;

		typedef typename rbt_type::pointer pointer;
		typedef typename rbt_type::const_pointer const_pointer;
		typedef typename rbt_type::reference reference;
		typedef typename rbt_type::const_reference const_reference;
		typedef typename rbt_type::iterator iterator;
		typedef typename rbt_type::const_iterator const_iterator;
		typedef typename rbt_type::reverse_iterator reverse_iterator;
		typedef typename rbt_type::
			const_reverse_iterator const_reverse_iterator;
		typedef typename rbt_type::size_type size_type;
		typedef typename rbt_type::difference_type difference_type;

		map(const Compare&comp=Compare()):rbt(comp){}

		template<typename InputIterator>
		map(InputIterator first, InputIterator last,const Compare&comp=Compare()):rbt(comp)
		{
			rbt.rbt_insert_unique(first, last);
		}

		map(const self&x) :rbt(x.rbt) {}

		self& operator=(self x)
		{
			rbt.swap(x.rbt);
			return *this;
		}

		iterator begin() { return rbt.begin(); }
		iterator end() { return rbt.end(); }
		const_iterator begin()const { return rbt.begin(); }
		const_iterator end()const { return rbt.end(); }

		const_iterator cbegin()const { return rbt.cbegin(); }
		const_iterator cend()const { return rbt.cend(); }

		reverse_iterator rbegin() { retrn rbt.rbgegin(); }
		reverse_iterator rend() { return rbt.rend(); }
		const_reverse_iterator rbegin()const { return rbt.rbegin(); }
		const_reverse_iterator rend()const { return rbt.rend(); }

		const_reverse_iterator crbegin()const { return rbt.crbegin(); }
		const_reverse_iterator crend()const { return rbt.crend(); }

		bool empty()const { return rbt.empty(); }
		size_type size()const { return rbt.size(); }

		data_type& operator[](const key_type&k)
		{
			return (*((insert({ k,data_type() })).first)).second;
		}

		void swap(self&x) { rbt.swap(x.rbt); }

		std::pair<iterator, bool>insert(const value_type&x)
		{
			return rbt.rbt_insert_unique(x);
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			rbt.rbt_insert_unique(first, last);
		}

		iterator erase(iterator position)
		{
			return rbt.erase(position);
		}

		void erase(const key_type&x)
		{
			rbt.delete_key(x);
		}

		void clear()
		{
			rbt.clear();
		}

		iterator find(const key_type&x)
		{
			return rbt.find(x);
		}

		const_iterator find(const key_type&x)const
		{
			return rbt.find(x);
		}

		iterator lower_bound(const key_type&x)
		{
			return rbt.lower_bound(x);
		}

		const_iterator lower_bound(const key_type&x)const
		{
			return rbt.lower_bound(x);
		}

		iterator upper_bound(const key_type&x)
		{
			return rbt.upper_bound(x);
		}

		const_iterator upper_bound(const key_type&x)const
		{
			return rbt.upper_bound(x);
		}

	private:
		rbt_type rbt;
	};

}

namespace qmj
{
	template<typename key_type,
		typename data_type,
		typename Compare = std::less<key_type>>
		class multimap
	{
	public:
		typedef key_type key_type;
		typedef data_type data_type;
		typedef std::pair<const key_type, data_type> value_type;
		typedef Compare key_Compare;
	private:
		class keyOfValue :
			public unary_function<value_type, key_type>
		{
		public:
			const key_type& operator()(const value_type&x)const
			{
				return x.first;
			}
		};
	public:

		typedef rb_tree<key_type,value_type, keyOfValue,Compare>
			rbt_type;
		typedef multimap<key_type, data_type, Compare> self;

		typedef typename rbt_type::pointer pointer;
		typedef typename rbt_type::const_pointer const_pointer;
		typedef typename rbt_type::reference reference;
		typedef typename rbt_type::const_reference const_reference;
		typedef typename rbt_type::iterator iterator;
		typedef typename rbt_type::const_iterator const_iterator;
		typedef typename rbt_type::reverse_iterator reverse_iterator;
		typedef typename rbt_type::
			const_reverse_iterator const_reverse_iterator;
		typedef typename rbt_type::size_type size_type;
		typedef typename rbt_type::difference_type difference_type;

		explicit multimap(const Compare&comp=Compare()):
			rbt(comp) {}

		template<typename InputIterator>
		multimap(InputIterator first, InputIterator last,
			const Compare& comp = Compare()) :
			rbt(comp)
		{
			rbt.rbt_insert_equal(first, last);
		}

		multimap(const self&x) :rbt(x.rbt) {}

		self& operator=(self x)
		{
			rbt.swap(x);
			return *this;
		}

		iterator begin() { return rbt.begin(); }
		iterator end() { return rbt.end(); }
		const_iterator begin()const { return rbt.cbegin(); }
		const_iterator end()const { return rbt.cend(); }

		const_iterator cbegin()const { return rbt.cbegin(); }
		const_iterator cend()const { return rbt.cend(); }

		reverse_iterator rbegin() { retrn rbt.rbgegin(); }
		reverse_iterator rend() { return rbt.rend(); }
		const_reverse_iterator rbegin()const { return rbt.crbegin(); }
		const_reverse_iterator rend()const { return rbt.crend(); }

		const_reverse_iterator crbegin()const { return rbt.crbegin(); }
		const_reverse_iterator crend()const { return rbt.crend(); }

		bool empty()const { return rbt.empty(); }
		size_type size()const { return rbt.size(); }

		void swap(self&x) { rbt.swap(x.rbt); }

		iterator insert(const value_type&x)
		{
			return rbt.rbt_insert_equal(x);
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			rbt.rbt_insert_equal(first, last);
		}

		iterator erase(iterator position)
		{
			return rbt.erase(position);
		}

		void erase(const key_type&x)
		{
			rbt.delete_key(x);
		}

		void clear()
		{
			rbt.clear();
		}

		iterator find(const key_type&x)
		{
			return rbt.find(x);
		}

		const_iterator find(const key_type&x)const
		{
			return rbt.find(x);
		}

		iterator lower_bound(const key_type&x)
		{
			return rbt.lower_bound(x);
		}

		const_iterator lower_bound(const key_type&x)const
		{
			return rbt.lower_bound(x);
		}

		iterator upper_bound(const key_type&x)
		{
			return rbt.upper_bound(x);
		}

		const_iterator upper_bound(const key_type&x)const
		{
			return rbt.upper_bound(x);
		}

		std::pair<iterator, iterator> equal_range(const key_type&x)
		{
			return rbt.equal_range(x);
		}

		std::pair<iterator, iterator> equal_range(const key_type&x)const
		{
			return rbt.equal_range(x);
		}

	private:
		rbt_type rbt;
	};

}


#endif