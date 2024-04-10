#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <algorithm>
#include <queue>

using namespace std;

int land[43][43];
int L, N, Q;
int r, c, h, w, k;
int ret;

const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0,1,0,-1 };

typedef struct knight {
	int id, r, c, h, w, k;
} knight;

knight Knight[34];
int visited[34]; // 이동 + 데미지 계산해줘야함
int first_health[34];
int live[34];

bool is_wall(knight K) { // 가장자리 넘지 않음이 이전 함수로 확인됨 벽만 확인
	for (int i = K.r; i <= K.r + K.h - 1; i++) {
		for (int j = K.c; j <= K.c + K.w - 1; j++) {
			if (land[i][j] == 2) return true;
		}
	}
	return false;
}

bool met(knight K1, knight K2, int dir) { //K1이 이동해온놈 K2 가 가만히 있는놈
	if (dir == 0) { // K1이 K2의 아랫부분에 겹칠듯
		if (K1.r == K2.r + K2.h - 1) { // 우선 아랫부분에 겹치는가?
			if (K1.c + K1.w - 1 < K2.c || K2.c + K2.w - 1 < K1.c) {
				// K1의 우측 열이 K2좌측열보다 왼쪽이거나 그 반대일 경우 false;
				return false;
			}
			return true;
		}
	}
	if (dir == 1) { // K1이 K2의 왼쪽에 겹치는 경우
		if (K1.c + K1.w - 1 == K2.c) {
			if (K1.r + K1.h - 1 < K2.r || K2.r + K2.h - 1 < K1.r) return false;
			return true;
		}
	}
	if (dir == 2) { // K1이 K2의 위쪽에 겹칠 때
		if (K1.r + K1.h - 1 == K2.r) {
			if (K1.c + K1.w - 1 < K2.c || K2.c + K2.w - 1 < K1.c) return false;
			return true;
		}
	}
	if (dir == 3) { // K1이 K2의 오른쪽에 겹칠 때
		if (K1.c == K2.c + K2.w - 1) {
			if (K1.r + K1.h - 1 < K2.r || K2.r + K2.h - 1 < K1.r) return false;
			return true;
		}
	}
	return false; // default 안겹친다.
}

bool can_move(int start, int dir) {
	queue<int> q;
	visited[start] = true;
	q.push(start);
	while (q.size()) {
		int idx = q.front(); q.pop();
		knight moved_knight = Knight[idx];
		moved_knight.r += dy[dir], moved_knight.c += dx[dir];
		if (moved_knight.r < 1 || moved_knight.r + moved_knight.h - 1 > L || moved_knight.c < 1 || moved_knight.c + moved_knight.w - 1 > L) return false;
		if (is_wall(moved_knight)) return false;
		
		for (int i = 1; i <= N; i++) {
			if (live[i] == 0) continue; //죽인놈은 이동 x
			if (i == idx) continue; // 자기자신은 제외
			if (visited[i]) continue; // 방문했다면 제외 근데 한 방향이라서 필요없긴할듯?
			if (met(moved_knight, Knight[i], dir)) { // 겹친다면 bfs에 추가
				visited[i] = true; q.push(i);
			}
		}
	}

	return true;
}

void move(int dir) {
	for (int i = 1; i <= N; i++) {
		if (live[i] == 0) continue; //죽인놈은 이동 x 필요없긴함.
		if (visited[i] == true) { // 표시한 놈들만 이동
			Knight[i].r += dy[dir];
			Knight[i].c += dx[dir];
		}
	}
	return;
}

 void give_damage(int idx) {
	knight K = Knight[idx];
	for (int i = K.r; i <= K.r + K.h - 1; i++) {
		for (int j = K.c; j <= K.c + K.w - 1; j++) {
			if (land[i][j] == 1 && K.k>0) {
				K.k--;
			}
		}
	}
	Knight[idx].k = K.k;
	if (K.k == 0) live[idx] = 0;
	return;
}

void kill(int idx) {
	for (int i = 1; i <= N; i++) {
		if (i == idx) continue;
		if (visited[i] == true) {
			give_damage(i);
		}
	}
	return;
}

void print_debug() {
	cout << "id r c h w k" << "\n";
	for (int i = 1; i <= N; i++) {
		knight K = Knight[i];
		cout << K.id << " " << K.r << " " << K.c << " " << K.h << " " << K.w << " " << K.k << "\n";
	}
	cout << "\n";
}


int print_result() {
	int ret = 0;
	for (int i = 1; i <= N; i++) {
		if (live[i]) {
			ret += first_health[i] - Knight[i].k;
		}
	}
	return ret;
}

int main() {

	//freopen("input.txt", "r", stdin);
	cin >> L >> N >> Q;

	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> land[i][j];
		}
	}

	for (int i = 1; i <= N; i++) {
		cin >> r >> c >> h >> w >> k;
		Knight[i] = {i, r, c, h, w, k};
		first_health[i] = k;
	}

	fill(live, live + 34, 1);

	for (int i = 0; i < Q; i++) {
		//cout << i << " turn starts" << "\n";
		int idx, dir;
		cin >> idx >> dir;
		//cout << "command : "<< idx << " " << dir << "\n";
		//print_debug();
		memset(visited, 0, sizeof(visited));
		if (can_move(idx, dir)) { // bfs
			move(dir);
			kill(idx); // idx 제외 데미지 + ret에더하기
		}
		//print_debug();
	}

	cout << print_result() << "\n";

	return 0;
}