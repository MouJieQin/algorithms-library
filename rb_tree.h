#ifndef _RB_TREE_
#define _RB_TREE_
#include<utility>

namespace qmj
{
	typedef bool rbt_color_type;
	const rbt_color_type rbt_red = false;
	const rbt_color_type rbt_black = true;

	template<typename value_type>
	struct rb_tree_node
	{
		typedef rb_tree_node<value_type>* link_type;

		rb_tree_node(const value_type&value=value_type(), rbt_color_type color = rbt_red,
			const link_type p = nullptr, const link_type left = nullptr, 
			const link_type right = nullptr) :
			value(value), color(color), p(p), left(left), right(right) {}


		link_type p;
		link_type left;
		link_type right;
		value_type value;
		rbt_color_type color;

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
	struct rb_tree_iterator
	{
		template<typename key_type,typename value_type, typename keyOfValue, typename Compare>
		friend class rb_tree;

		typedef bidirectional_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef ptrdiff_t difference_type;

		typedef rb_tree_iterator
			<value_type, value_type&, value_type*> iterator;

		typedef rb_tree_iterator
			<value_type, const value_type&, const value_type*> const_iterator;

		typedef rb_tree_iterator<value_type, Ref, Ptr> self;
		typedef rb_tree_node<value_type>* link_type;

		rb_tree_iterator(const link_type x=link_type()):node(x) {}
		rb_tree_iterator(const self&iter):node(iter.node) {}

		bool operator==(const self&iter)const { return node == iter.node; }
		bool operator!=(const self&iter)const { return !operator==(iter); }

		reference operator*()const { return node->value; }
		pointer operator->()const { return &(operator*()); }

