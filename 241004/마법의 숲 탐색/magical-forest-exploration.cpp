#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <string.h>
#include <math.h>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
void init() { ios_base::sync_with_stdio(0); cin.tie(NULL); cout.tie(NULL); }

typedef struct golem {
	int y, x, d;
}Golem;

Golem g[1004];
int land[80][80];
int spirit[1004]; // 정렬이 나오는 위치
const int dy[] = { -1,0,1,0 };
const int dx[] = { 0,1,0,-1 };
int r, c, k;
int idx;
pii check_down[] = { {1,-1},{2,0},{1,1} };
pii check_left[] = { {-1,-1},{0,-2},{1,-1},{1,-2},{2,-1} };
pii check_right[] = { {-1,1},{0,2},{1,1},{1,2},{2,1} };
int y, x, d;
int max_y;
int ret;

bool is_out(int y, int x) {
	if (y<0 || y > r+2 || x<1 || x > c) return true;
	return false;
}

bool checkDown() {
	for (pii a : check_down) {
		int ny = y + a.first, nx = x + a.second;
		if (is_out(ny, nx) || land[ny][nx]) return false;
	}
	return true;
}

bool checkLeft() {
	for (pii a : check_left) {
		int ny = y + a.first, nx = x + a.second;
		if (is_out(ny, nx) || land[ny][nx]) return false;
	}
	return true;
}

bool checkRight() {
	for (pii a : check_right) {
		int ny = y + a.first, nx = x + a.second;
		if (is_out(ny, nx) || land[ny][nx]) return false;
	}
	return true;
}

void paint() {
	land[y][x] = idx;
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		if (i == d) land[ny][nx] = 1e9 + idx;
		else land[ny][nx] = idx;
	}
}

void checkConnected() {
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		if (land[ny][nx] == idx || land[ny][nx] == 0) continue;
		int a = land[ny][nx];
		if (a > 1e9) a -= 1e9;
		max_y = max(max_y, spirit[a]);
	}
	return;
}

void go() {
	y = g[idx].y; x = g[idx].x; d = g[idx].d;
	while (1) {
		if (checkDown()) {
			y++;
		}
		else if (checkLeft()) {
			y++; x--; d = (d + 3) % 4;
		}
		else if (checkRight()) {
			y++; x++; d = (d + 1) % 4;
		}
		else break;
	}
	//cout << y << " : " << x << " : " << d << "\n";
	if (y <= 3) memset(land, 0, sizeof(land));
	else {
		paint();
		max_y = y +1 -2; // 2만큼 빼줘야함.
		y = y + dy[d]; x = x + dx[d];
		if(land[y][x] > 1e9) checkConnected();
		ret += max_y;
		//cout << idx << " : " << max_y << " 더해요~" << '\n';
		spirit[idx] = max_y;
	}
	return;
}

int main() {
	init();
	cin >> r >> c >> k;
	for (int i = 1; i <= k; i++) {
		int c, d;
		cin >> c >> d;
		g[i] = { 1,c,d };
	}
	for (int i = 1; i <= k; i++) {
		idx++; go();
	}
	cout << ret << '\n';
}