#include"Astar.h"

SNode::SNode() : vertex(std::make_pair(0, 0)), cost(0), next(nullptr), parent(nullptr)
{
}

SNode::SNode(std::pair<int, int> vertex, int cost, SNode* next, SNode* parent) : vertex(vertex), cost(cost), next(next), parent(parent)
{
}

SNode::SNode(const SNode& other) : vertex(other.vertex), cost(other.cost), next(nullptr), parent(other.parent)
{
}

SNode::SNode(SNode&& other) noexcept : vertex(std::move(other.vertex)), cost(other.cost), next(other.next), parent(other.parent)
{
	other.next = nullptr;
	other.parent = nullptr;
}

SNode::~SNode()
{
	vertex = std::make_pair(-1, -1);
	next = nullptr;
	parent = nullptr;
}

SNode* CAlgorithm::Push_front(SNode*& head, SNode*& parent, std::pair<int, int> vertex, int cost)
{
	SNode* newNode = new SNode(vertex, cost);
	newNode->next = head;
	newNode->parent = parent;
	return newNode;
}

SNode* CAlgorithm::Pop_front(SNode*& head)
{
	if (head == nullptr)
	{
		return nullptr;
	}

	SNode* newHead = head->next;
	delete head;
	return newHead;
}

bool CAlgorithm::isInList(SNode*& head, std::pair<int, int> vertex, int cost)
{
	SNode* current = head;
	while (current != nullptr)
	{
		if (current->vertex == vertex && current->cost == cost)
		{
			return true;
		}
		current = current->next;
	}
	return false;
}

SNode* CAlgorithm::findNode(SNode*& head, std::pair<int, int> vertex, int cost)
{
	SNode* current = head;
	while (current != nullptr)
	{
		if (current->vertex == vertex && current->cost == cost)
		{
			return current;
		}
		current = current->next;
	}
	return nullptr;
}

