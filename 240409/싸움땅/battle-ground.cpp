#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;

int score[34];
vector<int> land[24][24];

int n, m, k;
const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

typedef struct player {
	int idx, x, y, power, gun, dir;
} player;

vector<player> players;

void first_move(player& p) {
	int nx = p.x + dx[p.dir];
	int ny = p.y + dy[p.dir];
	if (nx < 1) {
		nx = 2, p.dir = (p.dir + 2) % 4;
	}
	if (nx > n) {
		nx = n - 1, p.dir = (p.dir + 2) % 4;
	}
	if (ny < 1) {
		ny = 2, p.dir = (p.dir + 2) % 4;
	}
	if (ny > n) {
		ny = n - 1, p.dir = (p.dir + 2) % 4;
	}
	p.x = nx, p.y = ny;
	p.dir = p.dir;
	return;
}

bool cmp(player& p, player& other_p) {
	if (p.power + p.gun != other_p.power + other_p.gun) {
		return p.power + p.gun > other_p.power + other_p.gun;
	}
	else {
		return p.power > other_p.power;
	}
}

void lost_gun(player& p) {
	land[p.x][p.y].push_back(p.gun);
	p.gun = 0;
	return;
}

void loser_move(player& p) {
	int now_dir = p.dir;
	int nx;
	int ny;
	while (1) {
		nx = p.x + dx[now_dir];
		ny = p.y + dy[now_dir];
		if(nx <1 || nx > n || ny < 1 || ny > n) {
			now_dir = (now_dir + 1) % 4; continue;
		}
		int flag = true;
		for (player& other_p : players) {
			if (p.idx == other_p.idx) continue;
			if (nx == other_p.x && ny == other_p.y) {
				now_dir = (now_dir + 1) % 4; 
				flag = false;
				break;
			}
		}
		if (flag) break;
	}
	p.x = nx;
	p.y = ny;
	p.dir = now_dir;
	return;
}

bool larger(int a, int b) {
	return a > b;
}

void change_gun(player& p) {
	sort(land[p.x][p.y].begin(), land[p.x][p.y].end(), larger);

	if (land[p.x][p.y][0] > p.gun) {
		int temp = p.gun;
		p.gun = land[p.x][p.y][0];
		land[p.x][p.y][0] = temp;
	}

	// 마지막 정렬은 디버깅용입니다.
	sort(land[p.x][p.y].begin(), land[p.x][p.y].end(), larger);
	return;
}

bool other_player(player& p) {
	for (player& other_p : players) {
		if (p.idx == other_p.idx) continue;
		if (p.x == other_p.x && p.y == other_p.y) { // 싸우자
			if (cmp(p, other_p)) { // p 가 이김
				score[p.idx] += p.power + p.gun - other_p.power - other_p.gun;
				lost_gun(other_p); // 총버려
				loser_move(other_p); // 움직여
				change_gun(other_p); // 총이있다면 집어
				change_gun(p); // 총바꿔
			}
			else { // other_p 가 이김
				score[other_p.idx] += other_p.power + other_p.gun - p.power - p.gun;
				lost_gun(p);
				loser_move(p);
				change_gun(p);
				change_gun(other_p);
			}
			return true;
		}
	}
	return false;
}


void play() {
	for (player& p : players) {
		first_move(p);
		if (other_player(p)) continue;
		change_gun(p);
	}
}

void print_debug() {
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) { //최대공격력 총만 출력
			cout << land[i][j][0] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	for (player p : players) {
		cout << p.idx << " " << p.x << " " << p.y << " " << p.power << " " << p.gun << " " << p.dir << "\n";
	}
	cout << "\n";
}

int main() {
	
	//freopen("input.txt", "r", stdin);
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			int gun;
			cin >> gun;
			land[i][j].push_back(gun);
		}
	}
	for (int i = 1; i <= m; i++) {
		int x, y, s, dir;
		cin >> x >> y >> dir >> s;
		players.push_back({ i, x, y, s, 0, dir }); // idx x y power gun direction
	}
	
	while (k--) {
		//cout << "new turn start" << "\n";
		//print_debug();
		play();
		//cout << "turn end" << "\n";
		//print_debug();
	}

	for (int i = 1; i <= m; i++) {
		cout << score[i] << " ";
	}
	cout << "\n"; 
	return 0;
}