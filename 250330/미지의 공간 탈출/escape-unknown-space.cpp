#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
void fastIO() {
  ios_base::sync_with_stdio(0);
  cin.tie(NULL);
  cout.tie(NULL);
}

typedef struct _State
{
  int dim, y, x;
}State;

int land[6][24][24]; // {dim, y, x};

int visited[6][24][24];
int fire_dist[6][24][24];
int machine_dist[6][24][24];
State timeMachine;
State exitPoint;

typedef struct _Fire
{
  int y, x, d, v;
}Fire;

Fire fire[14];
int n, m, f;
const int dy[] = {0, 0, 1, -1}; // 동서남북
const int dx[] = {1, -1, 0, 0}; // 동서남북

int mountain_start_y;
int mountain_start_x;

bool is_out(int dim, int y, int x)
{
  int len = (dim == 5) ? n : m;
  return (y < 0 || y >= len || x < 0 || x >= len);
}

State change_dim(int dim, int y, int x, int dir)
{
  State newState = {-1, -1, -1};
  int len = m;

  // 동 서 남 북 위순으로 탐색하자..

  switch (dim)
  {
    // 0. 동 -> {북, 남, 땅, 위} 3, 2, 5, 4
    case 0 : // 동서남북 순으로 탐색
      if (x == len) // 동의 동쪽 -> 북
      {
        newState = {3, y, 0};
      }
      if (x < 0) // 동의 서쪽 -> 남
      {
        newState = {2, y, len - 1};
      }
      if (y == len) // 동의 남쪽 -> 땅
      {
        newState = {5, mountain_start_y + (len - 1 - x), 
                        mountain_start_x + len};
      }
      if (y < 0) // 동의 북 -> 위
      {
        newState = {4, len - 1 - x, len - 1};
      }
      break;
    // 1. 서 -> {남, 북, 땅, 위} 2, 3, 5, 4
    case 1 :
      if (x == len) // 서의 동쪽 -> 남
      {
        newState = {2, y, 0};
      }
      if (x < 0) // 서의 서쪽 -> 북
      {
        newState = {3, y, len - 1};
      }
      if (y == len) // 서의 남쪽 -> 땅
      {
        newState = {5, mountain_start_y + x, mountain_start_x - 1};
      }
      if (y < 0) // 서의 북쪽 -> 위
      {
        newState = {4, x, 0};
      }
      break;
    // 2. 남 -> {동, 서, 땅, 북} 0, 1, 5, 4
    case 2:
      if (x == len) // 남의 동쪽 -> 동
      {
        newState = {0, y, 0};
      }
      if (x < 0) // 남의 서쪽 -> 서
      {
        newState = {1, y, len - 1};
      }
      if (y == len) // 남의 남쪽 -> 땅
      {
        newState = {5, mountain_start_y + len, mountain_start_x + x};
      }
      if (y < 0) // 남의 북쪽 -> 위
      {
        newState = {4, len - 1, x};
      }
      break;
    // 3. 북 -> {서, 동, 땅, 위} 1, 0, 5, 4
    case 3:
      if (x == len) // 북의 동쪽 -> 서
      {
        newState = {1, y, 0};
      }
      if (x < 0) // 북의 서쪽 -> 동
      {
        newState = {0, y, len - 1};
      }
      if (y == len) // 북의 남쪽 -> 땅
      {
        newState = {5, mountain_start_y - 1, mountain_start_x + (len - 1 - x)};
      }
      if (y < 0) // 북의 북쪽 -> 위
      {
        newState = {4, 0, len - 1 - x};
      }
      break;
    // 4. 위 -> {동, 서, 남, 북} 0 1 2 3
    case 4:
      if (x == len) // 위의 동쪽 -> 동
      {
        newState = {0, 0, len - 1 - y};
      }
      if (x < 0) // 위의 서쪽 -> 서
      {
        newState = {1, 0, y};
      }
      if (y == len) // 위의 남쪽 -> 남
      {
        newState = {2, 0, x};
      }
      if (y < 0) // 위의 북쪽 -> 북
      {
        newState = {3, 0, len - 1 - x};
      }
      break;
    default:
      break;
  }

  return newState;
}

State go_next(int dim, int y, int x, int dir)
{
  int ny = y + dy[dir];
  int nx = x + dx[dir];

  if (dim >= 0 && dim <= 4)
  {
    if (is_out(dim, ny, nx)) 
    {
      return change_dim(dim, ny, nx, dir); // 차원 바꾸기
    }
    else 
    {
      return {dim, ny, nx}; // 그냥 이동
    }
  }
  else
  {
    // 벽을 타는 것은 무시-> 못움직이는 걸로 처리
    // 진짜 밖에 나가는 경우는 못움직임
    if (is_out(dim, ny, nx) || land[dim][ny][nx] == 3)
    {
      return {dim, y, x}; // 입력을 리턴 -> 못움직인다
    }
    else
    {
      return {dim, ny, nx}; // 그냥 이동
    }
  }
}

