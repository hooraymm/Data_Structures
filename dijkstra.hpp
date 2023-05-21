
#ifndef __DIJKSTRA_H_
#define __DIJKSTRA_H_

#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>
#include <queue>
#include <optional>
#include "fheap.hpp"

// A vertex is typed as `vertex_t`. An edge is typed as `edge_t`.
using vertex_t = std::size_t;
using edge_weight_t = float;
using edge_t = std::tuple<vertex_t, vertex_t, edge_weight_t>;
using edges_t = std::vector<edge_t>;

enum class GraphType {
	UNDIRECTED,
	DIRECTED
};


class Graph {
public:
	Graph() = delete;
	Graph(const Graph&) = delete;
	Graph(Graph&&) = delete;

	// We assume that if num_vertices is V, a graph contains n vertices from 0 to V-1.
	Graph(size_t num_vertices, const edges_t& edges, GraphType type)
		: num_vertices(num_vertices), type(type) {
		this->num_vertices = num_vertices;
		graph.resize(num_vertices);

		if (type == GraphType::UNDIRECTED) {
			for (auto& edge : edges) {
				const auto& [from, to, weight] = edge;
				graph[from].emplace_back(from, to, weight);
				graph[to].emplace_back(to, from, weight);
			}
		}
		else {
			for (auto& edge : edges) {
				const auto& [from, to, weight] = edge;
				graph[from].emplace_back(from, to, weight);
			}
		}
	}

	size_t get_num_vertices() { return num_vertices; }
	std::vector<edge_t> adj_list(vertex_t v) { return graph[v]; }

private:
	size_t num_vertices;
	std::vector<std::vector<edge_t>> graph;
	GraphType type;
};

std::unordered_map<vertex_t, std::optional<std::tuple<vertex_t, edge_weight_t>>>
dijkstra_shortest_path(Graph& g, vertex_t src) {

	std::unordered_map<vertex_t, std::optional<std::tuple<vertex_t, edge_weight_t>>> M;

	// std::nullopt if vertex v is not reacheble from the source.
	for (vertex_t v = 0; v < g.get_num_vertices(); v++) M[v] = std::nullopt;

	float inf = 1e10;

	std::vector<edge_weight_t> dist(g.get_num_vertices(), inf);
	std::vector<edge_weight_t> previous(g.get_num_vertices(), -1);


	dist[src] = 0;
	previous[src] = src;
	FibonacciHeap<std::pair<edge_weight_t, vertex_t>> heap = {};
	heap.insert(std::make_pair(dist[src], src));


	while (heap.size()) {
		auto mn = heap.extract_min();
		auto u = mn.value().second;
		auto d_u = mn.value().first;

		if (d_u != dist[u])
			continue;

		const auto& adj_list = g.adj_list(u);

		for (const auto& e : adj_list) {
			const auto& [u, v, weight] = e;

			if (dist[u] + weight < dist[v]) {
				dist[v] = dist[u] + weight;
				previous[v] = u;
				heap.insert(std::make_pair(dist[v], v));
			}
		}
	}

	for (vertex_t v = 0; v < g.get_num_vertices(); v++) {
		if (previous[v] != -1) {
			M[v] = std::make_tuple(previous[v], dist[v]);
		}
	}

	return M;
}


#endif // __DIJKSTRA_H_
