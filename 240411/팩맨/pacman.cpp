#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <queue>
#include <string.h>

using namespace std;


int dead[1000004];
int land[6][6]; 
int tort_land[6][6];
const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1 }; // 12시에서 시게반대
const int dx[8] = {0, -1, -1, -1, 0, 1, 1, 1 };
const int packman_dy[4] = { -1, 0, 1, 0 }; // 상좌하우
const int packman_dx[4] = { 0, -1, 0, 1 };


pair<int, int> packman;
typedef struct monster {
	int y, x, dir;
} monster;

monster dp[5][5][10]; // y, x, 회전방향

queue<monster> egg;
vector<monster> Monster;

int id = 1;
int m, t;
int r, c, d;

void make_egg() {
	// 지금까지 생성된 총 몬스터에 대해서  : 1 ~ Monster.size()-1
	for (int i = 1; i < Monster.size(); i++) { 
		if (dead[i]) continue;
		monster m = Monster[i];
		egg.push({ m.y, m.x, m.dir });
	}
	return;
}

monster next_move(monster& m) { // 최종적으로 vector를 바꿔줌에 유의
	int y = m.y, x = m.x, dir = m.dir;
	for (int i = 0; i < 8; i++) {
		int ndir = (dir + i) % 8;
		int ny = y + dy[ndir], nx = x + dx[ndir];
		if (ny < 1 || ny > 4 || nx < 1 || nx > 4) continue;
		if (tort_land[ny][nx]) continue;
		if (make_pair(ny, nx) == packman) continue;
		y = ny, x = nx, dir = ndir; break;
	}
	//못찾았으면 그대로리턴
	return { y, x, dir };
}

void monster_move() { // land에 기록해놓자!
	memset(land, 0, sizeof(land));
	for (int idx = 1; idx < Monster.size(); idx++) {
		if (dead[idx]) continue; 
		monster m = Monster[idx];
		m = dp[m.y][m.x][m.dir];
		Monster[idx] = m; // update
		land[m.y][m.x]++;
	}
	return;
}

void kill_monster(pair<int, int> packman) {
	int y = packman.first, x = packman.second;
	for (int i = 1; i < Monster.size(); i++) {
		if (!dead[i] && y == Monster[i].y && x == Monster[i].x) {
			dead[i] = 1; tort_land[y][x] = 3;
		}
	}
	return;
}

void packman_move() {
	int max_count = -1;
	pair<int, int> move[3];
	int y = packman.first, x = packman.second;
	for (int i = 0; i < 4; i++) {
		int ny_1 = y + packman_dy[i], nx_1 = x + packman_dx[i];
		if (ny_1 < 1 || ny_1 > 4 || nx_1 < 1 || nx_1 > 4) continue;
		for (int j = 0; j < 4; j++) {
			int ny_2 = ny_1 + packman_dy[j], nx_2 = nx_1 + packman_dx[j];
			if (ny_2 < 1 || ny_2 > 4 || nx_2 < 1 || nx_2 > 4) continue;
			for (int k = 0; k < 4; k++) {
				int ny_3 = ny_2 + packman_dy[k], nx_3 = nx_2 + packman_dx[k];
				if (ny_3 < 1 || ny_3 > 4 || nx_3 < 1 || nx_3 > 4) continue;
				int cnt = 0;
				cnt += land[ny_1][nx_1];
				cnt += land[ny_2][nx_2];
				if(make_pair(ny_1, nx_1) != make_pair(ny_3, nx_3) && make_pair(ny_2, nx_2) != make_pair(ny_3, nx_3))cnt += land[ny_3][nx_3];
				//cout << ny_1 << " " <<nx_1 << " " <<ny_2 << " " <<nx_2 <<" " << ny_3 << " " << nx_3 << " " << cnt << " " << "\n";
				if (max_count < cnt) {
					max_count = cnt;
					move[0] = { ny_1, nx_1 }, move[1] = { ny_2, nx_2 }, move[2] = { ny_3, nx_3 };
				}
			}
		}
	}
	packman = move[2]; // packman위치 업데이트
	for (int i = 0; i < 3; i++) {
		kill_monster(move[i]);
	}
	return;
}

void torture_disappear() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (tort_land[i][j]) tort_land[i][j]--;
		}
	}
}

void dup_monster() {
	while (egg.size()) {
		monster m = egg.front(); egg.pop();
		Monster.push_back(m);
	}
	return;
}

int count_monster() {
	int cnt = 0;
	for (int i = 1; i < Monster.size(); i++) {
		if (!dead[i]) cnt++;
	}
	return cnt;
}

void print_debug() {
	cout << "packman : " << packman.first << " " << packman.second << "\n";
	cout << "monsters" << "\n";
	for (int i = 1; i < Monster.size(); i++) {
		cout << dead[i] << " " << Monster[i].y << " " << Monster[i].x << " " << Monster[i].dir << "\n";
	}
	cout << "\n";
	return;
}

void calculate_dp() {
	memset(dp, -1, sizeof(dp));
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (int dir = 0; dir < 8; dir++) {
				monster m = { i, j, dir };
				m = next_move(m);
				dp[i][j][dir] = m;
			}
		}
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	cin >> m >> t;
	cin >> r >> c;
	Monster.push_back({ -1, -1, -1 }); // index 0으로 쓰레기값
	packman = make_pair(r, c);
	for (int i = 1; i <= m; i++) {
		cin >> r >> c >> d;
		Monster.push_back({ r, c, d-1 }); // dir = 0~7로 생각하자.
	}
	
	int turn = 1;
	while (t--) {
		//cout << turn << "turn starts" << "\n";
		calculate_dp();
		//print_debug();
		make_egg();
		monster_move();
		//print_debug();
		//for (int i = 1; i <= 4; i++) {
		//	for (int j = 1; j <= 4; j++) {
		//		cout << land[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
		packman_move();
		//print_debug();
		torture_disappear();
		dup_monster();
		//print_debug();
		turn++;
	}
	cout << count_monster() << "\n";
	return 0;
}