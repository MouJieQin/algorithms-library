#ifndef _GRAPH_MST_
#define _GRAPH_MST_
/*最小生成树算法，使用斐波那契堆优化*/
#include<vector>
#include<forward_list>

#include"heap.h"
#include"graph.h"

namespace qmj
{

	template<typename weight_type>
	class edges_undirected_unweighted_graph
	{
	public:
		template<typename weight_type, typename Comp = std::less<weight_type>>
		friend void MST_kruskal(
			edges_undirected_unweighted_graph<weight_type>&graph,
			std::vector<qmj::graph_edge<weight_type>>&mst);

		edges_undirected_unweighted_graph() :max_node(0) {}
		edges_undirected_unweighted_graph(
			std::vector<graph_edge<weight_type>>&vec):max_node(0)
		{
			graph.resize(vec.size());
			size_t max;
			for (size_t i = 0; i != vec.size(); ++i)
			{
				max = vec[i].u > vec[i].v ? vec[i].u : vec[i].v;
				if (max > max_node)
					max_node = max;
				graph[i] = vec[i];
			}
		}

		size_t get_max_node()
		{
			return max_node;
		}
		size_t size()const
		{
			return graph.size();
		}
		void insert(const size_t u, const size_t v, const weight_type weight)
		{
			auto max = u > v ? u : v;
			if (max > max_node)
				max_node = max;
			graph.push_back(graph_edge(u, v, weight));
		}
	private:
		std::vector<qmj::graph_edge<weight_type>>graph;
		size_t max_node;
	};
	
	template<typename weight_type, 
		typename Comp = std::less<weight_type>>
	void MST_kruskal(//E(lgV)
		edges_undirected_unweighted_graph<weight_type>&graph,
		std::vector<qmj::graph_edge<weight_type>>&mst)
	{
		disjoint_forest_size_t disjoint_set;

		for (size_t i = 0; i <= graph.get_max_node(); ++i)
			disjoint_set.make_set(i);

		std::sort(graph.graph.begin(), graph.graph.end(),
			[](const qmj::graph_edge<weight_type>&lhs, const qmj::graph_edge<weight_type>&rhs)
		{
			return Comp()(lhs.weight, rhs.weight);
		}
		);
		
		std::vector<qmj::graph_edge<weight_type>>tmp;
		for (auto &i : graph.graph)
		{
			auto lef = disjoint_set.find_set(i.u);
			auto righ = disjoint_set.find_set(i.v);
			if (lef != righ)
			{
				disjoint_set.union_set(lef, righ);
				tmp.push_back(i);
			}
		}
		mst.swap(tmp);
	};
}

namespace qmj
{

	template<typename weight_type>
	struct adjList_undirected_weighted_graph_source_node:
		public adjList_weighted_graph_vertex_node<weight_type>
	{
		typedef adjList_weighted_graph_vertex_node<weight_type>* link_type;
		typedef adjList_undirected_weighted_graph_source_node<weight_type>* p_link_type;

		adjList_undirected_weighted_graph_source_node(const size_t key=0,
			const weight_type& weight=weight_type(),
			const link_type next=nullptr,
			const size_t pre=SIZE_MAX):
			adjList_weighted_graph_vertex_node<weight_type>(key,weight,next),
			pre(pre){}

		size_t pre;
	};

	template<typename weight_type>
	class adjList_undirected_weighted_graph
	{
	public:
		typedef adjList_undirected_weighted_graph_source_node<weight_type>* link_type;
		typedef typename std::vector<link_type>::iterator iterator;

		adjList_undirected_weighted_graph(const weight_type& weight_max) :
			weight_max(weight_max) {}
		adjList_undirected_weighted_graph
		(const adjList_undirected_weighted_graph<weight_type>&x):weight_max(x.weight_max)
		{
			copy_assign(x);
		}
		adjList_undirected_weighted_graph<weight_type>&
			operator=(adjList_undirected_weighted_graph<weight_type> x)
		{
			weight_max = x.weight_max;
			graph.swap(x.graph);
			return *this;
		}
		~adjList_undirected_weighted_graph()
		{
			destroy_graph();
		}

		adjList_undirected_weighted_graph(
			const std::vector<qmj::graph_edge<weight_type>>&vec,
			const weight_type& weight_max) :weight_max(weight_max)
		{
			for (auto &i : vec)
				insert(i.u, i.v, i.weight);
		}
		link_type operator[](const size_t index)
		{
			return graph[index];
		}
		iterator begin() { return graph.begin(); }
		iterator end() { return graph.end(); }
		weight_type& get_weight_max() { return weight_max; }
		size_t size()const
		{
			return graph.size();
		}
		void insert(const size_t u, const size_t v, 
			const weight_type& weight)
		{
			insert_imple(u, v, weight);
			insert_imple(v, u, weight);
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
		void copy_assign(const adjList_undirected_weighted_graph<weight_type>&x)
		{
			for (size_t i = 0; i != x.size(); ++i)
			{
				auto y = x.graph[i];
				graph.push_back(new
					adjList_undirected_weighted_graph_source_node<weight_type>
					(i,y->weight,nullptr,y->pre));
				auto cur = y->next;
				while (cur)
				{
					graph[i]->next = new
						adjList_weighted_graph_vertex_node<weight_type>
						(cur->key,cur->weight,graph[i]->next);
					
					cur = cur->next;
				}
			}
		}
		void insert_imple(const size_t u, const size_t v,const weight_type& weight)
		{
			for (auto i = graph.size(); i <= u; ++i)
				graph.push_back(new 
					adjList_undirected_weighted_graph_source_node<weight_type>(i,weight_max));

			auto nt = new 
				adjList_weighted_graph_vertex_node<weight_type>(v,weight,graph[u]->next);
			graph[u]->next = nt;
		}

	private:
		weight_type weight_max;
		std::vector<link_type>graph;
	};


	template<typename weight_type,
		typename graph_type=qmj::adjList_undirected_weighted_graph<weight_type>,
		typename Comp = std::less<weight_type>,
		typename Min_heap = qmj::fib_heap<typename graph_type::link_type,
			less_weight_type<weight_type,
			typename graph_type::link_type,
			Comp>>>
		void MST_prim(
			adjList_undirected_weighted_graph<weight_type>&graph,
			const size_t start = 0
		)
	{
		if (!graph.size())return;
		graph[start]->weight = 0;
		std::vector<typename Min_heap::link_type>map;
		Min_heap min_heap(graph.begin(),graph.end(), map);

		while (!min_heap.empty())
		{
			auto u = min_heap.extract_top();
			map[u->key] = nullptr;
			auto cur = u->next;
			while (cur)
			{
				auto k = cur->key;
				if (map[k] && cur->weight < graph[k]->weight)
				{
					graph[k]->pre = u->key;
					graph[k]->weight = cur->weight;
					min_heap.change_key(map[k]);
				}
				cur = cur->next;
			}
		}
		for (size_t i = 0; i != graph.size(); ++i)
			if (graph[i]->pre!=SIZE_MAX)
				std::cout << i << "\t" << graph[i]->pre << "\t" << graph[i]->weight << std::endl;
			else
				std::cout << i << "\t" << graph[i]->weight << std::endl;
	}
}


#endif