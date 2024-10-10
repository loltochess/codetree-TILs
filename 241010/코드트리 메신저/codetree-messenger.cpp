#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
#include <string>

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
void init() { ios_base::sync_with_stdio(0); cin.tie(NULL); cout.tie(NULL); }

int q, n, p, c, power;
string query;
int chat_room[100004][24];
int parent[100004];
int authority[100004];
int alarm[100004];

void print();

void update(int num, int idx, int power) {
	if (alarm[idx]) return;
	int p = parent[idx];
	int cnt = 1;
	while (p != -1 || power-cnt >= 0) {
		int a = alarm[p];
		chat_room[p][power - cnt] += num;
		p = parent[p]; cnt++;
		if (a == 1) return;
	}
}

void messengerInit() {
	parent[0] = -1;
	for (int i = 1; i <= n; i++) {
		cin >> p; parent[i] = p;
	}
	for (int i = 1; i <= n; i++) {
		cin >> power; authority[i] = min(power, 20);
	}
	for (int i = 1; i <= n; i++) {
		update(1, i, authority[i]);
	}
	return;
}

void turnOnOff(int flag, int c) {
	update(flag, c, authority[c]);
	for (int i = 1; i <= 20; i++) {
		if (chat_room[c][i]) {
			update(flag * chat_room[c][i], c, i);
		}
	}
}

void setAlarm() {
	cin >> c;
	if (alarm[c] == 0) {
		turnOnOff(-1, c);
		alarm[c] = 1;
	}
	else if (alarm[c] == 1) {
		alarm[c] = 0;
		turnOnOff(1, c);
	}
}

void changePower() {
	cin >> c >> power;
	update(-1, c, authority[c]); // -1 씩 해준다.(초기화)
	update(1, c, power); // +1 해준다. update
	authority[c] = power;
}

void changeParent() {
	int c_1, c_2;
	cin >> c_1 >> c_2;

	// swap 이전
	if (alarm[c_1] == 0) {
		turnOnOff(-1, c_1);
	}
	if (alarm[c_2] == 0) {
		turnOnOff(-1, c_2);
	}

	swap(parent[c_1], parent[c_2]);

	// swap 이후
	if (alarm[c_2] == 0) {
		turnOnOff(1, c_1);
	}
	if (alarm[c_1] == 0) {
		turnOnOff(1, c_2);
	}
}

void searchChatRoom() {
	cin >> c;
	int cnt = 0;
	for (int i = 0; i <= 20; i++) {
		cnt += chat_room[c][i];
	}
	cout << cnt << '\n';
}

void print() {
	cout << "노드부모" << '\n';
	for (int i = 0; i <= 14; i++) {
		cout << parent[i] << ' ';
	}
	cout << '\n';
	cout << "노드power" << '\n';
	for (int i = 0; i <= 14; i++) {
		cout << authority[i] << ' ';
	}
	cout << '\n';
	cout << "alarm 현황 " << '\n';
	for (int i = 0; i <= 14; i++) {
		cout << alarm[i] << ' ';
	}
	cout << '\n';

	for (int i = 0; i <= 14; i++) {
		cout << i << " 노드" << '\n';
		for (int j = 0; j <= 4; j++) {
			cout << chat_room[i][j] << " : ";
		}
		cout << '\n';
	}
}

int main() {
	init();
	cin >> n >> q;
	for (int i = 0; i < q; i++) {
		cin >> query;
		if (query == "100") {
			messengerInit();
		}
		if (query == "200") {
			setAlarm();
		}
		if (query == "300") {
			changePower();
		}
		if (query == "400") {
			changeParent();
		}
		if (query == "500") {
			searchChatRoom();
		}
		//print();
	}
	return 0;
}