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

int q, n, m, v, u, w, id, revenue, dest, s;
int dist_2d[2004][2004];
int dist[2004]; // 목적지까지의 SP
vector<pii> adj[2004]; // 가중치, 목적지
const int INF = 987654321;
priority_queue<pii, vector<pii>, greater<pii>> pq; // 가중치, 목적지 : 다익스트라위한 pq

pii land[30004]; // id -> {revenue,dest}
int visited[30004]; // id 들어오면 살리고, 없애면 죽이고

struct cmp {
	bool operator()(pii a, pii b) {
		if (a.first == b.first) return a.second > b.second;
		return a.first < b.first;
	}
};

priority_queue<pii, vector<pii>, cmp> cost_pq; // cost,id  : 시작지 바뀔때마다 새로 만들어야

string query;
vector<int> ret;

void dijkstra(int s) {
	fill(&dist[0], &dist[0] + 2000, INF);
	while (cost_pq.size()) cost_pq.pop();
	dist[s] = 0;
	pq.push({ 0,s });
	while (pq.size()) {
		auto a = pq.top(); pq.pop();
		int here_dist = a.first, here = a.second;
		if (dist[here] != here_dist) continue;
		for (auto b : adj[here]) {
			int there_dist = b.first, there = b.second;
			if (dist[there] > dist[here] + there_dist) {
				dist[there] = dist[here] + there_dist;
				pq.push({ dist[there], there });
			}
		}
	}
	for (int id = 1; id <= 30000; id++) {
		int revenue = land[id].first;
		int dest = land[id].second;
		if (visited[id] && revenue - dist[dest] >= 0) {
			cost_pq.push({ revenue - dist[dest], id });
		}
	}
}

void go(string query) {
	if (query == "100") {
		cin >> n >> m;
		for (int i = 0; i < m; i++) {
			cin >> v >> u >> w;
			if (dist_2d[v][u]) {
				if (w > dist_2d[v][u]) continue; // 최소가중치만 기록
			}
			dist_2d[v][u] = w;
			dist_2d[u][v] = w;
			adj[v].push_back({ w,u });
			adj[u].push_back({ w,v });
		}
	}
	if (query == "200") {
		cin >> id >> revenue >> dest;
		land[id] = { revenue,dest };
		visited[id] = true;
		if (revenue >= dist[dest]) {
			cost_pq.push({ revenue - dist[dest],id });
		}
	}
	if (query == "300") {
		cin >> id;
		visited[id] = 0;
	}
	if (query == "400") {
		while (cost_pq.size()) {
			pii a = cost_pq.top(); // cost, 목적지 id
			int id = a.second;
			cost_pq.pop();
			if (visited[id]) {
				visited[id] = 0; // 죽여주고 출력하고 리턴
				ret.push_back(id); return;
			}
		}
		ret.push_back(-1); return; // pq가 비었거나, 모두 죽었을때
	}
	if (query == "500") {
		cin >> s;
		dijkstra(s);
	}
}

int main() {
	init();
	cin >> q;
	for (int i = 0; i < q; i++) {
		cin >> query;
		go(query);
		if(i == 0) dijkstra(0);
	}
	for (int a : ret) cout << a << '\n';
	return 0;
}