#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <string.h>

using namespace std;

vector<int> line;
vector<pair<int, int> > put_num;

int land[35][35];
int visited[35][35];
int rs, re, cs, ce, end_y, end_x;
const int dy[4] = { -1, 0, 1, 0 }; // 반시계
const int dx[4] = { 0, -1, 0, 1 };


// 최소값에 1씩추가
// 제곱수로 벡터 늘려주고 zero padding 실시, 끝 번호는 기억한다.
// 12345 6 7891011
// land[6][6]에 첫 원소 넣고 snake 형성한다.
// 마지막 원소가 66보다 행이 크고거나 같고 열이 크거나 같을 때까지
// 시계방향 돌린다
// 도우 눕히고 반으로 두 번 접고
// 다시 눕혀준다.

int n, k;

void put_mil() {
	int min_mil = 4000;
	for (int i = 0; i < line.size(); i++) {
		if (min_mil > line[i]) min_mil = line[i];
	}
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == min_mil) line[i]++;
	}
}

void make_sequence(int dir) {
	int y = 11, x = 11;
	visited[y][x] = true;
	put_num.push_back({ y,x });
	for (int i = 1; i < 100; i++) {
		int ny = y + dy[dir], nx = x + dx[dir];
		int turn_right = true;
		for (int j = 0; j < 4; j++) {
			if (j == (dir + 2) % 4) continue;
			int my = ny + dy[j], mx = nx + dx[j];
			if (visited[my][mx]) turn_right = false;
		}
		if (turn_right) dir = (dir + 1) % 4;
		y = ny, x = nx;
		visited[y][x] = true;
		//cout << y << " " << x << " " << dir << "\n";
		put_num.push_back({ y,x });
	}
}

void rotate90() {
	int temp[35][35];
	memset(temp, 0, sizeof(temp));
	for (int i = 0; i < 35; i++) {
		for (int j = 0; j < 35; j++) {
			temp[i][j] = land[i][j];
		}
	}
	for (int i = 0; i < 35; i++) {
		for (int j = 0; j < 35; j++) {
			land[i][j] = temp[34 - j][i];
		}
	}
}

void snake() {
	pair<int, int> point;
	for (int i = 0; i < line.size(); i++) {
		point = put_num[i];
		land[point.first][point.second] = line[i];
		//cout << point.first << " " << point.second << "\n";
	}
	// 마지막 포인트가 오른쪽에 오도록 하자.
	int y = point.first, x = point.second;
	//while (1) {
	//	if (x > 11 && land[y - 1][x] == 0) {
	//		if (land[y - 1][x - 1] == 0) { // 아다리맞게 n x n 일 경우
	//			end_x = x, end_y = y + 2 * (11 - y);
	//			rotate90(); return;
	//		}
	//		else break;
	//	}
	//}
	if (land[y][x + 1] == 0) return;
	for (int ny = y; land[ny][x] != 0; ny++) { // 우하단 지점 찾기
		y = ny;
	}
	// 우하단에서 올라가는 점들 옮겨주기
	for (int ny = y - 1; land[ny][x] != 0; ny--) {
		land[y][x + y - ny] = land[ny][x];
		end_y = y, end_x = x + y - ny;
		land[ny][x] = 0;
	}

	return;
}

void find_square() {
	for (int i = 0; i < 35; i++) {
		if (land[i][11] != 0) {
			rs = i; break;
		}
	}
	for (int i = rs; i < 35; i++) {
		if (land[i][11] == 0) {
			re = i - 1; break;
		}
	}
	for (int j = 0; j < 35; j++) {
		if (land[11][j] != 0) {
			cs = j; break;
		}
	}
	for (int j = cs; j < 35; j++) {
		if (land[11][j] == 0) {
			ce = j - 1; break;
		}
	}
}

void push() {
	int cnt[35][35];
	int visited[35][35];
	memset(visited, 0, sizeof(visited));
	memset(cnt, 0, sizeof(cnt));
	for (int i = rs; i <= re; i++) {
		for (int j = cs; j <= ce; j++) {
			for (int dir = 0; dir < 4; dir++) {
				int ny = i + dy[dir], nx = j + dx[dir];
				if (land[ny][nx] > 0) {
					if (land[i][j] > land[ny][nx])
						cnt[i][j] -= abs(land[i][j] - land[ny][nx]) / 5;
					else
						cnt[i][j] += abs(land[i][j] - land[ny][nx]) / 5;
				}
			}
		}
	}
	if (end_x != ce) {
		for (int j = ce + 1; j <= end_x; j++) {
			for (int dir = 1; dir <= 3; dir += 2) {
				int ny = re, nx = j + dx[dir];
				if (land[ny][nx] > 0) {
					if (land[re][j] > land[ny][nx]) {
						cnt[re][j] -= abs(land[re][j] - land[ny][nx]) / 5;
					}
					else {
						cnt[re][j] += abs(land[re][j] - land[ny][nx]) / 5;
					}
				}
			}
		}
		for (int j = ce + 1; j <= end_x; j++) {
			land[re][j] += cnt[re][j];
		}
	}
	for (int i = rs; i <= re; i++) {
		for (int j = cs; j <= ce; j++) {
			land[i][j] += cnt[i][j];
		}
	}
	return;
}

