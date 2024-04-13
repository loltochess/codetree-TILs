#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
#include <map>

using namespace std;
int land[30][30];
vector<pair<int, int>> snake;
int n, m, d;
const int dy[4] = { 0, 1, 0 ,-1 }; // 우하좌상
const int dx[4] = { 1, 0, -1, 0 };
const int snake_dy[4] = { 0, 1, 0, -1 }; // 좌 하 우 상
const int snake_dx[4] = { -1, 0, 1, 0 };
int monster_line[700];
int visited[30][30];
int p;
int ret; 

map<pair<int, int>, int > point_2_idx;

void make_snake() {
	pair<int, int> middle_point = make_pair(n / 2 + 1, n / 2 + 1);
	int dir = 0;
	visited[middle_point.first][middle_point.second] = true;
	pair<int, int> point = middle_point;
	while (1) {
		int y = point.first, x = point.second;
		int ny = y + snake_dy[dir], nx = x + snake_dx[dir];
		bool change_dir = true;
		for (int i = 0;i < 4;i++) {
			if (i == (dir + 2) % 4) continue;
			int my = ny + snake_dy[i], mx = nx + snake_dx[i];
			if (visited[my][mx] == 1) change_dir = false;
		}
		if (change_dir) dir = (dir + 1) % 4;
		point = make_pair(ny, nx);
		snake.push_back(point);
		visited[ny][nx] = true;
		if (point.first == 1 && point.second == 1) break;
	}
	return;
}

void __init__() {
	make_snake();
	for (int i = 0;i < snake.size();i++) {
		monster_line[i] = land[snake[i].first][snake[i].second];
		point_2_idx[snake[i]] = i;
	}
}

int find_zero() {
	for (int i = 0;i < snake.size();i++) {
		if (monster_line[i] == 0) return i;
	}
	return -1;
}

int find_non_zero(int zero_idx) {
	for (int i = zero_idx + 1;i < snake.size();i++) {
		if (monster_line[i] != 0) return i;
	}
	return -1;
}

void zero_sort() {
	while (1) {
		int zero_idx = find_zero(); // 가장 앞에 있는 zero
		int non_zero_idx = find_non_zero(zero_idx); // 그거 뒤에 있는 nonzero
		if (non_zero_idx == -1) break; // 못찾으면 -1 리턴
		int temp = monster_line[zero_idx];
		monster_line[zero_idx] = monster_line[non_zero_idx];
		monster_line[non_zero_idx] = temp;
	}
	return;
}

bool find_four_attack() {
	bool is_attack = false;
	int cnt = 1;
	int now_num = monster_line[0];
	int end_idx;
	for (int i = 1;i < snake.size();i++) {
		int num = monster_line[i];
		if (num == 0) {
			end_idx = i; break;
		}
		if (now_num == num) {
			cnt++;
		}
		else {
			if (cnt >= 4) {
				is_attack = true;
				for (int j = i - 1; j >= i - cnt; j--) {
					ret += monster_line[j];
					monster_line[j] = 0;
				}
			}
			now_num = num;
			cnt = 1;
		}
	}
	// 마지막 line에 대한 해결
	if (cnt >= 4) {
		is_attack = true;
		for (int i = end_idx - 1; i >= end_idx - cnt; i--) {
			ret += monster_line[i];
			monster_line[i] = 0;
		}
	}

	if (is_attack == 0) return false;
	return true;
}

void tower_attack() {
	cin >> d >> p;
	// 1. 일자로 tower attack
	int y = n / 2 + 1, x = n / 2 + 1;
	while (p--) {
		int ny = y + dy[d], nx = x + dx[d];
		int idx = point_2_idx[make_pair(ny, nx)];
		ret += monster_line[idx];
		monster_line[idx] = 0;
		y = ny, x = nx;
	}
	// 2. zero_sort
	// 3. find_four_attack 반복
	while (1) {
		zero_sort();
		if(find_four_attack() == false) break;
	}
	return;
}

void get_new_line() {
	int temp[700];
	memset(temp, 0, sizeof(temp));

	vector<pair<int, int>> v;
	
	int cnt = 1;
	int now_num = monster_line[0];

	for (int i = 1;i < snake.size();i++) {
		int num = monster_line[i];
		if (num == 0) {
			v.push_back(make_pair(cnt, now_num)); break;
		}
		if (now_num == num) cnt++;
		else {
			v.push_back(make_pair(cnt, now_num));
			now_num = num; cnt = 1;
		}
	}

	int idx = 0;
	for (pair<int, int> a : v) {
		int cnt = a.first, num = a.second;
		temp[idx] = cnt, temp[idx + 1] = num;
		idx += 2;
	}

	memset(monster_line, 0, sizeof(monster_line)); // 아예 새로운 배열 만들기
	for (int i = 0;i < snake.size();i++) {
		monster_line[i] = temp[i];
	}
	return;
}

void update_land() {
	memset(land, 0, sizeof(land));
	for (int i = 0;i < snake.size();i++) {
		pair<int, int> point = snake[i];
		land[point.first][point.second] = monster_line[i];
	}
}

void print_debug() {
	cout << "land print " << "\n";
	for (int i = 1;i <= n;i++) {
		for (int j = 1;j <= n;j++) {
			cout << land[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	cout << "monster line" << "\n";
	for (int i = 0;i < snake.size();i++) {
		cout << monster_line[i] << " ";
	}
	cout << "\n";
}

int main() {
	//freopen("input.txt", "r", stdin);
	cin >> n >> m;
	for (int i = 1;i <= n;i++) {
		for (int j = 1;j <= n;j++) {
			cin >> land[i][j];
		}
	}

	__init__(); // 초기 snake, line 생성
	//print_debug();
	int turn = 1;
	while (m--) {
		tower_attack();
		get_new_line();
		update_land();
		//print_debug();
	}
	cout << ret << "\n";
}