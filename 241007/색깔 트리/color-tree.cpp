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

int q, m, p, color, md;

int parent[100004];
vector<int> child[100004];
int root_color[100004][6]; // [id][1,2,3,4,5 빨주노초파]
int max_depth[100004];
int now_depth[100004];
int ret;
string query;
vector<int> color_vec;
pii last_changed[100004]; // 최근에 변경된 time , color
vector<int> root_m;
vector<int> v;
vector<ll> ans;

bool check_depth(int node, int child_depth) { // parent부터시작
	if (node == -1) return true;
	if (now_depth[node] < child_depth + 1) {
		if (child_depth + 1 > max_depth[node]) return false;
		return check_depth(parent[node], child_depth + 1);
	}
	return true;
}

void depth_update(int node, int child_depth) { // node(m) 부터시작
	if (node == -1) return;
	if (now_depth[node] < child_depth + 1) {
		now_depth[node] = child_depth + 1;
		depth_update(parent[node], now_depth[node]);
	}
	return;
}

void color_update(int node, vector<int>& color_vec) {
	if (node == -1) return;
	for (int i = 1; i <= 5; i++) {
		root_color[node][i] += color_vec[i];
	}
	color_update(parent[node], color_vec);
}

void color_propagate(int node, int color, int t) {
	int cnt = 0;
	last_changed[node] = { t, color };
	for (int i = 1; i <= 5; i++) {
		cnt += root_color[node][i];
		root_color[node][i] = 0;
	}
	root_color[node][color] = cnt;
	for (int a : child[node]) {
		color_propagate(a, color, t);
	}
	return;
}

ll dfs(int node) {
	ll ret = 0;
	for (int i = 1; i <= 5; i++) {
		if (root_color[node][i]) ret++;
	}
	ret *= ret;
	for (int a : child[node]) {
		ret += dfs(a);
	}
	return ret;
}

void go(string query,int t) {
	if (query == "100") {
		cin >> m >> p >> color >> md;
		if (p == -1) { // root node
			v.push_back(m);
			root_m.push_back(m);
			parent[m] = -1; root_color[m][color]++; 
			max_depth[m] = md;  now_depth[m] = 1; 
			last_changed[m] = { t,color };
			return;
		}
		if (check_depth(p, 1)) {
			v.push_back(m);
			parent[m] = p; child[p].push_back(m);
			depth_update(m, 0);
			color_vec = { 0, 0, 0, 0, 0, 0 };
			color_vec[color]++;
			color_update(m, color_vec); // 자기자신부터 root 까지 root_color[m] 만큼 update
			max_depth[m] = md;
			last_changed[m] = { t, color };
		}
	}
	if (query == "200") {
		cin >> m >> color;
		int cnt = 0;
		for (int i = 1; i <= 5; i++) {
			color_vec[i] = -root_color[m][i];
			cnt += root_color[m][i];
			root_color[m][i] = 0;
		}
		color_update(parent[m], color_vec);
		root_color[m][color] = cnt;
		for (int i = 1; i <= 5; i++) {
			color_vec[i] = 0;
		}
		color_vec[color] = cnt;
		color_update(parent[m], color_vec);
		color_propagate(m, color, t);// -> 이하 root_color 및 last changed 도변경
	}
	if (query == "300") {
		cin >> m;
		ans.push_back((ll)last_changed[m].second);
	}
	if (query == "400") {
		ll ret = 0;
		for (int a : root_m) {
			ret += dfs(a);
		}
		ans.push_back(ret);
	}
}

void print() {
	for (int i : v) {
		cout << i << " parent : " << parent[i] << ", root color :";
		for (int j = 1; j <= 5; j++) {
			cout << root_color[i][j] << " ";
		}
		cout << " last_changed : " << last_changed[i].first << " , " << last_changed[i].second << ' ';
		cout << "max_depth : " << max_depth[i] << " : " << "now_depth " << now_depth[i] << '\n';
	}
}

int main() {
	init();
	cin >> q;
	for (int i = 1; i <= q; i++) {
		cin >> query;
		go(query,i);
		//print();
	}
	for (ll a : ans) {
		cout << a << '\n';
	}
	return 0;
}