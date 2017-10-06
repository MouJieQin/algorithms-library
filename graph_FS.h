#ifndef _GRAPH_FS_
#define _GRAPH_FS_
#include<vector>
#include<forward_list>
#include<set>
#include<utility>

#include"graph.h"
#include"queue_qmj.h"
#include"stack_qmj.h"

/*
adjList_undirected_unweighted_graph 
以size_t为节点（所有图算法都以size_t为节点）邻接链表无向图

BFS(adjList_undirected_unweighted_graph& graph,
std::vector<size_t>&vec, size_t s, size_t v)
广度优先搜索
接受邻接链表无向图和空的vector，调用结束后vec包含s到v的一条最短路径（包含s,v)
graph也会被改变

adjList_directed_unweighted_graph
邻接链表有向无权图

void DFS(adjList_directed_unweighted_graph&graph)
接受邻接链表有向无权重图，深度优先搜索，会改变图

template<typename graph_type,
typename Container=std::forward_list<size_t>>
bool  topological_sort(
graph_type&graph,
Container&lst)//拓扑排序
拓扑排序，结束调用后vec为拓扑排序


size_t strongly_connected_components(//强连通分量
adjList_directed_unweighted_graph&graph,
adjList_directed_unweighted_graph&graph_conect,
std::vector<std::vector<size_t>>&vec)
接受需要计算的邻接链表无向图graph，调用结束后graph_connect为以连通分量组成的新图
图的最大节点是联通分量的个数-1，
vec包含每个联通分量所包含的原图节点
*/

namespace qmj
{//BFS/广度优先搜索

		struct adjList_undirected_unweighted_graph_source_node
			:public adjList_unweighted_graph_vertex_node
	{
		typedef adjList_undirected_unweighted_graph_source_node* link_type;
		typedef adjList_unweighted_graph_vertex_node* list_link_type;

		adjList_undirected_unweighted_graph_source_node(
			const size_t k=0,
			const list_link_type n = nullptr,
			const size_t pre=SIZE_MAX,const size_t d=SIZE_MAX,
			const bool visited=false) :
			adjList_unweighted_graph_vertex_node(key,n),
			pre(SIZE_MAX), d(SIZE_MAX), visited(false) {}
			
		size_t pre;
		size_t d;
		bool visited;
	};

	struct adjList_undirected_unweighted_graph
	{
	public:
		typedef adjList_undirected_unweighted_graph_source_node* link_type;
		typedef typename std::vector<link_type>::iterator iterator;
		typedef adjList_undirected_unweighted_graph self;

		friend void BFS(adjList_undirected_unweighted_graph& graph,
			std::vector<size_t>&vec, size_t s, size_t v);

		adjList_undirected_unweighted_graph() = default;
		adjList_undirected_unweighted_graph(std::vector<std::vector<size_t>>&vec)
		{

			for (size_t i = 0; i != vec.size(); ++i)
				graph.push_back(new adjList_undirected_unweighted_graph_source_node(i));
			for (size_t i = 0; i < vec.size(); ++i)
				for (size_t j = 0; j < vec[i].size(); ++j)
					insert(i, vec[i][j]);
		}
		adjList_undirected_unweighted_graph(const self&x)
		{
			copy_assign(x);
		}
		self& operator=(self x)
		{
			graph.swap(x.graph);
			return *this;
		}
		~adjList_undirected_unweighted_graph()
		{
			destroy_graph();
		}

		iterator begin() { return graph.begin(); }
		iterator end() { return graph.end(); }

