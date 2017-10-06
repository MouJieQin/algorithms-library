#ifndef _GRAPH_
#define _GRAPH_

#include<vector>

namespace qmj
{
	struct adjList_unweighted_graph_vertex_node
	{
		typedef adjList_unweighted_graph_vertex_node* link_type;

		adjList_unweighted_graph_vertex_node(const size_t key = 0,
			const link_type next = nullptr) :key(key), next(next) {}

		size_t key;
		link_type next;
	};

	template<typename weight_type>
	struct adjList_weighted_graph_vertex_node
	{
		typedef adjList_weighted_graph_vertex_node<weight_type>* link_type;

		adjList_weighted_graph_vertex_node(
			const size_t k = 0, const weight_type& w = weight_type(),
			const link_type n = nullptr) :
			key(k), weight(w), next(n) {}

		size_t key;
		weight_type weight;
		link_type next;
	};
}

namespace qmj
{
	template<typename weight_type,
		typename graph_node_link,
		typename Comp = std::less<weight_type>>
		struct less_weight_type
	{
		bool operator()(const graph_node_link x, const graph_node_link y)const
		{
			return Comp()(x->weight, y->weight);
		}
	};
}

namespace qmj
{
	template<typename weight_type>
	struct graph_edge
	{
		typedef weight_type weight_type;

		template<typename weight_type, typename Comp = std::less<weight_type>>
		friend bool operator<(const graph_edge<weight_type>&lhs, const graph_edge<weight_type>&rhs);

		graph_edge(size_t u = 0, size_t v = 0, weight_type weight = weight_type()) :
			u(u), v(v), weight(weight) {}

		size_t u;
		size_t v;
		weight_type weight;
	};

	template<typename weight_type, typename Comp = std::less<weight_type>>
	bool operator<(const qmj::graph_edge<weight_type>&lhs, const qmj::graph_edge<weight_type>&rhs)
	{
		return Comp()(lhs.weight, rhs.weight);
	}
}

#endif