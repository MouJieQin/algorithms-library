#ifndef _DISJOINT_SET_
#define _DISJOINT_SET_
#include<map>
#include<set>
#include<vector>

namespace qmj
{//不相交集合的链表表示，如果不需删除推荐使用后面的不相交森林，
 //该删除操作也可以变为双链表后改进，不相交森林也可以增加删除操作
	template<typename key_type>
	struct set_list_node;

	template<typename key_type>
	struct set_list
	{
		set_list(size_t s, set_list_node<key_type>* n = nullptr) :
			size(s), next(n) {}

		size_t size;
		set_list_node<key_type>* next;
	};

	template<typename key_type>
	struct set_list_node
	{
		set_list_node(const key_type& k = key_type(),
			set_list<key_type>* h = nullptr,
			set_list_node<key_type>* t = nullptr) :
			key(k), head(h), next(t) {}

		key_type key;
		set_list<key_type>* head;
		set_list_node<key_type>* next;
	};

	template<typename key_type,
		typename Map = std::map<key_type, set_list_node<key_type>*>>
		class disjoint_set_list
	{
	public:
		typedef size_t size_type;
		typedef key_type key_type;
		typedef set_list<key_type>* head_link_type;
		typedef set_list_node<key_type>* next_link_type;

		disjoint_set_list() :node_count(0) {}

		template<typename Iter>
		disjoint_set_list(Iter first, Iter last) : node_count(0)
		{
			while (first != last)make_set(*first++);
		}

		~disjoint_set_list()
		{
			clear();
		}
		size_type size() { return node_count; }
		void clear()
		{
			std::set<set_list<key_type>*> head_set;
			for (auto &x : mp)
			{
				auto head = x.second->head;
				delete x.second;
				head_set.insert(head);
			}
			for (auto &x : head_set)
				delete x;

			Map tmp;
			mp.swap(tmp);
			node_count = 0;
		}
		void empty() { return size() == 0; }
		void erase(const key_type&k)
		{//因为使用的是单链表，删除操作花费较大
			auto head = mp[k]->head;
			auto tmp = mp[k];
			if (head->next == mp[k])
			{
				mp.erase(k);
				delete head;
			}
			else
			{
				auto cur = head->next;
				while (cur->next != mp[k])
					cur = cur->next;
				cur->next = mp[k]->next;
			}
			delete tmp;
			--node_count;
		}

		bool is_same_set(const key_type&x, const key_type&y)
		{
			return find_set(x) == find_set(y);
		}
		bool is_in_set(const key_type&k) { return mp.find(k) != mp.end(); }
		void make_set(const key_type& key)
		{
			auto n = new set_list_node<key_type>(key);
			auto s = new set_list<key_type>(1, n);
			n->head = s;
			mp.insert(make_pair(key, n));
			++node_count;
		}

		void union_set(const key_type& lef, const key_type& righ)
		{
			if (is_same_set(lef, righ))
				return;
			auto lhs = mp[lef]->head;
			auto rhs = mp[righ]->head;
			if (lhs->size > rhs->size)
				std::swap(lhs, rhs);

			auto cur = lhs->next;
			auto temp = cur;
			auto last = cur;
			while (cur)
			{
				last = cur;
				cur->head = rhs;
				cur = cur->next;
			}
			last->next = rhs->next;
			rhs->next = temp;

			rhs->size += lhs->size;
			delete lhs;
			lhs = nullptr;
		}
	protected:
		key_type find_set(const key_type& key)
		{
			return//确保要查找的元素在集合中
				mp[key]->head->next->key;
		}
	private:
		Map mp;
		size_type node_count;
	};

}

namespace qmj
{//不相交集合不提供删除节点操作，如需要删除需要增加一个节点
	template<typename key_type>
	struct set_tree_node
	{
		typedef key_type key_type;
		typedef set_tree_node<key_type>* link_type;
		set_tree_node(const key_type& key = key_type(),
			size_t rank = 0, link_type p = nullptr, link_type b = nullptr) :
			key(key), rank(rank), p(p) {}

		key_type key;
		size_t rank;
		link_type p;
		//	link_type c;//可以增加此孩子节点来执行删除操作，算法需要重新设计
	};

