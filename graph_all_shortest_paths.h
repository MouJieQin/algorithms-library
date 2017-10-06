#ifndef _GRAPH_ALL_SHOREST_PATHS_
#define _GRAPH_ALL_SHOREST_PATHS_
/*所有节点之间的最短路径算法*/
#include<vector>

#include"graph.h"

namespace qmj
{
	template<typename weight_type>
	class matrix_directed_weighted_graph
	{
	public:

		template<typename weight_type,
			typename Comp = std::less<weight_type>>
			friend	bool have_a_nagetive_loop(
				matrix_directed_weighted_graph<weight_type>&graph,
				Comp cmp);

		template<typename weight_type,
			typename Comp = std::less<weight_type>>
			friend bool floyd_warshall
			(matrix_directed_weighted_graph<weight_type>&graph,
				Comp cmp);

		matrix_directed_weighted_graph(const weight_type&weight_max) :
			weight_max(weight_max) {}
		matrix_directed_weighted_graph(
			std::vector<qmj::graph_edge<weight_type>>&edges,
			const size_t max_node,
			const weight_type& weight_max) :weight_max(weight_max),
			graph_matrix(max_node + 1, std::vector<weight_type>(max_node + 1, weight_max)),
			precursor_matrix(max_node + 1, std::vector<size_t>(max_node + 1, SIZE_MAX))
		{
			for (size_t i = 0; i != graph_matrix.size(); ++i)
			{
				graph_matrix[i][i] = 0;
				precursor_matrix[i][i] = i;
			}

			for (auto &i : edges)
			{
				graph_matrix[i.u][i.v] = i.weight;
				precursor_matrix[i.u][i.v] = i.u;
			}
		}

		weight_type& get_weight_max() { return weight_max; }
		size_t size() { return graph_matrix.size(); }

		void print_all_pair_weight(const std::vector<std::vector<weight_type>>&vect)
		{
			for (size_t i = 0; i != vect.size(); ++i)
			{
				for (size_t j = 0; j != vect.size(); ++j)
					vect[i][j] == weight_max ? cout << "∞" << "\t" : cout << vect[i][j] << "\t";
				cout << endl;
			}
			cout << "\n\n" << endl;
		}

		weight_type get_min_weight(const size_t i, const size_t j)
		{
			return weight_matrix[i][j];
		}

		void get_shortest_path(
			std::vector<size_t>&vec,
			const size_t i, const size_t j)
		{
			std::vector<size_t>tmp;
			get_shortest_path_imple(tmp, i, j);
			vec.swap(tmp);
		}

		void print_pairs_shortest_path(const size_t i, const int j)
		{
			print_pairs_shortest_path_imple(i, j);
			cout << "\t weight: " << weight_matrix[i][j] << endl;
		}

	private:
		void get_shortest_path_imple(
			std::vector<size_t>&vec,
			const size_t i, const int j)
		{
			if (precursor_matrix[i][j] == SIZE_MAX)
				return;
			else
			{
				if (i == j)
					vec.push_back(i);
				else
				{
					get_shortest_path_imple(vec, i, precursor_matrix[i][j]);
					vec.push_back(j);
				}
			}
		}
		void print_pairs_shortest_path_imple(const size_t i, const int j)
		{
			if (precursor_matrix[i][j] == SIZE_MAX)
				cout << "No path from " << i << " to " << j << " exists" << endl;
			else
			{
				if (i == j)
					cout << i << " ";
				else
				{
					print_pairs_shortest_path_imple(i, precursor_matrix[i][j]);
					cout << " → " << j;
				}
			}
		}
	private:
		weight_type weight_max;
		std::vector<std::vector<weight_type>>graph_matrix;
		std::vector<std::vector<size_t>>precursor_matrix;
		std::vector<std::vector<weight_type>>weight_matrix;
	};

}

