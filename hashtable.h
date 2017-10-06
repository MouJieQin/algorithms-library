#ifndef _HASHTABLE_
#define _HASHTABLE_

#include<vector>
#include<utility>
#include<algorithm>

#include"hashfunction.h"

namespace qmj
{
	template<typename value_type>
	struct hashtable_node
	{
	public:
		typedef hashtable_node<value_type>* link_type;

		hashtable_node(const value_type&value=value_type(),
			const link_type next=nullptr):
			value(value),next(next){}

		value_type value;
		link_type next;
	};

	template<typename key_type,
		typename value_type,
		typename HashFunction,
		typename ExtractKey,
		typename EqualKey>
		class hashtable;

	template<typename key_type,
		typename value_type,
		typename Ref ,
		typename Ptr,
		typename HashFunction,
		typename ExtractKey,
		typename EqualKey = std::equal_to<key_type>>
	struct hashtable_iterator
	{
		typedef forward_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef size_t size_type;
		typedef Ref reference;
		typedef Ptr pointer;

		typedef hashtable_iterator<
			key_type,
			value_type,
			Ref,
			Ptr,
			HashFunction,
			ExtractKey,
			EqualKey> self;

		typedef hashtable_iterator
			<key_type, 
			value_type,
			value_type&,
			value_type*,
			HashFunction,
			ExtractKey, 
			EqualKey> iterator;

		typedef hashtable_iterator
			<key_type, 
			value_type,
			const value_type&,
			const value_type*,
			HashFunction,
			ExtractKey,
			EqualKey> const_iterator;

		typedef const hashtable<
			key_type,
			value_type,
			HashFunction,
			ExtractKey,
			EqualKey>* const_hashtable_link;

		typedef hashtable_node<value_type>* link_type;

		hashtable_iterator(
			const link_type cur=nullptr,
			const const_hashtable_link ht=nullptr):
			cur(cur),ht(ht){}

		bool operator==(const self& it)const
		{
			return cur == it.cur;
		}

		bool operator!=(const self& it)const
		{
			return !operator==(it);
		}
		
		reference operator*()const
		{
			return cur->value;
		}

		pointer operator->()const
		{
			return &(operator*());
		}

