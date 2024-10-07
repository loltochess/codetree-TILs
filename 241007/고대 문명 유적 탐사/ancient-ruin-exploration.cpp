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

int land[7][7];
int temp[7][7];
int visited[7][7];
int erase[7][7];
int will_erase[7][7];
int ret;
int max_score;
int min_rot;
int _y, _x;
int k, m;
queue<int> q;
const int dy[] = { 0,1,0,-1 };
const int dx[] = { 1,0,-1,0 };

void game_init() {
	ret = 0; max_score = 0; min_rot = 4;
}

void cw90(int y, int x) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[y+j][x+2-i] = land[y + i][x + j];
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			land[y + i][x + j] = temp[y + i][x + j];
		}
	}
}

bool is_out(int y, int x) {
	if (y < 0 || y>4 || x < 0 || x > 4) return true;
	return false;
}

int dfs(int y, int x, int num) {
	int cnt = 1;
	visited[y][x] = true;
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		if (is_out(ny, nx) || visited[ny][nx] || land[ny][nx] != num) continue;
		cnt += dfs(ny, nx, num);
	}
	return cnt;
}

void paint(int y, int x, int num) {
	will_erase[y][x] = 1;
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		if (is_out(ny, nx) || will_erase[ny][nx] || land[ny][nx] != num) continue;
		paint(ny, nx, num);
	}
	return;
}

bool search_piece(int y, int x, int k) {
	int score = 0;
	memset(will_erase, 0, sizeof(will_erase));
	for (int num = 1; num <= 7; num++) {
		memset(visited, 0, sizeof(visited));
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (land[i][j] == num && !visited[i][j]) {
					int cnt = dfs(i, j, land[i][j]);
					if (cnt >= 3) {
						score += cnt;
						paint(i, j, land[i][j]);
					}
				}
			}
		}
	}
	if (score == 0) return false;
	if (max_score == score && min_rot > k) {
		_y = y; _x = x; min_rot = k; 
		memcpy(erase, will_erase, sizeof(will_erase));
	}
	else if (max_score < score) {
		_y = y; _x = x; min_rot = k; 
		memcpy(erase, will_erase, sizeof(will_erase));
		max_score = score;
	}
	return true;
}

void erase_add_piece() {
	for (int j = 0; j < 5; j++) { // 열이 작은거부터
		for (int i = 4; i >= 0; i--) { // 행이 큰거부터
			if (erase[i][j]) {
				land[i][j] = q.front();
				q.pop();
			}
		}
	}
}

void go() {
	for (int j = 1; j <= 3; j++) { // 열이 작은거부터
		for (int i = 1; i <= 3; i++) { // 행이 작은거부터
			for (int k = 1; k <= 3; k++) {
				cw90(i - 1, j - 1); // 좌상단 좌표를 기준점으로 시계방향회전
				search_piece(i - 1, j - 1, k); // 중심 기준과 좌상단 기준은 정렬차이없음.
			}
			cw90(i - 1, j - 1); // 270도에서 한번더 회전하면 원상복구
		}
	}
	if (max_score == 0) exit(0); // 유물 획득 불가능
	ret += max_score;
	for (int i = 0; i < min_rot; i++) {
		cw90(_y , _x);
	}
	erase_add_piece();
	while (1) {
		max_score = 0; // max_score >= 3 일때 erase가 이루어짐.
		if (!search_piece(0, 0, 0)) break;
		erase_add_piece();
		ret += max_score;
	}
	cout << ret << ' ';
	return;
}

int main() {
	init();
	cin >> k >> m;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> land[i][j];
		}
	}
	for (int i = 0; i < m; i++) {
		int num; cin >> num; q.push(num);
	}
	for (int i = 0; i < k; i++) {
		game_init();
		go();
	}
	return 0;
}