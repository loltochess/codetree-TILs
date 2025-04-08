#include <iostream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>
#include <map>

using namespace std;
typedef pair<int, int> pii;
typedef long long ll;

void fastIO()
{
	ios_base::sync_with_stdio(0);
	cin.tie(NULL);
	cout.tie(NULL);
}

// 1 -> 3, n -> n + 2
int land[80][80];
// bfs용
int visited[80][80];
vector<pii> v;

typedef struct _State
{
	int y, x, dir;
} State;

const int dy[] = { -1, 0, 1, 0 }; // 북 동 남 서
const int dx[] = { 0, 1, 0, -1 };

vector<pii> southDir = {
	{1, -1}, {1, 1}, {2, 0}
};

vector<pii> westDir = {
	{-1, -1}, {0, -2}, {1, -2}, {1, -1}, {2, -1}
};

vector<pii> eastDir = {
	{-1, 1}, {0, 2}, {1, 1}, {1, 2}, {2, 1}
};

int r, c, k;
int ret;
int nowRobotIdx;

bool is_out(int y, int x)
{
	// y는 0~r+2 까지, x는 1~c 까지임에 유의
	return (y > r + 2 || y < 0 || x > c || x < 1);
}

// 해당 모든 좌표에 이동할 수 있는가?
bool isCleared(State golem, const vector<pii>& v)
{
	int y = golem.y;
	int x = golem.x;
	for (pii a : v)
	{
		int ny = y + a.first;
		int nx = x + a.second;
		if (is_out(ny, nx) || land[ny][nx]) return false;
	}
	return true;
}

// visited배열에 적으면서한다.
// land에서 nowRobotIdx은 로봇, *10000은 출구임에 유의한다.
queue<pii> q;

int bfs(int y, int x)
{
	memset(visited, 0, sizeof(visited));
	
	int ret = y;
	visited[y][x] = 1;
	
	q.push({ y, x });
	
	while (q.size())
	{
		pii a = q.front();
		q.pop();

		int y = a.first;
		int x = a.second;
		//cout << y << " : " << x << " : " << land[y][x] << endl;
		ret = max(ret, y);

		for (int i = 0; i < 4; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if (is_out(ny, nx) || visited[ny][nx] || land[ny][nx] == 0) continue;

			// 만약 {y,x} 가 출구가 아닐 경우 -> 자기 로봇만 갈 수 있음
			if (land[y][x] < 10000)
			{
				if (land[ny][nx] == land[y][x] || land[ny][nx] == land[y][x] * 10000)
				{
					visited[ny][nx] = 1;
					q.push({ ny, nx });
				}
			}
			// 출구일 경우 아무데나 갈 수 있음.
			else
			{
				visited[ny][nx] = 1;
				q.push({ ny, nx });
			}
		}
	}

	return ret;
}

void solve(int x, int d)
{
	State golem = { 1, x, d };
	
	while (1)
	{
		if (isCleared(golem, southDir))
		{
			golem.y += 1;
		}
		else if (isCleared(golem, westDir))
		{
			golem.y += 1;
			golem.x -= 1;
			golem.dir = (golem.dir + 3) % 4;
		}
		else if (isCleared(golem, eastDir))
		{
			golem.y += 1;
			golem.x += 1;
			golem.dir = (golem.dir + 1) % 4;
		}
		else
		{
			//cout << golem.y << " : " << golem.x << " : " << golem.dir << "에서 못움직임 " << endl;
			break;
		}
	}

	// 골렘 일부가 나와 있는 경우 -> 모두 복귀 후 종료
	if (golem.y <= 3)
	{
		memset(land, 0, sizeof(land)); // clear!
		//cout << "골렘 나가!" << endl;
		return;
	}

	// 골렘을 색칠한다.
	land[golem.y][golem.x] = nowRobotIdx;
	for (int i = 0; i < 4; i++)
	{
		int color = (i == golem.dir) ? (10000 * nowRobotIdx) : nowRobotIdx;
		land[golem.y + dy[i]][golem.x + dx[i]] = color;
	}

	// bfs 해서 최대한 남쪽으로 보내면서, 도달하는 최대 y를 리턴한다.
	int max_y = bfs(golem.y, golem.x);

	//cout << x << " : " << d << " 쿼리에 대한 행 값 : " << max_y - 2 << endl;
	ret += max_y - 2;
	return;
}

int main()
{
	fastIO();
	cin >> r >> c >> k;
	nowRobotIdx = 0;
	for (int i = 0; i < k; i++)
	{
		int x, d;
		cin >> x >> d;
		nowRobotIdx++;
		solve(x, d);
	}
	cout << ret << endl;
}

