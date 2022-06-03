#include <bits/stdc++.h>

using namespace std;

int getNum(int n, int a, int b) {
	return n * (a - 1) + b;
}

pair<int, int> getCell(int n, int num) {
	return {(num-1)/n+1, (num-1)%n+1};
}

int queenBeat(int n, int pos1, int pos2) {
	auto fi = getCell(n, pos1);
	auto se = getCell(n, pos2);
	return (fi.first == se.first || fi.second == se.second || abs(fi.first-se.first) == abs(fi.second-se.second));
}

vector<vector<int>> clauses;

void genBeatClauses(int n, int i, int j, int k) {
	int revI = n - i + 1;
	int revJ = n - j + 1;
	int revK = n - k + 1;
	int t1   = i - (j - k);
	int t2   = i + (j - k);
	clauses.push_back({-getNum(n, revJ, revI), -getNum(n, revK, revI)});
	if (1 <= t1 && t1 <= n) {
		clauses.push_back({-getNum(n, j, i), -getNum(n, k, t1)});
	}
	if (1 <= t2 && t2 <= n) {
		clauses.push_back({-getNum(n, j, i), -getNum(n, k, t2)});
	}
	clauses.push_back({-getNum(n, i, j), -getNum(n, i, k)});
}

vector<vector<int>> getClauses(int n) {
	clauses.clear();
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			for (int k = 1; k <= n; ++k) {
				if (j == k) continue;
				genBeatClauses(n, i, j, k);
			}
		}	
	}
	for (int i = 1; i <= n; ++i) {
		vector<int> clause;
		for (int j = 1; j <= n; ++j) {
			clause.push_back((i-1)*n+j);
		}
		clauses.push_back(clause);
	}
	return clauses;
}

int main(int argc, char * argv[]) {
	for (int n = 10; n <= 500; n += 5) {
		string name = "queenTests/test_N=" + to_string(n);
		freopen(name.c_str(), "w", stdout);
		auto cls = getClauses(n);
		cout << "p cnf " << n * n << " " << clauses.size() << "\n"; 	
		for (auto i : clauses) {
			for (auto j : i) {
				cout << j << " ";
			}
			cout << 0 << "\n";
		}		
	}
	
	return 0;
}
