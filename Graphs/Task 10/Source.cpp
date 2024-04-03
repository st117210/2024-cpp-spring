#include<iostream>
#include<cmath>
#include<algorithm>


class CEvolution {
public:
	CEvolution();
	CEvolution(int levels);
	~CEvolution();

	long long findCommonAncestor(long long type1, long long type2);

private:
	int _levels;
};

class CGraph {
public:
	CGraph();
	CGraph(int vertexes, int edges);
	~CGraph();

	void readMatrix(int vertexes);
	void readEdges(int edges);
	void printSearchRoads();
	void PrintMatrix();
	void PrintEdges();
	void trLights(int vertexes);
	void colorHill(int vertexes);
	void colorBrige(int vertexes);
	void travelRing(int vertexes);
	void starwarsPL();
	int numberEdgesFromMatrix();
	void SetVertexes(int vertexes);
	void listEdges();
	void adjList();
	void printArrPow();

private:
	int searchRoads();
	void createMatr();
	void createEdges();
	void fillingMatr(int vertexes);
	void fillingEdges(int edges);
	void powVert();

	int** _matrix;
	int _vertexes;
	int _edges;
	int** _edgesMatr;
	int* _arrColors;
	int* _listVert;
	int* _arrAdj;
	int* _arrPow;

	void deleteAll();
	void deleteMatr();
	void deleteEdges();
	void deleteArrColors();
	void deleteListVert();
	void deleteArrAdj();
	void deleteArrPow();
};

int main(int argc, char* argv[])
{
	int v = 0;
	std::cin >> v;

	CGraph pV;
	pV.readMatrix(v);
	pV.printArrPow();


	return EXIT_SUCCESS;
}

CEvolution::CEvolution() : _levels(0) {}

CEvolution::CEvolution(int levels) : _levels(levels) {}

CEvolution::~CEvolution() {}

long long CEvolution::findCommonAncestor(long long type1, long long type2)
{
	while (type1 != type2)
	{
		if (type1 > type2)
		{
			type1 /= 2;
		}
		else
		{
			type2 /= 2;
		}
	}
	return type1;
}

CGraph::CGraph() : _vertexes(0), _edges(0), _matrix(nullptr), _edgesMatr(nullptr), _arrColors(nullptr), _listVert(nullptr), _arrAdj(nullptr), _arrPow(nullptr)
{
}

CGraph::CGraph(int vertexes, int edges) : _vertexes(vertexes), _edges(edges), _matrix(nullptr), _edgesMatr(nullptr), _arrColors(nullptr), _listVert(nullptr), _arrAdj(nullptr), _arrPow(nullptr)
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
	deleteArrColors();
	deleteListVert();
	deleteArrAdj();
	deleteArrPow();
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
		_matrix[_edgesMatr[i][0]][_edgesMatr[i][1]] = 1;
	}
}

void CGraph::fillingEdges(int edges)
{
	if (_matrix == nullptr)
	{
		std::cout << "Matrix is empty" << std::endl;
		return;
	}
	createEdges();

	int edgeIndex = 0;
	for (int i = 0; i < _vertexes; ++i) {
		for (int j = 0; j < _vertexes; ++j)
		{
			if (_matrix[i][j] == 1) {
				_edgesMatr[edgeIndex][0] = i + 1;
				_edgesMatr[edgeIndex][1] = j + 1;
				++edgeIndex;
			}
		}
	}
}

int CGraph::numberEdgesFromMatrix()
{
	_edges = 0;
	for (int i = 0; i < _vertexes; i++)
	{
		for (int j = 0; j < _vertexes; j++)
		{
			_edges += _matrix[i][j];
		}
	}

	return _edges;
}

void CGraph::SetVertexes(int vertexes)
{
	_vertexes = vertexes;
}

void CGraph::listEdges()
{
	for (int i = 1; i <= _vertexes; i++)
	{
		int countEd = 0;
		for (int j = 0; j < _edges; j++)
		{
			if (_edgesMatr[j][0] == i)
			{
				++countEd;
			}
		}
		std::cout << countEd << " ";

		_listVert = new int[countEd] { 0 };
		int listIndex = 0;
		for (int q = 0; q < _edges; q++)
		{
			if (_edgesMatr[q][0] == i)
			{
				_listVert[listIndex] = _edgesMatr[q][1];
				++listIndex;
			}
		}

		std::sort(_listVert, _listVert + countEd);

		for (int f = 0; f < countEd; f++)
		{
			std::cout << _listVert[f] << " ";
		}
		std::cout << std::endl;
	}
}

void CGraph::adjList()
{
	createMatr();
	int numEd = 0;
	int vertEd = 0;
	for (int i = 1; i < _vertexes; i++)
	{
		std::cin >> numEd;

		if (numEd != 0)
		{
			if (numEd == 1)
			{
				std::cin >> vertEd;

				_matrix[i][vertEd] = 1;
			}
			else
			{
				_arrAdj = new int[numEd] { 0 };
				for (int j = 0; j < numEd; j++)
				{
					std::cin >> _arrAdj[j];
				}

				for (int p = 0; p < numEd; p++)
				{
					_matrix[i][_arrAdj[p]] = 1;
				}
			}
		}
	}
}

void CGraph::printArrPow()
{
	if (_arrPow == nullptr)
	{
		powVert();
	}
	for (int i = 0; i < _vertexes; i++)
	{
		std::cout << _arrPow[i] << " ";
	}
}

void CGraph::powVert()
{
	_arrPow = new int[_vertexes] { 0 };
	for (int i = 0; i < _vertexes; i++)
	{
		int count = 0;
		for (int j = 0; j < _vertexes; j++)
		{
			count += _matrix[i][j];
		}
		_arrPow[i] = count;
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

void CGraph::PrintEdges()
{
	if (_edgesMatr == nullptr)
	{
		if (_matrix == nullptr)
		{
			std::cout << "Graph empty" << std::endl;
			return;
		}
		fillingEdges(numberEdgesFromMatrix());
	}
	for (int i = 0; i < _edges; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			std::cout << _edgesMatr[i][j] << " ";
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

void CGraph::starwarsPL()
{
	int countPlanet = 0;
	for (int i = 1; i <= _edges + 1; i++)
	{
		int count = 0;
		for (int j = 0; j < _edges; j++)
		{
			for (int q = 0; q < 2; q++)
			{
				if (_edgesMatr[j][q] == i)
				{
					++count;
				}
			}
		}

		if (count > 1)
		{
			++countPlanet;
		}
	}

	std::cout << countPlanet << std::endl;
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

void CGraph::deleteListVert()
{
	delete[] _listVert;
}

void CGraph::deleteArrAdj()
{
	delete[] _arrAdj;
}

void CGraph::deleteArrPow()
{
	delete[] _arrPow;
}