void CAlgorithm::removeNode(SNode*& head, std::pair<int, int> vertex, int cost)
{
	SNode* current = head;
	SNode* previous = nullptr;

	// Ищем узел по заданным значениям координат и стоимости
	while (current != nullptr)
	{
		if (current->vertex == vertex && current->cost == cost)
		{
			// Если найденный узел - первый в списке
			if (previous == nullptr)
			{
				head = current->next;
				delete current;
				return;
			}
			else
			{
				previous->next = current->next;
				delete current;
				return;
			}
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}

}

std::ostream& operator<<(std::ostream& os, SNode*& head)
{
	if (head == nullptr)
	{
		os << "head = nullptr!";
		return os;
	}

	SNode* current = head; // Переменная current указывает на текущий элемент списка
	while (current != nullptr)
	{
		os << "(" << current->vertex.first << "," << current->vertex.second << "," << current->cost << ") "; // Вывод данных узла
		current = current->next; // Переход к следующему узлу
	}
	return os;
}

CAlgorithm::CAlgorithm() : _node(nullptr), _reachable(nullptr), _explored(nullptr) {}

CAlgorithm::CAlgorithm(SNode* node, SNode* reachable, SNode* explored) : _node(node), _reachable(reachable), _explored(explored) {}

CAlgorithm::CAlgorithm(const CAlgorithm& other) : _node(other._node), _reachable(other._reachable), _explored(other._explored) {}

CAlgorithm::CAlgorithm(CAlgorithm&& other) noexcept : _node(std::move(other._node)), _reachable(std::move(other._reachable)), _explored(std::move(other._explored)) {}

CAlgorithm::~CAlgorithm() 
{
	deleteNode(_explored);
	deleteNode(_reachable);
	deleteNode(_node);
}

SNode* CAlgorithm::A_star(std::pair<int, int> begin, std::pair<int, int> end)
{
	_explored = nullptr;
	_reachable = nullptr;

	SNode* goal_node = new SNode(end, getCost(end));
	SNode* reachable = Push_front(_reachable, _reachable, begin, getCost(begin));
	SNode* explored = _explored;

	while (reachable != nullptr)
	{

		SNode* node = choose_node(reachable, goal_node);

		_node = Push_front(_node, node->parent, node->vertex, node->cost);

		explored = Push_front(explored, node->parent, node->vertex, node->cost);

		removeNode(reachable, node->vertex, node->cost);

		SNode* new_reachable = get_new_reachable(node, explored);

		SNode* finishNode = finish(new_reachable, begin, end, _node, goal_node, node, explored, reachable);
		if (finishNode != nullptr)
		{
			return finishNode;
		}

		processNewReachableNodes(reachable, new_reachable);
	}

	return nullptr;
}

SNode* CAlgorithm::build_workAstar()
{
	SNode* workAstar = nullptr;
	SNode* current = _node;
	while (current != nullptr)
	{
		workAstar = Push_front(workAstar, current->parent, current->vertex, current->cost);
		current = current->next;
	}
	deleteNode(_node);

	return workAstar;
}

SNode* CAlgorithm::choose_node(SNode*& reachable, SNode*& goal_node)
{
	int min_cost = 9999;

	SNode* newNode = nullptr;

	SNode* current = reachable;
	while (current != nullptr)
	{

		int cost_start_to_node = (current->parent != nullptr) ? (getCost(current->parent->vertex) + getCost(current->vertex)) : getCost(current->vertex); //g
		int cost_node_to_goal = heuristic_function(current, goal_node); //h
		int total_cost = cost_start_to_node + cost_node_to_goal; //f

		if (min_cost > total_cost)
		{
			min_cost = total_cost;
			newNode = current;
		}
		current = current->next;
	}

	SNode* best_node = nullptr;
	if (newNode != nullptr)
	{
		best_node = Push_front(best_node, newNode->parent, newNode->vertex, newNode->cost);
	}

	return best_node;
}

SNode* CAlgorithm::build_path(SNode*& node)
{

	SNode* path = nullptr;
	SNode* current = node;

	while (current->parent != nullptr)
	{
		path = Push_front(path, current->parent, current->vertex, current->cost);
		current = current->parent;
	}

	return path;
}

SNode* CAlgorithm::get_adjacent_nodes(SNode*& node)
{
	SNode* adjacent_nodes = nullptr;

	// Проверяем существование вершины слева
	if (node->vertex.first - 1 >= 0 && getCost({ node->vertex.first - 1, node->vertex.second }) != 0)
	{
		int cost_left = getCost({ node->vertex.first - 1, node->vertex.second });
		adjacent_nodes = Push_front(adjacent_nodes, node, { node->vertex.first - 1, node->vertex.second }, cost_left);
	}

	// Проверяем существование вершины справа
	if (node->vertex.first + 1 < numVertices && getCost({ node->vertex.first + 1, node->vertex.second }) != 0)
	{
		int cost_right = getCost({ node->vertex.first + 1, node->vertex.second });
		adjacent_nodes = Push_front(adjacent_nodes, node, { node->vertex.first + 1, node->vertex.second }, cost_right);
	}

	// Проверяем существование вершины сверху
	if (node->vertex.second - 1 >= 0 && getCost({ node->vertex.first, node->vertex.second - 1 }) != 0)
	{
		int cost_up = getCost({ node->vertex.first, node->vertex.second - 1 });
		adjacent_nodes = Push_front(adjacent_nodes, node, { node->vertex.first, node->vertex.second - 1 }, cost_up);
	}

	// Проверяем существование вершины снизу
	if (node->vertex.second + 1 < numVertices && getCost({ node->vertex.first, node->vertex.second + 1 }))
	{
		int cost_down = getCost({ node->vertex.first, node->vertex.second + 1 });
		adjacent_nodes = Push_front(adjacent_nodes, node, { node->vertex.first, node->vertex.second + 1 }, cost_down);
	}

	return adjacent_nodes;
}

SNode* CAlgorithm::get_new_reachable(SNode*& node, SNode*& explored)
{
	SNode* new_reachable = nullptr;

	SNode* adj = get_adjacent_nodes(node);
	SNode* currentNode = adj;

	while (currentNode != nullptr)
	{
		if (!isInList(explored, currentNode->vertex, currentNode->cost) &&
			!isInList(new_reachable, currentNode->vertex, currentNode->cost))
		{
			int costN = getCost(currentNode->vertex);
			new_reachable = Push_front(new_reachable, currentNode->parent, currentNode->vertex, costN);
		}
		currentNode = currentNode->next;
	}

	return new_reachable;
}


void CAlgorithm::update_node(SNode*& adjacent, SNode*& node)
{
	int new_cost = node->cost;
	if (new_cost < adjacent->cost)
	{
		adjacent->parent = node;
		adjacent->cost = new_cost;
	}
}

int CAlgorithm::heuristic_function(SNode* node1, SNode* node2)
{
	if (heuristics == 1)
	{
		return manhattan_distance(node1, node2);
	}
	if (heuristics == 2)
	{
		return chebyshev_distance(node1, node2);
	}
	if (heuristics == 3)
	{
		return euclideanDistance(node1, node2);
	}
}

int CAlgorithm::manhattan_distance(SNode* node1, SNode* node2)
{
	return abs(node1->vertex.first - node2->vertex.first) + abs(node1->vertex.second - node2->vertex.second);
}

int CAlgorithm::chebyshev_distance(SNode* node1, SNode* node2)
{
	return std::max(std::abs(node1->vertex.first - node2->vertex.first), std::abs(node1->vertex.second - node2->vertex.second));
}

double CAlgorithm::euclideanDistance(SNode* node1, SNode* node2)
{
	int dx = abs(node1->vertex.first - node2->vertex.first);
	int dy = abs(node1->vertex.second - node2->vertex.second);
	return std::sqrt(dx * dx + dy * dy);
}

int CAlgorithm::getCost(std::pair<int, int> vertex)
{
	return Matrix[vertex.second][vertex.first];
}

void CAlgorithm::processNewReachableNodes(SNode*& reachable, SNode*& new_reachable)
{
	SNode* currentNode = new_reachable;
	while (currentNode != nullptr)
	{
		if (!isInList(reachable, currentNode->vertex, currentNode->cost))
		{
			reachable = Push_front(reachable, currentNode->parent, currentNode->vertex, currentNode->cost);
		}
		else
		{
			SNode* existingNode = findNode(reachable, currentNode->vertex, currentNode->cost);
			update_node(currentNode, reachable);
		}
		currentNode = currentNode->next;
	}
}

SNode* CAlgorithm::finish(SNode*& new_reachable, std::pair<int, int> begin, std::pair<int, int> end, SNode*& _node, SNode*& goal_node, SNode*& node, SNode*& explored, SNode*& reachable)
{
	SNode* current = new_reachable;
	while (current != nullptr)
	{
		// Проверяем, совпадают ли координаты текущей вершины с координатами конечной вершины
		if (current->vertex == end)
		{
			// Если координаты совпадают, значит, конечная вершина найдена
			_node = Push_front(_node, _node, goal_node->vertex, goal_node->cost);

			SNode* pathf = build_path(_node);
			pathf = Push_front(pathf, pathf->parent, begin, getCost(begin));
			deleteNode(goal_node);
			deleteNode(node);
			deleteNode(explored);
			deleteNode(reachable);

			return pathf;
		}
		// Переходим к следующей вершине в списке
		current = current->next;
	}

	return nullptr;
}

void CAlgorithm::deleteNode(SNode*& head)
{
	while (head != nullptr)
	{
		SNode* temp = head;
		head->parent = nullptr;
		head = head->next;
		if (head != nullptr)
		{
			head->parent = nullptr;
		}
		delete temp;
	}
}

void readMatrixFromFile(std::ifstream& inputFile, int**& matrix, int numVertices)
{
	// Выделение памяти под матрицу
	matrix = new int* [numVertices];
	for (int i = 0; i < numVertices; ++i)
	{
		matrix[i] = new int[numVertices];
	}

	// Считывание значений из файла в матрицу
	for (int i = 0; i < numVertices; ++i)
	{
		for (int j = 0; j < numVertices; ++j)
		{
			inputFile >> matrix[i][j];
		}
	}
}

void initializeMatrix(int**& matrix, int numVertices, int maxValue)
{
	// Выделение памяти под матрицу
	Matrix = new int* [numVertices];

	// Заполнение матрицы случайными числами
	for (int i = 0; i < numVertices; ++i)
	{
		Matrix[i] = new int[numVertices];
		for (int j = 0; j < numVertices; ++j)
		{
			Matrix[i][j] = rand() % maxValue;
		}
	}
}

void printMatrix(int** matrix, int size)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void deleteMatrix(int** matrix, int numVertices)
{
	for (int i = 0; i < numVertices; ++i)
	{
		delete[] Matrix[i];
	}
	delete[] Matrix;
}