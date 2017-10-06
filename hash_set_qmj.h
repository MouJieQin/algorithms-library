#ifndef _HASH_SET_QMJ_
#define _HASH_SET_QMJ_

#include"hashtable.h"

namespace qmj
{

	template<typename value_type,
		typename HashFunction = qmj::hash<value_type>,
		typename EqualKey = std::equal_to<value_type>>
		class  hash_set
	{
	private:
		struct ExtractKey:
			public unary_function<value_type,bool>
		{
			const value_type& operator()(const value_type&x)const
			{
				return x;
			}
		};
	public:
		typedef hashtable<
			value_type, value_type, HashFunction,
			ExtractKey>  hashtable_type;

		typedef hash_set<value_type, HashFunction, EqualKey> self;

		typedef typename hashtable_type::key_type key_type;
		typedef typename hashtable_type::value_type value_type;
		typedef typename hashtable_type::hasher hasher;
		typedef typename hashtable_type::key_equal key_equal;
		typedef typename hashtable_type::size_type size_type;

		typedef typename hashtable_type::difference_type difference_type;
		typedef typename hashtable_type::const_pointer pointer;
		typedef typename hashtable_type::const_pointer const_pointer;
		typedef typename hashtable_type::const_reference reference;
		typedef typename hashtable_type::const_reference const_reference;

		typedef typename hashtable_type::const_iterator iterator;
		typedef typename hashtable_type::const_iterator const_iterator;

	public:
		hash_set(size_type n=100,const hasher& hf=hasher(),
			const key_equal&eql=key_equal()):ht(n,hf,eql){}

		template<typename InputIterator>
		hash_set(InputIterator first, InputIterator last,
			const size_t n = 100, const hasher& hf = hasher(),
			const key_equal&eql = key_equal()) :
			ht(n, hf, eql)
		{
			ht.insert_unique(first, last);
		}

		hash_set(const self&x):ht(x.ht){}

		self& operator=(self x)
		{
			ht.swap(x);
			return *this;
		}

		bool operator==(const self&x)const
		{
			return ht == x;
		}
		
		bool operator!=(const self&x)const
		{
			return !operator==(x);
		}

		size_type size()const
		{
			return rt.size(); 
		}

		bool empty()const
		{ 
			return rt.empty(); 
		}

		void swap(self&x)
		{
			ht.swap(x.ht);
		}

		iterator begin()const
		{
			return ht.cbegin();
		}

		iterator end()const
		{
			return ht.cend();
		}

	public:
		std::pair<iterator, bool>insert(const value_type&x)
		{
			auto p = ht.insert_unique(x);
			return
			{ (iterator&)p.first,p.second };
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			ht.insert_unique(first, last);
		}

		std::pair<iterator, bool>insert_noresize(const value_type&val)
		{
			auto p = ht.insert_unique_noresize(val);
			return
			{ (iterator&)p.first,p.second };
		}

		iterator find(const key_type&key)const
		{
			return (iterator&)ht.find(key);
		}

		void erase(const key_type&key)
		{
			ht.erase(key);
		}

		void erase(iterator&it)
		{
			ht.erase(it);
		}

		void erase(iterator&first, iterator&last)
		{
			ht.erase(first, last);
		}

		void clear() 
		{
			ht.clear();
		}

		void resize(const size_type n)
		{
			ht.resize(n);
		}

		size_type bucket_count()const
		{
			return ht.bucket_count();
		}

		size_type max_bucket_count()const
		{
			return ht.max_size();
		}

		size_type elements_in_bucket(const size_type n)const
		{
			return ht.elements_in_bucket();
		}

	private:
		hashtable_type ht;
	};
}


namespace qmj
{

	template<typename value_type,
		typename HashFunction = qmj::hash<value_type>,
		typename EqualKey = std::equal_to<value_type>>
		class  hash_multiset
	{
	private:
		struct ExtractKey :
			public unary_function<value_type, bool>
		{
			const value_type& operator()(const value_type&x)const
			{
				return x;
			}
		};
	public:
		typedef hashtable<
			value_type, value_type, HashFunction,
			ExtractKey>  hashtable_type;

		typedef hash_multiset<value_type, HashFunction, EqualKey> self;

		typedef typename hashtable_type::key_type key_type;
		typedef typename hashtable_type::value_type value_type;
		typedef typename hashtable_type::hasher hasher;
		typedef typename hashtable_type::key_equal key_equal;
		typedef typename hashtable_type::size_type size_type;


		typedef typename hashtable_type::difference_type difference_type;
		typedef typename hashtable_type::const_pointer pointer;
		typedef typename hashtable_type::const_pointer const_pointer;
		typedef typename hashtable_type::const_reference reference;
		typedef typename hashtable_type::const_reference const_reference;

		typedef typename hashtable_type::const_iterator iterator;
		typedef typename hashtable_type::const_iterator const_iterator;

	public:
		hash_multiset(size_type n = 100, const hasher& hf = hasher(),
			const key_equal&eql = key_equal()) :ht(n, hf, eql) {}

		template<typename InputIterator>
		hash_multiset(InputIterator first, InputIterator last,
			const size_t n = 100, const hasher& hf = hasher(),
			const key_equal&eql = key_equal()) :
			ht(n, hf, eql)
		{
			ht.insert_equal(first, last);
		}

		hash_multiset(const self&x):ht(x.ht){}

		hash_multiset& operator=(self x)
		{
			ht.swap(x.ht);
			return *this;
		}

		bool operator==(const self&x)const
		{
			return ht == x;
		}

		bool operator!=(const self&x)const
		{
			return !operator==(x);
		}

		size_type size()const
		{
			return rt.size();
		}

		bool empty()const
		{
			return rt.empty();
		}

		void swap(self&x)
		{
			ht.swap(x.ht);
		}

		iterator begin()const
		{
			return ht.cbegin();
		}

		iterator end()const
		{
			return ht.cend();
		}

	public:
		iterator insert(const value_type&x)
		{
			auto p = ht.insert_equal(x);
			return
			{ (iterator&)p.first,p.second };
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			ht.insert_equal(first, last);
		}

		iterator insert_noresize(const value_type&val)
		{
			return ht.insert_equal_noresize(val);
		}

		size_type count(const key_type&k)const
		{
			return ht.count(k);
		}

		iterator find(const key_type&key)const
		{
			return (iterator&)ht.find(key);
		}

		void erase(const key_type&key)
		{
			ht.erase(key);
		}

		void erase(iterator&it)
		{
			ht.erase(it);
		}

		void erase(iterator&first, iterator&last)
		{
			ht.erase(first, last);
		}

		void clear()
		{
			ht.clear();
		}

		void resize(const size_type n)
		{
			ht.resize(n);
		}

		size_type bucket_count()const
		{
			return ht.bucket_count();
		}

		size_type max_bucket_count()const
		{
			return ht.max_size();
		}

		size_type elements_in_bucket(const size_type n)const
		{
			return ht.elements_in_bucket();
		}

	private:
		hashtable_type ht;
	};
}


#endif