		void clear() { destroy_graph(); }
		size_t size()const
		{
			return graph.size();
		}
		void insert(const size_t s, const size_t v)
		{
			insert_imple(s, v);
			insert_imple(v, s);
		}
		void erase(const size_t s, const size_t v)
		{
			auto cur = graph[s]->next;
			if (cur)
			{
				if (cur->key == v)
					graph[s]->next = cur->next;
				else
				{
					auto last = cur;
					do
					{
						last = cur;
						cur = last->next;
					} while (cur&&cur->key != v);
					if (cur)
						last->next = cur->next;
				}
			}
			delete cur;
		}
		link_type operator[](const size_t index)
		{
			return graph[index];
		}
		void get_path(std::vector<size_t>&vec, const size_t s, const size_t v)
		{
			if (s == v)
				vec.push_back(s);
			else if (graph[v]->pre == SIZE_MAX)
				return;
			else
			{
				get_path(vec, s, graph[graph[v]->pre]->key);
				vec.push_back(v);
			}
		}
	private:
		void insert_imple(const size_t u, const size_t v)
		{
			for (size_t i = graph.size(); i <= u; ++i)
				graph.push_back(new adjList_undirected_unweighted_graph_source_node(i));
			auto node = new adjList_unweighted_graph_vertex_node(v, graph[u]->next);
			graph[u]->next = node;
		}
		void copy_assign(const adjList_undirected_unweighted_graph&x) 
		{
			for (size_t i = 0; i != x.size(); ++i)
			{
				auto y = x.graph[i];
				graph.push_back(new adjList_undirected_unweighted_graph_source_node
				(i,nullptr,y->pre,y->d,y->visited));
				auto cur = x.graph[i]->next;
				while (cur)
				{
					auto tmp = new 
						adjList_unweighted_graph_vertex_node(cur->key, graph[i]->next);
					graph[i]->next = tmp;
					cur = cur->next;
				}
			}
		}
		void destroy_ndoe(link_type x) { delete x; }
		void destroy_graph()
		{
			for (size_t i = 0; i != graph.size(); ++i)
			{
				auto cur = graph[i]->next;
				auto nt = cur;
				delete graph[i];
				while (cur)
				{
					nt = cur->next;
					delete cur;
					cur = nt;
				}
			}
			std::vector<link_type>temp;
			temp.swap(graph);
		}

	private:
		std::vector<link_type> graph;
	};

	void BFS(adjList_undirected_unweighted_graph& graph,
		std::vector<size_t>&vec,const size_t s,const size_t v)
	{
		qmj::queue<size_t>que;
		que.enqueue(s);
		graph[s]->d = 0;
		graph[s]->visited = true;

		while (!que.empty()) 
		{
			size_t vtr = que.dequeue();
			auto curr = graph[vtr]->next;

			while (curr) 
			{
				size_t k = curr->key;
				if (!graph[k]->visited) 
				{
					que.enqueue(k);
					graph[k]->visited = true;
					graph[k]->pre = graph[vtr]->key;
					graph[k]->d = graph[vtr]->d + 1;
				}
				curr = curr->next;
			}
		}
		std::vector<size_t>tmp;
		graph.get_path(tmp,s, v);
		vec.swap(tmp);
	}
}

namespace qmj
{//深度优先搜索

	struct adjList_directed_unweighted_graph_source_node:
		public adjList_unweighted_graph_vertex_node
	{
		typedef adjList_unweighted_graph_vertex_node* link_type;

		adjList_directed_unweighted_graph_source_node
		(const size_t k=0,const link_type n = nullptr, 
			const size_t d = 0, const size_t f = 0,
			const bool visited=false) :
			adjList_unweighted_graph_vertex_node(k,n),
			 d(d), f(f), visited(visited) {}

		size_t d;
		size_t f;
		bool visited;
	};

	struct adjList_directed_unweighted_graph
	{
		typedef adjList_directed_unweighted_graph_source_node* link_type;
		typedef typename std::vector<link_type>::iterator iterator;

		friend void DFS(adjList_directed_unweighted_graph&graph);

		friend void transposition_graph(adjList_directed_unweighted_graph&graph,
			adjList_directed_unweighted_graph&graph_tans);
		friend 	void dfs_visited_conect(
			adjList_directed_unweighted_graph&graph,
			qmj::stack<size_t>&stack,
			std::vector<std::vector<size_t>>&vec,
			const size_t s, bool flag);
		friend	void dfs(
			adjList_directed_unweighted_graph&graph,
			adjList_directed_unweighted_graph&graph_trans,
			qmj::stack<size_t>&stack,
			std::vector<std::vector<size_t>>&vec,
			bool flag);

		adjList_directed_unweighted_graph():time(0){}
		adjList_directed_unweighted_graph(std::vector<std::vector<size_t>>&vec):time(0)
		{
			for (size_t i = 0; i <vec.size(); ++i)
				graph.push_back(new adjList_directed_unweighted_graph_source_node(i));
			for (size_t i = 0; i < vec.size(); ++i)
				for (size_t j = 0; j < vec[i].size(); ++j)
					insert(i, vec[i][j]);
		}
		adjList_directed_unweighted_graph(const adjList_directed_unweighted_graph&x):time(x.time)
		{
			copy_assign(x);
		}
		~adjList_directed_unweighted_graph()
		{
			destroy_graph();
		}

