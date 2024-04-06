#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>

using namespace std;

int land[44][44];
int L, N, Q;
vector<int> knight_matrix[34]; // {좌상단 row 좌하단 row 좌상단 col, 우상단 col}
int hp_first[34]; //처음체력
int hp[34]; // 현재체력
int dead[34]; // 죽었나요?

int will_move[34];
int can_first_move = 0;

vector<int> move_direction[4] = {
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

void real_move_knight(int index, int direction) { // 진짜로 이동 with 사망처리
	//cout << index << " knight matrix moves to " << direction << "\n";
	// 겹치는 기사 없으면 실제로 이동 -> trap 세고 -> 사망일경우 사망처리하고 -> true 리턴
	move_knight(knight_matrix[index], direction);
	int damage = count_trap(knight_matrix[index]);
	//cout << index << " matrix 가 옮겨지면서 " << damage << " 만큼의 피해를 입었다." << "\n";
	hp[index] -= damage;
	if (hp[index] <= 0) {
		dead[index] = 1;
	}
	return;
}

int check(int index, int direction) {// 재귀적으로 check 받아서 확인되면 재귀적으로 이동시킨다.
	//cout << "check " << index << " matrix can move to " << direction << "\n";
	bool can_move = false;
	vector<int> temp;
	for (int i = 0; i < 4; i++) {
		temp.push_back(knight_matrix[index][i]);
	}
	move_knight(temp, direction); //temp가 이동한 matrix가 된다. 지역변수라서 알아서사라진다.
	if (is_wall(temp)) {
		//cout << index << " matrix can't move to " << direction <<" because of wall" << "\n";
		can_first_move += 10000;
		return 0; // 이동할수 없다면 false(0) return.
	}

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
			index_going_to_move.push_back(i);
		}
		else if(direction == 1 && cs == temp[3]) {//오른쪽 이동한 놈이 겹치려면 cs = temp의 ce
			index_going_to_move.push_back(i);
		}
		else if (direction == 2 && rs == temp[1]) {// 아래 이동한 놈은 rs = temp의 re
			index_going_to_move.push_back(i);
		}
		else if (direction == 3 && ce == temp[2]) {// 왼쪽 이동하면 ce = temp의 cs
			index_going_to_move.push_back(i);
		}
	}
	
	if (index_going_to_move.size() == 0) { // tree의 leaf node일 때 
		//cout << index << " matrix can move direction " << direction << " but don't know about its siblings." << " \n";
		will_move[index] = 1;
		return 2; 
	}

	int will_move[40];
	int cnt = 0;
	memset(will_move, 0, sizeof(will_move)); // leaf node만 재귀적으로 move 한다.
	// 다음 stage로 보내는 temp 배열 만들기
	for (auto idx : index_going_to_move) {
		vector<int> temptemp;
		for (int j = 0; j < 4; j++) {
			temptemp.push_back(knight_matrix[idx][j]);
		}
		if (check(idx, direction) == 2) {
			cnt++;
		}
		else if (check(idx, direction) == 1) {
			cnt++;
		}
	}
	
	if (cnt == index_going_to_move.size()) {
		//real_move_knight(index, direction);
		will_move[index] = 1;
		return 1; // leaf node가 아닌 자식 node는 1로 리턴한다.
	}

	return false;
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
		memset(will_move, 0, sizeof(will_move));
		can_first_move = 0;
		int idx, d;
		cin >> idx >> d;
		if (!dead[idx]) {// 죽은 knight가 아니라면 옮길수있다
			if (check(idx, d)) { // 처음 민 놈은 데미지안입음!
				if (can_first_move == 0) {
					for (int i = 1; i <= N; i++) {
							real_move_knight(i, d);
					}
				}
				int heal = count_trap(knight_matrix[idx]);
				//cout << idx << " matrix 가 함정을 " << heal << "개 밟았으므로 처음놈은 회복시켜준다." << "\n";
				if (dead[idx]) dead[idx] = 0;
				hp[idx] += heal;
			}
		}
		for (int i = 1; i <= N; i++) {
			//cout << hp_first[i] << " " << hp[i] << "\n";
		}
	}

	int ret = 0;
	for (int i = 1; i <= N; i++) {
		//cout << hp_first[i] << " " << hp[i] << "\n";
		if (hp[i] > 0) ret += hp_first[i] - hp[i];
	}
	cout << ret << "\n";
}