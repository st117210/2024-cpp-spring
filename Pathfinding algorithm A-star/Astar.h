#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <fstream>
#include <cmath>


extern int numVertices;
extern int** Matrix;
extern int heuristics;

struct SNode
{
	std::pair<int, int> vertex;
	int cost;
	SNode* next;
	SNode* parent; // Указатель на родительскую вершину

	// Конструктор по умолчанию
	SNode();

	// Конструктор с параметрами
	SNode(std::pair<int, int> vertex, int cost = 0, SNode* next = nullptr, SNode* parent = nullptr);

	// Конструктор копирования
	SNode(const SNode& other);

	// Конструктор перемещения
	SNode(SNode&& other) noexcept;

	// Деструктор
	~SNode();
};

class CAlgorithm
{
public:
	CAlgorithm();
	CAlgorithm(SNode* node, SNode* reachable, SNode* explored);
	CAlgorithm(const CAlgorithm& other);
	CAlgorithm(CAlgorithm&& other) noexcept;
	~CAlgorithm();

	SNode* A_star(std::pair<int, int> begin, std::pair<int, int> end);
	SNode* build_workAstar();
	void deleteNode(SNode*& head);

private:
	SNode* _node;
	SNode* _reachable;
	SNode* _explored;

	SNode* choose_node(SNode*& reachable, SNode*& goal_node);
	SNode* build_path(SNode*& node);
	SNode* get_adjacent_nodes(SNode*& node);
	SNode* get_new_reachable(SNode*& node, SNode*& explored);
	void update_node(SNode*& adjacent, SNode*& node);
	int heuristic_function(SNode* node1, SNode* node2);
	int manhattan_distance(SNode* node1, SNode* node2);
	int chebyshev_distance(SNode* node1, SNode* node2);
	double euclideanDistance(SNode* node1, SNode* node2);
	int getCost(std::pair<int, int> vertex);
	void processNewReachableNodes(SNode*& reachable, SNode*& new_reachable);
	SNode* finish(SNode*& new_reachable, std::pair<int, int> begin, std::pair<int, int> end, SNode*& _node, SNode*& goal_node, SNode*& node, SNode*& explored, SNode*& reachable);

	SNode* Push_front(SNode*& head, SNode*& parent, std::pair<int, int> vertex, int cost);
	bool isInList(SNode*& head, std::pair<int, int> vertex, int cost);
	SNode* findNode(SNode*& head, std::pair<int, int> vertex, int cost);
	void removeNode(SNode*& head, std::pair<int, int> vertex, int cost);

	friend std::ostream& operator<<(std::ostream& os, SNode*& head);
};

void readMatrixFromFile(std::ifstream& inputFile, int**& matrix, int numVertices);
void initializeMatrix(int**& matrix, int numVertices, int maxValue);
void printMatrix(int** matrix, int size);
void deleteMatrix(int** matrix, int numVertices);
