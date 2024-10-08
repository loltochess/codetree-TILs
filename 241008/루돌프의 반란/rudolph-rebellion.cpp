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

typedef struct santa {
	int state, y, x, score;
}Santa;

Santa s[34];
int land[54][54];
int n, m, p, c, d, pid, deer_y, deer_x, sy, sx;
const int dy[] = { -1,-1,0,1,1,1,0,-1 };
const int dx[] = { 0,1,1,1,0,-1,-1,-1 };
int dead_cnt;

int calc(int deer_y, int deer_x, int sy, int sx) {
	return abs(deer_y - sy) * abs(deer_y - sy) + abs(deer_x - sx) * abs(deer_x - sx);
}

bool is_out(int ny, int nx) {
	if (ny<1 || ny>n || nx<1 || nx>n) return true;
	return false;
}

void interaction(int pid, int dir) {
	int y = s[pid].y, x = s[pid].x;
	int ny = y + dy[dir], nx = x + dx[dir];
	if (is_out(ny, nx)) {
		s[pid].state = -1; return;
	}
	s[pid].y = ny; s[pid].x = nx;
	if (land[ny][nx]) {
		interaction(land[ny][nx], dir);
	}
	land[ny][nx] = pid;
	return;
}

void collision(int flag, int pid, int dir) {
	int y = s[pid].y, x = s[pid].x;
	int ny, nx;
	if (flag == 0) {
		ny = y + c * dy[dir];
		nx = x + c * dx[dir];
		s[pid].score += c;
	}
	else {
		ny = y + d * dy[dir];
		nx = x + d * dx[dir];
		s[pid].score += d;
	}
	s[pid].y = ny; s[pid].x = nx;
	land[y][x] = 0;
	if (is_out(ny, nx)) {
		s[pid].state = -1; return;
	}
	if (land[ny][nx] == 0) {
		land[ny][nx] = pid;
		return;
	}
	else { // 연쇄작용
		interaction(land[ny][nx], dir);
		land[ny][nx] = pid;
	}
}

void deer_move() {
	int min_dist = 1e9;
	int min_sy = -1, min_sx = -1;
	for (int i = 1; i <= p; i++) {
		if (s[i].state == -1) continue;
		int sy = s[i].y, sx = s[i].x;
		int dist = calc(deer_y, deer_x, sy, sx);
		if (min_dist > dist || (min_dist == dist && min_sy < sy) || (min_dist == dist && min_sy == sy && min_sx < sx)) {
			min_dist = dist; min_sy = sy; min_sx = sx;
		}
	}
	int min_dir = -1;
	for (int i = 0; i < 8; i++) {
		int ny = deer_y + dy[i], nx = deer_x + dx[i];
		if (is_out(ny, nx)) continue;
		int dist = calc(ny, nx, min_sy, min_sx);
		if (min_dist > dist) {
			min_dist = dist; min_dir = i;
		}
	}
	deer_y += dy[min_dir]; deer_x += dx[min_dir];
	if (land[deer_y][deer_x]) {
		int pid = land[deer_y][deer_x];
		s[pid].state = 2; // paralyzed
		collision(0, pid, min_dir); // {flag, pid, min_dir}
	}
}

void santa_move(int pid) {
	int min_dir = -1;
	int y = s[pid].y, x = s[pid].x;
	int min_dist = calc(deer_y, deer_x, y, x);
	for (int i = 0; i < 8; i += 2) {
		int ny = y + dy[i], nx = x + dx[i];
		if (is_out(ny, nx) || land[ny][nx]) continue;
		int dist = calc(deer_y, deer_x, ny, nx);
		if (min_dist > dist) {
			min_dist = dist; min_dir = i;
		}
	}
	if (min_dir == -1) return;
	int ny = y + dy[min_dir];
	int nx = x + dx[min_dir];
	if (ny == deer_y && nx == deer_x) {
		s[pid].state = 1; // paralyzed
		land[y][x] = 0;
		s[pid].y = ny; s[pid].x = nx;
		collision(1, pid, (min_dir + 4) % 8);
	}
	else {
		land[y][x] = 0;
		land[ny][nx] = pid;
		s[pid].y = ny;
		s[pid].x = nx;
	}
	return;
}

void print() {
	cout << "deer y : " << deer_y << " , " << "deer_x : " << deer_x << '\n';
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i == deer_y && j == deer_x) cout << 77 << ' ';
			else cout << land[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}


void go() {
	deer_move();
	for (int i = 1; i <= p; i++) {
		if (s[i].state == -1) continue;

		if (s[i].state > 0) s[i].state--;
		else if (s[i].state == 0) santa_move(i);
		//print();
	}
}


int main(){
	init();
	cin >> n >> m >> p >> c >> d;
	cin >> deer_y >> deer_x;
	for (int i = 0; i < p; i++) {
		cin >> pid >> sy >> sx;
		s[pid] = { 0,sy,sx,0 };
		land[sy][sx] = pid;
	}
	//print();
	for (int i = 0; i < m; i++) {
		go();
		if (dead_cnt == p) break;
		for (int i = 1; i <= p; i++) {
			if (s[i].state >= 0) s[i].score++;
		}
		/*for (int i = 1; i <= 4; i++) {
			cout << i << " : " << s[i].state << " " << s[i].y << " " << s[i].x << " " << s[i].score << '\n';
		}*/
	}
	for (int i = 1; i <= p; i++) {
		cout << s[i].score << ' ';
	}
	cout << '\n';
	return 0;
}