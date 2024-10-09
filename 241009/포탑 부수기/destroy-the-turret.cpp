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

typedef struct tower {
	ll atk;
	int last, y, x;
}Tower;

vector<Tower> v;

ll attack[14][14];
int last_attacked[14][14];
int n, m, k;
int visited[14][14];
int dfs_visited[14][14];
int repair[14][14];
int atk_y, atk_x; // attack
int def_y, def_x; // defend
const int dy[] = {0,1,1,1,0,-1,-1,-1}; // 우 하 좌 상
const int dx[] = { 1,1,0,-1,-1,-1,0,1 };
queue<pii> q;
int turn;

// v.front() -> 공격자 v.last() -> 피공격자
bool cmp(const Tower& a, const Tower& b) {
	if (a.atk != b.atk) return a.atk < b.atk;
	if (a.last != b.last) return a.last > b.last;
	if (a.y + a.x != b.y + b.x) return a.y + a.x > b.y + b.x;
	return a.x > b.x;
}

bool is_out(int y, int x) {
	if (y < 0 || y >= n || x < 0 || x >= m) return true;
	return false;
}

void print() {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << attack[i][j] << ',' << last_attacked[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << '\n';

}

void bfs() {
	visited[atk_y][atk_x] = 1;
	q.push({ atk_y, atk_x });
	while (q.size()) {
		pii a = q.front(); q.pop();
		int y = a.first, x = a.second;
		for (int i = 0; i < 8; i += 2) {
			int ny = y + dy[i], nx = x + dx[i];
			ny += n; nx += m;
			ny %= n; nx %= m;
			if (attack[ny][nx] == 0 || visited[ny][nx]) continue;
			visited[ny][nx] = visited[y][x] + 1;
			q.push({ ny,nx });
		}
	}
}

bool dfs(int y, int x, vector<pii>& vv) {
	if (vv.size() == visited[def_y][def_x]) {
		if (y == def_y && x == def_x) return true;
		else return false;
	}
	
	for (int i = 0; i < 8; i += 2) {
		int ny = y + dy[i], nx = x + dx[i];
		ny += n; nx += m;
		ny %= n; nx %= m;
		if (attack[ny][nx] == 0 || dfs_visited[ny][nx]) continue;
		vv.push_back({ ny,nx });
		dfs_visited[ny][nx] = true;
		if (dfs(ny, nx, vv)) return true; // 아무튼 끝났다는뜻!
		vv.pop_back();
		dfs_visited[ny][nx] = false;
	}

	return false;
}

void attacked(int y, int x, int power) {
	repair[y][x] = true;
	attack[y][x] -= power;
	if (attack[y][x] < 0) attack[y][x] = 0;
}

void lazer() {
	memset(dfs_visited, 0, sizeof(dfs_visited));
	vector<pii> vv;
	vv.push_back({ atk_y, atk_x });
	dfs(atk_y, atk_x, vv);
	repair[atk_y][atk_x] = 1;
	int power = attack[atk_y][atk_x];
	for (int i = 1; i < (int)vv.size() - 1; i++) {
		int y = vv[i].first, x = vv[i].second;
		attacked(y, x, power / 2);
	}
	attacked(def_y, def_x, power);
}

void bomb() {
	repair[atk_y][atk_x] = 1;
	int power = attack[atk_y][atk_x];
	attacked(def_y, def_x, power);
	int y = def_y, x = def_x;
	for (int i = 0; i < 8; i++) {
		int ny = y + dy[i], nx = x + dx[i];
		ny += n; nx += m;
		ny %= n; nx %= m;
		if (attack[ny][nx] == 0) continue;
		if (ny == atk_y && nx == atk_x) continue;
		attacked(ny, nx, power / 2);
	}
}

void go() {
	// 공격자 피공격자 찾기
	memset(repair, 0, sizeof(repair));
	v.clear();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (attack[i][j]) v.push_back({ attack[i][j], last_attacked[i][j], i, j });
		}
	}
	if ((int)v.size() <= 1) return;
	sort(v.begin(), v.end(), cmp);
	atk_y = v.front().y; atk_x = v.front().x;
	last_attacked[atk_y][atk_x] = turn;
	attack[atk_y][atk_x] += (n + m);
	def_y = v.back().y; def_x = v.back().x;
	
	memset(visited, 0, sizeof(visited));
	bfs();
	if (visited[def_y][def_x]) lazer();
	else bomb();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (repair[i][j] == 0 && attack[i][j]) attack[i][j]++;
		}
	}
}

int main() {
	init();
	cin >> n >> m >> k;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> attack[i][j];
		}
	}
	for (turn = 1; turn <= k; turn++) {
		//cout << turn << " turn " << '\n';
		go();
		if ((int)v.size() <= 1) break;
		//print();
	}

	ll ret = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, attack[i][j]);
		}
	}
	cout << ret << '\n';
	return 0;
}