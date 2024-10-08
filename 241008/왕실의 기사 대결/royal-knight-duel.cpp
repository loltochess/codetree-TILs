#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
void init() { ios_base::sync_with_stdio(0); cin.tie(NULL); cout.tie(NULL); }

typedef struct knight {
	int r, c, h, w, k;
}Knight;

Knight knight[34];
int knight_array[44][44];
int land[44][44];
int l, n, q;
const int dy[] = { -1,0,1,0 }; // 상우하좌
const int dx[] = { 0,1,0,-1 };
int visited[34];
int hp[34];
int ret;

bool is_knight(int y, int x, int id) {
	if (knight_array[y][x] > 0 && knight_array[y][x] != id) return true;
	return false;
}

bool is_out(int y, int x) {
	if (y<1 || y>l || x<1 || x>l) return true;
	return false;
}

bool cant_go(int y, int x) {
	if (is_out(y, x) || land[y][x] == 2) return true;
	return false;
}

bool can_move(int id, int dir) { 
	visited[id] = true;
	for (int i = 0; i < knight[id].h; i++) {
		for (int j = 0; j < knight[id].w; j++) {
			int ny = knight[id].r + i + dy[dir];
			int nx = knight[id].c + j + dx[dir];
			if (cant_go(ny, nx)) return false;
			if (is_knight(ny, nx, id) && visited[knight_array[ny][nx]] == 0) {
				if(can_move(knight_array[ny][nx], dir) == 0) return false;
			}
		}
	}
	return true;
}

void move_knight(int id, int dir, int flag) {
	visited[id] = true;
	for (int i = 0; i < knight[id].h; i++) {
		for (int j = 0; j < knight[id].w; j++) {
			int ny = knight[id].r + i + dy[dir];
			int nx = knight[id].c + j + dx[dir];
			// 이동할 수 있음이 결정되었고, knight 연쇄 이동만 파악하면 된다.
			if (is_knight(ny, nx, id) && visited[knight_array[ny][nx]] == 0) {
				move_knight(knight_array[ny][nx], dir, 1); // flag == 1이면 데미지계산
			}
		}
	}
	// 원래의 흔적을 지워야한다.
	for (int i = 0; i < knight[id].h; i++) {
		for (int j = 0; j < knight[id].w; j++) {
			int y = knight[id].r + i;
			int x = knight[id].c + j;
			knight_array[y][x] = 0;
		}
	}
	// 이동한 곳 업데이트
	for (int i = 0; i < knight[id].h; i++) {
		for (int j = 0; j < knight[id].w; j++) {
			int ny = knight[id].r + i + dy[dir];
			int nx = knight[id].c + j + dx[dir];
			knight_array[ny][nx] = id;
			if (flag == 1 && land[ny][nx] == 1) { // flag == 1 이면 데미지계산
				knight[id].k--;
			}
		}
	}
	knight[id].r += dy[dir];
	knight[id].c += dx[dir];
	if (knight[id].k <= 0) { // 죽었다면 array에서 지우자.
		for (int i = 0; i < knight[id].h; i++) {
			for (int j = 0; j < knight[id].w; j++) {
				int y = knight[id].r + i;
				int x = knight[id].c + j;
				knight_array[y][x] = 0;
			}
		}
	}
	return;
}

void go(int id, int dir) {
	if (knight[id].k <= 0) return; // 죽으면 이동 x
	memset(visited, 0, sizeof(visited));
	if (can_move(id, dir) == 0) return; // 이동 못함
	memset(visited, 0, sizeof(visited));
	move_knight(id, dir, 0); // 이동 <- 이동할수있음이 결정됨.
}

void print() {
	for (int i = 1; i <= l; i++) {
		for (int j = 1; j <= l; j++) {
			cout << knight_array[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << "hp : ";
	for (int i = 1; i <= l; i++) cout << knight[i].k << " ";
	cout << '\n';
}

int main() {
	init();
	cin >> l >> n >> q;
	for (int i = 1; i <= l; i++) {
		for (int j = 1; j <= l; j++) {
			cin >> land[i][j];
		}
	}
	for (int i = 1; i <= n; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		knight[i] = { r,c,h,w,k };
		hp[i] = k;
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				knight_array[r + y][c + x] = i;
			}
		}
	}
	//print();
	for (int i = 0; i < q; i++) {
		int id, dir;
		cin >> id >> dir;
		go(id, dir);
		//print();
	}
	for (int i = 1; i <= n; i++) {
		if (knight[i].k > 0) {
			ret += hp[i] - knight[i].k;
		}
	}
	cout << ret << '\n';
	return 0;
}