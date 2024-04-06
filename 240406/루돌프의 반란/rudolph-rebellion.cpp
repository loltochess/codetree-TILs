#pragma warning(disable: 4996) 
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>

using namespace std;

int N, M, P, C, D;
int score[34];
vector<pair<int, pair<int, int> >> santa;
int dead[34];
int paralyzed[34];
pair<int, int> deer;
const pair<int, int> move_deer[8] = { {-1,0} , {-1, 1}, {0, 1}, {1, 1},
									{1, 0} , {1, -1}, {0, -1}, {-1, -1}};
const pair<int, int> move_santa[4] = { {-1, 0}, {0, 1}, {1, 0}, {0, - 1 } };

int find_dist(int y, int x, int sy, int sx) {
	return (y - sy) * (y - sy) + (x - sx) * (x - sx);
}

int find_shortest_santa() {
	int min_idx = -1;
	int min_dist = 100000;
	pair<int, int> min_location = { 100, 100 };
	for (int i = 0; i < santa.size(); i++) {
		if (dead[i]) continue;
		int santa_idx = santa[i].first;
		int sy = santa[i].second.first;
		int sx = santa[i].second.second;
		int y = deer.first, x = deer.second;
		int dist = find_dist(y, x, sy, sx);
		if (min_dist > dist) {
			min_dist = dist;
			min_idx = santa_idx;
			min_location = { sy, sx };
		}
		else if (min_dist == dist) {
			if (min_location.first < sy) {
				min_dist = dist; min_idx = santa_idx;
				min_location = { sy, sx };
			}
			else if (min_location.first == sy && min_location.second < sx) {
				min_dist = dist; min_idx = santa_idx; min_location = { sy, sx };
			}
		}
	}
	return min_idx;
}

void collision(int santa_idx, int delta_y, int delta_x, int point, bool deer2santa, bool santa2deer, bool santa2santa) {
	score[santa_idx] += point; // 포인트를 얻습니다.
	if (santa2santa) score[santa_idx] -= point; // santas2santa면 포인트 못얻음.
	if (deer2santa || santa2deer) paralyzed[santa_idx] = 2; // 산타끼리 충돌이면 마비 없음


	// 산타 -> 루돌프면 D칸 , 루돌프 -> 산타면 C칸, 산타 -> 산타는 1칸
	int ny, nx;
	ny = santa[santa_idx].second.first + delta_y * point;
	nx = santa[santa_idx].second.second + delta_x * point;


	if (ny < 1 || ny > N || nx < 1 || nx > N) {
		dead[santa_idx] = 1;
		santa[santa_idx].second.first = -1, santa[santa_idx].second.second = -1; //디버깅쉽게 표시해놓기
		//cout << santa_idx+1 << " Santa dead" << "\n";
		return;// 산타 죽이고 리턴
	}
	for (int i = 0; i < santa.size(); i++) { // 또 충돌되는 산타 찾기
		if (i == santa_idx) continue; //당연히 나 자신은 제외
		int sy = santa[i].second.first, sx = santa[i].second.second;
		if (ny == sy && nx == sx) {//찾았다
			santa[santa_idx].second.first = ny;
			santa[santa_idx].second.second = nx;
			//cout << santa_idx + 1 << " collision moves to " << ny << " " << nx << "\n";
			//cout << "Collision Occurs : " << santa_idx + 1 << " to " << i + 1 << "\n";
			collision(i, delta_y, delta_x, 1, 0, 0, 1);
			return;
		}
	}
	santa[santa_idx].second.first = ny;
	santa[santa_idx].second.second = nx;
	//cout << santa_idx + 1 << " collision moves to " << ny << " " << nx << "\n";
	return;
}

void deer_turn() {
	int santa_idx = find_shortest_santa();
	if (santa_idx == -1) return;
	int y = deer.first, x = deer.second;
	int sy = santa[santa_idx].second.first, sx = santa[santa_idx].second.second;
	int move_direction = -1;
	int min_dist = 100000;
	for (int i = 0; i < 8; i++) {
		int ny = y + move_deer[i].first, nx = x + move_deer[i].second;
		if (ny < 1 || ny > N || nx < 1 || nx > N) continue;
		if (ny == sy && nx == sx) { // 충돌하면 충돌시키고 바로 리턴!
			deer.first = ny, deer.second = nx;
			//cout << "Deer collision moves to " << ny << " " << nx << "\n";
			collision(santa_idx, move_deer[i].first, move_deer[i].second, C, 1, 0, 0);
			return;
		}
		int dist = find_dist(ny, nx, sy, sx);
		if (dist <= min_dist) {
			min_dist = dist; move_direction = i;
		}
	}
	deer.first += move_deer[move_direction].first; deer.second += move_deer[move_direction].second;
}