namespace qmj
{
	template<typename weight_type,
		typename Comp = std::less<weight_type>>
		bool have_a_nagetive_loop(
			matrix_directed_weighted_graph<weight_type>&graph,
			Comp cmp = Comp())
	{
		auto weight_max = graph.weight_max;
		std::vector<std::vector<weight_type>>&graph_matrix = graph.graph_matrix;
		std::vector<std::vector<weight_type>>&weight_matrix = graph.weight_matrix;

		for (size_t i = 0; i != graph.size(); ++i)
			for (size_t j = 0; j != graph.size(); ++j)
			{
				if (cmp(weight_matrix[i][j], weight_max))
				{
					for (size_t k = 0; k != graph.size(); ++k)
					{
						if (k != j
							&&cmp(graph_matrix[k][j], weight_max)
							&& cmp(weight_matrix[i][k], weight_max))
						{
							auto weight = weight_matrix[i][k] + graph_matrix[k][j];
							if (cmp(weight, weight_matrix[i][j]))
								return false;
						}
					}
				}
			}
		return true;
	}
}

namespace qmj
{
	template<typename weight_type,
		typename Comp=std::less<weight_type>>
	bool floyd_warshall(//所有节点对的最短路径，V^3
		matrix_directed_weighted_graph<weight_type>&graph,
		Comp cmp=Comp())
	{
		if (!graph.size())
			return false;

		std::vector<std::vector<weight_type>>shortest_paths_weight_graph(graph.graph_matrix);
		std::vector<std::vector<weight_type>>&last(shortest_paths_weight_graph);
		std::vector<std::vector<size_t>>&precursor_matrix(graph.precursor_matrix);

		const weight_type weight_max(graph.weight_max);
		const size_t n = graph.size();
		
		for (size_t k = 0; k != n; ++k)
		{
			for (size_t i = 0; i != n; ++i)
			{
				auto last_ik = last[i][k];
				if (cmp(last_ik, weight_max))
				{
					for (size_t j = 0; j != n; ++j)
					{
						auto last_kj = last[k][j];
						if (cmp(last_kj ,weight_max))
						{
							auto weight = last_ik + last_kj;
							if (cmp(weight, last[i][j]))
							{
								shortest_paths_weight_graph[i][j] = weight;
								precursor_matrix[i][j] = precursor_matrix[k][j];
							}
						}
					}
				}
			}
		}
		graph.weight_matrix.swap(shortest_paths_weight_graph);
		return have_a_nagetive_loop(graph,cmp);
	}
}

namespace qmj
{
	class matrix_directed_graph
	{
	public:
		friend void transitive_closure(matrix_directed_graph&graph);

		template<typename weight_type>
		matrix_directed_graph(
			std::vector<qmj::graph_edge<weight_type>>&edges,
			const size_t max_node):
			graph_matrix(max_node+1,std::vector<bool>(max_node+1,false))
		{
			for (size_t i = 0; i != graph_matrix.size(); ++i)
				graph_matrix[i][i] = true;
			for (auto &i : edges)
				graph_matrix[i.u][i.v] = true;
		}

		void insert(const size_t u, const size_t v)
		{
			size_t max = u > v ? u : v;
			if (graph_matrix.size() <= max)
			{
				graph_matrix.resize(max);
				for (size_t i = 0; i != graph_matrix.size(); ++i)
				{
					graph_matrix[i].resize(max,false);
					graph_matrix[i][i] = true;
				}
				graph_matrix[u][v] = true;
			}
		}
		bool have_path(const size_t u, const size_t v)
		{
			return transitive_closure[u][v];
		}
		size_t size()const { return graph_matrix.size(); }
	private:
		std::vector<std::vector<bool>>graph_matrix;
		std::vector<std::vector<bool>>transitive_closure;
	};

	void transitive_closure(matrix_directed_graph&graph)
	{//传递闭包,V^3
		const size_t n = graph.size();
		std::vector<std::vector<bool>>
			tmp_closure(graph.graph_matrix);
		for (size_t k = 0; k != n; ++k)
			for (size_t i = 0; i != n; ++i)
				for (size_t j = 0; j != n; ++j)
					tmp_closure[i][j] = tmp_closure[i][j]
					| (tmp_closure[i][k] & tmp_closure[k][j]);
		graph.transitive_closure.swap(tmp_closure);
	}
}


#endif