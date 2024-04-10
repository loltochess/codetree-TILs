#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

typedef struct node{
	int index;
	int authority;
	int parent;
	int left;
	int right;
	int alarm;
} node;

node Nodes[100004];

int Q, N, p, a, c;
int power;
int command;

void print_node() {
	cout << "index authority parent left right alarm" << "\n";
	for (int i = 0; i <= N; i++) {
		node n = Nodes[i];
		cout << n.index << " " << n.authority << " " << n.parent << " " << n.left << " " << n.right << " " << n.alarm << "\n";
	}
	return;
}

int count_chats(int idx, int depth) {
	//if (depth > Nodes[idx].authority) { // 이러면 보낼수없어
	//	return 0;
	//}
	if (depth > 0 && Nodes[idx].alarm == 1) { // 알람차단됨
		return 0;
	}
	int cnt = 0;
	if (depth <= Nodes[idx].authority && depth != 0) {
		cnt += 1;
	}
	if (Nodes[idx].left != -1) {
		cnt += count_chats(Nodes[idx].left, depth + 1);
	}
	if (Nodes[idx].right != -1) {
		cnt += count_chats(Nodes[idx].right, depth + 1);
	}
	return cnt;
}

void swap_node(int idx1, int idx2) {
	int temp_idx1 = idx1;
	int temp_idx2 = idx2;
	int one_parent = Nodes[idx1].parent;
	int two_parent = Nodes[idx2].parent;
	int one_child_left = Nodes[idx1].left;
	int one_child_right = Nodes[idx1].right;
	int two_child_left = Nodes[idx2].left;
	int two_child_right = Nodes[idx2].right;

	if (Nodes[one_parent].left == temp_idx1) { // left일 때
		if (Nodes[two_parent].left == temp_idx2) {
			Nodes[one_parent].left = temp_idx2;
			Nodes[two_parent].left = temp_idx1;
		}
		else {
			Nodes[one_parent].left = temp_idx2;
			Nodes[two_parent].right = temp_idx1;
		}
	}
	else if (Nodes[one_parent].right == temp_idx1) {
		if (Nodes[two_parent].left == temp_idx2) {
			Nodes[one_parent].right = temp_idx2;
			Nodes[two_parent].left = temp_idx1;
		}
		else {
			Nodes[one_parent].right = temp_idx2;
			Nodes[two_parent].right = temp_idx1;
		}
	}


	Nodes[idx1].parent = two_parent;
	Nodes[idx2].parent = one_parent;
	Nodes[one_child_left].parent = temp_idx2;
	Nodes[one_child_right].parent = temp_idx2;
	Nodes[two_child_left].parent = temp_idx1;
	Nodes[two_child_left].parent = temp_idx1;

	return;
}

int main() {

	//freopen("input.txt", "r", stdin);
	cin >> N >> Q;

	for (int i = 0; i <= N; i++) {
		Nodes[i] = { i, -1, -1, -1, -1, 0};
	}

	// 100 command 확실
	cin >> command;
	for (int i = 1; i <= N; i++) {
		cin >> p;
		Nodes[i].parent = p;
		if (Nodes[p].left == -1) {
			Nodes[p].left = i;
		}
		else Nodes[p].right = i;
	}
	for (int i = 1; i <= N; i++) {
		cin >> a;
		Nodes[i].authority = a;
	}
	//print_node();

	for (int i = 1; i < Q; i++) { // 명령 하나 빼고진행
		cin >> command;
		if (command == 200) {
			cin >> c;
			//cout << command << " " << c << "\n";
			if (Nodes[c].alarm == 1) Nodes[c].alarm = 0;
			else Nodes[c].alarm = 1;
		}
		if (command == 300) {
			cin >> c >> power;
			//cout << command << " " << c << " " << power << "\n";
			Nodes[c].authority = power;
		}
		if (command == 400) {
			int c1, c2;
			cin >> c1 >> c2;
			//cout << command << " " << c1 << " " << c2 << "\n";
			swap_node(c1, c2); // child도 바꿈에 유의
		}
		if (command == 500) {
			cin >> c;
			//cout << command << " " << c << "\n";
			cout << count_chats(c, 0) << "\n";
		}
		//print_node();
		//cout << "\n";
	}
	return 0;
}