void flatten() {
	line.clear();
	for (int c = cs; c <= ce; c++) {
		for (int r = re; r >= rs; r--) {
			line.push_back(land[r][c]);
		}
	}
	if (land[re][ce + 1] != 0) { // 꼬랑지 남아있을 경우
		for (int c = ce + 1; land[re][c] != 0; c++) {
			line.push_back(land[re][c]);
		}
	}
	return;
}


void two_fold() {
	int half_size = line.size() / 2;
	vector<int> reversed_1;
	for (int i = 0; i < half_size; i++) {
		reversed_1.push_back(line[half_size - 1 - i]);
		//cout << reversed_1[i] << " ";
	}
	//cout << "\n";
	int world[4][50];
	int cnt_world[4][50];
	memset(world, 0, sizeof(world));
	memset(cnt_world, 0, sizeof(cnt_world));
	for (int i = 0; i < half_size / 2; i++) {
		world[2][i] = reversed_1[half_size / 2 + i];
		world[3][i] = line[half_size + half_size / 2 + i];
		world[0][i] = line[half_size + half_size / 2 - 1 - i];
		world[1][i] = reversed_1[half_size / 2 - 1 - i];
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < half_size / 2; j++) {
			for (int dir = 0; dir < 4; dir++) {
				int ny = i + dy[dir], nx = j + dx[dir];
				if (ny < 0 || ny >= 4 || nx < 0 || nx >= line.size() / 4) continue;
				if (world[ny][nx] > 0) {
					if (world[i][j] > world[ny][nx]) {
						cnt_world[i][j] -= abs(world[i][j] - world[ny][nx]) / 5;
					}
					else {
						cnt_world[i][j] += abs(world[i][j] - world[ny][nx]) / 5;
					}
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < line.size() / 4; j++) {
			world[i][j] += cnt_world[i][j];
			//cout << world[i][j] << " ";
		}
		//cout << "\n";
	}
	int idx = 0;
	for (int j = 0; j < line.size() / 4; j++) {
		for (int i = 3; i >= 0; i--) {
			line[idx] = (world[i][j]); idx++;
		}
	}
	return;
}


void print_debug() {
	cout << "rs  re  cs  ce end_y end_x" << "\n";
	cout << rs << " " << re << " " << cs << " " << ce << " " << end_y << " " << end_x << "\n";

	cout << "line : ";
	for (int a : line) {
		cout << a << " ";
	}
	cout << "\n";


	for (int i = 0; i <=35; i++) {
		for (int j = 0; j <=35; j++) {
			cout << land[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

bool check() {
	int max_num = -1;
	int min_num = 100000;
	for (int a : line) {
		if (a > max_num) max_num = a;
		if (a < min_num) min_num = a;
	}
	return max_num - min_num <= k;
}


int main() {

	//freopen("input.txt", "r", stdin);

	cin >> n >> k;

	int fold_num = 0;
	int fold_len = 1;
	int temp = n;
	while (temp > fold_len) {
		temp -= fold_len;
		fold_num++;
		fold_len++;
	}
	int dir;
	if (fold_num % 4 == 0) dir = 3;
	else if (fold_num % 4 == 1) dir = 2;
	else if (fold_num % 4 == 2) dir = 1;
	else dir = 0;
	//cout << fold_num << " " << dir << "\n";
	make_sequence(dir);

	for (int i = 1; i <= n; i++) {
		int num;
		cin >> num;
		line.push_back(num);
	}
	int turn = 1;

	vector<int> line;

	while (turn) {
		put_mil();
		snake();
		find_square();
		//print_debug();
		push();
		flatten();
		//print_debug();
		two_fold();
		snake();
		find_square();
		//print_debug();
		push();
		if (check()) break;
		turn++;
	}
	cout << turn << "\n";
	return 0;
}