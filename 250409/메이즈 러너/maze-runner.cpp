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

// 직사각형 길이, 좌상단 y ,x 
typedef struct _Square
{
	int len, sy, sx;
} Square;

// man에 대한 live, y , x
typedef struct _State
{
	int live, y, x;
} State;

vector<State> manVec;

// wall 내구력
int wall[14][14];
int wallTemp[14][14];

// man 에 대한 bitmasking
ll man[14][14];
ll manTemp[14][14];

int exitY;
int exitX;
int manExitCnt;
int n, m, k;
int distSum;

// 상 하 좌 우 (상 하 우선순위임)
const int dy[] = { -1, 1, 0, 0 };
const int dx[] = { 0, 0, -1, 1 };

void init()
{
	cin >> n >> m >> k;
	manVec.resize(m);

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cin >> wall[i][j];
		}
	}

	for (int i = 0; i < m; i++)
	{
		cin >> manVec[i].y >> manVec[i].x;
		manVec[i].live = 1;

		man[manVec[i].y][manVec[i].x] |= (1 << i);
	}

	cin >> exitY >> exitX;
}

int calcDist(int y, int x)
{
	return abs(y - exitY) + abs(x - exitX);
}

// (1 ~ n) 까지임에 유의
bool is_out(int y, int x)
{
	return (y < 1 || y > n || x < 1 || x > n);
}

void manMove()
{
	for (int idx = 0; idx < m; idx++)
	{
		// 죽은놈
		if (manVec[idx].live == 0) continue;
		
		int y = manVec[idx].y;
		int x = manVec[idx].x;

		int now_dist = calcDist(y, x);
		for (int i = 0; i < 4; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];
			if (is_out(ny, nx) || wall[ny][nx]) continue;

			// 거리가 짧아진다면 이동 후 break
			if (calcDist(ny, nx) < now_dist)
			{
				man[y][x] &= ~(1 << idx);
				man[ny][nx] |= (1 << idx);
				manVec[idx].y = ny;
				manVec[idx].x = nx;
				distSum++;

				// 만약 출구에 도달했다면 죽인다.
				// man 에서 실제로 지워야 회전 시 이동하지 않는다.
				if (ny == exitY && nx == exitX)
				{
					manVec[idx].live = 0;
					man[ny][nx] = 0;
					manExitCnt++;
				}

				break;
			}
		}
	}
}

// b로 바꾸어야 하는가?
bool changeAble(Square a, Square b)
{
	if (a.len > b.len) return true;
	if (a.len == b.len)
	{
		if (a.sy > b.sy) return true;
		if (a.sy == b.sy && a.sx > b.sx)
		{
			return true;
		}
	}

	return false;
}

Square findSquare()
{
	Square bestSquare = { 100, 0, 0 };
	for (const State s : manVec)
	{
		Square square = { 100, 0, 0 };
		if (s.live == 0) continue;
		
		// 현재 state 점과 exit 점을 비교할거임
		// 최대 y값, 최대 x값 저장함
		int minY = min(exitY, s.y);
		int maxY = max(exitY, s.y);

		int minX = min(exitX, s.x);
		int maxX = max(exitX, s.x);

		int deltaY = abs(exitY - s.y);
		int deltaX = abs(exitX - s.x);

		// 1. y값 변화가 더 큰경우, 좌상단 y좌표는 minY로 고정된다.
		if (deltaY >= deltaX)
		{
			square.len = deltaY;
			square.sy = minY;
			for (int j = 1; j <= minX; j++)
			{
				if (j + deltaY >= maxX)
				{
					square.sx = j;
					break;
				}
			}
		}

		// 2. x값 변화가 더 큰 경우, 좌상단 x좌표는 minX이다.
		else
		{
			square.len = deltaX;
			square.sx = minX;
			for (int i = 1; i <= minY; i++)
			{
				if (i + deltaX >= maxY)
				{
					square.sy = i;
					break;
				}
			}
		}

		if (changeAble(bestSquare, square))
		{
			bestSquare = square;
		}
	}

	return bestSquare;
}

// wall 을 돌리면서, 내구도를 1 깎는다.
// man은 그냥 돌린다.
void rotate90(Square square)
{
	memcpy(wallTemp, wall, sizeof(wall));
	memcpy(manTemp, man, sizeof(man));
	
	int len = square.len;
	int sy = square.sy;
	int sx = square.sx;

	bool exitChanged = false;

	for (int i = 0; i <= len; i++)
	{
		for (int j = 0; j <= len; j++)
		{
			// 벽 회전
			wallTemp[sy + j][sx + len - i] = wall[sy + i][sx + j];
			if (wallTemp[sy + j][sx + len - i] != 0)
			{
				wallTemp[sy + j][sx + len - i]--;
			}

			// 사람 회전
			manTemp[sy + j][sx + len - i] = man[sy + i][sx + j];
			// 벡터 업데이트
			for (int idx = 0; idx < m; idx++)
			{
				if (manVec[idx].live == 0) continue;
				if (man[sy + i][sx + j] & (1 << idx))
				{
					manVec[idx].y = sy + j;
					manVec[idx].x = sx + len - i;
				}
			}

			// exit 관리
			if (exitChanged == 0 && sy + i == exitY && sx + j == exitX)
			{
				exitY = sy + j;
				exitX = sx + len - i;
				exitChanged = true;
			}
		}
	}


	memcpy(wall, wallTemp, sizeof(wall));
	memcpy(man, manTemp, sizeof(man));
}

int main()
{
	fastIO();
	init();
	while (k--)
	{
		manMove();
		if (manExitCnt == m)
		{
			break;
		}
		Square square = findSquare();
		rotate90(square);
	}

	cout << distSum << '\n';
	cout << exitY << " " << exitX << '\n';
}
