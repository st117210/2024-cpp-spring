#include<iostream>

class CGraph {
public:
	CGraph();
	CGraph(int vertexes, int edges);
	~CGraph();

	void readMatrix(int vertexes);
	void readEdges(int edges);
	void fillingMatr(int vertexes);
	void printSearchRoads();
	void PrintMatrix();
	void trLights(int vertexes);

private:

	int searchRoads();
	void createMatr();
	void createEdges();

	int** _matrix;
	int _vertexes;
	int _edges;
	int** _edgesMatr;

	void deleteAll();
	void deleteMatr();
	void deleteEdges();
};

int main(int argc, char* argv[])
{
	int e = 0;
	int v = 0;
	std::cin >> v;
	std::cin >> e;

	CGraph trafficlights;
	trafficlights.readEdges(e);
	trafficlights.trLights(v);

	return EXIT_SUCCESS;
}

CGraph::CGraph() : _vertexes(0), _edges(0), _matrix(nullptr), _edgesMatr(nullptr)
{
}

CGraph::CGraph(int vertexes, int edges) : _vertexes(vertexes), _edges(edges), _matrix(nullptr), _edgesMatr(nullptr)
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

void CGraph::createEdges()
{
	if (_edges == 0)
	{
		return;
	}

	_edgesMatr = new int* [_edges];
	for (int i = 0; i < _edges; i++)
	{
		_edgesMatr[i] = new int[2] { 0 };
	}
}

void CGraph::deleteAll()
{
	deleteMatr();
	deleteEdges();
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

void CGraph::readEdges(int edges)
{
	_edges = edges;
	createEdges();
	for (int i = 0; i < _edges; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			std::cin >> _edgesMatr[i][j];
		}
	}
	std::cout << std::endl;
}

void CGraph::fillingMatr(int vertexes)
{
	_vertexes = vertexes + 1;
	createMatr();
	for (int i = 0; i <= _vertexes + 1; i++)
	{
		for (int j = 0; j <= _vertexes + 1; j++)
		{
			_matrix[_edgesMatr[j][1]][_edgesMatr[j][0]] = 1;
			_matrix[_edgesMatr[j][0]][_edgesMatr[j][1]] = 1;
		}
	}
}

void CGraph::PrintMatrix()
{
	if (_matrix == nullptr)
	{
		if (_edgesMatr == nullptr)
		{
			std::cout << "Graph empty" << std::endl;
			return;
		}
		fillingMatr(_vertexes);
	}
	for (int i = 1; i < _vertexes; ++i)
	{
		for (int j = 1; j < _vertexes; ++j)
		{
			std::cout << _matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void CGraph::trLights(int vertexes)
{
	_vertexes = vertexes;
	for (int q = 1; q <= _vertexes; q++)
	{
		int sumEd = 0;
		for (int i = 0; i < _edges; ++i)
		{

			for (int j = 0; j < 2; ++j)
			{
				if (_edgesMatr[i][j] == q)
				{
					++sumEd;
				}
			}
		}
		std::cout << sumEd << " ";
	}

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

void CGraph::deleteEdges()
{
	if (_edgesMatr != nullptr)
	{
		for (int i = 0; i < _edges; ++i)
		{
			delete[] _edgesMatr[i];
		}
		delete[] _edgesMatr;
		_edgesMatr = nullptr;
	}
}

