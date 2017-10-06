/*
*order-statistic tree 顺序统计树，用于需要经常增删的情况
*/
#ifndef _OS_TREE_
#define _OS_TREE_
namespace qmj
{
	typedef bool ost_color_type;
	const ost_color_type ost_red = false;
	const ost_color_type ost_black = true;

	template<typename key_type>
	struct os_tree_node
	{
		typedef key_type key_type;
		typedef os_tree_node<key_type>* link_type;

		os_tree_node(const key_type&k=key_type(), bool col = ost_red, size_t s = 1, 
			const link_type par = nullptr, const link_type lef = nullptr,
			const link_type righ = nullptr) :
			key(k), color(col), size(s), p(par), left(lef), right(righ) {}

		key_type key;
		ost_color_type color;
		size_t size;
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
	struct os_tree_iterator
	{
		template<typename key_type, typename Compare>
		friend class os_tree;

		typedef value_type value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef const value_type& const_reference;
		typedef const value_type* const_pointer;
		typedef os_tree_iterator
			<value_type, value_type&, value_type*> iterator;
		typedef os_tree_iterator
			<value_type, const value_type&, const value_type*> const_iterator;
		typedef os_tree_iterator<value_type, Ref, Ptr>self;
		typedef os_tree_node<value_type>* link_type;

		os_tree_iterator(const link_type x=link_type()):node(x) {}
		os_tree_iterator(const iterator&iter):node(iter.node) {}

		bool operator==(const iterator&iter)const { return node == iter.node; }
		bool operator!=(const iterator&iter)const { return !operator==(iter); }

