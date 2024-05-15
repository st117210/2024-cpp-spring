#include"Astar.h"
#include"Menu.h"


GridVisualizer::GridVisualizer(sf::RenderWindow& window, int numVertices, float cellSize, float padding)
	: window(window), numVer(numVertices), cellSize(cellSize), padding(padding)
{
	// ������������� ����� ������
	cell.setSize(sf::Vector2f(cellSize, cellSize));
	cell.setFillColor(sf::Color::Yellow);
	cell.setOutlineColor(sf::Color::Black);
	cell.setOutlineThickness(1.0f);

	// ������������� �������� ��������� � �������� �����
	startMarker.setSize(sf::Vector2f(cellSize, cellSize));
	startMarker.setFillColor(sf::Color::Blue);

	endMarker.setSize(sf::Vector2f(cellSize, cellSize));
	endMarker.setFillColor(sf::Color::Red);
}

void GridVisualizer::createGrid()
{
	// ���������� ������ ������� �����
	float gridSize = cellSize * numVer;

	// ������ ��������� ������� ����� � ������ �������
	float startX = (window.getSize().x - gridSize) / 2 + padding;
	float startY = (window.getSize().y - gridSize) / 2 + padding;

	// ��������� �����
	for (int i = 0; i < numVer; ++i)
	{
		for (int j = 0; j < numVer; ++j)
		{
			cell.setPosition(startX + i * cellSize, startY + j * cellSize);

			// ���������, ����� �� �������� � ������� ����
			if (Matrix[j][i] == 0)
			{
				// ���� ��, ����������� ������ ������ ������
				cell.setFillColor(sf::Color::Black);
			}
			else
			{
				// ����� ���������� ����������� ����
				cell.setFillColor(sf::Color::White);
			}

			window.draw(cell);
		}
	}
}

void GridVisualizer::drawStartMarker(std::pair<int, int> startCoords)
{
	float startX = (window.getSize().x - (cellSize * numVertices)) / 2 + padding + startCoords.first * cellSize;
	float startY = (window.getSize().y - (cellSize * numVertices)) / 2 + padding + startCoords.second * cellSize;

	startMarker.setPosition(startX, startY);
	window.draw(startMarker);
}

void GridVisualizer::drawEndMarker(std::pair<int, int> endCoords)
{
	float endX = (window.getSize().x - (cellSize * numVer)) / 2 + padding + endCoords.first * cellSize;
	float endY = (window.getSize().y - (cellSize * numVer)) / 2 + padding + endCoords.second * cellSize;

	endMarker.setPosition(endX, endY);
	window.draw(endMarker);
}

void GridVisualizer::drawWorkAstar(SNode* workAstar)
{
	// ������ ���������� ��� ����������� ������ A* (������ �������� ������� ������)
	float smallerCellSize = cellSize / 2.0f;

	// ����������� ������ ������� �� ������ workAstar
	while (workAstar != nullptr)
	{
		// ���������� ������� ���������� �� �����
		float pathX = (window.getSize().x - (cellSize * numVer)) / 2 + padding + workAstar->vertex.first * cellSize + (cellSize - smallerCellSize) / 2;
		float pathY = (window.getSize().y - (cellSize * numVer)) / 2 + padding + workAstar->vertex.second * cellSize + (cellSize - smallerCellSize) / 2;

		// ��������� ����������
		sf::RectangleShape smallerCell(sf::Vector2f(smallerCellSize, smallerCellSize));
		smallerCell.setFillColor(sf::Color::Magenta);
		smallerCell.setPosition(pathX, pathY);

		window.draw(smallerCell);

		// ������� � ��������� ������� � ������
		workAstar = workAstar->next;
	}
}

void GridVisualizer::drawPathMarker(std::pair<int, int> pathCoords)
{
	float pathX = (window.getSize().x - (cellSize * numVer)) / 2 + padding + pathCoords.first * cellSize + cellSize / 2;
	float pathY = (window.getSize().y - (cellSize * numVer)) / 2 + padding + pathCoords.second * cellSize + cellSize / 2;

	sf::CircleShape circle(cellSize / 4); // ��������� ������ �����
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(pathX, pathY);
	circle.setOrigin(cellSize / 4, cellSize / 4); // ���������� ����� ����� � ��� �����

	window.draw(circle);
}

