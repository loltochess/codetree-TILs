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

int now_idx = 1;
map<string, int> domainToIdx; // domain -> domain idx
map<int, int> domainUrlReadyQ[304]; // domain idx , number -> 있는지?

typedef struct task {
	int p, t, num;

	bool operator < (const task& _t) const {
		if (p != _t.p) return p > _t.p;
		return t > _t.t;
	}
}Task;

priority_queue<Task> pq[304];
int machineDomain[50004];

int s[304];
int g[304];
int e[304];
priority_queue<int, vector<int>, greater<int>> nextMachine;
int q, n, t, p, j_id;
string u;
int ret;
string query;

void machine_init() {
	cin >> n >> u;
	for (int i = 1; i <= 50000; i++) nextMachine.push(i);
	string domain = u.substr(0,u.find('/'));
	if (domainToIdx[domain] == 0) {
		domainToIdx[domain] = now_idx;
		now_idx++;
	}
	int idx = domainToIdx[domain];
	int num = stoi(u.substr(u.find('/')+1,100));
	domainUrlReadyQ[idx][num] = 1;
	pq[idx].push({ 1,0,num });
	ret++;
}

void request() {
	cin >> t >> p >> u;
	string domain = u.substr(0,u.find('/'));
	if (domainToIdx[domain] == 0) {
		domainToIdx[domain] = now_idx;
		now_idx++;
	}
	int idx = domainToIdx[domain];
	int num = stoi(u.substr(u.find('/')+1, 100));
	if (domainUrlReadyQ[idx][num] == 1) return;
	domainUrlReadyQ[idx][num] = 1;
	pq[idx].push({ p,t,num });
	ret++;
}

void attempt() {
	cin >> t;
	if (nextMachine.size() == 0) return;
	int min_p = 1e9;
	int min_t = 1e9;
	int min_domainIdx = 0;
	int min_num = -1;
	for (int i = 1; i < now_idx; i++) {
		if (e[i] > t) continue; // 채점불가능

		if (pq[i].size()) {
			Task task = pq[i].top();
			if (min_p > task.p) {
				min_p = task.p; min_t = task.t;
				min_domainIdx = i; min_num = task.num;
			}
			else if (min_p == task.p && min_t > task.t) {
				min_t = i; min_domainIdx = i; min_num = task.num;
			}
		}
	}
	if (min_domainIdx) { // 들어갈 곳이 있음
		pq[min_domainIdx].pop();
		int machine = nextMachine.top(); nextMachine.pop();
		domainUrlReadyQ[min_domainIdx][min_num] = 0;
		machineDomain[machine] = min_domainIdx;
		ret--;
		s[min_domainIdx] = t;
		e[min_domainIdx] = 1e9;
	}
}

void end_machine() {
	cin >> t >> j_id;
	if (machineDomain[j_id] == 0) return;

	int domainIdx = machineDomain[j_id];

	g[domainIdx] = t - s[domainIdx];
	e[domainIdx] = s[domainIdx] + 3 * g[domainIdx];
	
	nextMachine.push(j_id);
	machineDomain[j_id] = 0;
}

int main() {
	init();
	cin >> q;
	for (int i = 0; i < q; i++) {
		cin >> query;
		if (query == "100") {
			machine_init();
		}
		if (query == "200") {
			request();
		}
		if (query == "300") {
			attempt();
		}
		if (query == "400") {
			end_machine();
		}
		if (query == "500") {
			cin >> t;
			cout << ret << '\n';
		}
	}
	return 0;
}