	template<typename key_type,
		typename Map = std::map<key_type, set_tree_node<key_type>*>>
		class disjoint_forest
	{
	public:
		typedef key_type key_type;
		typedef set_tree_node<key_type>* link_type;
		typedef size_t size_type;

		disjoint_forest() :node_count(0) {}

		template<typename Iter>
		disjoint_forest(Iter first, Iter last) { while (first != last)make_set(*first++); }

		~disjoint_forest() { clear(); }

		size_type size() { return node_count; }
		bool empty() { return node_count == 0; }
		void clear()
		{
			for (auto &x : mp)
				delete x.second;

			Map tmp;
			mp.swap(tmp);
			node_count = 0;
		}

		bool is_in_forest(const key_type&x) { return mp.find(x) != mp.end(); }
		bool is_same_set(const key_type&x, const key_type&y)
		{
			return find_set(x) == find_set(y);
		}

		void make_set(const key_type& k)
		{
			mp.insert(make_pair(k,
				new set_tree_node<key_type>(k)));
			mp[k]->p = mp[k];
			++node_count;
		}

		void union_set(link_type lef, link_type righ)
		{
			
			if (lef->rank > righ->rank)
				righ->p = lef;
			else
			{
				lef->p = righ;
				if (lef->rank = righ->rank)
					righ->rank += 1;
			}
		}

		void union_set(const key_type& lhs, const key_type& rhs)
		{
			auto lef = find_set(lhs);
			auto righ = find_set(rhs);
			union_set(lef, righ);
		}

	protected:
		link_type find_set(const key_type& k)
		{
			auto t = mp[k];
			auto curr = t;
			while (curr != curr->p)
				curr = curr->p;
			while (t != curr)
			{
				t->p = curr;
				t = t->p;
			}
			return curr;
		}

	private:
		Map mp;
		size_type node_count;
	};
}

namespace qmj
{//不相交集合不提供删除节点操作，如需要删除需要增加一个节点
	//针对size_t从0开始比较密集的元素提供的不相交集合
	struct set_tree_node_size_t
	{
		typedef set_tree_node_size_t* link_type;
		set_tree_node_size_t(const size_t key = 0,
			size_t rank = 0, link_type p = nullptr, link_type b = nullptr) :
			key(key), rank(rank), p(p) {}

		size_t key;
		size_t rank;
		link_type p;
		//	link_type c;//可以增加此孩子节点来执行删除操作，算法需要重新设计
	};

	class disjoint_forest_size_t
	{
	public:
		typedef set_tree_node_size_t* link_type;
		typedef size_t size_type;

		disjoint_forest_size_t() :node_count(0) {}

		template<typename Iter>
		disjoint_forest_size_t(Iter first, Iter last) { while (first != last)make_set(*first++); }

		~disjoint_forest_size_t() { clear(); }

		size_type size() { return node_count; }
		bool empty() { return node_count == 0; }
		void clear()
		{
			for (auto &x : mp)
				delete x;
			std::vector<link_type>tmp;
			mp.swap(tmp);
			node_count = 0;
		}

		bool is_in_forest(const size_t x) { return x < mp.size(); }
		bool is_same_set(const size_t x, const size_t y)
		{
			return find_set(x) == find_set(y);
		}

		void make_set(const size_t k)
		{
			for (size_t i = mp.size(); i <= k; ++i)
				mp.push_back(new set_tree_node_size_t(k));
			mp[k]->p = mp[k];
			++node_count;
		}

		void union_set(link_type lef, link_type righ)
		{
			if (lef->rank > righ->rank)
				righ->p = lef;
			else
			{
				lef->p = righ;
				if (lef->rank = righ->rank)
					righ->rank += 1;
			}
		}
		void union_set(const size_t lhs, const size_t rhs)
		{
			auto lef = find_set(lhs);
			auto righ = find_set(rhs);
			union_set(lef, righ);
		}

		link_type find_set(const size_t k)
		{
			auto t = mp[k];
			auto curr = t;
			while (curr != curr->p)
				curr = curr->p;
			while (t != curr)
			{
				t->p = curr;
				t = t->p;
			}
			return curr;
		}

	private:
		std::vector<link_type> mp;
		size_type node_count;
	};
}


#endif