void santa_turn() {
	for (int i = 0; i < P; i++) {
		if (paralyzed[i]) continue;
		if (dead[i]) continue; //죽었으면 당연히 못움직임.
		int y = santa[i].second.first, x = santa[i].second.second;
		int dist = find_dist(y, x, deer.first, deer.second);
		int move_direction = -1;
		int next_y = y, next_x = x;
		for (int j = 0; j < 4; j++) {
			int ny = y + move_santa[j].first, nx = x + move_santa[j].second;
			if (ny <1 || ny > N || nx < 1 || nx > N) continue;
			bool cant_move = false; // 산타랑 겹쳐서 못움직임
			for (int k = 0; k < P; k++) {
				if (dead[k] || santa[k].first == i) continue; // 이미 죽은 산타거나 자신은 제외
				if (ny == santa[k].second.first && nx == santa[k].second.second) {
					cant_move = true;
					break;
				}
			}
			if (cant_move) continue;
			int new_dist = find_dist(ny, nx, deer.first, deer.second);
			if (new_dist < dist) { //  < 쓰면 자동으로 상우하좌 우선순위 맞춰짐.
				dist = new_dist,  next_y = ny, next_x = nx; move_direction = j;
			}
		}
		
		santa[i].second.first = next_y, santa[i].second.second = next_x; // 움직인다 또는 가만히있는다.
		if ((next_y == deer.first) && (next_x == deer.second)){
			//산타 -> 루돌프 충돌 이때 반대방향 = (move_direction+2) % 4
			int opposite_direction = (move_direction + 2) % 4;
			int delta_y = move_santa[opposite_direction].first;
			int delta_x = move_santa[opposite_direction].second;
			collision(i, delta_y, delta_x, D, 0, 1, 0);
		}
	}
	return;
}

int main(int argc, char** argv) {

	//freopen("input.txt", "r", stdin);
	int T;
	//cin >> T;

	for (int test_case = 1; test_case <= 1; ++test_case) {


		////////////////////////////////////////
		memset(score, 0, sizeof(score));
		memset(paralyzed, 0, sizeof(paralyzed));
		memset(dead, 0, sizeof(dead));
		santa.clear();
		cin >> N >> M >> P >> C >> D;
		cin >> deer.first >> deer.second;
		for (int i = 0; i < P; i++) {
			int idx, y, x;
			cin >> idx >> y >> x;
			santa.push_back({ idx-1, {y, x} }); // 산타 실제 번호는 무시하고 i = 0  -> santa = 1 순서로간다. 너무헷갈려서
		}
		sort(santa.begin(), santa.end()); 
		
		// 게임 시작!
		while (M--) {
			//cout << M << " turn" << "\n";
			//cout << "deer_turn" << "\n";
			deer_turn();
			//cout << deer.first << " " << deer.second << "\n";
			int cnt = 0;
			for (int i = 0; i < P; i++) {
				if(dead[i]) cnt++;
			}
			if (cnt == P) break;
			//cout << "santa_turn" << "\n";
			santa_turn();
			for (int i = 0; i < P; i++) {
				if (!dead[i]) score[i]++;
			}
			//for (int i = 0; i < P; i++) {
			//	cout << i + 1 << " : " << santa[i].second.first << " " << santa[i].second.second << "\n";
			//}
			for (int i = 0; i < P; i++) {
				if (paralyzed[i]) paralyzed[i]--;
			}
			//cout << "score " << "\n";
			//for (int i = 0; i < P; i++) {
			//	cout << score[i] << " ";
			//}
			//cout << "\n";
		}
		//cout << "final score " << "\n";
		for (int i = 0; i < P; i++) {
			cout << score[i] << " ";
		}
		cout << "\n";
		

		//점수 출력
		////////////////////////////////////////
		//cout<< "#" << test_case << " " << ans << "\n";
	}

	return 0;

}