		const_reference operator*()const { return node->key; }
		const_pointer operator->()const { return &(operator*()); }
		self& operator--()
		{
			if (!node->left)
				node = os_tree_node<value_type>::maximum(node->p);
			else if (node->left->left)
				node = os_tree_node<value_type>::maximum(node->left);
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
				node = os_tree_node<value_type>::minimum(node->right);
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

	template<typename key_type,typename Compare=std::less<key_type>>
	class os_tree 
	{
	public:
		friend struct os_tree_iterator<key_type, key_type&, key_type*>;
		typedef key_type key_type;
		typedef key_type* pointer;
		typedef const key_type* const_pointer;
		typedef key_type& reference;
		typedef const key_type& const_reference;
		typedef os_tree_node<key_type>* link_type;
		typedef os_tree_node<key_type> node_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef os_tree_iterator<key_type, key_type&, key_type*> iterator;
		typedef os_tree<key_type, Compare> self;

		os_tree(Compare comp = Compare()) :
			nil(new os_tree_node<key_type>(key_type(), ost_black, 0)),
			root(nil),comp(comp),node_count(0){}

		template<typename ForwardIterator>
		os_tree(ForwardIterator first, ForwardIterator last, Compare comp = Compare()) :
			os_tree(comp)
		{
			while (first != last)insert(*first++);
		}

		~os_tree()
		{
			ost_destroy(get_root());
			destroy_node(nil);
		}

		os_tree(const self&x):
			nil(new os_tree_node<key_type>(key_type(), ost_black, 0)),
			root(nil), comp(x.comp), node_count(0)
		{ copy_assign(x); }

		self& operator=(self x) 
		{
			swap(x);
			return *this;
		}

		void print_ost() { print(get_root()); }//调试用

		iterator begin() { return iterator(minimum(get_root())); }
		iterator end() { return iterator(nil); }

		void swap(self&x)
		{
			std::swap(nil, x.nil);
			std::swap(root, x.root);
			std::swap(node_count, x.node_count);
		}
		void clear()
		{
			ost_destroy(get_root());
			root = nil;
			node_count = 0;
		}

		size_type size() { return node_count; }

		iterator erase(iterator x)
		{ 
			auto temp = x;
			auto pred = ++temp;
			ost_delete(x.node);
			return pred;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator iter=first;
			while (iter != last)
				iter = erase(iter);
			return iter;
		}

		iterator find(const key_type& k);

		iterator insert(const key_type& key);
		void delete_key(const key_type&key) { ost_delete(find(key).node); }
		size_t key_rank(const key_type&key);

		size_t rank(iterator x);
		iterator select(const size_t k);
	protected:
		void destroy_node(link_type x) { delete x; }
		static link_type minimum(link_type rt) 
		{ return os_tree_node<key_type>::minimum(rt); }
		static link_type maximum(link_type rt)
		{ return os_tree_node<key_type>::maximum(rt); }
		void ost_destroy(link_type x);
		void left_rotate(link_type tar);
		void right_rotate(link_type tar);	
		void ost_delete(link_type tar);
		void print(link_type cur, size_t counter = 0) //调试用
		{
			if (cur != nil) 
			{
				print(cur->right, counter + 1);
				for (size_t n = counter; n != -1; --n)
					cout << "\t";
				cout << cur->key;
				cur->color ? cout << "|黑|" : cout << "|红|";
				cout << cur->size << "\n";
				cout << endl;
				print(cur->left, counter + 1);
			}
		}
		private:
			void ost_delete_fixup(link_type x);
			iterator ost_insert_fixup(link_type tar);
			void copy_assign(const link_type cur)
			{
				if (cur->left)
				{
					insert(cur->interval);
					copy_assign(cur->left);
					copy_assign(cur->right);
				}
			}
			void ost_transplant(link_type lhs, link_type rhs)
			{
				if (lhs->p == nil)
					root = rhs;
				else if (lhs == lhs->p->left)
					lhs->p->left = rhs;
				else
					lhs->p->right = rhs;
				rhs->p = lhs->p;
				rhs->size = lhs->size;
			}
		link_type get_root() { return root; }

	private:
		link_type nil;
		link_type root;
		size_t node_count;
		Compare comp;
	};

	template<typename key_type, typename Compare>
		size_t os_tree<key_type, Compare>::
	key_rank(const key_type&key)
	{
		size_t rk = 0;
		auto cur = get_root();
		while (cur != nil)
		{
			if (comp(key, cur->key))
				cur = cur->left;
			else if (comp(cur->key, key))
			{
				rk += cur->left->size + 1;
				cur = cur->right;
			}
			else
				return rk + cur->left->size + 1;
		}
		return rk;
	}

	template<typename key_type, typename Compare>
		size_t os_tree<key_type, Compare>::
	rank(iterator x)
	{
		auto tar = x.node;
		auto y = tar->p;
		size_t r = tar->left->size + 1;
		while (y != nil)
		{
			if (tar == y->right)
				r += y->left->size + 1;
			tar = y;
			y = y->p;
		}
		return r;
	}

	template<typename key_type, typename Compare>
		void os_tree<key_type, Compare>::
	ost_destroy(link_type x)
	{
		if (x != nil)
		{
			ost_destroy(x->left);
			ost_destroy(x->right);
			destroy_node(x);
		}
	}

	template<typename key_type, typename Compare>
	typename os_tree<key_type, Compare>::iterator
		os_tree<key_type, Compare>::
	select(const size_t k)
	{
		size_t n = k;
		auto cur = get_root();
		size_t r = cur->left->size + 1;
		while (cur != nil && r != n)
		{
			if (comp(n, r))
				cur = cur->left;
			else
			{
				cur = cur->right;
				n -= r;
			}
			r = cur->left->size + 1;
		}
		if (cur == nil)
			return iterator(nil);
		return iterator(cur);
	}

	template<typename key_type, typename Compare>
	typename os_tree<key_type, Compare>::iterator
		os_tree<key_type, Compare>::
	find(const key_type& k)
	{
		auto cur = get_root();
		while (cur != nil)
			if (comp(k, cur->key))
				cur = cur->left;
			else if (comp(cur->key, k))
				cur = cur->right;
			else
				return iterator(cur);
		return
			iterator(nil);
	}

	template<typename key_type, typename Compare>
	typename os_tree<key_type, Compare>::iterator 
		os_tree<key_type, Compare>::
		insert(const key_type& key)
	{
		++node_count;
		link_type tar = new os_tree_node<key_type>(key, ost_red, 1, nil, nil, nil);
		if (root == nil)
		{
			root = tar;
			tar->color = ost_black;
			nil->p = root;
			return iterator(tar);
		}
		link_type curr = root;
		link_type par = curr;
		while (curr != nil)
		{
			curr->size = curr->size + 1;
			par = curr;
			curr = comp(key, curr->key) ? curr->left : curr->right;
		}
		if (comp(key, par->key))
			par->left = tar;
		else
			par->right = tar;
		tar->p = par;
		return
			ost_insert_fixup(tar);
	}

	template<typename key_type, typename Compare>
	typename os_tree<key_type, Compare>::iterator
		os_tree<key_type, Compare>::
		ost_insert_fixup(link_type tar)
	{
		auto iter = iterator(tar);
		while (!tar->p->color)
		{
			link_type grandpar = tar->p->p;
			if (tar->p == grandpar->left)
			{
				if (!grandpar->right->color)
				{
					tar->p->color = ost_black;
					grandpar->right->color = ost_black;
					grandpar->color = ost_red;
					tar = grandpar;
				}
				else
				{
					if (tar == tar->p->right)
					{
						left_rotate(tar->p);
						tar = tar->left;
					}
					grandpar->color = ost_red;
					tar->p->color = ost_black;
					right_rotate(grandpar);
				}
			}
			else
			{
				if (!grandpar->left->color)
				{
					tar->p->color = ost_black;
					grandpar->left->color = ost_black;
					grandpar->color = ost_red;
					tar = grandpar;
				}
				else
				{
					if (tar == tar->p->left)
					{
						right_rotate(tar->p);
						tar = tar->right;
					}
					grandpar->color = ost_red;
					tar->p->color = ost_black;
					left_rotate(grandpar);
				}
			}
		}

		root->color = ost_black;
		nil->p = root;
		return iter;
	}

	template<typename key_type, typename Compare>
	void os_tree<key_type, Compare>::
		ost_delete(link_type tar)
	{
		if (tar == nil)
			return;
		for (auto par = tar; par != nil; par = par->p)
			par->size -= 1;
		link_type y = tar;
		ost_color_type y_original_color = y->color;
		link_type x;
		if (tar->left == nil)
		{
			x = tar->right;
			ost_transplant(tar, x);
		}
		else if (tar->right == nil)
		{
			x = tar->left;
			ost_transplant(tar, x);
		}
		else
		{
			auto y = tar->right;
			while (y->left != nil)
			{
				y->size -= 1;
				y = y->left;
			}
			y_original_color = y->color;
			x = y->right;
			if (y->p == tar)
				x->p = y;//当x是哨兵时为删除做准备
			else
			{
				ost_transplant(y, x);
				y->right = tar->right;
				y->right->p = y;
			}
			ost_transplant(tar, y);
			y->left = tar->left;
			y->left->p = y;
			y->color = tar->color;
		}

		--node_count;
		if (y_original_color)
			ost_delete_fixup(x);
		destroy_node(tar);
		nil->p = root;
	}

	template<typename key_type, typename Compare>
	void os_tree<key_type, Compare>::
		ost_delete_fixup(link_type x)
	{
		while (x != root&&x->color)
		{
			if (x == x->p->left)
			{
				link_type w = x->p->right;
				if (!w->color)
				{//情况1，如果兄弟节点是红色
					x->p->color = ost_red;
					w->color = ost_black;
					left_rotate(x->p);
					w = x->p->right;
				}

				if (w->right->color&&w->left->color)
				{//情况二，兄弟节点的两个子节点都是黑色
					w->color = ost_red;
					x = x->p;
				}
				else if (w->right->color)
				{//兄弟节点的左节点为红色
					w->color = ost_red;
					w->left->color = ost_black;
					right_rotate(w);
					w = x->p->right;
				}
				if (!w->right->color)
				{//兄弟节点的右节点为红色
					w->color = x->p->color;
					x->p->color = ost_black;
					w->right->color = ost_black;
					left_rotate(x->p);
					x = root;
				}
			}
			else {
				link_type  w = x->p->left;
				if (!w->color)
				{
					x->p->color = ost_red;
					w->color = ost_black;
					right_rotate(x->p);
					w = x->p->left;
				}
				if (w->right->color&&w->left->color)
				{
					w->color = ost_red;
					x = x->p;
				}
				else if (w->left->color)
				{
					w->color = ost_red;
					w->right->color = ost_black;
					left_rotate(w);
					w = x->p->left;
				}
				if (!w->left->color)
				{
					w->color = x->p->color;
					x->p->color = ost_black;
					w->left->color = ost_black;
					right_rotate(x->p);
					x = root;
				}
			}
		}
		x->color = ost_black;
	}

	template<typename key_type, typename Compare>
	void os_tree<key_type, Compare>::
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

		taRight->size = tar->size;
		tar->size = tar->left->size + tar->right->size + 1;
	}

	template<typename key_type, typename Compare>
	void os_tree<key_type, Compare>::
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

		tarLeft->size = tar->size;
		tar->size = tar->left->size + tar->right->size + 1;
	}

}

#endif