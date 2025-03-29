#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
void fastIO() {
  ios_base::sync_with_stdio(0);
  cin.tie(NULL);
  cout.tie(NULL);
}

typedef struct _Warrior
{
  int y, x, freeze, live;
}Warrior;

vector<Warrior> warrior_vec;
int park_dist[54][54]; // medusa에서 park까지의 최단거리
int visited[4][54][54];

// 북쪽이 0, 이후 시계방향으로 1,2, ... 7
const int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};

vector<vector<int>> dirs = {
  {7, 0, 1},
  {3, 4, 5},
  {7, 6, 5},
  {3, 2, 1}
};

int n,m;
int my, mx, py, px; // medusa, park
int road[54][54];
int warrior_land[54][54];

typedef struct _Info
{
  int warrior_move_sum;
  int warrior_freezed;
  int warrior_attack_succeed;
} Info;

Info turn_info;

bool is_out(int y, int x)
{
  return (y < 0 || y >= n || x < 0 || x >= n);
}

int go(int y, int x)
{
  if (y == py && x == px) return park_dist[y][x] = 0;
  
  int& ret = park_dist[y][x];
  if (ret != -1) return ret;

  ret = 1e9;
  for (int i = 0; i < 4; i++)
  {
    int dir = dirs[i][1];
    int ny = y + dy[dir];
    int nx = x + dx[dir];
    if (is_out(ny, nx) || road[ny][nx] == 1) continue;
    ret = min(ret, 1 + go(ny, nx));
  }
  
  return ret;
}

void calc_dist()
{
  memset(park_dist, -1, sizeof(park_dist));
  go(my, mx);
}

void medusa_move()
{
  for (int i = 0; i < 4; i++)
  {
    // 상 하 좌 우 순서.
    int medusa_dir = dirs[i][1];
    int ny = my + dy[medusa_dir];
    int nx = mx + dx[medusa_dir];
    if (is_out(ny, nx) || park_dist[ny][nx] == -1) continue;
    // 최단경로 존재하는 경우만 구하는중.
    if (park_dist[my][mx] == park_dist[ny][nx] + 1)
    {
      my = ny;
      mx = nx;
      for (Warrior& w : warrior_vec)
      {
        if (w.live == 0) continue;
        if (w.y == my && w.x == mx)
        {
          w.live = 0;
          warrior_land[w.y][w.x] -= 1;
        }
      }
      return;
    }
  }
  return;
}

int max_freeze_dir;

void medusa_look()
{
  memset(visited, 0, sizeof(visited));
  int max_freeze_cnt = 0;
  for (int i = 0; i < 4; i++)
  {
    vector<int> look_dir = dirs[i];
    queue<pii> q;
    int freeze_cnt = 0;
    q.push({my, mx});
    while(q.size())
    {
      pii here = q.front(); q.pop();
      int y = here.first;
      int x = here.second;
      for (int dir : look_dir)
      {
        int ny = y + dy[dir];
        int nx = x + dx[dir];
        if (is_out(ny, nx) || visited[i][ny][nx]) continue;
        if (warrior_land[ny][nx])
        {
          freeze_cnt += warrior_land[ny][nx];
          visited[i][ny][nx] = 1;
          queue<pii> warrior_q;
          vector<int> warrior_dir;
          if (ny == my || nx == mx)
          {
            warrior_dir.push_back(look_dir[1]);
          }
          else
          {
            if ( (ny - my) / (nx - mx) > 0)
            {
              warrior_dir.push_back(look_dir[0]);
              warrior_dir.push_back(look_dir[1]);
            }
            else
            {
              warrior_dir.push_back(look_dir[1]);
              warrior_dir.push_back(look_dir[2]);
            }
          }
          warrior_q.push({ny, nx});
          while(warrior_q.size())
          {
            pii here = warrior_q.front(); warrior_q.pop();
            int y = here.first;
            int x = here.second;
            for (int dir : warrior_dir)
            {
              int ny = y + dy[dir];
              int nx = x + dx[dir];
              if (is_out(ny, nx)) continue;
              visited[i][ny][nx] = 100;
              warrior_q.push({ny, nx});
            }
          }
        }
        else
        {
          q.push({ny, nx});
          visited[i][ny][nx] = 1;
        }
      }
    }

    if (max_freeze_cnt < freeze_cnt)
    {
      max_freeze_cnt = freeze_cnt;
      max_freeze_dir = i;
    }
  }

  // for (int dir = 0 ; dir < 4; dir++)
  // {
  //   cout << dir << endl;
  //   for (int i = 0; i < n; i++)
  //   {
  //     for (int j = 0; j < n; j++)
  //     {
  //       cout << visited[dir][i][j] << ' ';
  //     }
  //     cout << endl;
  //   }
  // }

  //cout << max_freeze_cnt << " : " << max_freeze_dir << endl;

  for (Warrior& w : warrior_vec)
  {
    if (w.live == 0) continue;
    if (visited[max_freeze_dir][w.y][w.x] == 1)
    {
      w.freeze = 1;
      turn_info.warrior_freezed++;
    }
  }
}