		adjList_directed_unweighted_graph&
			operator=(adjList_directed_unweighted_graph x)
		{
			graph.swap(x.graph);
			time = x.time;
			return *this;
		}
		link_type operator[](const size_t i) 
		{
			return graph[i];
		}
		iterator begin() { return graph.begin(); }
		iterator end() { return graph.end(); }
		void swap(adjList_directed_unweighted_graph&x)
		{
			std::swap(time, x.time);
			std::swap(graph, x.graph);
		}
		size_t size()const
		{
			return graph.size();
		}
		void clear()
		{
			destroy_graph();
		}
		void erase(const size_t u, const size_t v)
		{
			auto cur = graph[u]->next;
			if (cur)
			{
				if (cur->key == v)
					graph[u]->next = cur->next;
				else
				{
					auto last = cur;
					do
					{
						last = cur;
						cur = last->next;
					} while (cur&&cur->key != v);
					if (cur)
						last->next = cur->next;
				}
			}
			delete cur;
		}
		void insert(const size_t u, const size_t v) 
		{
			auto max = u > v ? u : v;
			for (size_t i = graph.size(); i <= max; ++i)
				graph.push_back(new adjList_directed_unweighted_graph_source_node(i));
			auto t = new adjList_unweighted_graph_vertex_node(v, graph[u]->next);
			graph[u]->next = t;
		}

		void print_adjList() 
		{
			for (size_t i = 0; i < graph.size(); ++i) 
			{
				std::cout << i << " |" << graph[i]->d << "/" << graph[i]->f << "→\t";
				auto curr = graph[i]->next;
				while (curr) 
				{
					std::cout << curr->key << "→\t";
					curr = curr->next;
				}
				std::cout << std::endl;
			}
		}
	private:
		void destroy_graph()
		{
			for (size_t i = 0; i != graph.size(); ++i)
			{
				auto cur = graph[i]->next;
				delete graph[i];
				auto nt = cur;
				while (cur)
				{
					nt = cur->next;
					delete cur;
					cur = nt;
				}
			}
			std::vector<link_type>tmp;
			tmp.swap(graph);
		}
		void copy_assign(const adjList_directed_unweighted_graph&x)
		{
			for (size_t i = 0; i != x.size(); ++i)
			{
				auto y = x.graph[i];
				graph.push_back(new adjList_directed_unweighted_graph_source_node
				(i,nullptr,y->d,y->f,y->visited));
				auto cur = x.graph[i]->next;
				while (cur)
				{
					graph[i]->next = new
						adjList_unweighted_graph_vertex_node(cur->key, graph[i]->next);

					cur = cur->next;
				}
			}
		}
	protected:
		void dfs_visited(const size_t s);
	private:
		size_t time;
		std::vector<link_type>graph;
	};


	void adjList_directed_unweighted_graph::dfs_visited(const size_t s) 
	{
		time += 1;
		graph[s]->d = time;
		graph[s]->visited = true;
		auto curr = graph[s]->next;

		while (curr) 
		{
			if (!graph[curr->key]->visited)
				dfs_visited(curr->key);
			curr = curr->next;
		}
		time = time + 1;
		graph[s]->f = time;
	}

	void DFS(adjList_directed_unweighted_graph&graph)//深度优先搜索
	{
		for (size_t i = 0; i < graph.size(); ++i)
			if (!graph[i]->visited)
				graph.dfs_visited(i);
	}

}

namespace qmj
{//拓扑排序
	struct adjList_directed_unweighted_graph_source_node_top :
		public adjList_unweighted_graph_vertex_node
	{
		typedef adjList_unweighted_graph_vertex_node* link_type;

		adjList_directed_unweighted_graph_source_node_top
		(const size_t k = 0, const link_type n = nullptr,
			bool visited = false) :
			adjList_unweighted_graph_vertex_node(k, n),
			visited(visited) {}

		bool visited;
	};

	template<typename graph_type,
		typename Container=std::forward_list<size_t>>
	void dfs_top_sort(
		graph_type& graph,
		const size_t s,
		Container&lst,
		size_t& count)
	{
		graph[s]->visited = true;
		auto curr = graph[s]->next;
		while (curr)
		{
			size_t k = curr->key;
			if (graph[k]->visited)
			{
				if (graph[k]->indegree == count)
					count = 0;
			}
			else
			{
				graph[k]->indegree = count;
				dfs_top_sort(graph, k, lst,count);
			}
			curr = curr->next;
		}
		lst.push_front(s);
	}

