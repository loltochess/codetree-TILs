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

typedef struct task {
	int p, t;
	string u;
}Task;

struct cmp{
	bool operator()(Task a, Task b) {
		if (a.p != b.p) return a.p > b.p;
		return a.t > b.t;
	}
};

unordered_map<string, pii> domainHistory; // domain -> {start , gap}
Task runningQ[50004]; // j_id -> {p,time,url}
priority_queue<Task, vector<Task>, cmp> readyQ; // {p,time,url}
vector<Task> bufferQ; // p,time,url
unordered_map<string, int> domainRunning; // domain -> {0 or 1}
unordered_map<string, int> urlReadyQ; // url -> {0,1} is in readyQ?
priority_queue<int, vector<int>, greater<int>> nextMachine; // 쉬고있는놈중에 가장 숫자작은.

int q, n, t, p, j_id;
string u;
string query;
vector<int> v;

string getDomain(string& u) {
	return u.substr(0,u.find('/'));
}

void go() {
	cin >> query;
	if (query == "100") {
		cin >> n >> u;
		for (int i = 1; i <= n; i++) nextMachine.push(i);
		readyQ.push({ 1,0,u });
		urlReadyQ[u] = 1;
	}
	if (query == "200") {
		cin >> t >> p >> u;
		if (urlReadyQ[u] != 0) {
			return;
		}
		//cout << "추가됨." << '\n';
		urlReadyQ[u] = 1;
		readyQ.push({ p,t,u });
	}
	if (query == "300") {
		cin >> t;
		if (nextMachine.size() == 0) return; // 쉬는 기계없음.
		int flag = true;
		while (readyQ.size()) {
			flag = true;
			Task a = readyQ.top();
			//cout << a.p << " : " << a.t << " : " << a.u << '\n';
			string domain = getDomain(a.u);
			if (domainRunning[domain]) flag = false;
			if (domainHistory[domain].first + 3 * domainHistory[domain].second > t) flag = false;
			if (flag) { // 사용가능
				int machine = nextMachine.top();
				nextMachine.pop();
				readyQ.pop();
				runningQ[machine] = { a.p, t, a.u };
				urlReadyQ[a.u] = 0;
				//cout << "사용가능 : " << a.p << " : " << t << " : " << a.u << '\n';
				break;
			}
			else { // 불가능하므로 버퍼에 넣고 다음거 꺼내기.
				bufferQ.push_back(a);
				readyQ.pop();
			}
		}
		while (bufferQ.size()) {
			readyQ.push(bufferQ.back());
			bufferQ.pop_back();
		}
	}
	if (query == "400") {
		cin >> t >> j_id;
		Task nowTask = runningQ[j_id];
		if (nowTask.p == 0) return; // 채점이 없음.
		int gap = t - nowTask.t;
		string domain = getDomain(nowTask.u);
		domainHistory[domain] = { nowTask.t, gap };
		//cout << domain << " : " << nowTask.t << " : " << nowTask.t + gap << '\n';
		runningQ[j_id] = { 0, 0, "" }; // 없어~
		domainRunning[domain] = 0;
		nextMachine.push(j_id);
	}
	if (query == "500") {
		cin >> t;
		v.push_back((int)readyQ.size());
	}
}

int main() {
	init();
	cin >> q;
	for (int i = 0; i < q; i++) {
		go();
	}

	for (int a : v) cout << a << '\n';
	cout << '\n';
	return 0;
}