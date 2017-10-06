#ifndef _GRAPH_SHORTEST_PATH_
#define _GRAPH_SHORTEST_PATH_
/*节点间的最短路径算法*/
#include<vector>

#include"graph.h"
#include"graph_FS.h"
#include"heap.h"

namespace qmj
{

	template<typename weight_type>
	struct adjList_directed_weighted_graph_source_node_bellman:
		public adjList_weighted_graph_vertex_node<weight_type>
	{
		typedef adjList_weighted_graph_vertex_node<weight_type>* link_type;
		typedef adjList_directed_weighted_graph_source_node_bellman<weight_type> self;

		adjList_directed_weighted_graph_source_node_bellman(
			const size_t k = 0, const weight_type& w = weight_type(),
			const link_type n = nullptr, const size_t pre = SIZE_MAX) :
			adjList_weighted_graph_vertex_node<weight_type>(k,w,n),
			pre(pre) {}

		size_t pre;
	};

	template<typename weight_type>
		class adjList_directed_weighted_graph_bellman
		{
		public:
			typedef adjList_directed_weighted_graph_source_node_bellman<weight_type>* link_type;
			typedef typename std::vector<link_type>::iterator iterator;

			adjList_directed_weighted_graph_bellman(const weight_type& weight_max) :
				weight_max(weight_max) {}

			adjList_directed_weighted_graph_bellman(const std::vector<graph_edge<weight_type>>&vec,
				const weight_type& weight_max) :weight_max(weight_max)
			{
				for (auto &i : vec)
					insert(i.u, i.v, i.weight);
			}
			adjList_directed_weighted_graph_bellman(
				const adjList_directed_weighted_graph_bellman<weight_type>&x):
				weight_max(x.weight_max)
			{
				copy_assign(x);
			}
			~adjList_directed_weighted_graph_bellman(){ destroy_graph(); }

			adjList_directed_weighted_graph_bellman<weight_type>&
				operator=(adjList_directed_weighted_graph_bellman<weight_type> x)
			{
				weight_max = x.weight_max;
				graph.swap(x.graph);
				return *this;
			}

			link_type operator[](const size_t index)
			{
				return graph[index];
			}

			iterator begin() { return graph.begin(); }
			iterator end() { return graph.end(); }
			iterator empty() { return graph.empty(); }

			weight_type& get_weight_max() { return weight_max; }

			void clear() { destroy_graph(); }
			void get_path(const size_t s, const size_t v, std::vector<size_t>&vec)
			{
				std::vector<size_t>tmp;
				size_t count = 0;
				get_path_imple(s, v, tmp,count);
				vec.swap(tmp);
			}

