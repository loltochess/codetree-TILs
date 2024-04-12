#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string.h>
#include <stdio.h>

using namespace std;

int land[24][24];
int visited[24][24];

const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0, 1, 0, -1 };

typedef struct line {
	pair<int, int> start;
	pair<int, int> end;
} line ;

vector<line> Line;

int n, m, k;
int ret = 0;

void find_line(line& L) {
	queue<pair<int, int>> q;
	pair<int, int> start = L.start;
	memset(visited, 0, sizeof(visited));
	q.push(start);
	visited[start.first][start.second] = true;
	while (q.size()) {
		pair<int, int> now = q.front(); q.pop();
		int y = now.first, x = now.second;
		for (int i = 0; i < 4; i++) {
			int ny = y + dy[i], nx = x + dx[i];
			if (ny<1 || ny > n || nx < 1 || nx > n) continue;
			if (visited[ny][nx]) continue;
			if (land[ny][nx] == 2) {
				visited[ny][nx] = true;
				q.push({ ny, nx });
				continue;
			}
			if (land[ny][nx] == 3) { // end find!
				L.end = make_pair(ny, nx);
				return;
			}
		}
	}
	return;
}

void move() {
	// start 랑 end만 update하면 아름답다!
	for (line &L : Line) {
		int sy = L.start.first, sx = L.start.second;
		for (int i = 0; i < 4; i++) {
			int sny = sy + dy[i], snx = sx + dx[i];
			if (sny < 1 || sny > n || snx < 1 || snx > n) continue;
			if (land[sny][snx] == 4) {
				L.start = make_pair(sny, snx);
				land[sny][snx] = 1;
				land[sy][sx] = 2;
				break;
			}
		}

		int ey = L.end.first, ex = L.end.second;
		for (int i = 0; i < 4; i++) {
			int eny = ey + dy[i], enx = ex + dx[i];
			if (eny < 1 || eny > n || enx < 1 || enx > n) continue;
			if (land[eny][enx] == 2) {
				L.end = make_pair(eny, enx);
				land[eny][enx] = 3;
				land[ey][ex] = 4;
			}
		}
	}
}

void change_dir(pair<int,int> start) {
	for (line& L : Line) {
		if (L.start == start) {
			pair<int, int> temp = start;
			L.start = L.end;
			L.end = temp;
			land[L.start.first][L.start.second] = 1;
			land[L.end.first][L.end.second] = 3;
			return;
		}
	}
	return;
}

int find_index(pair<int, int> point) {

	if (land[point.first][point.second] == 1) {
		change_dir(point);
		return 1;
	}
	
	memset(visited, 0, sizeof(visited));
	queue<pair<int, int>> q;
	pair<int, int> start;
	q.push(point);
	visited[point.first][point.second] = 1;
	while (q.size()) {
		pair<int, int> now = q.front(); q.pop();
		int y = now.first, x = now.second;
		for (int i = 0; i < 4; i++) {
			int ny = y + dy[i], nx = x + dx[i];
			if (ny< 1 || ny>n || nx<1 || nx>n) continue;
			if (visited[ny][nx]) continue;
			if (land[ny][nx] == 2) {
				visited[ny][nx] = visited[y][x] + 1;
				q.push(make_pair(ny, nx));
			}
			if (land[ny][nx] == 1) { // 시작점찾음.
				start = make_pair(ny, nx);
				visited[ny][nx] = visited[y][x] + 1;
				break;
			}
		}
	}
	// 리턴 전에 방향을 바꾸자!
	change_dir(start);

	return visited[start.first][start.second];
}

void throw_ball(int turn) {
	if (turn <= n) {
		for (int j = 1; j <= n; j++) {
			if (land[turn][j] >= 1 && land[turn][j] <= 3) {
				int index = find_index(make_pair(turn,j));
				ret += index * index;
				return;
			}
		}
	}
	else if (turn > n && turn <= 2 * n) {
		for (int i = n; i >= 1; i--) {
			if (land[i][turn - n] >= 1 && land[i][turn - n] <= 3) {
				int index = find_index(make_pair(i, turn - n));
				ret += index * index;
				return;
			}
		}
	}
	else if (turn > 2*n && turn <= 3*n) {
		turn -= 2 * n;
		for (int j = n; j >= 1; j--) {
			if (land[n + 1 - turn][j] >= 1 && land[n + 1 - turn][j] <= 3) {
				int index = find_index(make_pair(n + 1 - turn, j));
				ret += index * index;
				return;
			}
		}
	}
	else if(turn > 3*n && turn <= 4*n){
		turn -= 3 * n;
		for (int i = 1; i <= n; i++) {
			if (land[i][n + 1 - turn] >= 1 && land[i][n + 1 - turn] <= 3) {
				int index = find_index(make_pair(i, n + 1 - turn));
				ret += index * index;
				return;
			}
		}
	}
}

void print_debug() {
	cout << "Lines : start_y start_x end_y end_x" << "\n";
	for (line L : Line) {
		cout << L.start.first << " " << L.start.second << " " << L.end.first << " " << L.end.second << "\n";
	}
	cout << " Land " << "\n";

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << land[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "ret : " << ret << "\n";
	cout << "\n";

	return;
}

int main() {

	//freopen("input.txt", "r", stdin);

	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> land[i][j];
			if (land[i][j] == 1) {
				Line.push_back({ { i, j }, { -1, -1 } });
			}
		}
	}

	for (line &L : Line) { // 줄 완성하기!
		find_line(L);
	}

	int turn = 1;

	//print_debug();
	while (k--) {
		//cout << turn << " turn starts " << "\n";
		move();
		//cout << "move end" << "\n";
		//print_debug();
		throw_ball(turn);
		//cout << turn << " throw end" << "\n";
		//print_debug();
		turn++;
		if (turn == 4 * n + 1) turn = 1;
	}
	cout << ret << "\n";
}