void printMenu()
{
	std::cout << "������ ��������� ��������� �������� ������ ���� �*" << std::endl;
	std::cout << std::endl;
	std::cout << "������ ���� �� ����� ���������� ����� ������, ����� ���� - �������" << std::endl;
	std::cout << "������� ���� ���������� �������� �������� �� ����� �����" << std::endl;
	std::cout << "�������� ���������� ���������� ������ ���������, �� ���� �� ������, ������� ��������� �������� ������ ���� �*" << std::endl;
	std::cout << "������ ������ ���������� �����, ����� ������� ������ ������" << std::endl;
	std::cout << std::endl;
	std::cout << "������� ���� ��������� � ������� � � ���� output.txt" << std::endl;
	std::cout << std::endl;

	std::cout << "�������� ��������:" << std::endl;
	std::cout << "0 - ����� �� ���������" << std::endl;
	std::cout << "1 - ������ ����� ����� �������" << std::endl;
	std::cout << "2 - ������ ����� ����� ����" << std::endl;
	std::cout << "3 - ������������� ��������� �����" << std::endl;
}

void printPath(SNode* path)
{
	if (path != nullptr)
	{
		std::cout << "Path: " << std::endl;
		while (path != nullptr)
		{
			std::cout << "(" << path->vertex.first << ", " << path->vertex.second << ") " << std::endl;
			path = path->next;
		}
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Path: ���� �� ������!" << std::endl;
	}
}

void runVisualization(const std::pair<int, int>& begin, const std::pair<int, int>& end, SNode* workAstar, SNode* path, int numVertices)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "A* Algorithm Visualization");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		// ��������� �������
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// ������� ����
		window.clear(sf::Color::White);

		// ����������� �����
		GridVisualizer visualizer(window, numVertices);
		visualizer.createGrid();

		// ����������� ��������� � �������� �����
		visualizer.drawStartMarker(begin);
		visualizer.drawEndMarker(end);

		visualizer.drawWorkAstar(workAstar);

		// ����������� ����
		SNode* currentNode = path;
		while (currentNode != nullptr)
		{
			visualizer.drawPathMarker(currentNode->vertex);
			currentNode = currentNode->next;
		}

		// ����������� ����������� ����
		window.display();
	}
}

void writePathToFile(SNode* path)
{
	std::ofstream outputFile("output.txt");

	if (path != nullptr)
	{
		SNode* tmp = path;
		while (tmp != nullptr)
		{
			// �������� ���������� ���� � ����
			outputFile << "(" << tmp->vertex.first << ", " << tmp->vertex.second << ")" << std::endl;
			tmp = tmp->next;
		}
	}
	else
	{
		std::cout << "���� �� ������!" << std::endl;
	}

	// ������� ����
	outputFile.close();
}

int readHeuristics(int& heuristics)
{
	std::cout << "�������� ������������� �������:" << std::endl;
	std::cout << "1 - ������������� ����������" << std::endl;
	std::cout << "2 - ����������� ����������" << std::endl;
	std::cout << "3 - ��������� ����������" << std::endl;
	std::cin >> heuristics;

	// �������� �� �������� ����������
	while (std::cin.fail() || heuristics > 3 || heuristics < 1)
	{
		std::cin.clear(); // ����� ��������� ������
		std::cin.ignore(9999, '\n'); // ������� ������ �����
		std::cout << "������: ��� ����� ������������� �������, ������� ������ ��������: ";
		std::cin >> heuristics;
	}

	return heuristics;
}

int readNumVertices(int& numVertices)
{
	std::cout << "������� ���������� ������ (1<N<100): ";
	std::cin >> numVertices;

	// �������� �� �������� ����������
	while (std::cin.fail() || numVertices >= 100 || numVertices <= 1)
	{
		std::cin.clear(); // ����� ��������� ������
		std::cin.ignore(9999, '\n'); // ������� ������ �����
		std::cout << "������: ������� ����� ������ 100 � ������ 1: ";
		std::cin >> numVertices;
	}

	return numVertices;
}

int readMaxValue(int& maxValue)
{
	std::cout << "������� ������� ������� ��� ��������� ����� (>2): ";
	std::cin >> maxValue;

	// �������� �� �������� ����������
	while (std::cin.fail() || maxValue <= 2)
	{
		std::cin.clear(); // ����� ��������� ������
		std::cin.ignore(9999, '\n'); // ������� ������ �����
		std::cout << "������: ������� ������ ��������: ";
		std::cin >> maxValue;
	}

	return maxValue;
}

std::pair<int, int> getCoordinates(const std::string& message)
{
	int x = 0;
	int y = 0;

	std::cout << message << std::endl;
	while (!(std::cin >> x >> y))
	{
		std::cout << "������: ������� �������� �������� ��� X � Y" << std::endl;
		std::cin.clear(); // ������� ������ ������ �����
		std::cin.ignore(9999, '\n'); // ������� ������ �����
	}

	return std::make_pair(x, y);
}

bool isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

bool isMatrixValid(int** matrix, int size)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			int num = matrix[i][j];
			while (num != 0)
			{
				int digit = num % 10;
				if (!isDigit(char(digit + '0')))
				{
					std::cout << "������: ����� ������ ��������� ������ ����� � �� ������������� �����" << std::endl;
					return false;
				}
				num /= 10;
			}
		}
		return true;
	}
}
