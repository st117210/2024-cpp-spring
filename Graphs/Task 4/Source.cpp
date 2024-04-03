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
	void colorHill(int vertexes);
	void colorBrige(int vertexes);
	void travelRing(int vertexes);

private:
	int searchRoads();
	void createMatr();
	void createEdges();

	int** _matrix;
	int _vertexes;
	int _edges;
	int** _edgesMatr;
	int* _arrColors;

	void deleteAll();
	void deleteMatr();
	void deleteEdges();
	void deleteArrColors();
};

int main(int argc, char* argv[])
{
	int v = 0;
	std::cin >> v;

	CGraph travel;
	travel.readMatrix(v);
	travel.travelRing(v);

	return EXIT_SUCCESS;
}

CGraph::CGraph() : _vertexes(0), _edges(0), _matrix(nullptr), _edgesMatr(nullptr), _arrColors(nullptr)
{
}

CGraph::CGraph(int vertexes, int edges) : _vertexes(vertexes), _edges(edges), _matrix(nullptr), _edgesMatr(nullptr), _arrColors(nullptr)
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

/*void CGraph::travelRing(int vertexes)
{
	_vertexes = vertexes;

	int s = 0;
	int index = 0;
	int count = 1001;
	int i1 = 0;
	int j1 = 0;
	for (int i = 0; i < _vertexes; i++)
	{
		for (int j = 0; j < _vertexes; j++)
		{
			if (count > _matrix[i][j] && _matrix[i][j] != 0)
			{
				count = _matrix[i][j];
				i1 = i;
				j1 = j;
				index = i;
			}
		}
	}
	s += count;

	int i2 = 0;
	int j2 = 0;
	count = 1001;
	_matrix[j1][i1] = 1001;
	for (int i = 0; i < _vertexes; i++)
	{
		if (count > _matrix[j1][i] && _matrix[j1][i] != 0)
		{
			count = _matrix[j1][i];
			i2 = j1;
			j2 = i;
		}
	}
	s += count;

	_matrix[j2][i2] = 1001;
	for (int i = 0; i < _vertexes; i++)
	{
		for (int j = 0; j < _vertexes; j++)
		{
			if (_matrix[j2][i] == _matrix[index][j])
			{
				s += _matrix[j2][i];
			}
		}
	}

	std::cout << s;
}*/

void CGraph::deleteAll()
{
	deleteMatr();
	deleteEdges();
	deleteArrColors();
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
	if (_edgesMatr == nullptr)
	{
		std::cout << "Edges matrix is empty" << std::endl;
		return;
	}
	for (int i = 0; i < _edges; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			_matrix[_edgesMatr[i][1]][_edgesMatr[i][0]] = 1;
			_matrix[_edgesMatr[i][0]][_edgesMatr[i][1]] = 1;
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

	for (int i = 0; i < _vertexes; ++i)
	{
		for (int j = 0; j < _vertexes; ++j)
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

void CGraph::colorHill(int vertexes)
{
	_vertexes = vertexes;
	_arrColors = new int[_vertexes] { 0 };
	for (int i = 0; i < _vertexes; i++)
	{
		std::cin >> _arrColors[i];
	}
	std::cout << std::endl;
}

void CGraph::colorBrige(int vertexes)
{
	int count = 0;

	_vertexes = vertexes;
	for (int i = 0; i < _vertexes; i++)
	{
		for (int j = 0; j < _vertexes; j++)
		{
			if (_matrix[i][j] == 1)
			{
				_matrix[i][j] = _arrColors[i];
			}
		}
	}
	for (int i = 0; i < _vertexes; i++)
	{
		for (int j = 0; j < _vertexes; j++)
		{
			if (_matrix[i][j] != _matrix[j][i])
			{
				++count;
			}
		}
	}

	std::cout << count / 2;
}

void CGraph::travelRing(int vertexes)
{
	_vertexes = vertexes;
	int minDist = 10000000;

	for (int i = 0; i < _vertexes; ++i)
	{
		for (int j = 0; j < _vertexes; ++j)
		{
			for (int k = 0; k < _vertexes; ++k)
			{
				if (i != j && j != k && k != i)
				{
					int dist = _matrix[i][j] + _matrix[j][k] + _matrix[k][i];
					minDist = std::min(minDist, dist);
				}
			}
		}
	}

	std::cout << minDist;
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

void CGraph::deleteArrColors()
{
	delete[] _arrColors;
}

