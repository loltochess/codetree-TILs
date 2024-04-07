#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

int ret = 0;
int land [14][14];
int finished [14];
pair<int, int> out;
vector<pair<int, int>> v; // 참가자의 위치
int N, M, K;
const int dy[4] = { -1, 1, 0, 0 }; //상 하 우 좌 
const int dx[4] = { 0, 0, 1, -1 };


struct square {
	int rs, re, cs, ce;
};

typedef struct square square;

void print_map(square sq) {
	for (int i = sq.rs; i <= sq.re; i++) {
		for (int j = sq.cs; j <= sq.ce; j++) {
			cout << land[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void rotation90(square sq) {
	int temp [14][14];
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			temp[i][j] = land[i][j];
		}
	}
	//print_map(sq);

	//temp 에 회전배열 저장하고 out 이동
	int len = sq.re - sq.rs + 1;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			temp[sq.rs + i][sq.cs + j] = land[sq.re - j][sq.cs + i];
		}
	}
	//사람 이동
	for (int k = 0; k < M; k++) {
		if (finished[k]) continue;
		pair<int, int> a = v[k];
		int y = a.first, x = a.second;
		if (sq.rs <= y && y <= sq.re && sq.cs <= x && x <= sq.ce) {
			//cout << y << " " << x << "rotates" << "\n";
			v[k].first = x - sq.cs + sq.rs;
			v[k].second = sq.re - y + sq.cs;
		}
	}
	// 물체이동
	int y = out.first, x = out.second;
	out.first = x - sq.cs + sq.rs;
	out.second = sq.re - y + sq.cs;

	// 최종 복사 + 장애물감소
	for (int i = sq.rs; i <= sq.re; i++) {
		for (int j = sq.cs; j <= sq.ce; j++) {
			land[i][j] = temp[i][j];
			if (land[i][j] > 0) land[i][j]--;
		}
	}

	//print_map(sq);
}


square find_square() {
	int out_y = out.first, out_x = out.second;
	int rs, cs;
	int len = 2; // 한변 2에서 시작(1인 경우는 없음)
	bool flag = false;
	while (1) {
		if(len == 20) return { 0, 0, 0, 0 }; // error
		// new rs, new cs -> 시작 좌상단 지점
		int best_rs = out_y - len + 1;
		int best_cs = out_x - len + 1;
		// 좌상단 r좌표 작은순 -> c좌표 작은 순 으로 시작 좌상단 지점 본다.
		for (int i = 0; i < len; i++) { 
			for (int j = 0; j < len; j++) {
				int new_rs = best_rs + i;
				int new_cs = best_cs + j;
				if (new_rs < 1 || new_cs < 1) continue;
				for (int k = 0; k < M; k++) {
					if (finished[k]) continue;
					int y = v[k].first, x = v[k].second;
					if (new_rs <= y && y <= new_rs + len - 1 && new_cs <= x && x <= new_cs + len - 1) {
						// 조건 맞았으므로 이걸로 리턴하면 끝
						return { new_rs, new_rs + len - 1, new_cs, new_cs + len - 1 };
					}
				}
			}
		}
		len++;
	}
	return { 0, 0, 0, 0 }; // error
}

void user_turn() {
	int out_y = out.first, out_x = out.second;
	for (int idx = 0; idx < M; idx++) {
		if (finished[idx]) continue; // 끝났으면 통과
		pair<int, int> a = v[idx];
		int y = a.first, x = a.second;
		int now_dist = abs(y - out_y) + abs(x - out_x);
		for (int i = 0; i < 4; i++) {
			int ny = y + dy[i], nx = x + dx[i];
			if (ny < 1 || ny >N || nx < 1 || nx > N) continue;
			if (land[ny][nx] > 0) continue;

			if (ny == out_y && nx == out_x) { // 탈출조건달성
				v[idx].first = ny; v[idx].second = nx;
				ret++;
				finished[idx] = 1; break;
			}

			int dist = abs(ny - out_y) + abs(nx - out_x);
			if (dist < now_dist) { // 이렇게하면 상하를 우선시하고, 한번만 움직임.
				v[idx].first = ny; v[idx].second = nx;
				now_dist = dist;
				ret++;
				break;
			}
		}
	}

	//cout << "현재 유저 위치 : " << "\n";
	//for (int i = 0; i < M; i++) {
	//	if (finished[i]) continue;
	//	pair<int, int> a = v[i];
	//	cout << a.first << " " << a.second << "\n";
	//}
}

bool all_finished() {
	for (int i = 0; i < M; i++) {
		if (!finished[i]) return false;
	}
	return true;
}


int main() {

	//freopen("input.txt", "r", stdin);
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> land[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		int y, x;
		cin >> y >> x;
		v.push_back({ y,x });
	}
	int out_y, out_x;
	cin >> out_y >> out_x;
	out = { out_y, out_x };

	while (K--) {
		user_turn();
		//print_map({ 1, N, 1, N });
		if (all_finished()) break;
		square sq = find_square();
		rotation90(sq);
		/*cout << "out 좌표 : " << out.first << " " << out.second << "\n";
		cout << "현재까지 이동거리 " << ret << "\n";*/
	}

	cout << ret << "\n";
	cout << out.first << " " << out.second << "\n";
	return 0;
}