#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include <queue>

using namespace std;

typedef struct dice {
	int now, up, right;
}dice;

int ret;

dice D = { 2, 1, 3 };

int n, m;
int land[22][22];
const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0, 1, 0, -1 };
int connect_count[22][22];


void roll_up() {
	dice temp = D;
	D.now = 7 - temp.up;
	D.up = temp.now;
}

void roll_down() {
	dice temp = D;
	D.now = temp.up;
	D.up = 7 - temp.now;
}

void roll_right() {
	dice temp = D;
	D.up = 7 - temp.right;
	D.right = temp.up;
}

void roll_left() {
	dice temp = D;
	D.up = temp.right;
	D.right = 7 - temp.up;
}


typedef struct position {
	int y, x, dir;
};

position dice_position = { 1, 1, 1 };


void roll_dice() {
	int dir = dice_position.dir;
	if (dir == 0) {
		if (dice_position.y == 1) {
			dice_position.y = 2; dice_position.dir = 2;
			roll_down();
		}
		else {
			dice_position.y--;  roll_up();
		}
	}
	else if (dir == 1) {
		if (dice_position.x == n) {
			dice_position.x = n-1; dice_position.dir = 3;
			roll_left();
		}
		else {
			dice_position.x++;  roll_right();
		}
	}
	else if (dir == 2) {
		if (dice_position.y == n) {
			dice_position.y = n - 1; dice_position.dir = 0;
			roll_up();
		}
		else {
			dice_position.y++;  roll_down();
		}
	}
	else if (dir == 3) {
		if (dice_position.x == 1) {
			dice_position.x = 2; dice_position.dir = 1;
			roll_right();
		}
		else {
			dice_position.x--;  roll_left();
		}
	}
}

void calc_count() {
	int visited[22][22];
	memset(visited, 0, sizeof(visited));
	int cnt = 1;
	queue<pair<int,int> > q;
	int number = land[dice_position.y][dice_position.x];
	q.push({ dice_position.y, dice_position.x });
	visited[dice_position.y][dice_position.x] = true;
	while (q.size()) {
		pair<int, int> now = q.front(); q.pop();
		int y = now.first, x = now.second;
		for (int i = 0; i < 4; i++) {
			int ny = y + dy[i], nx = x + dx[i];
			if (ny<1 || ny>n || nx<1 || nx>n) continue;
			if (visited[ny][nx]) continue;
			if (land[ny][nx] != number) continue;
			visited[ny][nx] = true;
			cnt++;
			q.push(make_pair(ny, nx));
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (visited[i][j]) connect_count[i][j] = cnt;
		}
	}
	return;
}

void change_dir() {
	if (7 - D.up > land[dice_position.y][dice_position.x]) {
		dice_position.dir = (dice_position.dir + 1) % 4;
	}
	else if (7 - D.up < land[dice_position.y][dice_position.x]) {
		dice_position.dir = (dice_position.dir + 3) % 4;
	}
	return;
}

void print_debug() {
	cout << "up now right" << " \n";
	cout << D.up << " " << D.now << " " << D.right << "\n";
	cout << "y  x  dir" << "\n";
	cout << dice_position.y << " " << dice_position.x << " " << dice_position.dir << "\n";
	cout << "\n";
}

int main() {
	//freopen("input.txt", "r", stdin);
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> land[i][j];
		}
	}

	//print_debug();
	while (m--) {
		//cout << " ROLL " << "\n";
		roll_dice();
		//print_debug();
		if(connect_count[dice_position.y][dice_position.x] == 0 ) calc_count();
		int score = land[dice_position.y][dice_position.x] * connect_count[dice_position.y][dice_position.x];
		//cout << score << " 점 얻음\n";
		ret += score;
		//print_debug();
		change_dir();
		//print_debug();
	}
	cout << ret << "\n";
}