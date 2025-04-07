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

struct Node
{
	int left, right, idx, cnt;
	ll sum;
};

string indexToString[100005];
int indexToValue[100005];
map<string, int> strToIndex;
map<int, int> usedValue;
int entryCnt;
vector<Node> tree;

typedef struct _DynamicSegmentTree
{
	void updateRec(int node, int nodeLeft, int nodeRight, int value, int mapIndex, int count, int num)
	{
		if (value > nodeRight || value < nodeLeft)
		{
			return;
		}

		if (nodeLeft == nodeRight)
		{
			tree[node].idx = mapIndex;
			tree[node].cnt = count;
			tree[node].sum = num;
			return;
		}

		int mid = (nodeLeft + nodeRight) / 2;
		if (value <= mid)
		{
			if (!tree[node].left)
			{
				tree[node].left = (int)tree.size();
				tree.push_back({ 0, 0, 0, 0, (ll)0 });
			}
			updateRec(tree[node].left, nodeLeft, mid, value, mapIndex, count, num);
		}
		else
		{
			if (!tree[node].right)
			{
				tree[node].right = (int)tree.size();
				tree.push_back({ 0, 0, 0, 0, (ll)0 });
			}
			updateRec(tree[node].right, mid + 1, nodeRight, value, mapIndex, count, num);
		}

		tree[node].cnt = tree[tree[node].left].cnt + tree[tree[node].right].cnt;
		tree[node].sum = tree[tree[node].left].sum + tree[tree[node].right].sum;
		return;
	}

	ll querySumRec(int node, int nodeLeft, int nodeRight, int queryLeft, int queryRight)
	{
		if (queryRight < nodeLeft || queryLeft > nodeRight)
		{
			return 0;
		}

		if (queryLeft <= nodeLeft && nodeRight <= queryRight)
		{
			return tree[node].sum;
		}

		int mid = (nodeLeft + nodeRight) / 2;
		
		ll ret = 0;
		if (tree[node].left)
		{
			ret += querySumRec(tree[node].left, nodeLeft, mid, queryLeft, queryRight);
		}
		if (tree[node].right)
		{
			ret += querySumRec(tree[node].right, mid + 1, nodeRight, queryLeft, queryRight);
		}
		return ret;
	}

	ll queryRankRec(int node, int nodeLeft, int nodeRight, int rank)
	{
		if (nodeLeft == nodeRight)
		{
			return tree[node].idx;
		}

		int mid = (nodeLeft + nodeRight) / 2;
		if (tree[node].left)
		{
			if (tree[tree[node].left].cnt >= rank)
			{
				return queryRankRec(tree[node].left, nodeLeft, mid, rank);
			}
			return queryRankRec(tree[node].right, mid + 1, nodeRight, rank - tree[tree[node].left].cnt);
		}
		return queryRankRec(tree[node].right, mid + 1, nodeRight, rank);
	}

	void init()
	{
		tree.resize(2);
		tree[1] = { 0, 0, 0, 0, (ll)0 };
		entryCnt = 0;
		strToIndex.clear();
		usedValue.clear();
	}

	void rowInsert(string name, int value)
	{
		if (strToIndex[name] || usedValue[value])
		{
			cout << 0 << '\n';
			return;
		}

		strToIndex[name] = ++entryCnt;
		indexToString[entryCnt] = name;
		usedValue[value] = 1;
		indexToValue[entryCnt] = value;

		updateRec(1, 1, 1e9, value, entryCnt, 1, value);
		cout << 1 << '\n';
	}

	void rowDelete(string name)
	{
		if (!strToIndex[name])
		{
			cout << 0 << '\n';
			return;
		}

		int index = strToIndex[name];
		strToIndex.erase(name);
		usedValue.erase(indexToValue[index]);

		updateRec(1, 1, 1e9, indexToValue[index], index, 0, 0);
		cout << indexToValue[index] << '\n';
	}

	void queryRank(int k)
	{
		if (tree[1].cnt < k)
		{
			cout << "None" << '\n';
			return;
		}

		int index = queryRankRec(1, 1, 1e9, k);
		cout << indexToString[index] << '\n';
	}

	void querySum(int k)
	{
		cout << querySumRec(1, 1, 1e9, 1, k) << '\n';
	}
} DynamicSegmentTree;

int main()
{
	fastIO();

	DynamicSegmentTree dynamicSegmentTree;

	int query_cnt;
	cin >> query_cnt;

	for (int i = 0; i < query_cnt; i++)
	{
		string query;
		cin >> query;
		if (query == "init")
		{
			dynamicSegmentTree.init();
		}
		if (query == "insert")
		{
			string name;
			int value;
			cin >> name >> value;
			dynamicSegmentTree.rowInsert(name, value);
		}
		if (query == "delete")
		{
			string name;
			cin >> name;
			dynamicSegmentTree.rowDelete(name);
		}
		if (query == "rank")
		{
			int k;
			cin >> k;
			dynamicSegmentTree.queryRank(k);
		}
		if (query == "sum")
		{
			int k;
			cin >> k;
			dynamicSegmentTree.querySum(k);
		}
	}
}

