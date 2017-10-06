#ifndef _SET_QMJ_
#define _SET_QMJ_

#include"rb_tree.h"


namespace qmj
{
	template<typename key_type,
		typename Compare = std::less<key_type>>
		class set
	{
	private:
		struct keyOfValue :
			public unary_function<key_type, key_type>
		{
			const key_type& operator()(const key_type&key)const
			{
				return key;
			}
		};
	public:
		typedef key_type key_type;
		typedef key_type value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef rb_tree<key_type,key_type,keyOfValue, Compare> rbt_type;
		typedef typename rbt_type::iterator rbt_iterator;

		typedef typename rbt_type::const_pointer pointer;
		typedef typename rbt_type::const_pointer const_pointer;
		typedef typename rbt_type::const_reference reference;
		typedef typename rbt_type::const_reference const_reference;
		typedef typename rbt_type::const_iterator iterator;
		typedef typename rbt_type::const_iterator const_iterator;
		typedef typename rbt_type::const_reverse_iterator reverse_iterator;
		typedef typename rbt_type::const_reverse_iterator const_reverse_iterator;
		typedef typename rbt_type::size_type size_type;
		typedef typename rbt_type::difference_type difference_type;


		typedef set<key_type, Compare>	self;
		typedef typename std::pair<iterator, bool> pair_iterator_bool;

		explicit set(const Compare& comp=Compare()):rbt(comp){}

		template<typename InputIterator>
		set(InputIterator first, InputIterator last, const Compare& comp= Compare()):
			rbt(comp)
		{
			rbt.rbt_insert_unique(first, last);
		}

		set(const self& x):rbt(x.rbt){}

		self& operator=(self x)
		{
			rbt.swap(x.rbt);
			return *this;
		}

		bool operator==(const self&x)
		{
			return rbt == x.rbt;
		}

		bool operator!=(const self&x)
			
		{
			return rbt != x.rbt;
		}

		iterator begin()const { return rbt.begin(); }
		iterator end()const { return rbt.cend(); }

		reverse_iterator rbegin()const { return rbt.crbegin(); }
		reverse_iterator rend()const { return rbt.crend(); }


		bool empty() { return rbt.empty(); }
		size_type size() { return rbt.size(); }
		void swap(self&x) { rbt.swap(x.rbt); }

		iterator insert(const key_type& x)
		{
			pair<typename rbt_type::iterator, bool>p = rbt.rbt_insert_unique(x);
			return (iterator&)p.first;
		}

		template<typename InputIterator>
		void insert(InputIterator first,InputIterator last)
		{
			rbt.rbt_insert_unique(first,last);
		}

		void erase(const key_type& x)
		{
			rbt.delete_key(x);
		}

		iterator  erase(iterator position)
		{
			return (iterator&)rbt.erase((rbt_iterator&)position);
		}

		iterator erase(iterator first, iterator last)
		{
			return (iterator&)rbt.erase((rbt_iterator&)first, (rbt_iterator&)last);
		}

		void clear() { rbt.clear(); }

		iterator lower_bound(const key_type&x)const
		{
			return rbt.lower_bound(x);
		}

		iterator upper_bound(const key_type&x)const
		{
			return rbt.upper_bound(x);
		}

		iterator find(const key_type&x)const
		{
			return rbt.find(x);
		}

	private:
		rbt_type rbt;
	};
}

namespace qmj
{
	template<typename key_type,
		typename Compare=std::less<key_type>>
		class multiset
	{
	private:
		struct keyOfValue :
			public unary_function<key_type, key_type>
		{
			const key_type& operator()(const key_type&key)const
			{
				return key;
			}
		};
	public:
		typedef key_type key_type;
		typedef key_type value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef rb_tree<key_type,key_type,keyOfValue, Compare> rbt_type;
		typedef typename rbt_type::iterator rbt_iterator;

		typedef typename rbt_type::const_pointer pointer;
		typedef typename rbt_type::const_pointer const_pointer;
		typedef typename rbt_type::const_reference reference;
		typedef typename rbt_type::const_reference const_reference;
		typedef typename rbt_type::const_iterator iterator;
		typedef typename rbt_type::const_iterator const_iterator;
		typedef typename rbt_type::const_reverse_iterator reverse_iterator;
		typedef typename rbt_type::const_reverse_iterator const_reverse_iterator;
		typedef typename rbt_type::size_type size_type;
		typedef typename rbt_type::difference_type difference_type;


		typedef multiset<key_type, Compare>	self;
		typedef typename std::pair<iterator, bool> pair_iterator_bool;
	

		explicit multiset(const Compare& comp=Compare()):
			rbt(comp) {}

		template<typename InputIterator>
		multiset(InputIterator first, InputIterator last,
			const Compare& comp = Compare()) :rbt(comp)

		{
			rbt.rbt_insert_equal(first,last);
		}

		multiset(const self& x) :rbt(x.rbt) {}


		self& operator=(self x)
		{
			rbt.swap(x.rbt);
			return *this;
		}

		bool operator==(const self&x)
		{
			return rbt == x.rbt;
		}

		bool operator!=(const self&x)

		{
			return rbt != x.rbt;
		}

		iterator begin()const { return rbt.begin(); }
		iterator end()const { return rbt.cend(); }

		reverse_iterator rbegin()const { return rbt.crbegin(); }
		reverse_iterator rend()const { return rbt.crend(); }


		bool empty() { return rbt.empty(); }
		size_type size() { return rbt.size(); }
		void swap(self&x) { rbt.swap(x.rbt); }

		iterator insert(const key_type& x)
		{
			return (iterator&)rbt.rbt_insert_equal(x);
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			rbt.rbt_insert_equal(first, last);
		}

		void erase(const key_type& x)
		{
			rbt.delete_key(x);
		}

		iterator  erase(iterator position)
		{
			return (iterator&)rbt.erase((rbt_iterator&)position);
		}

		iterator erase(iterator first, iterator last)
		{
			return (iterator&)rbt.erase((rbt_iterator&)first, (rbt_iterator&)last);
		}

		void clear() { rbt.clear(); }

		iterator lower_bound(const key_type&x)const
		{
			return rbt.lower_bound(x);
		}

		iterator upper_bound(const key_type&x)const
		{
			return rbt.upper_bound(x);
		}

		iterator find(const key_type&x)const
		{
			return rbt.find(x);
		}

	private:
		rbt_type rbt;
	};
}


#endif