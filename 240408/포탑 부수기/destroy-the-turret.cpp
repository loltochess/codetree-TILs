#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <queue>

using namespace std;

int land[14][14];
int N, M, K;
int attack_record[14][14];
int attacked_record[14][14];

// turn 으로 attack attacked 포탑 파악
int turn = 1;

const int laser_dy[4] = { 0, 1, 0, -1 };
const int laser_dx[4] = { 1, 0, -1, 0 };
const int bomb_dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int bomb_dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

bool compare_attack_tower(pair< pair<int, int>, pair<int, int> > a, pair< pair<int, int>, pair<int, int> > b) {
	int a_i = a.second.first, a_j = a.second.second;
	int b_i = b.second.first, b_j = b.second.second;
	if (a.first.first != b.first.first) { // power
		return a.first.first < b.first.first;
	}
	if (a.first.second != b.first.second) { // attack_record
		return a.first.second > b.first.second;
	}
	int sum_a = a_i + a_j;
	int sum_b = b_i + b_j;
	if (sum_a != sum_b) { // row+col sum
		return (a.second.first + a.second.second) > (b.second.first + b.second.second);
	}
	return a.second.second > b.second.second; // col
}

bool compare_attacked_tower(pair< pair<int, int>, pair<int, int> > a, pair< pair<int, int>, pair<int, int> > b) {
	int a_i = a.second.first, a_j = a.second.second;
	int b_i = b.second.first, b_j = b.second.second;
	if (a.first.first != b.first.first) { // power
		return a.first.first > b.first.first;
	}
	if (a.first.second != b.first.second) { // attack_record
		return a.first.second < b.first.second;
	}
	int sum_a = a_i + a_j;
	int sum_b = b_i + b_j;
	if (sum_a != sum_b) { //row + col sum
		return (a.second.first + a.second.second) < (b.second.first + b.second.second);
	}
	return a.second.second < b.second.second; // col
}

pair<int,int> find_attack_tower() {
	// { {공격력, 최근공격}, {i, j} } vector생성
	vector< pair< pair<int, int>, pair<int, int> > > v; 
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (land[i][j]) {
				v.push_back({ {land[i][j], attack_record[i][j]}, {i,j}});
			}
		}
	}
	sort(v.begin(), v.end(), compare_attack_tower);

	//attack_record update
	int y = v[0].second.first;
	int x = v[0].second.second;
	attack_record[y][x] = turn;

	return v[0].second;
}

pair<int, int> find_attacked_tower() {
	vector< pair< pair<int, int>, pair<int, int> > > v;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (land[i][j]) {
				v.push_back({ {land[i][j], attack_record[i][j]}, {i,j} });
			}
		}
	}
	sort(v.begin(), v.end(), compare_attacked_tower);

	return v[0].second;
}

void print_graph(int temp[14][14]) {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			cout << temp[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void __init__(int visited[14][14], pair<int, int> prev[14][14]) {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			visited[i][j] = 0;
			prev[i][j] = { 0,0 };
		}
	}
	return;
}

bool laser(pair<int, int> from, pair<int, int> to) {
	int visited[14][14];
	pair<int,int> prev[14][14];
	__init__(visited, prev); // 초기화
	queue<pair<int, int>> q;
	q.push({ from.first, from.second });
	//cout << "queue 삽입 : " << from.first << " " << from.second << "\n";
	visited[from.first][from.second] = 1;
	//print_graph(visited);
	while (q.size()) {
		pair<int, int> a = q.front(); q.pop();
		int y = a.first, x = a.second;
		for (int i = 0; i < 4; i++) {
			int ny = y + laser_dy[i], nx = x + laser_dx[i];
			if (ny < 1) ny = N;
			if (ny > N) ny = 1;
			if (nx < 1) nx = M;
			if (nx > M) nx = 1;
			//cout << ny << " " << nx << " 확인" << "\n";
			if (land[ny][nx] == 0) continue;
			if (!visited[ny][nx]) {
				prev[ny][nx] = { y, x };
				visited[ny][nx] = visited[y][x] + 1;
				//cout << "queue 삽입 : " << ny << " " << nx << "\n";
				q.push({ ny, nx });
			}
		}
	}
	//print_graph(visited);
	// 경로못찾음
	if (!visited[to.first][to.second]) return false;

	// prev로 따라가면서 경로계산
	int damage = land[from.first][from.second];
	int half_damage = damage / 2;

	// to 제외 나머지 경로는 절반만 데미지들어감.
	pair<int, int> now = to;
	while(now != from){
		int y = now.first, x = now.second;
		pair<int, int> prev_tower = prev[y][x];
		int prev_y = prev_tower.first, prev_x = prev_tower.second;
		if (now == to) land[y][x] -= damage;
		else land[y][x] -= half_damage; // 데미지 계산
		if (land[y][x] < 0) land[y][x] = 0; //사망처리
		attacked_record[y][x] = turn; // 기록
		now = prev_tower; // 다음 타워로
	}
	return true;
}

void bomb(pair<int, int> from, pair<int, int> to) {
	int damage = land[from.first][from.second];
	int half_damage = damage / 2;
	//첫 데미지 계산
	land[to.first][to.second] -= damage;
	attacked_record[to.first][to.second] = turn;
	//8자 데미지 계산 from 은 제외
	for (int i = 0; i < 8; i++) {
		int y = to.first + bomb_dy[i];
		int x = to.second + bomb_dx[i];
		if (from == make_pair(y, x)) continue;
		if (y < 1) y = N;
		if (y > N) y = 1;
		if (x < 1) x = M;
		if (x > M) x = 1;
		if (land[y][x] > 0) {
			land[y][x] -= half_damage;
			attacked_record[y][x] = turn;
			if (land[y][x] < 0) land[y][x] = 0;
		}
	}
	return;
}

void heal() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (land[i][j] &&
				attack_record[i][j] != turn &&
				attacked_record[i][j] != turn) {
				land[i][j]++;
			}
		}
	}
}

int find_max() {
	int max = -1;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (max < land[i][j]) max = land[i][j];
		}
	}
	return max;
}

int main() {
	//freopen("input.txt", "r", stdin);
	
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			cin >> land[i][j];
		}
	}

	int turn = 1;
	while (turn <= K) {
		pair<int, int> from = find_attack_tower();
		pair<int, int> to = find_attacked_tower();
		land[from.first][from.second] += N + M;
		if (!laser(from, to)) bomb(from, to);
		heal();
		//print_graph(land);
		turn++;
	}
	cout << find_max() << "\n";
	return 0;
}