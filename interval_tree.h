#ifndef _INTERVAL_TREE_
#define _INTERVAL_TREE_
/*红黑树构建的区间树，兼容四种闭开区间情况(),(],[),[]*/
namespace qmj
{
	typedef bool intree_color_type;
	const intree_color_type intree_red = false;
	const intree_color_type intree_black = true;

	template<typename key_type>
	struct interval_node
	{
		template<typename key_type, typename Compare>
		friend bool is_overlap(const interval_node<key_type>& lhs,
			const interval_node<key_type>&rhs,Compare comp);

		typedef key_type key_type;
		interval_node(const key_type&low=key_type(),const key_type&high=key_type(),
			bool low_closed=true,bool high_closed=true):
			low(low),high(high),low_closed(low_closed),high_closed(high_closed){}


		bool operator==(const interval_node<key_type>&rhs)const
		{
			return high == rhs.high &&
				low_closed == rhs.low_closed&&
				high_closed == rhs.high_closed&&
				low == rhs.low;
		}

		bool operator!= (const interval_node<key_type>&rhs)const
		{	
			return !operator == (rhs);
		}

		key_type low;
		key_type high;
		bool low_closed;
		bool high_closed;
	};

	template<typename key_type,typename Compare=std::less<key_type>>
	bool is_overlap(const interval_node<key_type>& lhs,
		const interval_node<key_type>& rhs,Compare comp=Compare())
	{
		if (lhs.low_closed)
		{
			if (lhs.high_closed)
			{
				if (rhs.low_closed)
				{
					if (rhs.high_closed)
						return
						(!comp(rhs.high, lhs.low)) && (!comp(lhs.high, rhs.low));
					else
						return
						comp(lhs.low, rhs.high) && (!comp(lhs.high, rhs.low));
				}
				else
				{
					if (rhs.high_closed)
						return
						(!comp(rhs.high, lhs.low)) && comp(rhs.low, lhs.high);
					else
						return
						comp(lhs.low, rhs.high) && comp(rhs.low, lhs.high);
				}
			}
			else
			{
				if (rhs.high_closed)
					return
					(!comp(rhs.high, lhs.low)) && comp(rhs.low, lhs.high);
				else
					return
					comp(lhs.low, rhs.high) && comp(rhs.low, lhs.high);
			}
		}
		else
		{
			if (lhs.high_closed)
			{
				if (rhs.low_closed)
					return
					comp(lhs.low, rhs.high) && (!comp(lhs.high, rhs.low));
				else
					return
					comp(lhs.low, rhs.high) && comp(rhs.low, lhs.high);
			}
			else
				return
				comp(lhs.low, rhs.high) && comp(rhs.low, lhs.high);
		}
		return false;
	}
	
	template<typename key_type>
	struct interval_tree_node {
		typedef key_type key_type;
		typedef interval_tree_node<key_type>* link_type;
		typedef interval_node<key_type> interval_type;

		interval_tree_node(const intree_color_type col = intree_red,
			const interval_node<key_type>&interval=interval_node<key_type>(),
			const key_type max = key_type(),  
			const link_type par = nullptr,
			const link_type lef = nullptr, const link_type righ = nullptr) :
			color(col), interval(interval), max(max), p(par), left(lef), right(righ) {}

		intree_color_type color;
		interval_type interval;
		key_type max;

		link_type p;
		link_type left;
		link_type right;

		static link_type minimum(link_type rt)
		{
			while (rt->left->left)
				rt = rt->left;
			return rt;
		}
		static link_type maximum(link_type rt)
		{
			while (rt->right->right)
				rt = rt->right;
			return rt;
		}
	};


	template<typename value_type, typename Ref, typename Ptr>
	struct interval_tree_iterator
	{
		template<typename key_type, typename Compare>//友元声明所需
		class interval_tree;

		typedef value_type value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef const interval_node<value_type>& const_reference;
		typedef const interval_node<value_type>* const_pointer;
		typedef interval_tree_iterator
			<value_type, value_type&, value_type*>iterator;
		typedef interval_tree_iterator
			<value_type, const value_type&,const value_type*>const_iterator;
		typedef interval_tree_iterator<value_type, Ref, Ptr>self;
		typedef interval_tree_node<value_type>* link_type;