	template<typename graph_type,
	typename Container=std::forward_list<size_t>>
	bool  topological_sort(
		graph_type&graph,
		Container&lst)//拓扑排序
	{
		Container tmp;
		size_t count = 0;
		for (size_t i = 0; i != graph.size(); ++i)
			if (!graph[i]->visited)
			{
				graph[i]->indegree = ++count;
				dfs_top_sort<graph_type, Container>(graph, i, tmp, count);
				if(!count)
					return false;
			}
		tmp.swap(lst);
		return true;
	}

}

namespace qmj
{//强连通分量
	void transposition_graph(
		adjList_directed_unweighted_graph&graph,
		adjList_directed_unweighted_graph&graph_trans) 
	{
		adjList_directed_unweighted_graph tmp;
		for (size_t i = 0; i < graph.size(); ++i)
			tmp.graph.push_back(
				new adjList_directed_unweighted_graph_source_node(i));
		for (size_t i = 0; i < graph.size(); ++i) 
		{
			auto curr = graph[i]->next;
			while (curr) 
			{
				tmp.insert(curr->key, i);
				curr = curr->next;
			}
		}
		graph_trans.swap(tmp);
	}
	void dfs_visited_conect(
		adjList_directed_unweighted_graph&graph,
		qmj::stack<size_t>&stack,
		std::vector<std::vector<size_t>>&vec,
		const size_t s, bool flag) 
	{
		graph.time += 1;
		graph[s]->d = graph.time;
		graph[s]->visited = true;

		auto curr = graph[s]->next;
		while (curr)
		{
			if (!graph[curr->key]->visited)
				dfs_visited_conect(graph, stack, vec, curr->key, flag);
			curr = curr->next;
		}

		graph.time += 1;
		graph[s]->f = graph.time;

		if (flag)
			stack.push(s);
		else
			vec.back().push_back(s);
	}

	void dfs(
		adjList_directed_unweighted_graph&graph,
		adjList_directed_unweighted_graph&graph_trans,
		qmj::stack<size_t>&stack,
		std::vector<std::vector<size_t>>&vec,
		bool flag) 
	{
		graph.time = 0;
		if (flag) 
		{
			for (size_t i = 0; i < graph.size(); ++i)
				if (!graph[i]->visited)
					dfs_visited_conect(graph, stack, vec, i, flag);
		}
		else
		{
			while (!stack.empty())
			{
				size_t top = stack.pop_top();
				if (!graph_trans[top]->visited)
				{
					vec.resize(vec.size() + 1);
					dfs_visited_conect(graph_trans, stack, vec, top, flag);
				}
			}
		}
	}

	void get_conection_graph_imple(
		adjList_directed_unweighted_graph&graph,
		adjList_directed_unweighted_graph&graph_conect, 
		std::vector<std::vector<size_t>>&vec)
	{
		std::vector<size_t>disjoint_set(graph.size());
		std::set<std::pair<size_t, size_t>>graph_edge;
		for (size_t i = 0; i != vec.size(); ++i)
			for (size_t j = 0; j != vec[i].size(); ++j)
			{
				auto index = vec[i][j];
				disjoint_set[index] = i;
			}
		for (size_t i = 0; i != graph.size(); ++i)
		{
			auto cur = graph[i]->next;
			size_t k;
			while (cur)
			{
				k = cur->key;
				if (disjoint_set[i] != disjoint_set[k])
					graph_edge.insert({ disjoint_set[i], disjoint_set[k] });
				cur = cur->next;
			}
		}

		for (auto &r : graph_edge)
			graph_conect.insert(r.first, r.second);
	}

	size_t strongly_connected_components(//强连通分量
		adjList_directed_unweighted_graph&graph,
		adjList_directed_unweighted_graph&graph_conect,
		std::vector<std::vector<size_t>>&vec)
	{
		adjList_directed_unweighted_graph graph_trans;
		qmj::stack<size_t>stack;
		std::vector<std::vector<size_t>>tmp;
		dfs(graph, graph_trans, stack, tmp, true);

		transposition_graph(graph,graph_trans);

		dfs(graph, graph_trans, stack, tmp, false);
		vec.swap(tmp);

		adjList_directed_unweighted_graph graph_tmp;
		get_conection_graph_imple(graph, graph_tmp, vec);
		graph_conect.swap(graph_tmp);

		return vec.size();
	}
}




#endif