#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>

using namespace std;

int land[24][24];
int n, m, k, c;

const int tree_dy[4] = { -1, 0, 1, 0 };
const int tree_dx[4] = { 0, 1, 0, -1 };

const int chem_dy[4] = { -1, 1, 1, -1 };
const int chem_dx[4] = { 1, 1, -1, -1 };

long long ret = 0;

int near_tree(int y, int x) {
	int cnt = 0;
	for (int i = 0; i < 4; i++) {
		int ny = y + tree_dy[i];
		int nx = x + tree_dx[i];
		if (ny<1 || ny>n || nx <1 || nx > n) continue;
		if (land[ny][nx] > 0) cnt++;
	}
	return cnt;
}

void grow() {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (land[i][j] > 0) {
				land[i][j] += near_tree(i, j);
			}
		}
	}
	return;
}

void give_tree(vector<pair<int,int>> &tree, int y, int x) {
	for (int i = 0; i < 4; i++) {
		int ny = y + tree_dy[i];
		int nx = x + tree_dx[i];
		if (ny<1 || ny>n || nx<1 || nx>n) continue;
		if (land[ny][nx] == 0) {
			tree.push_back({ ny, nx });
		}
	}
}

void breed() {
	vector<pair<int, int>> trees[24][24];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (land[i][j] > 0) {
				give_tree(trees[i][j], i, j);
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (land[i][j] > 0) {
				if ((int)trees[i][j].size() == 0) continue;
				int tree = land[i][j] / (int)trees[i][j].size();
				for (pair<int, int> point : trees[i][j]) {
					land[point.first][point.second] += tree;
				}
			}
		}
	}
	return;
}

int count_kill_trees(int y, int x) {
	int cnt = 0;
	cnt += land[y][x]; // 그 자리 더하고 
	for (int i = 0; i < 4; i++) { // X자를 더하자
		for (int j = 1; j <= k; j++) { 
			// 0포함하면 중복덧셈임. 막히면 바로 break 해야함.
			int ny = y + j * chem_dy[i];
			int nx = x + j * chem_dx[i];
			if (ny < 1 || ny>n || nx<1 || nx>n) break;
			if (land[ny][nx] <= 0) {
				// 실제로 뿌릴땐 여기까지 뿌려줘야함..
				cnt += 0; // 주의!
				break;
			}
			cnt += land[ny][nx];
		}
	}
	return cnt;
}

bool cmp(pair<int, int> a, pair<int, int> b) {
	if (a.first != b.first) return a.first > b.first;
	return a.second > b.second;
}

void kill() {
	int max_tree = -1;
	pair<int, int> max_point = { 0, 0 };
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (land[i][j] > 0) {
				int cnt = count_kill_trees(i, j);
				if (max_tree < cnt) {
					max_tree = cnt; max_point = make_pair(i, j);
				}
				else if (max_tree == cnt) {
					if (cmp(max_point, make_pair(i, j))) {
						max_point = make_pair(i, j);
					}
				}
			}
		}
	}
	// ret 에 죽여버릴 tree 수 더해주기
	ret += max_tree > 0 ? max_tree : 0;
	// 포인트를 찾고 실제로 kill 하자.
	int y = max_point.first, x = max_point.second;
	land[y][x] = -(c+1); // c+1년 후 0으로 회복된다!
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= k; j++) {
			int ny = y + j * chem_dy[i];
			int nx = x + j * chem_dx[i];
			if (ny<1 || ny>n || nx<1 || nx>n) break;
			if (land[ny][nx] < -(c + 1)) break; // 벽에는 뿌리지 않음에 유의한다.
			if (land[ny][nx] <= 0) { //제초제가 뿌려졌거나, 나무가 없는 땅
				land[ny][nx] = -(c+1); break; // 뿌려주고 break
			}
			//나무가 있는 칸
			land[ny][nx] = -(c+1); // 나무있는 칸은 다음 칸도 전파
		}
	}
	return;
}

void heal() {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (land[i][j] < 0) land[i][j]++;
		}
	}
}

void print_debug() {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << land[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int main() {

	//freopen("input.txt", "r", stdin);
	cin >> n >> m >> k >> c;

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> land[i][j];
			if (land[i][j] == -1) land[i][j] = -2000; // -1000 보다 작으면 벽이다.
		}
	}
	
	int turn = 1; // 디버깅용

	while (m--) {
		//cout << turn << "turn starts " << "\n";
		//print_debug();
		grow();
		breed();
		kill();
		heal();
		//print_debug();
		//turn++;
	}

	cout << ret << "\n";
	return 0;
}