// 엄밀히는 bfs가 아님
// 장애물, 경계, 탈출구 만나기까지 직진만하면서 최솟값 갱신
// 이때, 땅에서 벽타는 경우는 새로 큐에 넣지 않는다.
// 이유: 벽에서 땅으로 가는 경로는 하나밖에 없으므로
//       입구막기의 효과가 존재하기 때문
void fire_bfs()
{
  memset(fire_dist, -1, sizeof(fire_dist));
  
  queue<Fire> q;
  for (int i = 0; i < f; i++)
  {
    q.push(fire[i]);
    fire_dist[5][fire[i].y][fire[i].x] = 0;
  }

  while(q.size())
  {
    Fire here = q.front(); q.pop();
    State nextLocation = go_next(5, here.y, here.x, here.d);
    // 경계에 도달하거나, 벽을 타야한다면 종료
    if (here.y == nextLocation.y && here.x == nextLocation.x) continue;
    // 빈공간만 이동 -> 장애물, 탈출구 만나면 종료 (탈출구엔 불 못붙음에 유의)
    if (land[5][nextLocation.y][nextLocation.x] == 0) 
    {
      // 최솟값 경신
      if (fire_dist[5][nextLocation.y][nextLocation.x] == -1)
      {
        fire_dist[5][nextLocation.y][nextLocation.x] 
          = fire_dist[5][here.y][here.x] + here.v;
      }
      fire_dist[5][nextLocation.y][nextLocation.x] = 
        min(fire_dist[5][nextLocation.y][nextLocation.x], 
          fire_dist[5][here.y][here.x] + here.v);
      // 최솟값 경신과 관계없이 queue에 넣음.
      q.push({nextLocation.y, nextLocation.x, here.d, here.v});
    }
  }
}

// timeMachine에 대한 진짜 bfs 
void machine_bfs()
{
  memset(machine_dist, -1, sizeof(machine_dist));

  queue<State> q;
  q.push(timeMachine);
  machine_dist[timeMachine.dim][timeMachine.y][timeMachine.x] = 0;

  while(q.size())
  {
    State here = q.front(); q.pop();
    for (int dir = 0; dir < 4; dir++)
    {
      State nextLocation = go_next(here.dim, here.y, here.x, dir);
      int ndim = nextLocation.dim;
      int ny = nextLocation.y;
      int nx = nextLocation.x;

      // 장애물은 못지나감
      if (land[ndim][ny][nx] == 1) continue;
      // 방문한 곳 못지나감
      if (machine_dist[ndim][ny][nx] != -1) continue;
      // 불이 먼저 도달한 경우
      if (ndim == 5 && fire_dist[ndim][ny][nx] != -1 && fire_dist[ndim][ny][nx] 
        <= machine_dist[here.dim][here.y][here.x] + 1) continue;

      machine_dist[ndim][ny][nx] = machine_dist[here.dim][here.y][here.x] + 1;
      q.push(nextLocation);
    }
  }
}

void print()
{
  cout << timeMachine.dim << " : " << timeMachine.y << " : " << timeMachine.x << endl;
  cout << "fire : " << endl;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      cout << fire_dist[5][i][j] << ' ';
    }
    cout << endl;
  }
  cout << endl;

  cout << "동 서 남 북 위 평면" << endl;
  for (int k = 0; k < 5; k++)
  {
    cout << " :  : " << k << " :  : " << endl;
    for (int i = 0; i < m; i++)
    {
      for (int j = 0; j < m; j++)
      {
        cout << machine_dist[k][i][j] << ' ';
      }
      cout << endl;
    }
    cout << endl;
  }
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      cout << machine_dist[5][i][j] << ' ';
    }
    cout << endl;
  }
}

int main()
{
  cin >> n >> m >> f;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      cin >> land[5][i][j];
      if (land[5][i][j] == 4)
      {
        exitPoint = {5, i, j};
      }
    }
  }
  for (int dim = 0; dim < 5; dim++)
  {
    for (int i = 0; i < m; i++)
    {
      for (int j = 0; j < m; j++)
      {
        cin >> land[dim][i][j];
        if (land[dim][i][j] == 2)
        {
          timeMachine = {dim, i, j};
        }
      }
    }
  }
  for (int i = 0; i < f; i++)
  {
    cin >> fire[i].y >> fire[i].x >> fire[i].d >> fire[i].v;
  }

  int flag = false;
  for (int i = 0; i < n; i++)
  {
    if(flag) break;
    for (int j = 0; j < n; j++)
    {
      if (land[5][i][j] == 3)
      {
        mountain_start_y = i;
        mountain_start_x = j;
        flag = true;
        break;
      }
    }
  }
  
  memset(machine_dist, -1, sizeof(machine_dist));
  fire_bfs();
  machine_bfs();
  //print();
  cout << machine_dist[exitPoint.dim][exitPoint.y][exitPoint.x] << endl;
}