		self& operator++()
		{
			const link_type old = cur;
			cur = cur->next;
			if (!cur)
			{
				size_type index = ht->get_bucket_num(ht->get_key(old->value));
				auto n = ht->buckets.size();
				while (!cur&&++index !=n )
					cur = ht->buckets[index];
			}
			return *this;
		}
		
		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}
	private:
		link_type cur;
		const_hashtable_link ht;
	};

	static const int num_primes = 28;
	static const unsigned long prime_list[num_primes]
		= {
		53,97,193,389,769,
		1543,3079,6151,12289,24593,
		49157,98317,196613,393241,786433,
		1572869,3145739,6291469,12582917,25165843,
		50331653,100663319,201326611,402653189,805306457,
		1610612741,3221225437ul,4294967291ul
	};

	inline unsigned long next_prime(unsigned long n)
	{
		const unsigned long* first = prime_list;
		auto last = first + num_primes;
		auto pos = std::lower_bound(first, last, n);
		return
			pos == last ? *(last - 1) : *pos;
	}


	template<typename key_type,
		typename value_type,
		typename HashFunction,
		typename ExtractKey,
		typename EqualKey=std::equal_to<key_type>>
		class hashtable
	{
	public:

		template<typename key_type,
			typename value_type,
			typename Ref,
			typename Ptr,
			typename HashFunction,
			typename ExtractKey,
			typename EqualKey = std::equal_to<key_type>>
			friend struct hashtable_iterator;

		typedef HashFunction hasher;
		typedef EqualKey key_equal;
		typedef value_type value_type;
		typedef value_type* pointer;
		typedef const value_type*  const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef key_type  key_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;


		typedef typename hashtable_iterator
			<key_type, 
			value_type,
			value_type&,
			value_type*,
			HashFunction, 
			ExtractKey, 
			EqualKey>::iterator iterator;

		typedef typename hashtable_iterator
			<key_type, 
			value_type,
			value_type&,
			value_type*,
			HashFunction,
			ExtractKey,
			EqualKey>::const_iterator const_iterator;

		typedef hashtable_node<value_type>* link_type;

		typedef hashtable < key_type,
			value_type,
			HashFunction,
			ExtractKey,
			EqualKey> self;


		hashtable(const size_t n=0,
			const HashFunction& hash=HashFunction(),
			const EqualKey& equals=EqualKey()) :
			hash(hash), equals(equals),
			get_key(ExtractKey()), num_elements(0)
		{
			init_buckets(n);
		}

		hashtable(const self&x):buckets(x.buckets.size(),nullptr),
			num_elements(x.num_elements)
		{
			for (size_t i = 0; i != buckets.size(); ++i)
			{
				auto cur = x.buckets[i];
				while (cur)
				{
					auto tmp = new hashtable_node<value_type>
						(cur->value, buckets[i]);
					buckets[i] = tmp;
					cur = cur->next;
				}
			}
		}


		self& operator=(self x)
		{
			num_elements = x.num_elements;
			buckets.swap(x.buckets);
			return *this;
		}

		~hashtable()
		{
			clear();
		}

		void swap(self&x)
		{
			std::swap(num_elements, x.num_elements);
			buckets.swap(x.buckets);
		}

		iterator begin()
		{
			return (iterator&)cbegin();
		}

		iterator end()
		{
			return iterator(nullptr, this);
		}

		const_iterator begin()const
		{
			return cbegin();
		}

		const_iterator end()const
		{
			return cend();
		}

		const_iterator cbegin()const
		{
			size_t i = 0;
			while (i != buckets.size() && !buckets[i])
				++i;
			return const_iterator(buckets[i], this);
		}

		const_iterator cend()const
		{
			return const_iterator(nullptr, this);
		}

		size_type size()const 
		{
			return num_elements; 
		}

		bool empty()const 
		{
			return !size(); 
		}

		void erase(const key_type&k)
		{
			auto n = get_bucket_num(k);
			auto cur = buckets[n];
			link_type last;
			if (cur)
			{
				if (equals(get_key(cur->value), k))
				{
					--num_elements;
					buckets[n] = cur->next;
				}
				else
				{
					do {
						last = cur;
						cur = cur->next;
					} while (cur && !equals(k, get_key(cur->value)));
					if (cur)
					{
						--num_elements;
						last->next = cur->next;
					}
				}
				delete cur;
			}
		}

		void erase(iterator& x)
		{
			erase(get_key(*x));
		}

		void erase(iterator& first, iterator& last)
		{
			while (first != last)erase(first++);
		}

		void clear()
		{
			for (size_t i = 0; i != buckets.size(); ++i)
			{
				auto cur = buckets[i];
				while (cur)
				{
					auto nt = cur->next;
					delete cur;
					cur = nt;
				}
			}
			std::vector<link_type>tmp;
			buckets.swap(tmp);
			num_elements = 0;
		}

		size_type count(const key_type&k)const
		{
			auto n = get_bucket_num(k);
			auto cur = buckets[n];
			size_type ct = 0;
			while (cur)
			{
				if (equals(get_key(cur->key), k))
					++ct;
				cur = cur->next;
			}
			return ct;
		}

		iterator find(const key_type&k)const
		{
			auto n = get_bucket_num(k);
			auto cur = buckets[n];
			while (cur && !equals(k, get_key(cur->value)))
				cur = cur->next;
			return iterator(cur, this);
		}

		size_type bucket_count()const { return buckets.size(); }
		size_type max_size()const
		{
			return prime_list[num_primes - 1];
		}

		size_type elements_in_bucket(const size_type n)const
		{
			auto cur = buckets[n];
			size_type ct = 0;
			while (cur)
			{
				++ct;
				cur = cur->next;
			}
			return ct;
		}

		std::pair<iterator, bool> insert_unique
		(const value_type& val)
		{
			resize(num_elements + 1);
			return
				insert_unique_noresize(val);
		}

		template<typename InputIterator>
		void insert_unique(InputIterator first, InputIterator last)
		{
			while (first != last)insert_unique(*first++);
		}
		
		void resize(const size_type new_n)
		{
			const size_type old_n = buckets.size();
			if (new_n > old_n) 
			{
				const size_type n = next_prime(new_n);
				if (n > old_n)
				{
					std::vector<link_type>tmp(n, nullptr);
					for (size_type i = 0; i != old_n; ++i)
					{
						auto first = buckets[i];
						while (first)
						{
							size_type new_bucket_num
								= get_bucket_num(get_key(first->value), n);
							buckets[i] = first->next;
							first->next = tmp[new_bucket_num];
							tmp[new_bucket_num] = first;
							first = buckets[i];
						}
					}
					buckets.swap(tmp);
				}
			}
		}
		std::pair<iterator, bool> insert_unique_noresize
		(const value_type& val)
		{
			const size_type n = get_bucket_num(get_key(val));
			auto first = buckets[n];

			for (auto cur = first; cur; cur = cur->next)
				if (equals(get_key(cur->value), get_key(val)))
					return
			{ iterator(cur,this),false };

			auto tmp = new 
				hashtable_node<value_type>(val,first);
			buckets[n] = tmp;
			++num_elements;
			return
			{ iterator(tmp,this),true };
		}

		iterator insert_equal(const value_type& val)
		{
			resize(num_elements);
			return insert_equal_noresize(val);
		}

		template<typename InputIterator>
		void insert_equal(InputIterator first, InputIterator last)
		{
			while (first != last)insert_equal(*first++);
		}

		iterator insert_equal_noresize(const value_type&val)
		{
			const size_type n = get_bucket_num(get_key(val));
			auto tmp = new
				hashtable_node<value_type>(val, buckets[n]);
			buckets[n] = tmp;
			++num_elements;
			return iterator(tmp, this);
		}
	private:

		void init_buckets(const size_type n)
		{
			const size_type n_buckets = next_prime(n);
			buckets.resize(n_buckets,nullptr);
		}

		size_t get_bucket_num(const key_type&key,
			const size_t n)const
		{
			return hash(key) % n;
		}

		size_type get_bucket_num(const key_type& key)const
		{
			return get_bucket_num(key, buckets.size());
		}

	private:
		hasher hash;
		key_equal equals;
		ExtractKey get_key;

		std::vector<link_type> buckets;
		size_type num_elements;

	};
}



#endif