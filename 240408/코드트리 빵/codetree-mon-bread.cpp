#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;

int land_debug[18][18];
int land[18][18]; // 디버깅용 0 공간 1 베이스캠프 2 편의점 10 막힘
int finished[18];
int blocked[18][18];
pair<int, int> now_location [34];
pair<int, int> object[34]; // 사람 최대 30명
vector<pair<int,int>> base; // 사람보다 많을수있음
int will_block[18][18];
int t = 1;
int n, m;

const int dy[4] = {-1, 0, 0, 1};
const int dx[4] = {0, -1, 1, 0};

void debug_print() {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (blocked[i][j]) land_debug[i][j] = -1;
			else if (will_block[i][j]) land_debug[i][j] = -2;
			else land_debug[i][j] = 0; // 나머지는 초기화
		}
	}
	for (int i = 1; i <= m; i++) {
		if(i <= t) land_debug[now_location[i].first][now_location[i].second] = i; // 사람 위치
		land_debug[object[i].first][object[i].second] = 100 * i; // 편의점은 100배수
	}

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << land_debug[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int calc_dist(pair<int, int> a, pair<int, int> b) {
	int sy = a.first, sx = a.second, ey = b.first, ex = b.second;
	queue<pair<int,int>> q;
	int visited[18][18];
	memset(visited, 0, sizeof(visited));
	q.push(a);
	visited[sy][sx] = 1;
	while (q.size()) {
		pair<int, int> point = q.front();
		q.pop();
		int y = point.first, x = point.second;
		for (int i = 0; i < 4; i++) {
			int ny = y + dy[i];
			int nx = x + dx[i];
			if (blocked[ny][nx]) continue;
			if (visited[ny][nx]) continue;
			if (ny < 1 || ny > n || nx < 1 || nx > n) continue;
			visited[ny][nx] = visited[y][x] + 1;
			q.push({ ny, nx });
		}
	}
	//cout << "bfs result " << a.first << " " << a.second << " to " << b.first << " " << b.second << "\n";
	//for (int i = 1; i <= n; i++) {
	//	for (int j = 1; j <= n; j++) {
	//		cout << visited[i][j] << " ";
	//	}
	//	cout << "\n";
	//}
	//cout << "\n";
	return visited[ey][ex]; //못찾았으면 0 리턴
}

void move() { 
	for (int i = 1; i <= m; i++) {
		if (finished[i]) continue; //끝난사람
		if (t <= i) continue; //아직 출발안한사람
		int y = now_location[i].first, x = now_location[i].second;
		int dist = calc_dist(now_location[i], object[i]);
		for (int j = 0; j < 4; j++) {
			int ny = y + dy[j];
			int nx = x + dx[j];
			if (blocked[ny][nx]) continue;
			if (ny < 1 || ny > n || nx < 1 || nx > n) continue;
			pair<int, int> new_point = make_pair(ny, nx);
			int new_dist = calc_dist(new_point, object[i]); // 시작 끝
			if (new_dist == 0) continue;
			if (new_dist < dist) {
				dist = new_dist; now_location[i] = new_point;
			}
			// 도달했을 때
			if (now_location[i] == object[i]) {
				will_block[ny][nx] = true;
				finished[i] = true; 
				now_location[i] = { -1, -1 };
				break;
			}
		}
	}
	return;
}

void block() { // will block을 blocked 시키자
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (will_block[i][j]) {
				if (blocked[i][j] == true) continue; // 이미 block된거면 의미없음
				int flag = true;
				for (pair<int, int> person : now_location) {
					if (person == make_pair(i,j)) flag = false; // 사람이있잖아
				}
				if (flag) blocked[i][j] = true;
			}
		}
	}
	return;
}

void base_go() {
	if (t > m) return;
	int idx = t;
	pair<int, int> store = object[idx];
	int y = store.first, x = store.second;
	int min_dist = 10000;
	pair<int, int> min_base = { -1, -1 };
	for (int i = 0; i < base.size(); i++) {
		int by = base[i].first, bx = base[i].second;
		if (!blocked[by][bx]) {
			int dist = calc_dist(base[i], store); // 시작 끝
			if (dist == 0) continue;
			int flag = false;
			if (dist < min_dist) flag = true;
			else if (dist == min_dist) {
				if (base[i].first < min_base.first) flag = true;
				else if (by == min_base.first) {
					if (bx < min_base.second) flag = true;
				}
			}
			if (flag) {
				min_dist = dist; min_base = base[i];
			}
		}
	}
	// 들어갔어!
	now_location[idx] = min_base;
	will_block[min_base.first][min_base.second] = true;
	return;
}

bool check() {
	for (int i = 1; i <= m; i++) {
		if (!finished[i]) return false;
	}
	return true;
}

int main() {
	//freopen("input.txt", "r", stdin);
	
	cin >> n >> m;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> land[i][j];
			if (land[i][j] == 1) base.push_back({ i,j });
		}
	}
	for (int i = 1; i <= m; i++) {
		int y, x;
		cin >> y >> x;
		object[i] = make_pair(y, x);
	}

	while (1) {
		if (t >= 10) return 2;
		//cout << t << " turn starts" << "\n";
		//debug_print();
		move();
		block();
		base_go();
		if (check()) break;
		//debug_print();
		t++;
	}

	cout << t << "\n";

	return 0;
}