			size_t size()const
			{
				return graph.size();
			}
			void insert(const size_t u, const size_t v, const weight_type&weight)
			{
				auto max = u > v ? u : v;
				for (size_t i = graph.size(); i <= max; ++i)
					graph.push_back(new
						adjList_directed_weighted_graph_source_node_bellman<weight_type>(i,weight_max));
				auto nt = new 
					adjList_weighted_graph_vertex_node<weight_type>(
						v,weight,graph[u]->next);
				graph[u]->next = nt;
			}
			void print() {
				for (size_t i = 0; i < graph.size(); ++i) {
					cout << i << "\t" << graph[i]->pre << "\t" << graph[i]->weight << endl;
				}
				cout << "\n" << endl;
			}
		private:
			void copy_assign(const adjList_directed_weighted_graph_bellman<weight_type>&x)
			{
				for (size_t i = 0; i != x.size(); ++i)
				{
					auto y = x.graph[i];
					graph.push_back(new 
						adjList_directed_weighted_graph_source_node_bellman<weight_type>
						(i,y->weight,nullptr,y->pre));
					auto cur = x.graph[i]->next;
					while (cur)
					{
						graph[i]->next = new
							adjList_weighted_graph_vertex_node<weight_type>
							(cur->key, cur->weight,graph[i]->next);
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

			void get_path_imple(const size_t s, const size_t v,
				std::vector<size_t>&vec,size_t& count)
			{
					if (count > graph.size())
						return;
					if (s == v)
						vec.push_back(s);
					else if (graph[v]->pre == SIZE_MAX)
						return;
					else
					{
						get_path_imple(s, graph[v]->pre, vec, ++count);
						if (count <= graph.size())
							vec.push_back(v);
					}
			}
		private:
			weight_type weight_max;
			std::vector<link_type> graph;
		};

		template<typename weight_type,
			typename graph_type=
			qmj::adjList_directed_weighted_graph_bellman<weight_type>,
		typename Comp=std::less<weight_type>>
		inline
		bool graph_relax (
			graph_type&graph,
			const size_t u, const size_t v, const weight_type& weight,
			Comp cmp=Comp())
		{
			if (cmp(graph[u]->weight, graph.get_weight_max()))
			{
				auto weight_new = graph[u]->weight + weight;
				if (cmp(weight_new,graph[v]->weight))
				{
					graph[v]->weight = weight_new;
					graph[v]->pre = u;
					return true;
				}
			}
			return false;
		}

		template<typename weight_type,
			typename graph_type = adjList_directed_weighted_graph_bellman<weight_type>
		,typename Comp=std::less<weight_type>>
			bool bellman_ford(//任意有向权重图的单源节点的最短路径
				graph_type&graph,//也可用于求解差分约束
				const size_t start,
				Comp cmp=Comp())
		{
			graph[start]->weight = 0;
			for (size_t i = 1; i != graph.size(); ++i)
				for (size_t j = 0; j != graph.size(); ++j) 
				{
					auto cur = graph[j]->next;
					while (cur) 
					{
						qmj::graph_relax<weight_type>
							(graph,j, cur->key, cur->weight,cmp);
						cur = cur->next;
					}
				}
			for (size_t i = 0; i != graph.size(); ++i)
			{//测试是否有负环
				auto cur = graph[i]->next;
				while (cur)
				{
					if (cmp(graph[i]->weight, graph.get_weight_max()))
						if (cmp(graph[i]->weight + cur->weight, graph[cur->key]->weight))
							return false;
					cur = cur->next;
				}
			}
			return true;
		}
}

namespace qmj
{
	template<typename weight_type,
		typename graph_type =
		adjList_directed_weighted_graph_bellman<weight_type>>
	void negative_loop(
		graph_type&graph,
		size_t k, const weight_type& weight_min)
	{
		const size_t n = graph.size();
		size_t count = 0;
		size_t last = k;
		do {
			graph[k]->weight = weight_min;
			k = graph[k]->pre;
		} while (++count <= n&&k != last);
	}

	template<typename weight_type,
		typename graph_type =
		adjList_directed_weighted_graph_bellman<weight_type>,
	typename Comp=std::less<weight_type>>
	void check_negative_loop(
		graph_type&graph,
		const size_t start,
		const weight_type& weight_min,
		size_t& count,
		Comp cmp=Comp())
	{
		if (count > graph.size())
			return;
		for (size_t i = 0; i != graph.size(); ++i)
		{
			if (cmp(graph[i]->weight, graph.get_weight_max()))
			{
				if (i != start
					&&graph[i]->weight != weight_min
					&&graph[graph[i]->pre]->weight == weight_min)
				{
					graph[i]->weight = weight_min;
					check_negative_loop(graph, start, weight_min, ++count, cmp);
				}
				else
				{
					auto cur = graph[i]->next;
					while (cur)
					{
						if (cmp(graph[i]->weight + cur->weight, graph[cur->key]->weight))
						{
							negative_loop(graph, i, weight_min);
							check_negative_loop(graph, start, weight_min, ++count, cmp);
						}
						cur = cur->next;
					}
				}
			}
		}
	}

	template<typename weight_type,
		typename graph_type= 
		adjList_directed_weighted_graph_bellman<weight_type>,
	typename Comp=std::less<weight_type>>
	void bellman_ford_set(//将负环和负环能够到达的节点权重设为负无穷
		graph_type&graph,
		const size_t start,
		const weight_type& weight_min,
		Comp cmp=Comp())
	{
		graph[start]->weight = 0;
		for (size_t i = 1; i != graph.size(); ++i)
			for (size_t j = 0; j != graph.size(); ++j)
			{
				auto cur = graph[j]->next;
				while (cur)
				{
					qmj::graph_relax<weight_type,graph_type>
						(graph, j, cur->key, cur->weight,cmp);
					cur = cur->next;
				}
			}
		size_t count = 0;
		check_negative_loop<weight_type,graph_type>
			(graph,start, weight_min,count,cmp);
		
	}
}

namespace qmj
{

	template<typename weight_type>
	struct adjList_directed_weighted_graph_source_node_dag :
		public adjList_directed_weighted_graph_source_node_bellman<weight_type>
	{
		typedef adjList_weighted_graph_vertex_node<weight_type>* link_type;
		typedef adjList_directed_weighted_graph_source_node_dag<weight_type> self;

		adjList_directed_weighted_graph_source_node_dag(
			const size_t k = 0, const weight_type& w = weight_type(),
			const link_type n = nullptr, const size_t pre = SIZE_MAX,
			const size_t indegree=0,bool visited=false) :
			adjList_directed_weighted_graph_source_node_bellman<weight_type>(k, w, n,pre),
			visited(visited),indegree(indegree) {}

		size_t indegree;
		bool visited;
	};

	template<typename weight_type>
	class adjList_directed_weighted_graph_dag
	{
	public:
		typedef adjList_directed_weighted_graph_source_node_dag<weight_type>* link_type;
		typedef typename std::vector<link_type>::iterator iterator;
		typedef adjList_directed_weighted_graph_dag<weight_type> self;

		adjList_directed_weighted_graph_dag(const weight_type& weight_max) :
			weight_max(weight_max) {}

		adjList_directed_weighted_graph_dag(const std::vector<graph_edge<weight_type>>&vec,
			const weight_type& weight_max) :weight_max(weight_max)
		{
			for (auto &i : vec)
				insert(i.u, i.v, i.weight);
		}
		adjList_directed_weighted_graph_dag(
			const adjList_directed_weighted_graph_dag<weight_type>&x) :
			weight_max(x.weight_max)
		{
			copy_assign(x);
		}
		~adjList_directed_weighted_graph_dag() { destroy_graph(); }

		adjList_directed_weighted_graph_dag<weight_type>&
			operator=( adjList_directed_weighted_graph_dag<weight_type>x)
		{
			weight_max = x.weight_max;
			graph.swap(x.graph);
			return *this;
		}

		link_type operator[](const size_t index)
		{
			return graph[index];
		}
		
		iterator begin() { return graph.begin(); }
		iterator end() { return graph.end(); }

		weight_type& get_weight_max() { return weight_max; }

		void clear() { destroy_graph(); }
		void get_path(const size_t s, const size_t v, std::vector<size_t>&vec)
		{
			std::vector<size_t>tmp;
			size_t count = 0;
			get_path_imple(s, v, tmp, count);
			vec.swap(tmp);
		}

		size_t size()const
		{
			return graph.size();
		}
		void insert(const size_t u, const size_t v, const weight_type&weight)
		{
			auto max = u > v ? u : v;
			for (size_t i = graph.size(); i <= max; ++i)
				graph.push_back(new
					adjList_directed_weighted_graph_source_node_dag<weight_type>(i, weight_max));
			auto nt = new
				adjList_weighted_graph_vertex_node<weight_type>(
					v, weight, graph[u]->next);
			graph[u]->next = nt;
		}
		void print() {
			for (size_t i = 0; i < graph.size(); ++i) 
				cout << i << "\t" << graph[i]->pre << "\t" << graph[i]->weight << endl;
			cout << "\n" << endl;
		}
	private:
		void copy_assign(const adjList_directed_weighted_graph_dag<weight_type>&x)
		{
			size_t s = x.size();
			for (size_t i = 0; i != x.size(); ++i)
			{
				auto y = x.graph[i];
				graph.push_back(new
					adjList_directed_weighted_graph_source_node_dag<weight_type>
					(i,y->weight,nullptr,y->pre,y->visited));
				auto cur = x.graph[i]->next;
				while (cur)
				{
					graph[i]->next = new
						adjList_weighted_graph_vertex_node<weight_type>
						(cur->key, cur->weight,graph[i]->next);
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

		void get_path_imple(const size_t s, const size_t v,
			std::vector<size_t>&vec, size_t& count)
		{
			if (count > graph.size())
				return;
			if (s == v)
				vec.push_back(s);
			else if (graph[v]->pre == SIZE_MAX)
				return;
			else
			{
				get_path_imple(s, graph[v]->pre, vec, ++count);
				if (count <= graph.size())
					vec.push_back(v);
			}
		}
	private:
		weight_type weight_max;
		std::vector<link_type> graph;
	};

	template<typename weight_type,
		typename graph_type=qmj::adjList_directed_weighted_graph_dag<weight_type>,
	typename Comp=std::less<weight_type>>
	bool dag_shortest_paths(graph_type&graph, const size_t start,
		Comp cmp=Comp())
	{//有向无环权重图的最短路径，返回是否是有环图V+E
		std::forward_list<size_t>top_sort;
		if(!qmj::topological_sort<graph_type>(graph, top_sort))
			return false;

		graph[start]->weight = 0;
		for (auto i : top_sort) 
		{
			auto cur = graph[i]->next;
			while (cur)
			{
				qmj::graph_relax<weight_type,graph_type>
					(graph,i, cur->key, cur->weight,cmp);
				cur = cur->next;
			}
		}
		return true;
	}
}

namespace qmj
{
	template<typename weight_type,
		typename graph_type= qmj::adjList_directed_weighted_graph_dag<weight_type>,
		typename Comp=std::less<weight_type>,
		typename Min_heap = qmj::fib_heap<typename graph_type::link_type,
		less_weight_type<weight_type,
		typename graph_type::link_type,
		Comp>>>
	void dijkstra(graph_type&graph,const size_t start,
	Comp cmp=Comp())
	{//非负有向权重图，VlgV+E
		if (!graph.size())return;

		graph[start]->weight = 0;
		std::vector<typename Min_heap::link_type>map;
		Min_heap min_heap(graph.begin(), graph.end(), map);

		while (!min_heap.empty())
		{
			auto u = min_heap.extract_top();
			auto cur = u->next;
			while (cur) 
			{
				if (qmj::graph_relax<weight_type, graph_type>
					(graph, u->key, cur->key, cur->weight,cmp))
					min_heap.change_key(map[cur->key]);

				cur = cur->next;
			}
		}
	}
}

#endif