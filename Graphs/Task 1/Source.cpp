#include<iostream>

class CGraph {
public:
	CGraph();
	CGraph(int vertexes, int edges);
	~CGraph();

	void readMatrix(int vertexes);
	void printSearchRoads();

private:
	int searchRoads();
	void createMatr();

	int** _matrix;
	int _vertexes;
	int _edges;

	void deleteAll();
	void deleteMatr();
};

int main(int argc, char* argv[])
{
	int v = 0;
	std::cin >> v;

	CGraph roads;
	roads.readMatrix(v);
	roads.printSearchRoads();

	return EXIT_SUCCESS;
}

CGraph::CGraph() : _vertexes(0), _edges(0), _matrix(nullptr)
{
}

CGraph::CGraph(int vertexes, int edges) : _vertexes(vertexes), _edges(edges), _matrix(nullptr)
{
}

CGraph::~CGraph()
{
	deleteAll();
}

void CGraph::createMatr()
{
	if (_vertexes == 0)
	{
		return;
	}

	_matrix = new int* [_vertexes];
	for (int i = 0; i < _vertexes; i++)
	{
		_matrix[i] = new int[_vertexes] { 0 };
	}
}

void CGraph::deleteAll()
{
	deleteMatr();
}

void CGraph::readMatrix(int vertexes)
{
	_vertexes = vertexes;
	createMatr();
	for (int i = 0; i < _vertexes; ++i)
	{
		for (int j = 0; j < _vertexes; ++j)
		{
			std::cin >> _matrix[i][j];
		}
	}
	std::cout << std::endl;
}

int CGraph::searchRoads()
{
	int count = 0;
	for (int i = 0; i < _vertexes; ++i)
	{
		for (int j = 0; j < _vertexes; j++)
		{
			if (_matrix[i][j] == 1)
			{
				++count;
			}
		}
	}

	return (count / 2);
}

void CGraph::printSearchRoads()
{
	std::cout << searchRoads() << std::endl;
}

void CGraph::deleteMatr()
{
	if (_matrix != nullptr)
	{
		for (int i = 0; i < _vertexes; ++i)
		{
			delete[] _matrix[i];
		}
		delete[] _matrix;
		_matrix = nullptr;
	}
}