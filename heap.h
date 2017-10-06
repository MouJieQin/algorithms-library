#ifndef _HEAP_
#define _HEAP_
#include<vector>
/*二叉堆和斐波那契堆*/
namespace qmj
{
	template<typename value_type,
		typename Compare =std::less<typename value_type>> 
		class binary_heap 
	{//二叉最大/最小堆
	public:
		typedef typename std::vector<value_type> container_type;
		typedef typename container_type::iterator iterator;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

		binary_heap() = default;
		binary_heap(iterator first, iterator last,const Compare& cmp = Compare()) :
			heap_v(first, last), comp(cmp)
		{
			build_heap();
		}
		binary_heap(const Compare& cmp) :comp(cmp){}

		binary_heap(const binary_heap&rhs):heap_v(rhs.heap_v),comp(rhs.comp){}
		binary_heap& operator=(binary_heap x)
		{
			comp = x.comp;
			heap_v.swap(x.heap_v);
			return *this;
		}

		bool empty() { return heap_v.empty(); }

		value_type top() { return heap_v[0]; }

		value_type extract_top()
		{//返回顶端元素并让其出队
			value_type top = heap_v[0];
			heap_v[0] = heap_v.back();
			heap_v.pop_back();
			heapify(0);
			return top;
		}

		void change_key(size_t index, const value_type& value)
		{//视情况(最大堆/最小堆)改变下标index的键值并上溯
			if (comp(heap_v[index],value))
				return;
			while (index && comp(value,heap_v[parent(index)])) 
			{
				heap_v[index] = heap_v[parent(index)];
				index = parent(index);
			}
			heap_v[index] = value;
		}

		void insert(const value_type&value)
		{
			heap_v.push_back(value);
			change_key(heap_v.size()-1, value);
		}

	private:
		void heapify( size_t index)
		{//将index节点下溯
			size_t priority_index = index;//具有较优节点的下标
			while (true)
			{
				auto left_index = left(index);
				auto right_index = right(index);
				if (left_index < heap_v.size() && comp(heap_v[left_index], heap_v[priority_index]))
					priority_index = left_index;
				if (right_index < heap_v.size() && comp(heap_v[right_index], heap_v[priority_index]))
					priority_index = right_index;
				if (priority_index == index)
					return;
				else
				{
					std::swap(heap_v[priority_index], heap_v[index]);
					index = priority_index;
				}
			} 
		}

		void build_heap() {
			if (heap_v.size() < 2)return;
			size_t index = parent(heap_v.size()) + 1;//有子节点的最大下标节点+1
			do {
				index -= 1;
				heapify(index);
			} while (index != 0);
		}

	private:
		//返回左右孩子或父节点下标
		size_t left(const size_t index) { return (index << 1) + 1; }
		size_t right(const size_t index) { return (index << 1) + 2; }
		size_t parent(const size_t index) { return (index - 1) >> 1; }
	private:
		std::vector<value_type>heap_v;
		Compare comp;
	};
}

namespace qmj
{
	template<typename value_type>
	struct fib_heap_node
	{
		typedef value_type value_type;
		typedef fib_heap_node<value_type>* link_type;

		fib_heap_node(const value_type& k, bool m = false, 
			size_t d = 0, link_type par = nullptr,
			link_type lef = nullptr, link_type righ = nullptr, link_type ch = nullptr) :
			key(k), mark(m), degree(d), p(par), left(lef), right(righ), child(ch) {}

		value_type key;
		size_t degree;
		link_type p;
		link_type left;
		link_type right;
		link_type child;
		bool mark;
	};


	template<typename value_type,
		typename Compare = std::less<typename value_type>>
		class fib_heap
	{//斐波那契最大/最小堆
	public:
		typedef value_type value_type;
		typedef typename std::vector<value_type> container_type;
		typedef typename container_type::size_type size_type;
		typedef fib_heap_node<value_type>* link_type;
		//typedef fib_heap_node* link_type;
		typedef fib_heap<value_type,Compare> self;

		fib_heap(Compare comp=Compare()) :n(0), header(nullptr),comp(comp) {}

		template<typename FowardIterator>
		fib_heap(FowardIterator first, FowardIterator last,
			Compare comp = Compare()) :fib_heap(comp)
		{
			while (first != last)insert(*first++);
		}
		template<typename FowardIterator>
		fib_heap(FowardIterator first, FowardIterator last,
			std::vector<link_type>&map,//这种映射堆中图中指针的方式给图算法使用
			Compare comp = Compare()) : fib_heap(comp)
		{
			while (first != last)insert(*first++,map);
		}

		~fib_heap(){destroy_heap(header);}

		bool empty() { return n == 0; }
		link_type top() { return header; }

		void insert(const value_type& key) 
		{
			link_type tar = new fib_heap_node<value_type>(key);
			insert_imple(tar);
		}

		void insert(const value_type&key, std::vector<link_type>&map)
		{
			link_type tar = new fib_heap_node<value_type>(key);
			map.push_back(tar);
			insert_imple(tar);
		}

