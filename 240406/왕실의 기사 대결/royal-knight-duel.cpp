#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <queue>


using namespace std;

int land[44][44];
int L, N, Q;
vector<int> knight_matrix[34]; // {좌상단 row 좌하단 row 좌상단 col, 우상단 col}
int hp_first[34]; //처음체력
int hp[34]; // 현재체력
int dead[34]; // 죽었나요?

int visited[34]; // 큐에 사용할 visited 배열

queue<int> q;

vector<int> move_direction[4] = {// rs re cs ce
	{-1, -1, 0, 0},
	{0, 0, 1, 1},
	{1, 1, 0, 0},
	{0, 0, -1, -1}
};

int count_trap(vector<int> &temp) {
	int cnt = 0;
	int row_start = temp[0];
	int row_end = temp[1];
	int col_start = temp[2];
	int col_end = temp[3];

	for (int i = row_start; i <= row_end; i++) {
		for (int j = col_start; j <= col_end; j++) {
			if (land[i][j] == 1) cnt++;
		}
	}
	return cnt;
}

void move_knight(vector<int>& v, int direction) {
	for (int i = 0; i < 4; i++) {
		v[i] += move_direction[direction][i];
	}
}

bool is_wall(vector<int>& temp) {
	bool flag = false;
	int row_start = temp[0];
	int row_end = temp[1];
	int col_start = temp[2];
	int col_end = temp[3];

	if (row_start < 1 || row_end > L || col_start < 1 || col_end > L) return true;

	for (int i = row_start; i <= row_end; i++) {
		for (int j = col_start; j <= col_end; j++) {
			if (land[i][j] == 2) return true;
		}
	}
	return false;
}


vector<int> check(vector<int> &temp, int index, int direction) {

	vector<int> index_going_to_move;

	// 옮겼을 때 겹치는 기사 찾기
	for (int i = 1; i <= N; i++) {
		if (i == index) continue; //자기자신은 당연히 제외
		if (dead[i]) continue; //죽은 자는 사라지니깐 가능
		int rs, re, cs, ce;
		rs = knight_matrix[i][0];
		re = knight_matrix[i][1];
		cs = knight_matrix[i][2];
		ce = knight_matrix[i][3];
		// 어차피 끝 줄 부분만 겹친다는 사실을 이용
		if (direction == 0 && re == temp[0]) {// 위로 이동한 놈이 겹치려면 re = temp의 첫 row(=rs)
			if((cs <= temp[2] && temp[2] <= ce) || (cs <= temp[3] && temp[3] <= ce)) index_going_to_move.push_back(i);
		}
		else if(direction == 1 && cs == temp[3]) {//오른쪽 이동한 놈이 겹치려면 cs = temp의 ce
			if((rs <= temp[0] && temp[0] <= re) || (rs <= temp[1] && temp[1] <= re))index_going_to_move.push_back(i);
		}
		else if (direction == 2 && rs == temp[1]) {// 아래 이동한 놈은 rs = temp의 re
			if ((cs <= temp[2] && temp[2] <= ce) || (cs <= temp[3] && temp[3] <= ce))index_going_to_move.push_back(i);
		}
		else if (direction == 3 && ce == temp[2]) {// 왼쪽 이동하면 ce = temp의 cs
			if ((rs <= temp[0] && temp[0] <= re) || (rs <= temp[1] && temp[1] <= re)) index_going_to_move.push_back(i);
		}
	}
	
	return index_going_to_move;
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
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		knight_matrix[i].push_back(r);
		knight_matrix[i].push_back(r + h - 1);
		knight_matrix[i].push_back(c);
		knight_matrix[i].push_back(c + w - 1);
		hp_first[i] = k;
		hp[i] = k;
	}

	for (int i = 0; i < Q; i++) {
		//cout << i + 1 << " turn " << "\n";
		int idx, d;
		cin >> idx >> d;
		//cout << idx << " " << d << "\n";
		bool gogo = true;
		if (dead[idx]) continue;// 죽은 knight가 아니라면 옮길수있다
		if (1) {
			memset(visited, 0, sizeof(visited)); // visited 초기화
			while (q.size()) { // 큐 초기화
				q.pop();
			}
			q.push(idx);
			visited[idx] = true;
			while (q.size()) {
				int id = q.front();
				//cout << "now id : " << id << "\n";
				q.pop();
				vector<int> index_next;
				vector<int> temp;
				for (int j= 0; j < 4; j++) {
					temp.push_back(knight_matrix[id][j]);
				}
				move_knight(temp, d);
				if (is_wall(temp)) {
					gogo = false; break;
				}
				index_next = check(temp, id, d);
				//cout << "index_next : " ;
				for (int a : index_next) {
					//cout << a << " ";
				}
				//cout << "\n";
				for (auto next : index_next) {
					visited[next] = true;  q.push(next);
				}
			}
		}
		if (gogo) {
			for (int i = 1; i <= N; i++) {
				if (!visited[i]) continue;
				move_knight(knight_matrix[i], d);
				int damage = count_trap(knight_matrix[i]);
				if (i == idx) continue;
				hp[i] -= damage;
				if (hp[i] <= 0) dead[i] = 1;
			}
		}

	}

	int ret = 0;
	for (int i = 1; i <= N; i++) {
		//cout << hp_first[i] << " " << hp[i] << "\n";
		if (hp[i] > 0) ret += hp_first[i] - hp[i];
	}
	cout << ret << "\n";
}