		interval_tree_iterator(const link_type x = link_type()):node(x) {}
		interval_tree_iterator(const iterator&iter) :node(iter.node) {}

		bool operator==(const iterator&iter)const { return node == iter.node; }
		bool operator!=(const iterator&iter)const { return !operator==(iter); }

		const_reference operator*()const { return node->interval; }
		const_pointer* operator->()const { return &(operator*()); }
		self& operator--()
		{
			if (!node->left)
				node = interval_tree_node<value_type>::maximum(node->p);
			else if (node->left->left)
				node = interval_tree_node<value_type>::maximum(node->left);
			else
			{
				while (node->p->p&&node->p->left == node)
					node = node->p;
				node = node->p;
			}
			return *this;
		}

		self operator--(int)
		{
			auto temp = *this;
			operator--();
			return temp;
		}

		self& operator++()
		{
			if (node->right->right)
				node = interval_tree_node<value_type>::minimum(node->right);
			else
			{
				while (node->p->p&&node->p->right == node)
					node = node->p;
				node = node->p;
			}
			return *this;
		}

		self operator++(int)
		{
			auto temp = *this;
			operator++();
			return temp;
		}

	private:
		link_type node;
	};
	
	template<typename key_type,
		typename Compare=std::less<key_type>>
	class interval_tree {
	public:
		friend struct interval_tree_iterator<key_type, key_type&, key_type*>;
		typedef key_type key_type;
		typedef key_type* pointer;
		typedef const key_type* const_pointer;
		typedef key_type& reference;
		typedef const key_type& const_reference;
		typedef interval_tree_node<key_type>* link_type;
		typedef interval_tree_node<key_type> node_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef interval_node<key_type> interval_type;
		typedef interval_tree_iterator<key_type, key_type&, key_type*> iterator;
		typedef interval_tree<key_type, Compare> self;

		interval_tree(Compare comp = Compare()) :
			nil(new interval_tree_node<key_type>(intree_black)),
			is_same(false), root(nil), comp(comp), node_count(0) {}

		interval_tree(bool low_closed, bool high_closed, Compare comp = Compare()) :
			nil(new interval_tree_node<key_type>(intree_black)),
			is_same(true), root(nil), comp(comp), node_count(0)
		{
			nil->interval.low_closed = low_closed;
			nil->interval.high_closed = high_closed;
		}

		template<typename ForwardIterator>
		interval_tree(ForwardIterator first, ForwardIterator last, 
			Compare comp = Compare()) :
			interval_tree(comp)
		{
			while (first != last)insert(*first++);
		}

		template<typename ForwardIterator>
		interval_tree(bool low_closed, bool high_closed,
			ForwardIterator first, ForwardIterator last,
			Compare comp = Compare()) :
			interval_tree(low_closed, high_closed, comp)
		{
			while (first != last)
			{
				auto temp = *first++;
				insert(temp.first, temp.second);
			}
		}

		interval_tree(const self&x):
			nil(new interval_tree_node<key_type>(intree_black)),
			is_same(x.is_same), root(nil), comp(x.comp), node_count(0)
		{
			copy_assign(x.root);
		}

		self& operator=(self x)
		{ 
			swap(x);
			return *this;
		}

		void swap(self&x)
		{
			std::swap(is_same, x.is_same);
			std::swap(root, x.root);
			std::swap(comp, x.comp);
			std::swap(node_count, x.node_count);
		}

		~interval_tree()
		{
			interval_tree_destroy(get_root());
			destroy_node(nil);
		}

		iterator erase(iterator x)
		{
			auto temp = x;
			auto pred = ++temp;
			intree_delete(x.node);
			return pred;
		}

		iterator erase(iterator first, iterator last)
		{
			auto iter = first;
			while (iter != last)
				iter = erase(iter);
			return iter;
		}