		void fib_union(self &lhs, self &rhs)
		{
			if (!lhs.header) 
			{
				header = rhs.header;
				n = rhs.n;
			}
			else if (!rhs.header) 
			{
				header = lhs.header;
				n = lhs.n;
			}
			else 
			{
				auto lhs_next = lhs.header->right;
				auto rhs_next = rhs.header->right;

				lhs.header->right = rhs_next;
				rhs.header->right = lhs_next;

				rhs_next->left = lhs.header;
				lhs_next->left = rhs.header;

				n = rhs.n + lhs.n;
				header = comp(lhs.header->key, rhs.header->key) ?
					lhs.header : rhs.header;
			}

			rhs.header = nullptr;
			lhs.header = nullptr;
			rhs.n = 0;
			lhs.n = 0;
		}

		value_type extract_top() 
		{
			link_type top = header;
			if (top) 
			{
				auto ch = top->child;
				if (ch) 
				{
					do 
					{
						ch->p = nullptr;
						auto temp = ch->right;
						insert_root_list(ch);
						ch = temp;
					} while (ch != top->child);
				}
				out_of_list(top);
				if (top == top->right)
					header = nullptr;
				else 
				{
					header = header->right;
					consolidate();
				}
				n -= 1;
			}
			auto value = top->key;
			delete top;
			return value;
		}

		void change_key(link_type tar)//当元素是指针，并且已经改变该指针所指对象的值
		{
			auto k = tar->key;
			auto par = tar->p;
			if (par&&comp(k, par->key))
			{
				cut(tar);
				cascading_cut(par);
			}
			if (comp(k, header->key))
				header = tar;
		}
		
		void change_key(link_type tar,const value_type& k) 
		{
			if (comp(k, tar->key))
			{
				tar->key = k;
				auto par = tar->p;
				if (par&&comp(k,par->key)) 
				{
					cut(tar);
					cascading_cut(par);
				}
				if (comp(k, header->key))
					header = tar;
			}
		}
		

		void destroy_heap(link_type rt)
		{
			if (rt)
			{
				auto next = rt;
				do
				{
					destroy_heap(rt->child);
					auto copy = next;
					next = next->right;
					delete copy;
				} while (next != rt);
			}
		}

		void get_header() { return header; }
		void print_fib() { print(get_header()); }/////////

		void print(link_type rt, int counter = 0) //////////////////////
		{
			if (rt) 
			{
				auto next = rt;
				do 
				{
					print(next->child, counter + 1);

					cout << "\n" << endl;
					for (int n = counter; n != -1; --n)
						cout << "\t";
					cout << next->key << "|";
					next->mark ? cout << "T" : cout << "F" << endl;

					next = next->right;
				} while (next != rt);
			}
		}
		protected:
			void insert_imple(link_type tar)
			{
				if (!header)
				{
					tar->left = tar;
					tar->right = tar;
					header = tar;
				}
				else
				{
					insert_root_list(tar);
					if (comp(tar->key, header->key))
						header = tar;
				}
				n += 1;
			}
	private:
		void insert_root_list(link_type tar)
		{
			auto righ = header->right;
			header->right = tar;
			tar->left = header;
			tar->right = righ;
			righ->left = tar;
		}

		void consolidate() 
		{
			vector<link_type>vect(static_cast<size_t>(log(n) / log(1.618)) + 1, nullptr);
			auto temp = header;
			vector<link_type>v_root;
			v_root.push_back(temp);
			for (auto t = temp->right; t != header; t = t->right)
				v_root.push_back(t);
			for (auto next : v_root) 
			{
				size_t d = next->degree;
				while (vect[d]) 
				{
					if (comp(vect[d]->key, next->key))
						swap(next, vect[d]);
					fib_heap_link(next, vect[d]);
					vect[d] = nullptr;
					d += 1;
				}
				vect[d] = next;
			}
			header = nullptr;
			for (auto i : vect) 
			{
				if (i) 
				{
					if (!header) 
					{
						header = i;
						header->left = header;
						header->right = header;
					}
					else 
					{
						insert_root_list(i);
						if (i->key < header->key)
							header = i;
					}
				}
			}
		}

		void out_of_list(link_type tar)
		{
			if (tar == tar->right) 
			{
				if (tar->p)
					tar->p->child = nullptr;
				else
					header = nullptr;
				return;
			}
			else 
			{
				if (tar->p)
					tar->p->child = tar->right;
			}
			auto lef = tar->left;
			auto righ = tar->right;
			lef->right = righ;
			righ->left = lef;
		}
		void fib_heap_link(link_type par, link_type ch)
		{
			out_of_list(ch);
			auto c = par->child;
			if (!c) 
			{
				par->child = ch;
				ch->left = ch;
				ch->right = ch;
			}
			else
			{
				auto next = c->right;
				c->right = ch;
				ch->left = c;
				ch->right = next;
				next->left = ch;
			}
			ch->p = par;
			par->degree += 1;
		}

		void cut(link_type tar)
		{
			tar->p->degree -= 1;
			tar->mark = false;
			out_of_list(tar);
			tar->p = nullptr;
			insert_root_list(tar);
		}

		void cascading_cut(link_type tar)
		{
			auto par = tar->p;
			while (par)
			{
				if (!par->mark) 
					par->mark = true;
				else 
				{
					cut(tar);
					par = par->p;
				}
			}
		}
	private:
		link_type header;
		size_t n;
		Compare comp;
	};
}

#endif