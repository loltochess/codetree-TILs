#include <iostream>
#include <queue>
#include <stdio.h>
#include <map>
#include <string.h>

using namespace std;
typedef long long LL;

map<int, int> pid2id;
int id2dist[2004];
int id2pid[2004];
LL id2score[2004];
int id2dup[2004];
int id2cnt[2004];
pair<int, int> id2pair[2004];
int k_visited[2004];
int Q, N, M, P;

const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0, 1, 0, -1 };


typedef struct rabbit {
	int count, y, x, pid;
} rabbit;

// pq 는 반대로 정렬함에 유의
struct cmp_rabbit {
	bool operator () (rabbit a, rabbit b) {
		if (a.count != b.count) return a.count > b.count;
		if (a.y + a.x != b.y + b.x) return a.y + a.x > b.y + b.x;
		if (a.y != b.y) return a.y > b.y;
		if (a.x != b.x) return a.x > b.x;
		return a.pid > b.pid;
	}
};


void __init__() {
	cin >> N >> M >> P;
	for (int i = 1; i <= P; i++) {
		int pid, d;
		cin >> pid >> d;
		id2pid[i] = pid;
		id2dist[i] = d;
		id2pair[i] = { 1, 1 };
        pid2id[pid] = i;
	}
}

bool cmp_go(pair<int, int> a, pair<int, int> b) {// 큰 걸 오른쪽에 놓으면됨
	if (a.first + a.second != b.first + b.second) return a.first + a.second < b.first + b.second;
	if (a.first != b.first) return a.first < b.first;
	return a.second < b.second;
}

pair<int, int> find_new_point(pair<int, int> point, int id) {
	int y = point.first, x = point.second;
    int dist = id2dist[id];
	pair<int, int> ret = make_pair(0, 0);
	for (int i = 0; i < 4; i++) {
		int temp_y = y, temp_x = x;
		int delta_y = abs(dist * dy[i]), delta_x = abs(dist * dx[i]);
		delta_y = delta_y % (2 * (N - 1));
		delta_x = delta_x % (2 * (M - 1));
        int dir = i;
		while (1) {
			if (dir == 0) { // 위로 이동할때
				if (delta_y <= temp_y - 1) {
					temp_y -= delta_y;
					break;
				}
				else {
					delta_y -= temp_y - 1;
					temp_y = 1;
					dir = (dir + 2) % 4;
				}
			}
			if (dir == 1) { // 오른쪽 이동
				if (delta_x <= M - temp_x) {
					temp_x += delta_x;
					break;
				}
				else {
					delta_x -= M - temp_x;
					temp_x = M;
					dir = (dir + 2) % 4;
				}
			}
			if (dir == 2) {//아래쪽이동
				if (delta_y <= N - temp_y) {
					temp_y += delta_y;
					break;
				}
				else {
					delta_y -= N - temp_y;
					temp_y = N;
					dir = (dir + 2) % 4;
				}
			}
			if (dir == 3) {//왼쪽이동
				if (delta_x <= temp_x - 1) {
					temp_x -= delta_x;
					break;
				}
				else {
					delta_x -= temp_x - 1;
					temp_x = 1;
					dir = (dir + 2) % 4;
				}
			}
		}
		pair<int, int> point = make_pair(temp_y, temp_x);
		
		if (cmp_go(ret, point)) ret = point;
	}
	return ret;
}

struct cmp_S {
	bool operator () (rabbit a, rabbit b) {
		if (a.y + a.x != b.y + b.x) return a.y + a.x < b.y + b.x;
		if (a.y != b.y)return a.y < b.y;
		if (a.x != b.x)return a.x < b.x;
		return a.pid < b.pid;
	}
};

void print_debug(){
    cout << "cnt y x pid" <<"\n";
    for(int i=1;i<=P;i++){
        cout << id2cnt[i] << " " << id2pair[i].first <<" " <<id2pair[i].second <<" " << id2pid[i] << "\n";
    }
    cout <<"\n";
}

void race() {
	int K, S;
	cin >> K >> S;
	memset(k_visited, 0, sizeof(k_visited));
	memset(id2dup, 0, sizeof(id2dup));
	LL total_score = 0;
	
	priority_queue<rabbit, vector<rabbit>, cmp_rabbit> select_rabbit;
	for (int i = 1; i <= P; i++) {
		select_rabbit.push({id2cnt[i], id2pair[i].first, id2pair[i].second, id2pid[i]});
	}
	
	// K 번 실행하기
	while (K--) {
        //print_debug();
		rabbit rb = select_rabbit.top();
        int id = pid2id[rb.pid];
		select_rabbit.pop(); // y,x 및 count 가 바뀌므로 다시 넣어줘야함
		pair<int, int> point = make_pair(rb.y, rb.x);
		pair<int, int> new_point = find_new_point(point, id);
		id2cnt[id]++;
		k_visited[id] = true;
		id2pair[id] = new_point;
		total_score += new_point.first + new_point.second;
		id2dup[id] += new_point.first + new_point.second;
		select_rabbit.push({id2cnt[id], new_point.first, new_point.second, rb.pid });
	}

    //print_debug();
	priority_queue<rabbit, vector<rabbit>, cmp_S> rabbit_S;
	for (int i = 1; i <= P; i++) {
		id2score[i] += total_score;
		id2score[i] -= id2dup[i];
		if (k_visited[i]) {
			int count = id2cnt[i];
			pair<int, int> a = id2pair[i];
			int pid = id2pid[i];
			rabbit_S.push({ count, a.first, a.second, pid });
		}
	}
	rabbit rb = rabbit_S.top();
	int id = pid2id[rb.pid];
	id2score[id] += S;
	return;
}

void change_dist() {
	int pid, L;
	cin >> pid >> L;
	int id = pid2id[pid];
	id2dist[id] *= L;
	return;
}


int main() {
    //freopen("input.txt","r",stdin);
    //freopen("output.txt","w",stdout);
	cin >> Q;
	while (Q--) {
		int command;
		cin >> command;
		if (command == 100) __init__();
		if (command == 200) race();
		if (command == 300) change_dist();
		if (command == 400) break;
	}
	LL ans = -1;
	for (int i = 1; i <= P; i++) {
		ans = max(ans, id2score[i]);
	}
	cout << ans << "\n";
	return 0;
}