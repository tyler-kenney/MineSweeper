#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
const short DEBUG = 1;

const int ROWS = 8;
const int COLS = 8;
const int BOARD_TOTAL = ROWS * COLS;
const int MINES_TOTAL = 8;

class Grid
{
protected:
	int map[BOARD_TOTAL] = { 0 };	// start with empty board
	int mineLocationX[MINES_TOTAL];
	int mineLocationY[MINES_TOTAL];
	int inputLog[BOARD_TOTAL] = { -1 };

public:
	Grid();
	void newMap();
	void displayAnswerGrid();
	void displayGrid();
	void minePlacement();
	void proximityPlacement();
	int randomNumberMine();
	void sortMines();
	bool isBattery(const int lr, const int lc);
	bool isMine(const int lr, const int lc);
	bool isEmpty(const int lr, const int lc);
	bool beenThere(const int lr, const int lc);
};

Grid::Grid()
{
	if (DEBUG)	cout << "Grid()\n";

	static int count = 0;
	if (!count)	//for only one initialization.
	{
		srand(time(0));	//initialize randomization.
		count++;
	}

	//initialize map.
	newMap();
	//if (DEBUG) displayGrid();
}

void Grid::newMap()
{
	if (DEBUG)	cout << "newMap()\n";
	minePlacement();
	proximityPlacement();
	//addProximity();
	//show answer map
	if (DEBUG)	displayAnswerGrid();

	//minePlacement();
	return;
}

void Grid::displayAnswerGrid()
{
	if (DEBUG)	cout << "displayGrid()\n";
	cout << "\tA\tB\tC\tD\tE\tF\tG\tH" << endl;
	int rowNum = 1;
	for (int i = 0; i < BOARD_TOTAL; i++)
	{
		if (i % COLS == 0)
			cout << endl << endl << rowNum++ << "|\t";

		cout << map[i] << '\t';
	}
	cout << endl;
	return;
}

void Grid::displayGrid()
{

	return;
}

void Grid::minePlacement()
{
	if (DEBUG)	cout << "minePlacement()\n";

	for (int i = 0; i < MINES_TOTAL; i++)
	{
		mineLocationX[i] = randomNumberMine();
		mineLocationY[i] = randomNumberMine();
		//check for location uniqueness.
		for (int y = i - 1; y >= 0; y--)
		{
			if (mineLocationY[i] == mineLocationY[y])
			{
				if (mineLocationX[i] == mineLocationX[y])
					i--;
			}
		}
		if (DEBUG)
		{
			cout << i + 1 << " X: " << mineLocationX[i] << ", ";
			cout << "Y: " << mineLocationY[i] << endl;
		}
	}
	sortMines();
	return;
}

int Grid::randomNumberMine()
{
	if (DEBUG)	cout << "randomNumberMine()\n";

	return (rand() % (COLS - 1 + 1)) + 1;
}

void Grid::sortMines()
{
	if (DEBUG)	cout << "sortMines()\n";
	for (int i = 1; i < MINES_TOTAL; i++)
	{
		if (mineLocationY[i] < mineLocationY[i - 1])
		{

			int tempY = mineLocationY[i - 1];
			int tempX = mineLocationX[i - 1];

			mineLocationY[i - 1] = mineLocationY[i];
			mineLocationX[i - 1] = mineLocationX[i];

			mineLocationY[i] = tempY;
			mineLocationX[i] = tempX;

			for (int i2 = i; i2 >= 1; i2--)
			{
				if (mineLocationY[i2] < mineLocationY[i2 - 1])
				{
					int tempY = mineLocationY[i2 - 1];
					int tempX = mineLocationX[i2 - 1];

					mineLocationY[i2 - 1] = mineLocationY[i2];
					mineLocationX[i2 - 1] = mineLocationX[i2];

					mineLocationY[i2] = tempY;
					mineLocationX[i2] = tempX;
				}
				else if (mineLocationY[i2] == mineLocationY[i2 - 1])
				{
					if (mineLocationX[i2] < mineLocationX[i2 - 1])
					{
						int tempY = mineLocationY[i2 - 1];
						int tempX = mineLocationX[i2 - 1];

						mineLocationY[i2 - 1] = mineLocationY[i2];
						mineLocationX[i2 - 1] = mineLocationX[i2];

						mineLocationY[i2] = tempY;
						mineLocationX[i2] = tempX;
					}
				}
			}
		}
		else if (mineLocationY[i] == mineLocationY[i - 1])
		{
			if (mineLocationX[i] < mineLocationX[i - 1])
			{
				int tempY = mineLocationY[i - 1];
				int tempX = mineLocationX[i - 1];

				mineLocationY[i - 1] = mineLocationY[i];
				mineLocationX[i - 1] = mineLocationX[i];

				mineLocationY[i] = tempY;
				mineLocationX[i] = tempX;
			}
		}

	}
	if (DEBUG)
	{
		for (int i = 0; i < MINES_TOTAL; i++)
		{
			cout << "X: " << mineLocationX[i] << ", ";
			cout << "Y: " << mineLocationY[i] << endl;
		}
		cout << endl;
	}
}