		void clear()
		{
			interval_tree_destroy(get_root());
			root = nil;
			node_count = 0;
		}

		void print_int_tree() { print(get_root()); }//调试用

		iterator begin() { return iterator(minimum(get_root())); }
		iterator end() { return iterator(nil); }
		size_type size() { return node_count; }
		void all_overlap_interval(const interval_type& inter,self& intree)
		{//将所有与inter重叠的区间插入intree，intree是区间树，intree原有的数据将丢失
			self tmp;
			get_all_overlap(tmp, get_root(), inter);
			tmp.swap(intree);
		}
		void all_overlap_interval(const key_type&low, const key_type&high,
			self& inter)
		{
			all_overlap_interval(interval_node<key_type>
				(low, high, nil->interval.low_closed, 
					nil->interval.high_closed),
				inter);
		}

		iterator overlap_interval(const key_type&low, const key_type&high)
		{
			return
				overlap_interval(interval_node<key_type>(low, high,
					nil->interval.low_closed, nil->interval.high_closed));
		}

		iterator overlap_interval(const interval_type&inter);//返回一个重叠区间的迭代器

		void left_rotate(link_type tar);
		
		void right_rotate(link_type tar);

		void delete_interval(const interval_type&inter)
		{
			interee_delete(find_del(inter));
		}

		void delete_interval(const key_type&low, const key_type&high)
		{
			delete_interval(interval_node<key_type>(low, high,
				nil->interval.low_closed, nil->interval.high_closed));
		}

		iterator insert(const key_type&low, const key_type&high)
		{
			return
				insert(interval_node<key_type>(low, high, 
					nil->interval.low_closed,nil->interval.high_closed));
		}
		iterator insert(const interval_type&interval);

	protected:
		void print(link_type rt, int counter = 0) 
		{
			if (rt != nil) 
			{
				print(rt->right, counter + 1);
				for (int n = counter; n != -1; --n)
					cout << "\t";
				rt->interval.low_closed ? cout << "[" : cout << "(";
				cout << rt->interval.low << "," << rt->interval.high;
				rt->interval.high_closed ? cout << "]|" : cout << ")|";
				cout<< rt->max;
				rt->color ? cout << "黑" : cout << "红";
				print(rt->left, counter + 1);
			}
			cout << endl;
		}
	protected:
		void inter_insert(link_type rt, link_type tar);
		void get_all_overlap(self& intree, link_type cur, const interval_type& inter);
		void destroy_node(link_type x) { delete x; }
		iterator int_insert_fixup(link_type tar);
		link_type get_root() { return root; }
		void interval_tree_destroy(link_type x);
		link_type find_del(const interval_type&inter);
		void interee_delete(link_type tar);
		void intree_delete_fixup(link_type x);
		void copy_assign(const link_type cur)
		{
			if (cur->left)
			{
				insert(cur->interval);
				copy_assign(cur->left);
				copy_assign(cur->right);
			}
		}
		void get_max(link_type tar)
		{
			tar->max = comp(tar->right->max, tar->left->max) ?
				tar->left->max : tar->right->max;
			if (comp(tar->max, tar->interval.high))
				tar->max = tar->interval.high;
		}
		void intree_transplant(link_type lhs, link_type rhs)
		{
			if (lhs->p == nil)
				root = rhs;
			else if (lhs == lhs->p->left)
				lhs->p->left = rhs;
			else
				lhs->p->right = rhs;
			rhs->p = lhs->p;
		}
		static link_type minimum(link_type rt)
		{
			return interval_tree_node<key_type>::minimum(rt);
		}
		static link_type maximum(link_type rt)
		{
			return interval_tree_node<key_type>::maximum(rt);
		}

	private:
		link_type nil;
		bool is_same;
		link_type root;
		Compare comp;
		size_t node_count;
	};

	template<typename key_type, typename Compare>
	void interval_tree<key_type, Compare>::
		get_all_overlap(self& intree, link_type cur, const interval_type& inter) 
	{
		while (cur != nil) 
		{
			if (is_overlap(cur->interval, inter))
				intree.insert(cur->interval);
			if (!comp(cur->left->max, inter.low))
				get_all_overlap(intree, cur->left, inter);
			cur = cur->right;
			while (cur != nil&&comp(inter.high, cur->interval.low))
				cur = cur->left;
		}
	}
	
