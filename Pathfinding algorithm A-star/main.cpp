#include"Astar.h"
#include"Menu.h"

const int WINDOW_WIDTH = 1366;
const int WINDOW_HEIGHT = 900;
const int GRID_SIZE = 10;
int numVertices = 0;
int** Matrix = nullptr;
int heuristics = 0;
const double PI = 3.14159265358979323846;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");

	bool exit = false;

	while (!exit)
	{

		system("cls");
		printMenu();

		int menuitem = 0;
		std::cin >> menuitem;

		switch (menuitem)
		{
		case 0: //Выйти из программы
			exit = true;
			break;
		case 1: //Ввести карту через консоль
		{
			readHeuristics(heuristics);
			readNumVertices(numVertices);

			std::cout << "Введите матрицу:" << std::endl;

			int wrong = 0;
			Matrix = new int* [numVertices];
			for (int i = 0; i < numVertices; ++i)
			{
				Matrix[i] = new int[numVertices];
				for (int j = 0; j < numVertices; ++j)
				{
					std::cin >> Matrix[i][j];

					int num = Matrix[i][j];
					while (num != 0)
					{
						int digit = num % 10;
						if (!isDigit(char(digit + '0')))
						{
							++wrong;
							break;
						}
						num /= 10;
					}
				}
			}
			if (wrong != 0)
			{
				std::cout << "ОШИБКА: сетка должна содержать только цифры и не отрицательные числа" << std::endl;
				deleteMatrix(Matrix, numVertices);
				system("pause");
				break;
			}

			std::pair<int, int> begin = getCoordinates("Введите координату начала пути X и Y:");
			std::pair<int, int> end = getCoordinates("Введите координату конца пути X и Y:");

			if (begin == end)
			{
				std::cout << "ОШИБКА: введены одинаковые координаты начала и конца " << std::endl;
				system("pause");
				break;
			}

			if (begin.first >= numVertices || begin.first < 0 || begin.second >= numVertices || begin.second < 0)
			{
				std::cout << "ОШИБКА: координата начала пути выходит за границы сетки" << std::endl;
				system("pause");
				break;
			}

			if (end.first >= numVertices || end.first < 0 || end.second >= numVertices || end.second < 0)
			{
				std::cout << "ОШИБКА: координата конца пути выходит за границы сетки" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[begin.first][begin.second] == 0)
			{
				std::cout << "ОШИБКА: начальная клетка не должна быть стеной (вес этой клетки не должен быть равен 0)" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[end.first][end.second] == 0)
			{
				std::cout << "ОШИБКА: конечная клетка не должна быть стеной (вес этой клетки не должен быть равен 0)" << std::endl;
				system("pause");
				break;
			}

			CAlgorithm search;

			SNode* path = search.A_star(begin, end);
			SNode* workAstar = search.build_workAstar();

			printPath(path);

			writePathToFile(path);

			runVisualization(begin, end, workAstar, path, numVertices);

			// Освободим память, выделенную под динамическую матрицу
			deleteMatrix(Matrix, numVertices);

			search.deleteNode(path);
			search.deleteNode(workAstar);

			system("pause");
			break;
		}
		case 2: //Ввести карту через файл
		{
			readHeuristics(heuristics);

			std::ifstream inputFile("input.txt"); // Открытие файла для чтения

			if (!inputFile.is_open())
			{
				std::cout << "ОШИБКА: не удалось открыть файл!" << std::endl;
				system("pause");
				break; // Выйти из программы с ошибкой
			}

			inputFile >> numVertices; // Считываем количество вершин

			// Динамическое выделение памяти под матрицу
			readMatrixFromFile(inputFile, Matrix, numVertices);

			// Считываем начальную и конечную координаты пути
			std::pair<int, int> begin(0, 0);
			std::pair<int, int> end(0, 0);
			inputFile >> begin.first >> begin.second;
			inputFile >> end.first >> end.second;

			// Закрываем файл после чтения
			inputFile.close();

			// Вывод данных для проверки
			std::cout << "Количество вершин: " << numVertices << std::endl;

			int wrong = 0;
			for (int i = 0; i < numVertices; i++)
			{
				for (int j = 0; j < numVertices; j++)
				{
					if (Matrix[i][j] < 0)
					{
						++wrong;
						break;
					}
				}
			}
			if (wrong != 0)
			{
				std::cout << "ОШИБКА: матрица в файле введена неверно" << std::endl;
				deleteMatrix(Matrix, numVertices);
				system("pause");
				break;
			}
			else
			{
				std::cout << "Матрица:" << std::endl;
				printMatrix(Matrix, numVertices);
			}

			if (begin == end)
			{
				std::cout << "ОШИБКА: координаты введены неправильно " << std::endl;
				system("pause");
				break;
			}

			if (begin.first >= numVertices || begin.first < 0 || begin.second >= numVertices || begin.second < 0)
			{
				std::cout << "ОШИБКА: координата начала пути выходит за границы сетки" << std::endl;
				system("pause");
				break;
			}

			if (end.first >= numVertices || end.first < 0 || end.second >= numVertices || end.second < 0)
			{
				std::cout << "ОШИБКА: координата конца пути выходит за границы сетки" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[begin.second][begin.first] == 0)
			{
				std::cout << "ОШИБКА: начальная клетка не должна быть стеной (вес этой клетки не должен быть равен 0)" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[end.second][end.first] == 0)
			{
				std::cout << "ОШИБКА: конечная клетка не должна быть стеной (вес этой клетки не должен быть равен 0)" << std::endl;
				system("pause");
				break;
			}

			std::cout << "Начальная координата пути: " << "(" << begin.first << ", " << begin.second << ")" << std::endl;
			std::cout << "Конечная координата пути: " << "(" << end.first << ", " << end.second << ")" << std::endl;
			std::cout << std::endl;

			if (begin == end)
			{
				std::cout << "ОШИБКА: введены одинаковые координаты начала и конца " << std::endl;
				system("pause");
				break;
			}

			if (begin.first >= numVertices || begin.first < 0 || begin.second >= numVertices || begin.second < 0)
			{
				std::cout << "ОШИБКА: координата начала пути выходит за границы сетки" << std::endl;
				system("pause");
				break;
			}

			if (end.first >= numVertices || end.first < 0 || end.second >= numVertices || end.second < 0)
			{
				std::cout << "ОШИБКА: координата конца пути выходит за границы сетки" << std::endl;
				system("pause");
				break;
			}

			for (int i = 0; i < numVertices; ++i)
			{
				for (int j = 0; j < numVertices; ++j)
				{
					if (Matrix[i][j] < 0)
					{
						std::cout << "ОШИБКА: сетка не должна содержать отрицательные числа" << std::endl;
						system("pause");
						break;
					}
				}
			}

			if (Matrix[begin.first][begin.second] == 0)
			{
				std::cout << "ОШИБКА: начальная клетка не должна быть стеной (вес этой клетки не должен быть равен 0)" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[end.first][end.second] == 0)
			{
				std::cout << "ОШИБКА: конечная клетка не должна быть стеной (вес этой клетки не должен быть равен 0)" << std::endl;
				system("pause");
				break;
			}

			CAlgorithm search;

			SNode* path = search.A_star(begin, end);
			SNode* workAstar = search.build_workAstar();

			printPath(path);

			writePathToFile(path);

			runVisualization(begin, end, workAstar, path, numVertices);

			// Освободим память, выделенную под динамическую матрицу
			deleteMatrix(Matrix, numVertices);

			search.deleteNode(path);
			search.deleteNode(workAstar);

			system("pause");
			break;
		}
		case 3: //Сгенерировать случайную карту
		{
			srand(time(nullptr));

			readHeuristics(heuristics);

			int maxValue = 0;
			readMaxValue(maxValue);
			std::cout << std::endl;

			if (maxValue <= 2)
			{
				std::cout << "Неверно введено значение верхней границы" << std::endl;
				system("pause");
				break;
			}

			numVertices = rand() % maxValue + 1;
			if (numVertices == 1)
			{
				++numVertices;
			}

			std::cout << "Количество вершин: " << numVertices << std::endl;

			initializeMatrix(Matrix, numVertices, maxValue);

			int x = rand() % (numVertices - 1);
			int y = rand() % (numVertices - 1);
			std::pair<int, int> begin(x, y);

			x = rand() % (numVertices - 1);
			y = rand() % (numVertices - 1);
			std::pair<int, int> end(x, y);

			if (begin == end)
			{
				begin = std::make_pair(0, 0);
				end = std::make_pair(1, 1);
			}

			Matrix[begin.second][begin.first] = 1;
			Matrix[end.second][end.first] = 1;

			std::cout << "Сгенерированная матрица:" << std::endl;
			printMatrix(Matrix, numVertices);

			std::cout << "Сгенерированные координаты начала пути X и Y: " << std::endl;
			std::cout << "(" << begin.first << ", " << begin.second << ")" << std::endl;
			std::cout << "Сгенерированные координаты конца пути X и Y: " << std::endl;
			std::cout << "(" << end.first << ", " << end.second << ")" << std::endl;

			CAlgorithm search;

			SNode* path = search.A_star(begin, end);
			SNode* workAstar = search.build_workAstar();

			printPath(path);

			writePathToFile(path);

			runVisualization(begin, end, workAstar, path, numVertices);

			// Освободим память, выделенную под динамическую матрицу
			deleteMatrix(Matrix, numVertices);

			search.deleteNode(path);
			search.deleteNode(workAstar);

			system("pause");
			break;
		}
		default:
		{
			std::cout << "Неизвестная команда" << std::endl;
			system("pause");
			break;
		}
		}
	}

	return EXIT_SUCCESS;
}