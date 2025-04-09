#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <climits>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <set>

using namespace std;
typedef pair<int, int> pii;
typedef long long ll;

void fastIO()
{
	ios_base::sync_with_stdio(0);
	cin.tie(NULL);
	cout.tie(NULL);
}

typedef struct _State
{
	int firstValue, rotCnt, x, y;
} State;

// 유적지
int land[7][7];
int temp[7][7];
// dfs용
int visited[7][7];
// 유물 추가
queue<int> q;

int k, m;

const int dy[] = { 0, 1, 0, -1 };
const int dx[] = { 1, 0, -1, 0 };

int ret;

void init()
{
	fastIO();
	cin >> k >> m;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cin >> land[i][j];
		}
	}

	for (int i = 0; i < m; i++)
	{
		int num;
		cin >> num;
		q.push(num);
	}
}

State cmp(State a, State b)
{
	if (a.firstValue == b.firstValue)
	{
		if (a.rotCnt <= b.rotCnt)
		{
			return a;
		}
		else
		{
			return b;
		}
	}
	
	if (a.firstValue > b.firstValue)
	{
		return a;
	}
	else
	{
		return b;
	}
}

void printL()
{
	cout << "print land " << endl;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << land[i][j] << ' ';
		}
		cout << endl;
	}
}

bool is_out(int y, int x)
{
	return (y < 0 || y >= 5 || x < 0 || x >= 5);
}

int dfs(int y, int x)
{
	visited[y][x] = true;

	int ret = 1;
	
	for (int i = 0; i < 4; i++)
	{
		int ny = y + dy[i];
		int nx = x + dx[i];
		if (is_out(ny, nx) || visited[ny][nx] || land[ny][nx] != land[y][x])
		{
			continue;
		}
		ret += dfs(ny, nx);
	}

	return ret;
}

void go(State& state)
{
	memset(visited, 0, sizeof(visited));
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (!visited[i][j])
			{
				int blockCnt = dfs(i, j);
				if (blockCnt >= 3)
				{
					state.firstValue += blockCnt;
					visited[i][j] = 100; // 3개 블록의 시작이라는 의미
				}
			}
		}
	}
	return;
}


void rotate90(int startY, int startX)
{
	memcpy(temp, land, sizeof(land));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp[startY + j][startX + 2 - i] = land[startY + i][startX + j];
		}
	}

	memcpy(land, temp, sizeof(temp));
}

void firstPang()
{
	State bestState = { 0, 0, 0, 0 };
	// 좌상단 좌표를 기준으로 회전
	// (0,0) 부터 (2,2) 까지 좌상단이 될 수 있음
	// x 작은 -> y 작은 순
	for (int j = 0; j <= 2; j++)
	{
		for (int i = 0; i <= 2; i++)
		{
			// state 초기화
			State state = { 0, 0, j, i };

			// 회전 90, 180, 270 후 계산해서 bestState 경신
			for (int k = 0; k < 3; k++)
			{
				state.firstValue = 0;
				rotate90(i, j);
				state.rotCnt++;
				go(state);
				bestState = cmp(bestState, state);
			}

			// 마지막에 한 번 더 회전시켜야 land가 정상화됨
			rotate90(i, j);
		}
	}

	// cout << bestState.rotCnt << " : " << bestState.y << " : " << bestState.x << endl;
	// 하나도 pang 할 수 없으면 프로그램 종료
	if (bestState.firstValue == 0)
	{
		exit(0);
	}

	// bestState 대로 land 돌린다음에 터트리자!
	for (int rot = 0; rot < bestState.rotCnt; rot++)
	{
		rotate90(bestState.y, bestState.x);
	}

	return;
}


queue<pii> pangQ;

void pang()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (visited[i][j] == 100)
			{
				pangQ.push({ i, j });
			}
		}
	}

	// 3개 넘는 놈들만 visited에 표시하자.
	memset(visited, 0, sizeof(visited));
	while (pangQ.size())
	{
		pii a = pangQ.front();
		pangQ.pop();

		dfs(a.first, a.second);
	}

	// pang 순서는 x 작은순 -> y 큰 순
	for (int j = 0; j < 5; j++)
	{
		for (int i = 4; i >= 0; i--)
		{
			if (visited[i][j])
			{
				int nextPiece = q.front();
				q.pop();
				land[i][j] = nextPiece;
			}
		}
	}
}

// firstPang 에서 돌린 상태로 끝나니깐
// 일단 터트려줘야 한다.
void secondPang()
{
	while (1)
	{
		State findState = { 0, 0, 0, 0 };
		go(findState);
		if (findState.firstValue == 0) break;
		ret += findState.firstValue;
		pang();
	}
}

void solve()
{
	ret = 0;
	firstPang();
	secondPang();
	//printL();
	cout << ret << ' ';
}

int main()
{
	init();
	for (int i = 0; i < k; i++)
	{
		solve();
	}
}