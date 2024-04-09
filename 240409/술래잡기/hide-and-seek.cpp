#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdio.h>

using namespace std;

typedef struct runner { // 도망자
	int id, live, y, x, dir;
} runner;

vector<runner> Runner;

runner chaser;

int visited_cw[104][104]; // 시계방향 이동관리
int visited_ccw[104][104]; // 반시계방향 이동관리
bool chaser_cw = true;

int tree[104][104]; //트리위치
int n, m, h, k;
int ret;
int turn = 1;
const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0, 1, 0, -1 };

int calc_dist(runner a, runner b) {
	return abs(a.y - b.y) + abs(a.x - b.x);
}

void runner_move(runner& r) {
	int now_dir = r.dir;
	int ny = r.y + dy[now_dir];
	int nx = r.x + dx[now_dir];
	if (ny < 1 || ny > n || nx < 1 || nx > n) {
		now_dir = (now_dir + 2) % 4;
		r.dir = now_dir; // 방향만 update
		runner_move(r); //방향만바꾸면 조건문 벗어남
		return;
	}

	// 술래를 만났다면 그대로
	if (ny == chaser.y && nx == chaser.x) return;
	// 아니라면 업데이트
	r.y = ny, r.x = nx;
	return;
}

void run() {
	for (runner& r : Runner){
		if (r.live == 0) continue;
		if (calc_dist(r, chaser) <= 3) {
			runner_move(r);
		}
	}
}

void __init__cw() {
	memset(visited_cw, 0, sizeof(visited_cw));
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			visited_cw[i][j] = 0;
		}
	}
	visited_cw[n / 2 + 1][n / 2 + 1] = 1;
	return;
}

void __init__ccw() {
	memset(visited_ccw, 0, sizeof(visited_ccw));
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			visited_ccw[i][j] = 0;
		}
	}
	visited_ccw[1][1] = true;
	return;
}

void chaser_move_cw() {
	int new_dir = chaser.dir;
	int ny = chaser.y + dy[chaser.dir], nx = chaser.x + dx[chaser.dir];
	bool change_dir = true;
	for (int i = 0; i < 4; i++) {
		if (i == (chaser.dir + 2) % 4) continue;
		//왔던 방향 제외 3방향 모두 방문안한 곳이면 틀어야함.
		int ky = ny + dy[i], kx = nx + dx[i];
		if (visited_cw[ky][kx]) { // 방문했다면 dir 바꿀필요없음
			change_dir = false; break;
		}
	}
	if (change_dir) new_dir = (new_dir + 1) % 4;
	chaser.y = ny;
	chaser.x = nx;
	chaser.dir = new_dir;
	visited_cw[chaser.y][chaser.x] = true;
	return;
}

void chaser_move_ccw() {
	int new_dir = chaser.dir;
	int ny = chaser.y + dy[chaser.dir], nx = chaser.x + dx[chaser.dir];
	bool change_dir = false;
	int ky = ny + dy[new_dir], kx = nx + dx[new_dir];
	if (ky < 1 || ky > n || kx < 1 || kx > n) change_dir = true;
	if (visited_ccw[ky][kx]) change_dir = true;
	if (change_dir) new_dir = (new_dir + 3) % 4;
	chaser.y = ny;
	chaser.x = nx;
	chaser.dir = new_dir;
	visited_ccw[chaser.y][chaser.x] = true;
	return;
}

void catch_runner () {
	int y = chaser.y, x = chaser.x, dir = chaser.dir;
	vector<pair<int, int>> will_catch;
	will_catch.push_back({y,x});
	will_catch.push_back({y + dy[dir], x + dx[dir] });
	will_catch.push_back({ y + 2 * dy[dir], x + 2 * dx[dir] });
	
	for (pair<int, int> point : will_catch) {
		int y = point.first, x = point.second;
		if (tree[y][x] == 1) continue;
		for (runner& r : Runner) {
			if (r.live == 0) continue; // 이미 죽었어 ㅜㅜ
			if (y == r.y && x == r.x) {
				// live 0 으로 바꾸고 나머지 디버깅위해 -1로 전환
				r.live = 0; r.y = -1, r.x = -1, r.dir = -1;
				ret += turn;
			}
		}
	}
}

void chase() {
	if (chaser_cw) { // 시계방향이동
		chaser_move_cw();
		if (chaser.y == 1 && chaser.x == 1) { // (1,1) 도달
			chaser.dir = 2;
			__init__ccw(); 
			chaser_cw = false;
		}
	}
	else {
		chaser_move_ccw();
		if (chaser.y == (n / 2 + 1) && chaser.x == (n / 2 + 1)) {
			chaser.dir = 0;
			__init__cw();
			chaser_cw = true;
		}
	}
	catch_runner();
}

void print_debug() {
	cout << "술래 : y  x  dir \n" << chaser.y << " " << chaser.x << " " << chaser.dir << "\n";
	cout << "도망자" << "\n";
	cout << "id  live  y  x  dir \n";
	for (runner r : Runner) {
		cout << r.id << " " << r.live << " " << r.y << " " << r.x << " " << r.dir << "\n";
	}
	cout << "\n";
}

int main() {

	//freopen("input.txt", "r", stdin);

	cin >> n >> m >> h >> k;
	chaser = { 0, 1, n / 2 + 1, n / 2 + 1, 0 };
	for (int i = 1; i <= m; i++) {
		// x, y 를 편의상 y,x 로 바꿨습니다 ^^
		int y, x, d;
		cin >> y >> x >> d;
		Runner.push_back({ i, 1, y, x, d }); // id live y x dir
	}
	for (int i = 1; i <= h; i++) {
		int y, x;
		cin >> y >> x;
		tree[y][x] = 1;
	}

	__init__cw();
	__init__ccw();
	//print_debug();
	while (turn <= k) {
		//cout << turn << "턴 시작" << "\n";
		run();
		chase();
		//print_debug();
		turn++;
	}
	cout << ret << "\n";
	return 0;
}