void Grid::proximityPlacement()
{
	int mine = MINES_TOTAL + 1;
	int empty = 0;
	int mineIndex = 0;

	for (int i = 0; i < BOARD_TOTAL; i++)
	{

		if (DEBUG) cout << "total: " << (mineLocationY[mineIndex] * COLS - COLS + mineLocationX[mineIndex]) - 1 << endl;

		if ((mineLocationY[mineIndex] * COLS - COLS + mineLocationX[mineIndex]) - 1 == i)
		{
			if (DEBUG) cout << "Mine Placed\n";
			map[i] += mine;
			//add orthogonal proximity
			//top
			if (i + COLS < BOARD_TOTAL)
				map[i + COLS]++;
			//bottom
			if (i - COLS >= 0)
				map[i - COLS]++;
			//right
			if (DEBUG) cout << "proximity right = " << (i + 1) % (COLS) << endl;
			if ((i + 1) % (COLS) > 0 && i + 1 < BOARD_TOTAL)
				map[i + 1]++;
			//left
			if (DEBUG) cout << "proximity left = " << (i - 1) % (COLS) << endl;
			if ((i) % (COLS) > 0 && i - 1 >= 0)
				map[i - 1]++;
			//add diagonal proximity
			// top right
			if ((i + 1) % (COLS) > 0 && i + COLS + 1 < BOARD_TOTAL)
				map[i + COLS + 1]++;
			//bottom left
			if ((i) % (COLS) > 0 && i - COLS - 1 >= 0)
				map[i - COLS - 1]++;
			//top left
			if ((i) % (COLS) > 0 && i + COLS - 1 < BOARD_TOTAL)
				map[i + COLS - 1]++;
			//bottom right
			if ((i + 1) % (COLS) > 0 && i - COLS + 1 >= 0)
				map[i - COLS + 1]++;

			mineIndex = (mineIndex + 1) % MINES_TOTAL;

			if (DEBUG) cout << "mineIndex: " << mineIndex << endl;

		}
		else
		{
			//map[i] = empty;
			continue;
		}
	}
}

bool Grid::isBattery(const int lr, const int lc)
{
	if (DEBUG)	cout << "isBattery(const int lr, const int lc)\n";
	return false;
}

bool Grid::isMine(const int lr, const int lc)
{
	if (DEBUG)	cout << "isMine(const int lr, const int lc\n";

	if (map[lr * COLS + COLS - lc] == 'M')
		return true;

	for (int i = 0; i < MINES_TOTAL; i++)
	{
		if (lr == mineLocationY[i] && lc == mineLocationX[i])
		{
			return true;
		}
	}
	return false;
}

bool Grid::isEmpty(const int lr, const int lc)
{
	if (DEBUG)	cout << "isEmpty(const int lr, const int lc)\n";

	if (map[(lr - 1) * (COLS)+lc] > 0)
		return false;
	else
		return true;

	return false;
}

bool Grid::beenThere(const int lr, const int lc)
{
	if (DEBUG)	cout << "beenThere(const int lr, const int lc)\n";

	for (int i = 0; i < BOARD_TOTAL; i++)
	{
		if (inputLog[i] == ((lr - 1) * (COLS)+lc))
			return true;
		else if (inputLog[i] == -1)
		{
			inputLog[i] = (lr - 1) * (COLS)+lc;
			break;
		}
	}

	return false;
}

class UserInterface : public Grid
{
protected:
	int input = 0;
	int userRow;
	char userCol;
	int flagCount = 0;
public:
	UserInterface();
	void menu();
	//
};


UserInterface::UserInterface()
{
	menu();
}

void UserInterface::menu()
{
	while (input != 5)	//input
	{
		cout << "Menu\n"
			<< "1. show Grid\n"
			<< "2. add flag (#)\n"
			<< "3. confirm site (i.e. A7)\n"
			<< "4. \n"
			<< "5. Quit Game\n"
			<< "===================================\n;"
			<< "Input command: ";
		cin >> input;

		switch (input)
		{
		case 1:

			break;
		case 2:
			if (flagCount < MINES_TOTAL)
			{

				flagCount++;

			}
		case 3:
		case 4:
		case 5:
			cout << "Thank you for playing\n";
			break;
		default:
			cout << "Input not valid. Please re enter command.\n";
			break;
		}
	}


	return;
}