		self& operator--()
		{
			if(!node->left)
				node = rb_tree_node<value_type>::maximum(node->p);
			else if (node->left->left)
				node = rb_tree_node<value_type>::maximum(node->left);
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
				node = rb_tree_node<value_type>::minimum(node->right);
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

	

	template<typename key_type,typename value_type,typename keyOfValue,typename Compare = std::less<key_type>>
	class rb_tree
	{
	public:
		typedef key_type key_type;
		typedef value_type value_type;
		typedef keyOfValue get_key;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef rb_tree_node<value_type>* link_type;
		typedef ptrdiff_t difference_type;

		typedef rb_tree_node<value_type> node_type;
		typedef size_t size_type;
		typedef typename 
			rb_tree_iterator<value_type, value_type&, value_type*>
			::iterator  iterator;
		typedef typename
			rb_tree_iterator<value_type, value_type&, value_type*>
			::const_iterator const_iterator;

		typedef reverse_iterator<const_iterator> const_reverse_iterator;
		typedef reverse_iterator<iterator> reverse_iterator;

		typedef rb_tree<key_type,value_type,keyOfValue, Compare> self;
		typedef ptrdiff_t difference_type;
		
		explicit rb_tree(Compare comp=Compare()):
			nil(new rb_tree_node<value_type>(value_type(), rbt_black)),
			root(nil),comp(comp),node_count(0){}

		rb_tree(const self&x) :
			nil(new rb_tree_node<value_type>(value_type(), rbt_black)),
			root(nil), comp(x.comp), node_count(0)
		{
			copy_assign(x.get_root());
		}

		self& operator=(self x) 
		{	
			swap(x);
			return *this;
		}

		~rb_tree()
		{
			rbt_destroy(get_root());
			destroy_node(nil);
		}

		bool operator==(const self&x)
		{
			if (size() != x.size())
				return false;
			auto first1 = begin();
			auto last1 = end();
			auto first2 = x.begin();
			for (; first1 != last1; ++first1, ++first2)
				if (comp(get_key()(*first1), get_key()(*first2)) 
					|| comp(get_key()(*first2), get_key()(*first1)))
					return false;
			return true;
		}

		bool operator!=(const self&x)
		{
			return !(operator==(x));
		}

		void print_rbt() { print(get_root()); }//调试用

		iterator end() 
		{ 
			return iterator(nil);
		}

		iterator begin()
		{
			return root == nil ? end() : iterator(minimum(get_root()));
		}

		const_iterator end()const
		{
			return const_iterator(nil);
		}

		const_iterator begin()const 
		{
			return root==nil?end():const_iterator(minimum(get_root()));
		}

		const_iterator cend()const 
		{ 
			return const_iterator(nil); 
		}

		const_iterator cbegin()const 
		{
			return root==nil?cend():const_iterator(minimum(get_root())); 
		}

		reverse_iterator rbegin() 
		{ 
			return reverse_iterator(end()); 
		}

		reverse_iterator rend()
		{ 
			return reverse_iterator(begin()); 
		}

		const_reverse_iterator rbeing()const
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend()const
		{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crbegin()const 
		{ 
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend()const
		{ 
			return const_reverse_iterator(cbegin());
		}

		void clear()
		{
			rbt_destroy(get_root());
			root = nil;
			node_count = 0;
		} 

		void swap(self&x)
		{
			std::swap(&x, this);
		}

		size_type size()const
		{
			return node_count; 
		}

		iterator erase(iterator& x)
		{
			auto temp = x;
			auto pred = ++temp;
			rbt_delete(x.node);
			return temp;
		}

		iterator erase(iterator& first, iterator& last)
		{
			iterator iter;
			while (first != last)
				iter = erase(first++);
			return iter;
		}

		void delete_key(const key_type&key)
		{
			rbt_delete(find(key).node);
		}

		const_iterator lower_bound_imple(const key_type&key)const;

		const_iterator lower_bound(const key_type&key)const
		{
			return lower_bound_imple(key);
		}

		iterator lower_bound(const key_type&key)
		{
			return (iterator&)lower_bound_imple(key);
		}

		const_iterator upper_bound_imple(const key_type&key)const;

		const_iterator upper_bound(const key_type&key)const
		{
			return upper_bound_imple(key);
		}

		iterator upper_bound(const key_type&key)
		{
			return (iterator&)upper_bound_imple(key);
		}

		std::pair<const_iterator,const_iterator>
			equal_range(const key_type&key)const
		{
			return{ lower_bound(key),upper_bound(key) };
		}

		std::pair<iterator, iterator> equal_range(const key_type&key)
		{
			return{ lower_bound(key),upper_bound(key) };
		}

		const_iterator find_imple(const key_type&key)const;

		const_iterator find(const key_type&key)const
		{
			return find_imple(key);
		}

		iterator find(const key_type&key)
		{
			return (iterator&)find_imple(key);
		}

		iterator rbt_insert_equal(const value_type& value);

		template<typename InputIterator>
		void rbt_insert_equal(InputIterator first,
			InputIterator last)
		{
			while (first != last)rbt_insert_equal(*first++);
		}

		std::pair<iterator, bool>
			rbt_insert_unique(const value_type& value);

		template<typename InputIterator>
		void rbt_insert_unique(InputIterator first,
			InputIterator last)
		{
			while (first != last)rbt_insert_unique(*first++);
		}

	protected:
			void print(link_type rt, int counter_hight = 0)//调试用
			{
				if (rt != nil) {
					print(rt->right, counter_hight + 1);
					for (auto n = counter_hight; n != -1; --n)
						cout << "\t";
					cout << rt->value;
					rt->color ? cout << "|黑" << endl : cout << "|红" << endl;
					print(rt->left, counter_hight + 1);
					cout << endl;
				}
			}
	protected:
		void destroy_node(link_type x) 
		{ 
			delete x; 
		}

		void rbt_delete(link_type tar);
		link_type get_root()const
		{
			return root; 
		}

		link_type get_nil()const
		{ 
			return nil; 
		}

		static link_type minimum(link_type rt)
		{ 
			return rb_tree_node<value_type>::minimum(rt);
		}

		static link_type maximum(link_type rt)
		{ 
			return rb_tree_node<value_type>::maximum(rt);
		}

		void rbt_left_rotate(link_type x);
		void rbt_right_rotate(link_type x);
		void rbt_destroy(link_type x);
		void rbt_delete_fixup(link_type x);
		iterator rbt_insert_fixup(link_type tar);
		void copy_assign(const link_type cur)
		{
			if (cur->left)
			{ 
				rbt_insert_equal(cur->value);
				copy_assign(cur->left);
				copy_assign(cur->right);
			}
		}
		void rbt_transplant(link_type lhs, link_type rhs)
		{
			if (lhs->p == nil)
				root = rhs;
			else if (lhs == lhs->p->left)
				lhs->p->left = rhs;
			else
				lhs->p->right = rhs;
			rhs->p = lhs->p;
		}
	private:
		link_type nil;
		link_type root;
	    Compare comp;
		size_type node_count;
	};

	template<typename key_type, typename value_type,typename keyOfValue, typename Compare = std::less<key_type>>
	void rb_tree<key_type,value_type,keyOfValue, Compare>::
		rbt_destroy(link_type x)
	{
		if (x != nil)
		{
			rbt_destroy(x->left);
			rbt_destroy(x->right);
			destroy_node(x);
		}
	}

	template<typename key_type, typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	typename rb_tree<key_type, value_type,keyOfValue, Compare>::const_iterator
		rb_tree<key_type,value_type,keyOfValue, Compare>::
		lower_bound_imple(const key_type&key)const
	{
		auto cur = get_root();
		auto result = cur;
		while (cur != nil)
		{
			result = cur;
			if (comp(get_key()(cur->value), key))
				cur = cur->right;
			else
				cur = cur->left;
		}
		if (result == nil)
			return cend();
		else if (comp(get_key()(result->value), key))
			return ++const_iterator(result);
		else
			return const_iterator(result);
	}

	template<typename key_type, typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	typename rb_tree<key_type, value_type,keyOfValue, Compare>::const_iterator
		rb_tree<key_type,value_type,keyOfValue, Compare>::
		upper_bound_imple(const key_type&key)const
	{
		auto cur = get_root();
		auto result = cur;
		while (cur != nil)
		{
			result = cur;
			if (comp(key, get_key()(cur->value)))
				cur = cur->left;
			else
				cur = cur->right;
		}
		if (result == nil)
			return cend();
		else if (comp(key, get_key()(result->value)))
			return const_iterator(result);
		else
			return ++const_iterator(result);
	}


	template<typename key_type, typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	typename rb_tree<key_type, value_type,keyOfValue, Compare>::const_iterator
		rb_tree<key_type,value_type,keyOfValue, Compare>::
	find_imple(const key_type&key)const
	{
		auto cur = get_root();
		while (cur != nil)
		{
			if (comp(key, get_key()(cur->value)))
				cur = cur->left;
			else if (comp(get_key()(cur->value), key))
				cur = cur->right;
			else
				return const_iterator(cur);
		}
		return cend();
	}


	template<typename key_type, typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	typename rb_tree<key_type, value_type,keyOfValue, Compare>::iterator
		rb_tree<key_type,value_type,keyOfValue, Compare>::
		rbt_insert_equal(const value_type& value)
	{
		auto cur = get_root();
		auto par = nil;
		while (cur != nil)
		{
			par = cur;
			cur = comp(get_key()(value), get_key()(par->value)) ? par->left : par->right;
		}
		auto tar = new rb_tree_node<value_type>(value, rbt_red, par, nil, nil);
		if (par == nil)
			root = tar;
		else if (comp(get_key()(value), get_key()(par->value)))
			par->left = tar;
		else
			par->right = tar;

		return rbt_insert_fixup(tar);
	}

	template<typename key_type,typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
		std::pair<typename rb_tree<key_type,value_type,keyOfValue, Compare>::iterator, bool>
		rb_tree<key_type,value_type,keyOfValue, Compare>::
		rbt_insert_unique(const value_type& value)
	{
		auto cur = get_root();
		auto par = nil;
		while (cur != nil)
		{
			par = cur;
			if (comp(get_key()(value), get_key()(cur->value)))
				cur = cur->left;
			else if (comp(get_key()(cur->value), get_key()(value)))
				cur = cur->right;
			else
				return
				std::pair<iterator, bool>(iterator(cur), false);
		}
		auto tar = new rb_tree_node < value_type >(value, rbt_red, par, nil, nil);
		if (par == nil)
			root = tar;
		else if (comp(get_key()(value), get_key()(par->value)))
			par->left = tar;
		else
			par->right = tar;
		return
			std::pair<iterator, bool>(rbt_insert_fixup(tar), true);
	}

	template<typename key_type,typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	typename rb_tree<key_type,value_type,keyOfValue, Compare>::iterator
		rb_tree<key_type,value_type,keyOfValue, Compare>::
		rbt_insert_fixup(link_type tar)
	{
		auto iter = iterator(tar);
		while (!tar->p->color) 
		{
			auto grandpar = tar->p->p;
			if (tar->p == grandpar->left)
			{
				if (!grandpar->right->color) 
				{
					grandpar->left->color = rbt_black;
					grandpar->right->color = rbt_black;
					grandpar->color = rbt_red;
					tar = grandpar;
				}
				else {
					if (tar == tar->p->right)
					{
						rbt_left_rotate(tar->p);
						tar = tar->left;
					}
					grandpar->color = rbt_red;
					grandpar->left->color = rbt_black;
					rbt_right_rotate(grandpar);
				}
			}
			else {
				if (!grandpar->left->color) 
				{
					grandpar->right->color = rbt_black;
					grandpar->left->color = rbt_black;
					grandpar->color = rbt_red;
					tar = grandpar;
				}
				else {
					if (tar == tar->p->left)
					{
						rbt_right_rotate(tar->p);
						tar = tar->right;
					}
					grandpar->color = rbt_red;
					grandpar->right->color = rbt_black;
					rbt_left_rotate(grandpar);
				}
			}
		}
		root->color = rbt_black;
		++node_count;
		nil->p = root;
		return iter;
	}


	template<typename key_type,typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	void rb_tree<key_type,value_type,keyOfValue,Compare>::
		rbt_delete(link_type tar)
	{//删除节点
		if (tar == nil)
			return;
		link_type y = tar;
		rbt_color_type y_original_color = y->color;
		link_type x;
		if (tar->left == nil) 
		{
			x = tar->right;
			rbt_transplant(tar, x);
		}
		else if (tar->right == nil)
		{
			x = tar->left;
			rbt_transplant(tar, x);
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
				rbt_transplant(y, x);
				y->right = tar->right;
				y->right->p = y;
			}
			rbt_transplant(tar, y);
			y->left = tar->left;
			y->left->p = y;
			y->color = tar->color;
		}

		--node_count;
		if (y_original_color)
			rbt_delete_fixup(x);
		destroy_node(tar);
		nil->p = root;
	}

	template<typename key_type,typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	void rb_tree<key_type,value_type,keyOfValue, Compare>::
		rbt_delete_fixup(link_type x) 
	{
		while (x != root&&x->color) 
		{
			if (x == x->p->left)
			{
				link_type w = x->p->right;
				if (!w->color) 
				{//情况1，如果兄弟节点是红色
					x->p->color = rbt_red;
					w->color = rbt_black;
					rbt_left_rotate(x->p);
					w = x->p->right;
				}

				if (w->right->color&&w->left->color) 
				{//情况二，兄弟节点的两个子节点都是黑色
					w->color = rbt_red;
					x = x->p;
				}
				else if (w->right->color) 
				{//兄弟节点的左节点为红色
					w->color = rbt_red;
					w->left->color = rbt_black;
					rbt_right_rotate(w);
					w = x->p->right;
				}
				if (!w->right->color) 
				{//兄弟节点的右节点为红色
					w->color = x->p->color;
					x->p->color = rbt_black;
					w->right->color = rbt_black;
					rbt_left_rotate(x->p);
					x = root;
				}
			}
			else 
			{
				link_type  w = x->p->left;
				if (!w->color) 
				{
					x->p->color = rbt_red;
					w->color = rbt_black;
					rbt_right_rotate(x->p);
					w = x->p->left;
				}
				if (w->right->color&&w->left->color) 
				{
					w->color = rbt_red;
					x = x->p;
				}
				else if (w->left->color) 
				{
					w->color = rbt_red;
					w->right->color = rbt_black;
					rbt_left_rotate(w);
					w = x->p->left;
				}
				if (!w->left->color) 
				{
					w->color = x->p->color;
					x->p->color = rbt_black;
					w->left->color = rbt_black;
					rbt_right_rotate(x->p);
					x = root;
				}
			}
		}
		x->color = rbt_black;
	}

	template<typename key_type,typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	void rb_tree<key_type,value_type,keyOfValue, Compare> ::
		rbt_left_rotate(link_type x)
	{//左旋
		auto xRight = x->right;
		xRight->p = x->p;

		if (xRight->left != nil)
			xRight->left->p = x;
		x->right = xRight->left;

		if (x->p == nil)
			root = xRight;
		else if (x->p->left == x)
			x->p->left = xRight;
		else
			x->p->right = xRight;
		xRight->left = x;
		x->p = xRight;
	}

	template<typename key_type,typename value_type, typename keyOfValue, typename Compare = std::less<key_type>>
	void rb_tree<key_type,value_type,keyOfValue, Compare>::
		rbt_right_rotate(link_type x)
	{//右旋
		auto xLeft = x->left;
		xLeft->p = x->p;

		if (xLeft->right != nil)
			xLeft->right->p = x;
		x->left = xLeft->right;

		if (x->p == nil)
			root = xLeft;
		else if (x->p->left == x)
			x->p->left = xLeft;
		else
			x->p->right = xLeft;
		xLeft->right = x;
		x->p = xLeft;
	}

}

#endif