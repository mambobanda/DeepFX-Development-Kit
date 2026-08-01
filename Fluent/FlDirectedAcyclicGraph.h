/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2020 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <unordered_map>
#include <stack>
#include "FlDataObject.h"

namespace fl {

	template<typename VertexId, typename VertexUserData>
	class DirectedAcyclicGraph {
	public:
		class Vertex {
		public:
			size_t index;
			VertexId id;
			std::string description;
			std::string path;
			DataObjectStaticMethod staticMethod;
			VertexUserData *userData;
			std::vector<Vertex*> edges;
		};
	private:
		std::vector<Vertex*> vertices;

		void topologicalSortUtil(Vertex *v, std::vector<std::pair<VertexId, bool>> &visited, std::stack<Vertex*> &Stack);
		bool isCyclicUtil(Vertex *v, std::vector<std::pair<VertexId, bool>> &visited, std::vector<std::pair<VertexId, bool>> &rs);
	public:

		DirectedAcyclicGraph();
		virtual ~DirectedAcyclicGraph();

		void addVertex(Vertex *);
		void addEdge(VertexId, Vertex*);
		void addEdge(Vertex*, Vertex*);

		std::vector<DirectedAcyclicGraph::Vertex*> &allVertices();

		std::vector<DirectedAcyclicGraph::Vertex*> topologicalSort();
		bool isCyclic();

		size_t vertexCount();
	};

	template<typename VertexId, typename VertexUserData>
	DirectedAcyclicGraph<VertexId, VertexUserData>::DirectedAcyclicGraph() {

	}

	template<typename VertexId, typename VertexUserData>
	DirectedAcyclicGraph<VertexId, VertexUserData>::~DirectedAcyclicGraph() {

	}

	template<typename VertexId, typename VertexUserData>
	size_t DirectedAcyclicGraph<VertexId, VertexUserData>::vertexCount() {
		return vertices.size();
	}

	template<typename VertexId, typename VertexUserData>
	void DirectedAcyclicGraph<VertexId, VertexUserData>::addVertex(Vertex *vt) {

		for (auto vert : vertices) if (vt->id == vert->id) return;

		size_t index = vertices.size();
		vt->index = index;
		vertices.push_back(vt);
	}

	template<typename VertexId, typename VertexUserData>
	void DirectedAcyclicGraph<VertexId, VertexUserData>::addEdge(VertexId id, Vertex *vt) {

		for (auto vert : vertices) {
			if (vert->id == id) {
				vert->edges.push_back(vt);
			}
		}

		for (auto vert : vertices) {
			if (vert->id == id) {
				for (auto e : vert->edges)
					if (e->id == vt->id)
						return;

				vert->edges.push_back(vt);
			}
		}
	}

	template<typename VertexId, typename VertexUserData>
	void DirectedAcyclicGraph<VertexId, VertexUserData>::addEdge(Vertex* a, Vertex* b) {

		for (auto e : a->edges) if (e->id == b->id) return;
		a->edges.push_back(b);
	}

	template<typename VertexId, typename VertexUserData>
	void DirectedAcyclicGraph<VertexId, VertexUserData>::topologicalSortUtil(Vertex *v, std::vector<std::pair<VertexId, bool>> &visited, std::stack<Vertex*> &Stack) {

		visited[v->index].second = true;

		std::vector<Vertex*>::template iterator i;
		for (i = v->edges.begin(); i != v->edges.end(); ++i) {
			if (!visited[(*i)->index].second) topologicalSortUtil(*i, visited, Stack);
		}

		Stack.push(v);
	}

	template<typename VertexId, typename VertexUserData>
	std::vector<typename  DirectedAcyclicGraph<VertexId, VertexUserData>::Vertex*> DirectedAcyclicGraph<VertexId, VertexUserData>::topologicalSort() {
		std::vector<Vertex*> output;
		std::stack<Vertex*> Stack;
		std::vector<std::pair<VertexId, bool>> visited;

		for (int i = 0; i < vertices.size(); i++) {
			std::pair<VertexId, bool> p(vertices[i]->id, false);
			visited.push_back(p);
		}

		for (int i = 0; i < vertices.size(); i++)
			if (visited[i].second == false) topologicalSortUtil(vertices[i], visited, Stack);

		while (!Stack.empty()) {
			output.push_back(Stack.top());
			Stack.pop();
		}

		return output;
	}

	template<typename VertexId, typename VertexUserData>
	bool DirectedAcyclicGraph<VertexId, VertexUserData>::isCyclic() {
		std::vector<std::pair<VertexId, bool>> visited;
		std::vector<std::pair<VertexId, bool>> recStack;

		for (int i = 0; i < vertices.size(); i++) {
			std::pair<VertexId, bool> p(vertices[i]->id, false);
			visited.push_back(p);
			recStack.push_back(p);
		}

		for (int i = 0; i < vertices.size(); i++)
			if (isCyclicUtil(vertices[i], visited, recStack))
				return true;

		return false;
	}

	template<typename VertexId, typename VertexUserData>
	bool DirectedAcyclicGraph<VertexId, VertexUserData>::isCyclicUtil(Vertex *v, std::vector<std::pair<VertexId, bool>> &visited, std::vector<std::pair<VertexId, bool>> &recStack) {

		if (visited[v->index].second == false) {

			visited[v->index].second = true;
			recStack[v->index].second = true;

			std::vector<Vertex*>::template iterator i;
			for (i = v->edges.begin(); i != v->edges.end(); ++i)
			{
				if (!visited[(*i)->index].second && isCyclicUtil(*i, visited, recStack))
					return true;
				else if (recStack[(*i)->index].second)
					return true;
			}

		}

		recStack[v->index].second = false;

		return false;
	}

	template<typename VertexId, typename VertexUserData>
	std::vector<typename  DirectedAcyclicGraph<VertexId, VertexUserData>::Vertex*> &DirectedAcyclicGraph<VertexId, VertexUserData>::allVertices() {
		return vertices;
	}
}