	template<typename key_type, typename Compare>
	typename interval_tree<key_type, Compare>::link_type
		interval_tree<key_type, Compare>::
		find_del(const interval_type&inter)
	{
		auto cur = get_root();
		while (cur != nil&&inter != cur->interval)
		{
			if (comp(inter.low, cur->interval.low))
				cur = cur->left;
			else
				cur = cur->right;
		}
		return cur;
	}

	template<typename key_type, typename Compare>
	typename interval_tree<key_type, Compare>::iterator
		interval_tree<key_type, Compare>::
		overlap_interval(const interval_type&inter)
	{//返回一个重叠区间
		auto cur = get_root();
		while (cur != nil &&
			(!is_overlap(inter, cur->interval)))
		{
			if (comp(inter.low, cur->left->max))
				cur = cur->left;
			else
				cur = cur->right;
		}
		return iterator(cur);
	}

	template<typename key_type, typename Compare>
	void interval_tree<key_type, Compare>::
	interval_tree_destroy(link_type x)
	{
		if (x != nil)
		{
			interval_tree_destroy(x->left);
			interval_tree_destroy(x->right);
			destroy_node(x);
		}
	}

	template<typename key_type, typename Compare>
	typename interval_tree<key_type, Compare>::iterator
		interval_tree<key_type, Compare>::
		insert(const interval_type&interval)
	{
		++node_count;
		link_type tar = new interval_tree_node<key_type>(intree_red, interval,
			interval.high, nil, nil, nil);
		if (root == nil)
		{
			root = tar;
			tar->color = intree_black;
			nil->p = root;
			return iterator(nil);
		}
		inter_insert(root, tar);
		return
			int_insert_fixup(tar);
	}

	template<typename key_type, typename Compare>
	void interval_tree<key_type, Compare>::
		right_rotate(link_type tar)
	{
		auto tarLeft = tar->left;
		if (tar->p == nil)
			root = tarLeft;
		else if (tar == tar->p->left)
			tar->p->left = tarLeft;
		else
			tar->p->right = tarLeft;
		tarLeft->p = tar->p;

		if (tarLeft->right != nil)
			tarLeft->right->p = tar;
		tar->left = tarLeft->right;

		tar->p = tarLeft;
		tarLeft->right = tar;

		tarLeft->max = tar->max;
		get_max(tar);
	}

	template<typename key_type, typename Compare>
	void interval_tree<key_type, Compare>::
		left_rotate(link_type tar)
	{
		auto taRight = tar->right;
		if (tar->p == nil)
			root = taRight;
		else if (tar == tar->p->left)
			tar->p->left = taRight;
		else
			tar->p->right = taRight;
		taRight->p = tar->p;

		if (taRight->left != nil)
			taRight->left->p = tar;
		tar->right = taRight->left;

		taRight->left = tar;
		tar->p = taRight;

		taRight->max = tar->max;
		get_max(tar);
	}

	template<typename key_type, typename Compare>
	void interval_tree<key_type, Compare>::
		inter_insert(link_type rt, link_type tar)
	{
		if (comp(tar->interval.low, rt->interval.low))
		{
			if (rt->left == nil)
			{
				rt->left = tar;
				tar->p = rt;
				if (comp(rt->max, tar->max))
					rt->max = tar->max;
			}
			else
			{
				inter_insert(rt->left, tar);
				get_max(rt);
			}
		}
		else
		{
			if (rt->right == nil)
			{
				rt->right = tar;
				tar->p = rt;
				if (comp(rt->max, tar->max))
					rt->max = tar->max;
			}
			else
			{
				inter_insert(rt->right, tar);
				get_max(rt);
			}
		}
	}