/*
void buildBoard()
{
	if (DEBUG)	cout << "buildBoard()\n";
	unsigned seed = time(0);

	srand(seed);

	int mineLocationX[MINES_TOTAL];
	int mineLocationY[MINES_TOTAL];
	for (int i = 0; i < MINES_TOTAL; i++)
	{
		mineLocationX[i] = (rand() % (COLS - 1 + 1)) + 1;
		mineLocationY[i] = (rand() % (ROWS - 1 + 1)) + 1;
		//check for location uniqueness.
		for (int y = i - 1; y >= 0; y--)
		{
			if (mineLocationY[i] == mineLocationY[y])
			{
				if (mineLocationX[i] == mineLocationX[y])
					i--;
			}
		}
		if (DEBUG)
		{
			cout << i + 1 << " X: " << mineLocationX[i] << ", ";
			cout << "Y: " << mineLocationY[i] << endl;
		}
	}

	if (DEBUG)	cout << endl;

	//organize mine list
	for (int i = 1; i < MINES_TOTAL; i++)
	{
		if (mineLocationY[i] < mineLocationY[i - 1])
		{

			int tempY = mineLocationY[i - 1];
			int tempX = mineLocationX[i - 1];

			mineLocationY[i - 1] = mineLocationY[i];
			mineLocationX[i - 1] = mineLocationX[i];

			mineLocationY[i] = tempY;
			mineLocationX[i] = tempX;

			for (int i2 = i; i2 >= 1; i2--)
			{
				if (mineLocationY[i2] < mineLocationY[i2 - 1])
				{
					int tempY = mineLocationY[i2 - 1];
					int tempX = mineLocationX[i2 - 1];

					mineLocationY[i2 - 1] = mineLocationY[i2];
					mineLocationX[i2 - 1] = mineLocationX[i2];

					mineLocationY[i2] = tempY;
					mineLocationX[i2] = tempX;
				}
				else if (mineLocationY[i2] == mineLocationY[i2 - 1])
				{
					if (mineLocationX[i2] < mineLocationX[i2 - 1])
					{
						int tempY = mineLocationY[i2 - 1];
						int tempX = mineLocationX[i2 - 1];

						mineLocationY[i2 - 1] = mineLocationY[i2];
						mineLocationX[i2 - 1] = mineLocationX[i2];

						mineLocationY[i2] = tempY;
						mineLocationX[i2] = tempX;
					}
				}
			}
		}
		else if (mineLocationY[i] == mineLocationY[i - 1])
		{
			if (mineLocationX[i] < mineLocationX[i - 1])
			{
				int tempY = mineLocationY[i - 1];
				int tempX = mineLocationX[i - 1];

				mineLocationY[i - 1] = mineLocationY[i];
				mineLocationX[i - 1] = mineLocationX[i];

				mineLocationY[i] = tempY;
				mineLocationX[i] = tempX;
			}
		}

	}
	if (DEBUG)
	{
		for (int i = 0; i < MINES_TOTAL; i++)
		{
			cout << "X: " << mineLocationX[i] << ", ";
			cout << "Y: " << mineLocationY[i] << endl;
		}
		cout << endl;
	}
	//create map
	char mine = '*';
	int mineIndex = 0;
	char map[BOARD_TOTAL];
	for (int i = 0; i < BOARD_TOTAL; i++)
	{

		if (DEBUG) cout << "total: " << mineLocationY[mineIndex] * COLS - COLS + mineLocationX[mineIndex] << endl;
		if (mineLocationY[mineIndex] * COLS - COLS + mineLocationX[mineIndex] == i)
		{
			map[i] = mine;
			mineIndex = (mineIndex + 1) % MINES_TOTAL;

			if (DEBUG) cout << "mineIndex: " << mineIndex << endl;

		}
		else
		{
			map[i] = 0;
		}
	}
	//show map
	if (DEBUG)
	{
		cout << "\tA\tB\tC\tD\tE\tF\tG\tH" << endl;
		int rowNum = 1;
		for (int i = 0; i < BOARD_TOTAL; i++)
		{
			if (i % COLS == 0)
				cout << endl << endl << rowNum++ << '\t';

			cout << map[i] << '\t';
		}
		cout << endl;
	}
	return;
}
*/
//Function Prototypes.
void ProgramGreeting();

int main()
{
	ProgramGreeting();
	//buildBoard();
	Grid grid;
	return 0;
}

void ProgramGreeting()
{
	cout << "Welcome to the Minefield search program game\n"
		<< "Author: Tyler Kenney\n\n";
}