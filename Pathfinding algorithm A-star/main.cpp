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
		case 0: //����� �� ���������
			exit = true;
			break;
		case 1: //������ ����� ����� �������
		{
			readHeuristics(heuristics);
			readNumVertices(numVertices);

			std::cout << "������� �������:" << std::endl;

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
				std::cout << "������: ����� ������ ��������� ������ ����� � �� ������������� �����" << std::endl;
				deleteMatrix(Matrix, numVertices);
				system("pause");
				break;
			}

			std::pair<int, int> begin = getCoordinates("������� ���������� ������ ���� X � Y:");
			std::pair<int, int> end = getCoordinates("������� ���������� ����� ���� X � Y:");

			if (begin == end)
			{
				std::cout << "������: ������� ���������� ���������� ������ � ����� " << std::endl;
				system("pause");
				break;
			}

			if (begin.first >= numVertices || begin.first < 0 || begin.second >= numVertices || begin.second < 0)
			{
				std::cout << "������: ���������� ������ ���� ������� �� ������� �����" << std::endl;
				system("pause");
				break;
			}

			if (end.first >= numVertices || end.first < 0 || end.second >= numVertices || end.second < 0)
			{
				std::cout << "������: ���������� ����� ���� ������� �� ������� �����" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[begin.first][begin.second] == 0)
			{
				std::cout << "������: ��������� ������ �� ������ ���� ������ (��� ���� ������ �� ������ ���� ����� 0)" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[end.first][end.second] == 0)
			{
				std::cout << "������: �������� ������ �� ������ ���� ������ (��� ���� ������ �� ������ ���� ����� 0)" << std::endl;
				system("pause");
				break;
			}

			CAlgorithm search;

			SNode* path = search.A_star(begin, end);
			SNode* workAstar = search.build_workAstar();

			printPath(path);

			writePathToFile(path);

			runVisualization(begin, end, workAstar, path, numVertices);

			// ��������� ������, ���������� ��� ������������ �������
			deleteMatrix(Matrix, numVertices);

			search.deleteNode(path);
			search.deleteNode(workAstar);

			system("pause");
			break;
		}
		case 2: //������ ����� ����� ����
		{
			readHeuristics(heuristics);

			std::ifstream inputFile("input.txt"); // �������� ����� ��� ������

			if (!inputFile.is_open())
			{
				std::cout << "������: �� ������� ������� ����!" << std::endl;
				system("pause");
				break; // ����� �� ��������� � �������
			}

			inputFile >> numVertices; // ��������� ���������� ������

			// ������������ ��������� ������ ��� �������
			readMatrixFromFile(inputFile, Matrix, numVertices);

			// ��������� ��������� � �������� ���������� ����
			std::pair<int, int> begin(0, 0);
			std::pair<int, int> end(0, 0);
			inputFile >> begin.first >> begin.second;
			inputFile >> end.first >> end.second;

			// ��������� ���� ����� ������
			inputFile.close();

			// ����� ������ ��� ��������
			std::cout << "���������� ������: " << numVertices << std::endl;

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
				std::cout << "������: ������� � ����� ������� �������" << std::endl;
				deleteMatrix(Matrix, numVertices);
				system("pause");
				break;
			}
			else
			{
				std::cout << "�������:" << std::endl;
				printMatrix(Matrix, numVertices);
			}

			if (begin == end)
			{
				std::cout << "������: ���������� ������� ����������� " << std::endl;
				system("pause");
				break;
			}

			if (begin.first >= numVertices || begin.first < 0 || begin.second >= numVertices || begin.second < 0)
			{
				std::cout << "������: ���������� ������ ���� ������� �� ������� �����" << std::endl;
				system("pause");
				break;
			}

			if (end.first >= numVertices || end.first < 0 || end.second >= numVertices || end.second < 0)
			{
				std::cout << "������: ���������� ����� ���� ������� �� ������� �����" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[begin.second][begin.first] == 0)
			{
				std::cout << "������: ��������� ������ �� ������ ���� ������ (��� ���� ������ �� ������ ���� ����� 0)" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[end.second][end.first] == 0)
			{
				std::cout << "������: �������� ������ �� ������ ���� ������ (��� ���� ������ �� ������ ���� ����� 0)" << std::endl;
				system("pause");
				break;
			}

			std::cout << "��������� ���������� ����: " << "(" << begin.first << ", " << begin.second << ")" << std::endl;
			std::cout << "�������� ���������� ����: " << "(" << end.first << ", " << end.second << ")" << std::endl;
			std::cout << std::endl;

			if (begin == end)
			{
				std::cout << "������: ������� ���������� ���������� ������ � ����� " << std::endl;
				system("pause");
				break;
			}

			if (begin.first >= numVertices || begin.first < 0 || begin.second >= numVertices || begin.second < 0)
			{
				std::cout << "������: ���������� ������ ���� ������� �� ������� �����" << std::endl;
				system("pause");
				break;
			}

			if (end.first >= numVertices || end.first < 0 || end.second >= numVertices || end.second < 0)
			{
				std::cout << "������: ���������� ����� ���� ������� �� ������� �����" << std::endl;
				system("pause");
				break;
			}

			for (int i = 0; i < numVertices; ++i)
			{
				for (int j = 0; j < numVertices; ++j)
				{
					if (Matrix[i][j] < 0)
					{
						std::cout << "������: ����� �� ������ ��������� ������������� �����" << std::endl;
						system("pause");
						break;
					}
				}
			}

			if (Matrix[begin.first][begin.second] == 0)
			{
				std::cout << "������: ��������� ������ �� ������ ���� ������ (��� ���� ������ �� ������ ���� ����� 0)" << std::endl;
				system("pause");
				break;
			}

			if (Matrix[end.first][end.second] == 0)
			{
				std::cout << "������: �������� ������ �� ������ ���� ������ (��� ���� ������ �� ������ ���� ����� 0)" << std::endl;
				system("pause");
				break;
			}

			CAlgorithm search;

			SNode* path = search.A_star(begin, end);
			SNode* workAstar = search.build_workAstar();

			printPath(path);

			writePathToFile(path);

			runVisualization(begin, end, workAstar, path, numVertices);

			// ��������� ������, ���������� ��� ������������ �������
			deleteMatrix(Matrix, numVertices);

			search.deleteNode(path);
			search.deleteNode(workAstar);

			system("pause");
			break;
		}
		case 3: //������������� ��������� �����
		{
			srand(time(nullptr));

			readHeuristics(heuristics);

			int maxValue = 0;
			readMaxValue(maxValue);
			std::cout << std::endl;

			if (maxValue <= 2)
			{
				std::cout << "������� ������� �������� ������� �������" << std::endl;
				system("pause");
				break;
			}

			numVertices = rand() % maxValue + 1;
			if (numVertices == 1)
			{
				++numVertices;
			}

			std::cout << "���������� ������: " << numVertices << std::endl;

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

			std::cout << "��������������� �������:" << std::endl;
			printMatrix(Matrix, numVertices);

			std::cout << "��������������� ���������� ������ ���� X � Y: " << std::endl;
			std::cout << "(" << begin.first << ", " << begin.second << ")" << std::endl;
			std::cout << "��������������� ���������� ����� ���� X � Y: " << std::endl;
			std::cout << "(" << end.first << ", " << end.second << ")" << std::endl;

			CAlgorithm search;

			SNode* path = search.A_star(begin, end);
			SNode* workAstar = search.build_workAstar();

			printPath(path);

			writePathToFile(path);

			runVisualization(begin, end, workAstar, path, numVertices);

			// ��������� ������, ���������� ��� ������������ �������
			deleteMatrix(Matrix, numVertices);

			search.deleteNode(path);
			search.deleteNode(workAstar);

			system("pause");
			break;
		}
		default:
		{
			std::cout << "����������� �������" << std::endl;
			system("pause");
			break;
		}
		}
	}

	return EXIT_SUCCESS;
}