int man_calc_dist(int y1, int x1, int y2, int x2)
{
  return abs(y1 - y2) + abs(x2 - x1);
}

vector<int> warrior_dir[2] = {
  {0, 4, 6, 2},
  {6, 2, 0, 4}
};

void warrior_move(int flag)
{
  //cout << "이동시작!!!" << endl;
  //cout << " 메두사는 " << " : " << my << " : " << mx << endl;
  for (Warrior& w : warrior_vec)
  {
    //cout << w.y << " : " << w.x << " : " << w.freeze << " :  " << w.live << endl;
    if (w.live == 0) {
      //cout << w.y << " : " << w.x << " 는 죽음" << endl;
      continue;
    }
    if (w.freeze == 1)
    {
      //cout << w.y << " : " << w.x << " 는 얼어있음" << endl;
      if(flag == 1) w.freeze = 0; 
      continue;
    }
    for (int i = 0; i < 4; i++)
    {
      // 상 하 좌 우
      int dir = warrior_dir[flag][i];
      int ny = w.y + dy[dir];
      int nx = w.x + dx[dir];
      if (is_out(ny, nx) || 
            visited[max_freeze_dir][ny][nx] == 1) continue;
      if (man_calc_dist(ny, nx, my, mx) + 1 
            == man_calc_dist(w.y, w.x, my, mx))
      {
        //cout << ny << " : " << nx << "로 이동.." << endl;
        warrior_land[w.y][w.x]--;
        w.y = ny;
        w.x = nx;
        turn_info.warrior_move_sum++;
        warrior_land[w.y][w.x]++;
        if (ny == my && nx == mx)
        {
          //cout << "장렬히 전사" << endl;
          w.live = 0;
          turn_info.warrior_attack_succeed++;
          warrior_land[w.y][w.x]--;
        }
        break;
      }
    }
  }
}

int main()
{
  cin >> n >> m;
  cin >> my >> mx >> py >> px;
  for (int i = 0; i < m; i++)
  {
    int y, x;
    cin >> y >> x;
    warrior_vec.push_back({y, x, 0, 1});
    warrior_land[y][x]++;
  }
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      cin >> road[i][j];
    }
  }

  calc_dist();

  if (park_dist[my][mx] == 1e9)
  {
    cout << -1 << endl; exit(0);
  }

  // cout << endl;
  // for (int i = 0; i < n; i++)
  // {
  //   for (int j = 0; j < n; j++)
  //   {
  //     cout << park_dist[i][j] << ' ';
  //   }
  //   cout << endl;
  // }

  // cout << endl;
  // for (int i = 0; i < n; i++)
  // {
  //   for (int j = 0; j < n; j++)
  //   {
  //     cout << warrior_land[i][j] << ' ';
  //   }
  //   cout << endl;
  // }
  // cout << endl;

  while (1)
  {
    turn_info = {0, 0, 0};
    medusa_move(); // 전사가 공격못하고 죽음
    if (my == py && mx == px)
    {
      cout << 0 << endl; return 0;
    }
    medusa_look(); // 전사를 얼림
    warrior_move(0); // 전사가 첫 번째 이동: 공격 후 죽음
    warrior_move(1); // 전사의 두 번째 이동: 공격 후 죽음
    cout << turn_info.warrior_move_sum << " " 
         << turn_info.warrior_freezed << " "
         << turn_info.warrior_attack_succeed << endl;
  }
}
