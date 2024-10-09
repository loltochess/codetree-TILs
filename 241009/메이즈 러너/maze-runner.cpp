#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
void init() { ios_base::sync_with_stdio(0); cin.tie(NULL); cout.tie(NULL); }

int land[14][14];
int land_temp[14][14];
pii out;
int n, m, k;
int man_land[14][14];
int man_temp[14][14];
int ret;
int cnt;
int r, c, h;
const int dy[] = { -1, 1, 0, 0 }; // 상 하 우 좌
const int dx[] = { 0, 0, 1, -1 };

bool is_out(int y, int x) {
	if (y<1 || y > n || x < 1 || x > n) return true;
	return false;
}

int calc(int y, int x) {
	return abs(out.first - y) + abs(out.second - x);
}

void print() {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << land[i][j] << ',' << man_land[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

void man_move(int y, int x) {
	int dist = calc(y, x);
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		if (is_out(ny, nx) || land[ny][nx]) continue;
		if (ny == out.first && nx == out.second) { // 탈출
			cnt++; ret += man_land[y][x]; return;
		}
		if (dist > calc(ny,nx)) {
			ret += man_land[y][x]; 
			man_temp[ny][nx] += man_land[y][x]; return;
		}
	}
	// 움직일수없을때
	man_temp[y][x] += man_land[y][x];
	return;
}

void make_square(int _r, int _c) {
	int _h = max(out.first - _r, out.second - _c);
	for (int i = _h; max(_r, _c) + i <= n; i++) {
		if (h <= i) return; // h가 작은게 우선 나머지는 자동정렬
		for (int y = _r; y <= _r + i; y++) {
			for (int x = _c; x <= _c + i; x++) {
				if (man_land[y][x]) {
					r = _r; c = _c; h = i; return;
				}
			}
		}
	}
	return;
}

void cw90() {
	int out_changed = false;
	for (int i = 0; i <= h; i++) {
		for (int j = 0; j <= h; j++) {
			if (out_changed == 0 && r + i == out.first && c + j == out.second) {
				out.first = r + j;
				out.second = c + h - i;
				out_changed = true;
				//cout << out.first << " : " << out.second << '\n';
			}
			land_temp[r + j][c + h - i] = land[r + i][c + j];
			man_temp[r + j][c + h - i] = man_land[r + i][c + j];
			if (land_temp[r + j][c + h - i]) land_temp[r + j][c + h - i]--;
		}
	}
	for (int i = 0; i <= h; i++) {
		for (int j = 0; j <= h; j++) {
			land[r + i][c + j] = land_temp[r + i][c + j];
			man_land[r + i][c + j] = man_temp[r + i][c + j];
		}
	}
}

void go() {
	// 1. man_move
	memset(man_temp, 0, sizeof(land));
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (man_land[i][j]) man_move(i, j);
		}
	}
	memcpy(man_land, man_temp, sizeof(man_temp));
	if (cnt == m) return;
	// 2. make_square
	h = 1e9;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i <= out.first && j <= out.second) {
				make_square(i, j);
			}
		}
	}
	//cout << r << " : " << c << " : " << h << '\n';
	// 3. rotate
	cw90();
	//print();
	return;
}

int main() {
	init();
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> land[i][j];
		}
	}
	for (int i = 0; i < m; i++) {
		int y, x; cin >> y >> x;
		man_land[y][x]++;
	}
	cin >> out.first >> out.second;
	for (int i = 0; i < k; i++) {
		go();
		if (cnt == m) break;
	}
	cout << ret << '\n' << out.first << ' ' << out.second << '\n';
	return 0;
}