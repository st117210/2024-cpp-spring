#pragma once
#include"Astar.h"

class GridVisualizer
{
public:
	GridVisualizer(sf::RenderWindow& window, int numVertices, float cellSize = 25.0f, float padding = 10.0f);

	void createGrid();
	void drawStartMarker(std::pair<int, int> startCoords);
	void drawEndMarker(std::pair<int, int> endCoords);
	void drawWorkAstar(SNode* workAstar);
	void drawPathMarker(std::pair<int, int> pathCoords);

private:
	sf::RenderWindow& window;
	int numVer;
	float cellSize;
	float padding;
	sf::RectangleShape cell;
	sf::RectangleShape startMarker;
	sf::RectangleShape endMarker;
	sf::RectangleShape pathMarker;
};

void printPath(SNode* path);
void runVisualization(const std::pair<int, int>& begin, const std::pair<int, int>& end, SNode* workAstar, SNode* path, int numVertices);
void printMenu();
void writePathToFile(SNode* path);
int readHeuristics(int& heuristics);
int readNumVertices(int& numVertices);
int readMaxValue(int& maxValue);
std::pair<int, int> getCoordinates(const std::string& message);
bool isDigit(char c);
bool isMatrixValid(int** matrix, int size);

