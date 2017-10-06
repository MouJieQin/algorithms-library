#ifndef _HASH_MAP_QMJ_
#define _HASH_MAP_QMJ_

#include"hashtable.h"

namespace qmj
{
	template<typename key_type,
		typename data_type,
		typename HashFunction = qmj::hash<key_type>,
		typename EqualKey = std::equal_to<key_type>>
		class hash_map
	{	
	private:
		struct ExtractKey:
			public unary_function<pair<key_type,data_type>,bool>
		{
			const key_type& operator()
				(const std::pair<const key_type, data_type>&p)const
			{
				return p.first;
			}
		};

	public:
		typedef hashtable<
			key_type,
			std::pair<const key_type,data_type>,
			HashFunction,
			ExtractKey>  hashtable_type;

		typedef data_type data_type;
		typedef typename hashtable_type::key_type key_type;
		typedef typename hashtable_type::value_type value_type;
		typedef typename hashtable_type::hasher hasher;
		typedef typename hashtable_type::key_equal key_equal;
		typedef typename hashtable_type::size_type size_type;

	public:
		typedef hash_map<key_type,data_type,
			HashFunction,EqualKey> self;

		typedef typename hashtable_type::difference_type difference_type;
		typedef typename hashtable_type::pointer pointer;
		typedef typename hashtable_type::const_pointer const_pointer;
		typedef typename hashtable_type::reference reference;
		typedef typename hashtable_type::const_reference const_reference;

		typedef typename hashtable_type::iterator iterator;
		typedef typename hashtable_type::const_iterator const_iterator;

		hash_map(const size_type n = 100,
			const hasher& hf = hasher(),
			const key_equal& eql = key_equal()) :
			ht(n, hf, eql) {}

		template<typename InputIterator>
		hash_map(InputIterator first, InputIterator last,
			const size_type n = 100,
			const hasher& hf = hasher(),
			const key_equal& eql = key_equal()) :
			ht(n, hf, eql)
		{
			ht.insert_unique(first, last);
		}

		hash_map(const self&x):ht(x.ht){}

		self& operator=(self x)
		{
			ht.swap(x.ht);
			return *this;
		}

		data_type& operator[](const key_type&k)
		{
			return
				(*((ht.insert_unique({ k,data_type() })).first)).second;
		}

		bool operator==(const self&x)const
		{
			return ht == x.ht;
		}

		bool operator!=(const self&x)const
		{
			return !operator!=(x);
		}

		size_type size()const
		{
			return ht.size();
		}

		size_type max_size()const
		{
			return ht.max_size();
		}

		bool empty()const
		{
			return ht.empty();
		}

		void swap(self&x)
		{
			ht.swap(x);
		}

		iterator begin()
		{
			return ht.begin();
		}

		iterator end()
		{
			return ht.end();
		}

		const_iterator begin()const
		{
			return ht.end();
		}

		const_iterator end()const
		{
			reuturn ht.end();
		}

		const_iterator cbegin()const
		{
			return ht.cbegin();
		}

		const_iterator cend()const
		{
			return ht.cend();
		}

		public:
			std::pair<iterator,bool>
				insert(const value_type&x)
			{
				return ht.insert_unique(x);
			}
			
			template<typename InputIterator>
			void insert(InputIterator first, InputIterator last)
			{
				ht.insert(first, last);
			}

			std::pair<iterator,bool>
				insert_noresize(const value_type&x)
			{
				return ht.insert_unique_noresize(x);
			}

			iterator find(const key_type&k)const
			{
				return ht.find(k);
			}

			void erase(const key_type&k)
			{
				ht.erase(k);
			}

			void erase(const iterator& iter)
			{
				ht.erase(iter);
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

			size_type elements_in_buckets(const size_type n)const
			
			{
				return ht.elements_in_bucket(n);
			}
			
	private:
		hashtable_type ht;
	};
}

namespace qmj
{
	template<typename key_type,
		typename data_type,
		typename HashFunction = qmj::hash<key_type>,
		typename EqualKey = std::equal_to<key_type>>
		class hash_multimap
	{
	private:
		struct ExtractKey:
			public unary_function<pair<key_type,data_type>,bool>
		{
			const key_type& operator()
				(const std::pair<const key_type, data_type>&p)const
			{
				return p.first;
			}
		};

	public:
		typedef hashtable<
			key_type,
			std::pair<const key_type, data_type>,
			HashFunction,
			ExtractKey>  hashtable_type;

		typedef data_type data_type;
		typedef typename hashtable_type::key_type key_type;
		typedef typename hashtable_type::value_type value_type;
		typedef typename hashtable_type::hasher hasher;
		typedef typename hashtable_type::key_equal key_equal;
		typedef typename hashtable_type::size_type size_type;

	public:

		typedef hash_multimap<key_type, data_type,
			HashFunction, EqualKey> self;

		typedef typename hashtable_type::difference_type difference_type;
		typedef typename hashtable_type::pointer pointer;
		typedef typename hashtable_type::const_pointer const_pointer;
		typedef typename hashtable_type::reference reference;
		typedef typename hashtable_type::const_reference const_reference;

		typedef typename hashtable_type::iterator iterator;
		typedef typename hashtable_type::const_iterator const_iterator;

		hash_multimap(const size_type n = 100,
			const hasher& hf = hasher(),
			const key_equal& eql = key_equal()) :
			ht(n, hf, eql) {}

		template<typename InputIterator>
		hash_multimap(InputIterator first, InputIterator last,
			const size_type n = 100,
			const hasher& hf = hasher(),
			const key_equal& eql = key_equal()) :
			ht(n, hf, eql)
		{
			ht.insert_equal(first, last);
		}

		hash_multimap(const self&x) :ht(x.ht) {}

		self& operator=(self x)
		{
			ht.swap(x.ht);
			return *this;
		}

		bool operator==(const self&x)const
		{
			return ht == x.ht;
		}

		bool operator!=(const self&x)const
		{
			return !operator!=(x);
		}

		size_type size()const
		{
			return ht.size();
		}

		size_type max_size()const
		{
			return ht.max_size();
		}

		bool empty()const
		{
			return ht.empty();
		}

		void swap(self&x)
		{
			ht.swap(x);
		}

		iterator begin()
		{
			return ht.begin();
		}

		iterator end()
		{
			return ht.end();
		}

		const_iterator begin()const
		{
			return ht.end();
		}

		const_iterator end()const
		{
			return ht.end();
		}

		const_iterator cbegin()const
		{
			return ht.cbegin();
		}

		const_iterator cend()const
		{
			return ht.cend();
		}

	public:
		iterator
			insert(const value_type&x)
		{
			return ht.insert_equal(x);
		}

		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			ht.insert(first, last);
		}

		iterator
			insert_noresize(const value_type&x)
		{
			return ht.insert_equal_noresize(x);
		}

		iterator find(const key_type&k)const
		{
			return ht.find(k);
		}

		void erase(const key_type&k)
		{
			ht.erase(k);
		}

		void erase(const iterator& iter)
		{
			ht.erase(iter);
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

		size_type elements_in_buckets(const size_type n)const

		{
			return ht.elements_in_bucket(n);
		}

	private:
		hashtable_type ht;
	};
}

#endif