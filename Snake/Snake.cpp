#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <vector>

#define width 47
#define height 23

using namespace std;

class FoodPosition {
public:
	int pos_x;
	int pos_y;

	FoodPosition() {
		srand(time(NULL));
		pos_x = (rand() % (width - 2)) + 1;
		pos_y = (rand() % (height - 2)) + 1;
	}
};

class SnakePosition {
public:
	int pos_x;
	int pos_y;
	char direction = 'w';
	SnakePosition(int x, int y) {
		pos_x = x;
		pos_y = y;
	}
};

class SnakeBody {
public:
	vector<SnakePosition> body;

	void addsize(SnakePosition& sp) {
		body.emplace_back(sp);
	}
};

bool containbody(SnakeBody* sb, int i, int j) {
	for (unsigned int n = 0; n < sb->body.size(); n++) {
		if (i == sb->body[n].pos_y && j == sb->body[n].pos_x)
			return true;
	}

	return false;
}

bool collideBody(SnakePosition* snakehead, SnakeBody* sb) {
	for (unsigned int i = 0; i < sb->body.size() - 1; i++) {		//-1 to exclude head
		if (snakehead->pos_x == sb->body[i].pos_x && snakehead->pos_y == sb->body[i].pos_y) {
			return true;
		}
	}

	return false;
}

bool foodCollideBody(FoodPosition* fp, SnakeBody* sb) {
	for (unsigned int i = 0; i < sb->body.size(); i++) {
		if (fp->pos_x == sb->body[i].pos_x && fp->pos_y == sb->body[i].pos_y) {
			return true;
		}
	}

	return false;
}

void frame(FoodPosition* fpos, SnakeBody* sb, int score) {
	cout << "SCORE : " << score << "\n";
	/*cout << "pos X : " << fpos->pos_x << "\n";
	cout << "pos y : " << fpos->pos_y << "\n";*/

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0) {
				cout << "\t\t#";
			}
			else {
				if (i == 0 || i == height - 1 || j == width - 1) {
					cout << "#";
				}
				else if (i == fpos->pos_y && j == fpos->pos_x) {
					cout << "$";
				}
				else if (containbody(sb, i, j)) {
					cout << "o";
				}
				else {
					cout << " ";
				}
			}
		}
		cout << "\n";
	}
}

int main()
{
	SnakePosition* sp = new SnakePosition(23, 12);
	FoodPosition* fp = new FoodPosition();
	SnakeBody* sb = new SnakeBody();
	sb->addsize(*sp);
	int score = 0;
	bool gameover = false;

	while (!gameover) {
		Sleep(40);

		if (_kbhit()) {
			switch (_getch()) {
			case 'w':
				if (sb->body.size() == 1 || sb->body.size() > 1 && sp->direction != 's')
					sp->direction = 'w';
				break;
			case 's':
				if (sb->body.size() == 1 || sb->body.size() > 1 && sp->direction != 'w')
					sp->direction = 's';
				break;
			case 'a':
				if (sb->body.size() == 1 || sb->body.size() > 1 && sp->direction != 'd')
					sp->direction = 'a';
				break;
			case 'd':
				if (sb->body.size() == 1 || sb->body.size() > 1 && sp->direction != 'a')
					sp->direction = 'd';
				break;
			}
		}

		if (sp->direction != NULL) {
			//body follow head
			for (unsigned int i = 0; i < sb->body.size() - 1; i++) {
				sb->body[i] = sb->body[i + 1];
			}
		}

		//head change direction
		if (sp->direction == 'w') {
			sb->body[sb->body.size() - 1].pos_y--;
		}
		else if (sp->direction == 's') {
			sb->body[sb->body.size() - 1].pos_y++;
		}
		else if (sp->direction == 'a') {
			sb->body[sb->body.size() - 1].pos_x--;
		}
		else if (sp->direction == 'd') {
			sb->body[sb->body.size() - 1].pos_x++;
		}

		SnakePosition* snakehead = new SnakePosition(sb->body[sb->body.size() - 1].pos_x, sb->body[sb->body.size() - 1].pos_y);

		//crash body
		if (collideBody(snakehead, sb)) {
			gameover = true;
		}

		//hit wall
		if (snakehead->pos_x == 0 || snakehead->pos_x == width - 1) {
			if (sp->direction == 'a') {
				snakehead = new SnakePosition(width - 2, snakehead->pos_y);
			}
			else {
				snakehead = new SnakePosition(1, snakehead->pos_y);
			}

			sb->body[sb->body.size() - 1] = *snakehead;
		}
		else if (snakehead->pos_y == 0 || snakehead->pos_y == height - 1) {
			if (sp->direction == 'w') {
				snakehead = new SnakePosition(snakehead->pos_x, height - 2);
			}
			else {
				snakehead = new SnakePosition(snakehead->pos_x, 1);
			}

			sb->body[sb->body.size() - 1] = *snakehead;
		}

		/*hit wall die
		if (snakehead->pos_y == 0 || snakehead->pos_y == height - 1 || snakehead->pos_x == 0 || snakehead->pos_x == width - 1) {
			gameover = true;
		}*/

		//get food
		if (snakehead->pos_x == fp->pos_x && snakehead->pos_y == fp->pos_y) {
			sb->addsize(*new SnakePosition(fp->pos_x, fp->pos_y));
			//to prevent food appear in snake body
			do {
				fp = new FoodPosition();
			} while (foodCollideBody(fp, sb));

			score++;
		}

		frame(fp, sb, score);

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });		//no need cls
	}
}