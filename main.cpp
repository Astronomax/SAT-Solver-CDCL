#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <map>
#include <queue>

using namespace std;

int clauses_cnt, vars_cnt;
const int CLAUSES_LIMIT = 10000;
const int VARS_LIMIT = 1000;
vector<int> clauses[CLAUSES_LIMIT];
int sign[CLAUSES_LIMIT][VARS_LIMIT];

int values[VARS_LIMIT];
int decision_level = 0;
int decision_literals[VARS_LIMIT];
int trails[VARS_LIMIT];

int assigned_vars_cnt = 0;
int assignation_time[VARS_LIMIT];
int assignation_order[VARS_LIMIT];
vector<int> implication_graph[VARS_LIMIT];

int check_conflicts() {
    for (int i = 0; i < clauses_cnt; i++) {
        bool unsatisfied = true;
        for (int j : clauses[i]) {
            if (values[j] == -1)
                unsatisfied = false;
            if (sign[i][j] == 2)
                unsatisfied = false;
            if (sign[i][j] == values[j])
                unsatisfied = false;
        }
        if (unsatisfied) return i;
    }
    return -1;
}

int unit_propagate() {
    for (int i = 0; i < clauses_cnt; i++) {
        set<int> unassigned_vars;
        for (int j : clauses[i])
            if (values[j] == -1)
                unassigned_vars.insert(j);
        bool other_is_false = true;
        for (int j : clauses[i]) {
            if (values[j] == -1)
                continue;
            if (sign[i][j] == 2)
                other_is_false = false;
            if (values[j] == sign[i][j])
                other_is_false = false;
        }

        if (other_is_false && unassigned_vars.size() == 1) {
            int var = *unassigned_vars.begin();
            if (sign[i][var] != 2) {
                set<int> preds;
                for (int j : clauses[i])
                    if (values[j] != -1)
                        preds.insert(j);
                for (auto &j : preds)
                    implication_graph[j].push_back(var);
                trails[var] = decision_level;
                values[var] = sign[i][var];
                assignation_order[assigned_vars_cnt] = var;
                assignation_time[var] = assigned_vars_cnt;
                ++assigned_vars_cnt;
                return var;
            }
        }
    }
    return -1;
}

bool is_UIP(int from, int u) {
    queue<int> que;
    que.push(from);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        if (v == u) continue;
        else if (v == -1) return false;
        for (auto &j : implication_graph[v])
            que.push(j);
    }
    return true;
}

vector<pair<int, int>> analyze_conflict(int falsified) {
    for (auto &j : clauses[falsified])
        implication_graph[j].push_back(-1);

    vector<pair<int, int>> clause;
    int last_decision = decision_literals[decision_level];
    int last_decision_time = assignation_time[last_decision];

    map<int, bool> is_pred;
    is_pred[assignation_order[assigned_vars_cnt - 1]] = true;
    for (int i = assigned_vars_cnt - 2; i >= last_decision_time; i--) {
        int current_var = assignation_order[i];
        for (auto &j : implication_graph[current_var])
            is_pred[current_var] |= is_pred[j];
    }

    int UIP = last_decision;
    for (int i = last_decision_time; i < assigned_vars_cnt; i++) {
        int current_var = assignation_order[i];
        if (is_UIP(last_decision, current_var))
            UIP = current_var;
    }

    set<int> B;
    queue<int> que;
    for (auto &j : implication_graph[UIP])
        que.push(j);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        if(v != -1)
            for (auto &j : implication_graph[v])
                que.push(j);
        B.insert(v);
    }

    for (int i = 0; i < assigned_vars_cnt; i++) {
        int v = assignation_order[i];
        if (trails[v] == 0) continue;
        if (B.find(v) == B.end()) {
            for (auto &j : implication_graph[v]) {
                if (B.find(j) != B.end()) {
                    clause.emplace_back(v, 1 - values[v]);
                    break;
                }
            }
        }
    }
    for (auto &j : clauses[falsified])
        implication_graph[j].pop_back();
    return clause;
}

