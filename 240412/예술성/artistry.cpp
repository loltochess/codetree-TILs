#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string.h>
#include <stdio.h>

using namespace std;

int touched[900][900];
int idx_land[35][35];
int land[35][35];
int number[900];
int counted_number[900];
int visited[35][35];
pair<int, int> start[900];
int idx;
int n;
int ret;

const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0 , 1, 0, -1 };

void __init__() {
	memset(touched, 0, sizeof(touched));
	memset(idx_land, 0, sizeof(idx_land));
	memset(number, 0, sizeof(number));
	memset(counted_number, 0, sizeof(number));
	memset(visited, 0, sizeof(visited));
	memset(start, 0, sizeof(start));
	idx = 0;
}

void first_dfs(int y, int x) {
	visited[y][x] = idx;
	idx_land[y][x] = idx;
	counted_number[idx]++;
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		if (ny < 1 || ny > n || nx < 1 || nx > n) continue;
		if (visited[ny][nx]) continue;
		if (land[ny][nx] == land[y][x]) {
			first_dfs(ny, nx);
		}
	}
}

void idx_setting() {
	for(int i=1;i<=n;i++){
		for (int j = 1; j <= n; j++) {
			if (!visited[i][j]) {
				idx++;
				start[idx] = make_pair(i, j);
				number[idx] = land[i][j];
				first_dfs(i, j);
			}
		}
	}
}

void second_dfs(int idx, pair<int, int> point) {
	visited[point.first][point.second] = true;
	int y = point.first, x = point.second;
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		if (ny<1 || ny>n || nx<1 || nx>n) continue;
		if (idx_land[ny][nx] != idx_land[y][x]) {
			int other_idx = idx_land[ny][nx];
			touched[idx][other_idx]++; // touch update 1->2 ++
		} // 2 번에서 dfs 돌릴 때 2->1 ++ 해줌.
		if (visited[ny][nx]) continue;
		if (idx_land[ny][nx] == idx_land[y][x]) {
			second_dfs(idx, make_pair(ny, nx));
		}
	}
}

void calc_touched() { // bfs로 touched 구하기
	// land의 수가 아닌, idx_land에서 idx가 맞닿는 것임에 유의!
	memset(visited, 0, sizeof(visited));
	for (int i = 1; i <= idx; i++) {
		pair<int, int> point = start[i];
		second_dfs(i, point);
	}
}

void calc_score() {
	for (int i = 1; i <= idx; i++) {
		for (int j = i + 1; j <= idx; j++) { // 겹치지않게 조합계산
			if (touched[i][j] != 0) {
				ret += (counted_number[i] + counted_number[j]) * number[i] * number[j] * touched[i][j];
			}
		}
	}
}

void rotate90(int temp_land[35][35], int rs, int cs, int len) {
	int re = rs + len - 1;
	int ce = cs + len - 1;
	
	vector<vector<int>> v(len, vector<int>(len, 0));
	
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			v[i][j] = temp_land[i + rs][j + cs];
		}
	}

	vector<vector<int>> temp(len, vector<int>(len, 0));
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			temp[i][j] = v[len - 1 - j][i];
		}
	}

	for (int i = rs; i <= re; i++) {
		for (int j = cs; j <= ce; j++) {
			temp_land[i][j] = temp[i - rs][j - cs];
		}
	}

	return;
}

void rotate_land() {
	int temp_land[35][35];
	memset(temp_land, 0, sizeof(temp_land));
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			temp_land[i][j] = land[i][j];
		}
	}
	int half = n / 2 + 1;
	
	for (int i = 1; i <= n/2; i++) {
		temp_land[i][half] = land[half][n + 1 - i];
		temp_land[half][half + i] = land[half + i][half];
		temp_land[half + i][half] = land[half][half-i];
		temp_land[half][i] = land[i][half];
	}

	rotate90(temp_land, 1, 1, half - 1);
	rotate90(temp_land, 1, 1 + half, half - 1);
	rotate90(temp_land, 1 + half, 1, half - 1);
	rotate90(temp_land, 1 + half, 1 + half, half - 1);


	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			land[i][j] = temp_land[i][j];
		}
	}
}

void print_debug() {
	cout << "land" << "\n";
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << land[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	cout << "idx_land " << "\n";
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cout << idx_land[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	cout << "idx to number" << "\n";
	for (int i = 1; i <= idx; i++) {
		cout << number[i] << " ";
	}
	cout << "\n";
	cout << "idx to number count " << "\n";
	for (int i = 1; i <= idx; i++) {
		cout << counted_number[i] << " ";
	}
	cout << "\n";
	cout << "idx  y  x" << "\n";
	for (int i = 1; i <= idx; i++) {
		cout << i << " : " << start[i].first << " " << start[i].second << "\n";
	}
	cout << "ret : " << ret << "\n";
	cout << "\n";
}

int main() {

	//freopen("input.txt", "r", stdin);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> land[i][j];
		}
	}

	int turn = 1;

	int t = 4;
	while (t--) {
		//cout << turn << " turn starts" << "\n";
		__init__();
		idx_setting();
		calc_touched();
		if(t == 4) calc_score();
		//print_debug();
		//cout << turn << " rotate starts " << "\n";
		rotate_land();
		__init__();
		idx_setting();
		calc_touched();
		calc_score();
		//print_debug();
		turn++;
	}

	cout << ret << "\n";

	return 0;
}