#include"Astar.h"
#include"Menu.h"


GridVisualizer::GridVisualizer(sf::RenderWindow& window, int numVertices, float cellSize, float padding)
	: window(window), numVer(numVertices), cellSize(cellSize), padding(padding)
{
	// Инициализация формы ячейки
	cell.setSize(sf::Vector2f(cellSize, cellSize));
	cell.setFillColor(sf::Color::Yellow);
	cell.setOutlineColor(sf::Color::Black);
	cell.setOutlineThickness(1.0f);

	// Инициализация маркеров начальной и конечной точек
	startMarker.setSize(sf::Vector2f(cellSize, cellSize));
	startMarker.setFillColor(sf::Color::Blue);

	endMarker.setSize(sf::Vector2f(cellSize, cellSize));
	endMarker.setFillColor(sf::Color::Red);
}

void GridVisualizer::createGrid()
{
	// Вычисление общего размера сетки
	float gridSize = cellSize * numVer;

	// Расчет начальной позиции сетки с учетом отступа
	float startX = (window.getSize().x - gridSize) / 2 + padding;
	float startY = (window.getSize().y - gridSize) / 2 + padding;

	// Отрисовка сетки
	for (int i = 0; i < numVer; ++i)
	{
		for (int j = 0; j < numVer; ++j)
		{
			cell.setPosition(startX + i * cellSize, startY + j * cellSize);

			// Проверяем, равно ли значение в матрице нулю
			if (Matrix[j][i] == 0)
			{
				// Если да, закрашиваем ячейку черным цветом
				cell.setFillColor(sf::Color::Black);
			}
			else
			{
				// Иначе используем стандартный цвет
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
	// Размер квадратика для отображения работы A* (меньше обычного размера ячейки)
	float smallerCellSize = cellSize / 2.0f;

	// Отображение каждой вершины из списка workAstar
	while (workAstar != nullptr)
	{
		// Вычисление позиции квадратика на сетке
		float pathX = (window.getSize().x - (cellSize * numVer)) / 2 + padding + workAstar->vertex.first * cellSize + (cellSize - smallerCellSize) / 2;
		float pathY = (window.getSize().y - (cellSize * numVer)) / 2 + padding + workAstar->vertex.second * cellSize + (cellSize - smallerCellSize) / 2;

		// Отрисовка квадратика
		sf::RectangleShape smallerCell(sf::Vector2f(smallerCellSize, smallerCellSize));
		smallerCell.setFillColor(sf::Color::Magenta);
		smallerCell.setPosition(pathX, pathY);

		window.draw(smallerCell);

		// Переход к следующей вершине в списке
		workAstar = workAstar->next;
	}
}

void GridVisualizer::drawPathMarker(std::pair<int, int> pathCoords)
{
	float pathX = (window.getSize().x - (cellSize * numVer)) / 2 + padding + pathCoords.first * cellSize + cellSize / 2;
	float pathY = (window.getSize().y - (cellSize * numVer)) / 2 + padding + pathCoords.second * cellSize + cellSize / 2;

	sf::CircleShape circle(cellSize / 4); // Уменьшаем радиус круга
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(pathX, pathY);
	circle.setOrigin(cellSize / 4, cellSize / 4); // Перемещаем центр круга в его центр

	window.draw(circle);
}

void printMenu()
{
	std::cout << "Данная программа выполняет алгоритм поиска пути А*" << std::endl;
	std::cout << std::endl;
	std::cout << "Начало пути на карте отмечается синим цветом, конец пути - красным" << std::endl;
	std::cout << "Искомый путь отмечается зелеными кружками на сетке карты" << std::endl;
	std::cout << "Розовыми квадратами отмечается работа алгоритма, то есть те клетки, которые обработал алгоритм поиска пути А*" << std::endl;
	std::cout << "Черным цветом отмечаются стены, через которые нельзя пройти" << std::endl;
	std::cout << std::endl;
	std::cout << "Искомый путь выводится в консоль и в файл output.txt" << std::endl;
	std::cout << std::endl;

	std::cout << "Выберите действие:" << std::endl;
	std::cout << "0 - Выйти из программы" << std::endl;
	std::cout << "1 - Ввести карту через консоль" << std::endl;
	std::cout << "2 - Ввести карту через файл" << std::endl;
	std::cout << "3 - Сгенерировать случайную карту" << std::endl;
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
		std::cout << "Path: путь не найден!" << std::endl;
	}
}

void runVisualization(const std::pair<int, int>& begin, const std::pair<int, int>& end, SNode* workAstar, SNode* path, int numVertices)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "A* Algorithm Visualization");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		// Обработка событий
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// Очистка окна
		window.clear(sf::Color::White);

		// Отображение сетки
		GridVisualizer visualizer(window, numVertices);
		visualizer.createGrid();

		// Отображение начальной и конечной точек
		visualizer.drawStartMarker(begin);
		visualizer.drawEndMarker(end);

		visualizer.drawWorkAstar(workAstar);

		// Отображение пути
		SNode* currentNode = path;
		while (currentNode != nullptr)
		{
			visualizer.drawPathMarker(currentNode->vertex);
			currentNode = currentNode->next;
		}

		// Отображение содержимого окна
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
			// Записать координаты пути в файл
			outputFile << "(" << tmp->vertex.first << ", " << tmp->vertex.second << ")" << std::endl;
			tmp = tmp->next;
		}
	}
	else
	{
		std::cout << "Путь не найден!" << std::endl;
	}

	// Закрыть файл
	outputFile.close();
}

int readHeuristics(int& heuristics)
{
	std::cout << "Выберите эвристическую функцию:" << std::endl;
	std::cout << "1 - Манхэттенское расстояние" << std::endl;
	std::cout << "2 - Чебышевское расстояние" << std::endl;
	std::cout << "3 - Евклидово расстояние" << std::endl;
	std::cin >> heuristics;

	// Проверка на успешное считывание
	while (std::cin.fail() || heuristics > 3 || heuristics < 1)
	{
		std::cin.clear(); // Сброс состояния ошибки
		std::cin.ignore(9999, '\n'); // Очистка буфера ввода
		std::cout << "ОШИБКА: нет такой эвристической функции, введите другое значение: ";
		std::cin >> heuristics;
	}

	return heuristics;
}

int readNumVertices(int& numVertices)
{
	std::cout << "Введите количество вершин (1<N<100): ";
	std::cin >> numVertices;

	// Проверка на успешное считывание
	while (std::cin.fail() || numVertices >= 100 || numVertices <= 1)
	{
		std::cin.clear(); // Сброс состояния ошибки
		std::cin.ignore(9999, '\n'); // Очистка буфера ввода
		std::cout << "ОШИБКА: введите число меньше 100 и больше 1: ";
		std::cin >> numVertices;
	}

	return numVertices;
}

int readMaxValue(int& maxValue)
{
	std::cout << "Введите верхнюю границу для случайных чисел (>2): ";
	std::cin >> maxValue;

	// Проверка на успешное считывание
	while (std::cin.fail() || maxValue <= 2)
	{
		std::cin.clear(); // Сброс состояния ошибки
		std::cin.ignore(9999, '\n'); // Очистка буфера ввода
		std::cout << "ОШИБКА: введите другое значение: ";
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
		std::cout << "Ошибка: введите числовые значения для X и Y" << std::endl;
		std::cin.clear(); // Очистка флагов ошибок ввода
		std::cin.ignore(9999, '\n'); // Очистка буфера ввода
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
					std::cout << "ОШИБКА: сетка должна содержать только цифры и не отрицательные числа" << std::endl;
					return false;
				}
				num /= 10;
			}
		}
		return true;
	}
}