void back_jump(const vector<pair<int, int>> &clause) {
    int level = 0;
    for (auto &j : clause)
        if (trails[j.first] < decision_level)
            level = max(level, trails[j.first]);

    int to = (level == 0) ? -1 : assignation_time[decision_literals[level]];

    while (assigned_vars_cnt - 1 > to) {
        implication_graph[assignation_order[assigned_vars_cnt - 1]].clear();
        assignation_time[assignation_order[assigned_vars_cnt - 1]] = -1;
        values[assignation_order[assigned_vars_cnt - 1]] = -1;
        trails[assignation_order[assigned_vars_cnt - 1]] = -1;
        assignation_order[assigned_vars_cnt - 1] = -1;
        --assigned_vars_cnt;
    }
    for (int i = decision_level; i > level; i--)
        decision_literals[i] = -1;
    decision_level = level;
}

void make_new_decision() {
    int var = 0;
    for (int i = 0; i < vars_cnt; i++)
        if (values[i] == -1)
            var = i;
    values[var] = 0;
    assignation_order[assigned_vars_cnt] = var;
    assignation_time[var] = assigned_vars_cnt;
    ++assigned_vars_cnt;
    decision_literals[++decision_level] = var;
}

void add_clause(const vector<pair<int, int>>& clause) {
    vector<pair<bool, bool>> presented(VARS_LIMIT, make_pair(false, false));
    for (auto &i : clause) {
        if (i.second == 0)
            presented[i.first].first = true;
        else presented[i.first].second = true;
    }
    for (int i = 0; i < VARS_LIMIT; i++) {
        if (presented[i].first && presented[i].second)
            sign[clauses_cnt][i] = 2;
        else if (presented[i].first)
            sign[clauses_cnt][i] = 0;
        else if (presented[i].second)
            sign[clauses_cnt][i] = 1;
    }
    assert(clauses_cnt < CLAUSES_LIMIT);
    clauses[clauses_cnt].resize(clause.size());
    for (int i = 0; i < clause.size(); i++)
        clauses[clauses_cnt][i] = clause[i].first;
    ++clauses_cnt;
}

int main() {
    //freopen("input.txt", "r", stdin);

    fill(&sign[0][0], &sign[0][0] + CLAUSES_LIMIT * VARS_LIMIT, -1);
    fill(&values[0], &values[0] + VARS_LIMIT, -1);
    fill(&decision_literals[0], &decision_literals[0] + VARS_LIMIT, -1);
    fill(&trails[0], &trails[0] + VARS_LIMIT, -1);
    fill(&assignation_time[0], &assignation_time[0] + VARS_LIMIT, -1);
    fill(&assignation_order[0], &assignation_order[0] + VARS_LIMIT, -1);

    int n; cin >> n;
    for (int i = 0; i < n; i++) {
        int k; cin >> k;
        vector<pair<int, int>> clause(k);
        for (int j = 0; j < k; j++) {
            int literal; cin >> literal;
            if (literal > 0)
                clause[j] = make_pair(literal - 1, 1);
            else clause[j] = make_pair(-literal - 1, 0);
        }
        add_clause(clause);
        for (auto &j : clause)
            vars_cnt = max(vars_cnt, j.first + 1);
    }
    assert(vars_cnt < VARS_LIMIT);

    while (assigned_vars_cnt < vars_cnt) {
        bool no_conflicts = true;

        while (unit_propagate() != -1) {
            int falsified = check_conflicts();
            if (falsified != -1) {
                if (decision_level == 0) {
                    cout << "UNSAT";
                    return 0;
                }
                vector<pair<int, int>> c = analyze_conflict(falsified);
                add_clause(c);
                back_jump(c);
                no_conflicts = false;
                break;
            }
        }
        if (no_conflicts && assigned_vars_cnt < vars_cnt) {
            make_new_decision();
            assert(check_conflicts() == -1);
        }
    }
    cout << "SAT" << "\n";
    for (int i = 0; i < vars_cnt; i++)
        cout << values[i] << " ";
    return 0;
}