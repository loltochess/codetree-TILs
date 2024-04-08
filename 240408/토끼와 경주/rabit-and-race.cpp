#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

typedef struct rabbit rabbit;

vector<rabbit> table; // {pid, d, count, score, y, x}
int Q;
int N, M, P;
const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0, 1, 0, -1 };

typedef long long LL;

struct rabbit {
	LL pid, dist, count;
	LL score;
	LL y, x;
};

void print_rabbit(vector<rabbit> table) {
	cout << " pid  dist  count  score  y  x   " << "\n";
	for (rabbit rb : table) {
		cout << rb.pid << " " << rb.dist << " " << rb.count << " " << rb.score << " " << rb.y << " " << rb.x << "\n";
	}
	cout << "\n";
}

bool choose_rabbit (rabbit a, rabbit b) {
	if (a.count != b.count) return a.count < b.count;
	if (a.y + a.x != b.y + b.x) return a.y + a.x < b.y + b.x;
	if (a.y != b.y) return a.y < b.y;
	if (a.x != b.x) return a.x < b.x;
	return a.pid < b.pid;
}

pair<LL, LL> move(rabbit a, LL dir) {
	LL delta_y, delta_x;
	LL ny = a.y, nx = a.x;
	delta_y = abs(a.dist * dy[dir]);
	delta_x = abs(a.dist * dx[dir]);
	//delta_y = delta_y % (N + ny - 1);
	//delta_x = delta_x % (M + nx - 1);
	while (1) { // delta y 또는 delta x 가 0이 될 때까지
		if (dir == 0) { // ny  <- a.y 에서 얼마나 더 상승할거냐?
			if (delta_y <= ny - 1) {
				ny -= delta_y;
				break;
			}
			else {
				delta_y -= ny - 1; // row = 1에서 반대로 턴!
				ny = 1;
				dir = (dir + 2) % 4; //반대방향으로 더 줄여보자.
			}
		}
		if (dir == 1) { // nx <- a.x 에서 얼마나더 오른쪽가냐?
			if (delta_x <= M - nx) {
				nx += delta_x;
				break;
			}
			else {
				delta_x -= M - nx;
				nx = M;
				dir = (dir + 2) % 4;
			}
		} 
		if (dir == 2) { // ny <- a.y에서 얼마나 더 아래쪽으로가냐?
			if (delta_y <= N - ny) {
				ny += delta_y;
				break;
			}
			else {
				delta_y -= N - ny;
				ny = N;
				dir = (dir + 2) % 4;
			}
		}
		if (dir == 3) { // nx <- a.x 에서 얼마나 더 왼쪽으로?
			if (delta_x <= nx - 1) {
				nx -= delta_x;
				break;
			}
			else {
				delta_x -= nx - 1;
				nx = 1;
				dir = (dir + 2) % 4;
			}
		}
	}
	return make_pair(ny, nx);
}

bool compare_point(pair<LL, LL>a, pair<LL, LL>b) {
	if (a.first + a.second != b.first + b.second) return a.first + a.second > b.first + b.second;
	else if (a.first != b.first) return a.first > b.first;
	return a.second > b.second;
}

bool compare_S(rabbit a, rabbit b) {
	if (a.y + a.x != b.y + b.x) return a.y + a.x > b.y + b.x;
	if (a.y != b.y) return a.y > b.y;
	if (a.x != b.x) return a.x > b.x;
	return a.pid > b.pid;
}

pair<LL, LL>  choose_point(rabbit a) {
	LL y = a.y, x = a.x;
	vector<pair<LL, LL>> v;
	for (int i = 0; i < 4; i++) {
		pair<LL,  LL> new_point = move(a, i);
		v.push_back(new_point);
	}
	sort(v.begin(), v.end(), compare_point);
	return v[0];
}

void race(LL K, LL S) {
	//cout << "race begins" << "\n";
	//print_rabbit(table);
	//cout << "\n";

	while (K--) {
		sort(table.begin(), table.end(), choose_rabbit);
		table[0].count++;
		pair<int, int > moved_point = choose_point(table[0]);
		table[0].y = moved_point.first;
		table[0].x = moved_point.second;
		for (int i = 1; i < table.size(); i++) {
			table[i].score += table[0].y + table[0].x;
		}
		//print_rabbit(table);
	}

	sort(table.begin(), table.end(), compare_S);
	table[0].score += S;
	return;
}


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	//freopen("input.txt", "r", stdin);

	cin >> Q;
	int command;
	cin >> command; // 100 이라 치고
	cin >> N >> M >> P;
	for (int j = 0; j < P; j++) {
		LL pid, d;
		cin >> pid >> d;
		table.push_back({ pid, d , 0, 0, 1, 1 });
	}
	for (int j = P; j <= Q; j++) {
		cin >> command;
		//cout << command << "\n";
		if (command == 200) {
			LL K, S;
			cin >> K >> S;
			race(K, S);
		}
		else if (command == 300) {
			LL pid_t, L;
			cin >> pid_t >> L;
			for (int j = 0; j < table.size(); j++) {
				if (table[j].pid == pid_t) {
					table[j].dist *= L;
				}
			}
		}
		else if (command == 400) {
			LL ret = -1;
			for (rabbit r : table) {
				LL now = r.score;
				if (now > ret) ret = now;
			}
			cout << ret << "\n";
		}
	}
	return 0;
}