	template<typename key_type, typename Compare>
	typename interval_tree<key_type,Compare>::iterator
		interval_tree<key_type, Compare>::
		int_insert_fixup(link_type tar)
	{
		auto iter = iterator(tar);
		while (!tar->p->color)
		{
			link_type grandpar = tar->p->p;
			if (tar->p == grandpar->left)
			{
				if (!grandpar->right->color)
				{
					tar->p->color = intree_black;
					grandpar->right->color = intree_black;
					grandpar->color = intree_red;
					tar = grandpar;
				}
				else
				{
					if (tar == tar->p->right)
					{
						left_rotate(tar->p);
						tar = tar->left;
					}
					grandpar->color = intree_red;
					tar->p->color = intree_black;
					right_rotate(grandpar);
				}
			}
			else
			{
				if (!grandpar->left->color)
				{
					tar->p->color = intree_black;
					grandpar->left->color = intree_black;
					grandpar->color = intree_red;
					tar = grandpar;
				}
				else
				{
					if (tar == tar->p->left)
					{
						right_rotate(tar->p);
						tar = tar->right;
					}
					grandpar->color = intree_red;
					tar->p->color = intree_black;
					left_rotate(grandpar);
				}
			}
		}
		root->color = intree_black;
		nil->p = root;
		return iter;
	}

	template<typename key_type, typename Compare>
	void interval_tree<key_type, Compare>::
		interee_delete(link_type tar)
	{
		if (tar == nil)
			return;

		link_type y = tar;
		intree_color_type y_original_color = y->color;
		link_type x;
		if (tar->left == nil)
		{
			x = tar->right;
			intree_transplant(tar, x);
		}
		else if (tar->right == nil)
		{
			x = tar->left;
			intree_transplant(tar, x);
		}
		else
		{
			y = minimum(tar->right);
			y_original_color = y->color;
			x = y->right;
			if (y->p == tar)
				x->p = y;//当x是哨兵时为删除做准备
			else
			{
				intree_transplant(y, x);
				y->right = tar->right;
				y->right->p = y;
			}
			intree_transplant(tar, y);
			y->left = tar->left;
			y->left->p = y;
			y->color = tar->color;
		}

		auto cur = x->p;
		while (cur != nil)
		{
			get_max(cur);
			cur = cur->p;
		}

		--node_count;
		if (y_original_color)
			intree_delete_fixup(x);
		destroy_node(tar);
		nil->p = root;
	}

	template<typename key_type, typename Compare>
	void interval_tree<key_type, Compare>::
		intree_delete_fixup(link_type x)
	{
		while (x != root&&x->color)
		{
			if (x == x->p->left)
			{
				link_type w = x->p->right;
				if (!w->color)
				{//情况1，如果兄弟节点是红色
					x->p->color = intree_red;
					w->color = intree_black;
					left_rotate(x->p);
					w = x->p->right;
				}

				if (w->right->color&&w->left->color)
				{//情况二，兄弟节点的两个子节点都是黑色
					w->color = intree_red;
					x = x->p;
				}
				else if (w->right->color)
				{//兄弟节点的左节点为红色
					w->color = intree_red;
					w->left->color = intree_black;
					right_rotate(w);
					w = x->p->right;
				}
				if (!w->right->color)
				{//兄弟节点的右节点为红色
					w->color = x->p->color;
					x->p->color = intree_black;
					w->right->color = intree_black;
					left_rotate(x->p);
					x = root;
				}
			}
			else {
				link_type  w = x->p->left;
				if (!w->color)
				{
					x->p->color = intree_red;
					w->color = intree_black;
					right_rotate(x->p);
					w = x->p->left;
				}
				if (w->right->color&&w->left->color)
				{
					w->color = intree_red;
					x = x->p;
				}
				else if (w->left->color)
				{
					w->color = intree_red;
					w->right->color = intree_black;
					left_rotate(w);
					w = x->p->left;
				}
				if (!w->left->color)
				{
					w->color = x->p->color;
					x->p->color = intree_black;
					w->left->color = intree_black;
					right_rotate(x->p);
					x = root;
				}
			}
		}
		x->color = intree_black;
	}

}


#endif