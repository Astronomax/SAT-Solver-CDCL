#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

typedef pair<int, int> literal;
typedef vector<literal> clause;

class solver {
public:
    void fit(const vector<clause> &c) {
        solved = false;
        clear();
        vector<int> order;
        for (auto &i : c)
            for (auto &j : i)
                order.push_back(j.first);
        sort(order.begin(), order.end());
        order.erase(unique(order.begin(), order.end()), order.end());
        for (auto &i : c) {
            for (auto &j : i) {
                if (!coords.count(j.first)) {
                    auto it = lower_bound(order.begin(), order.end(), j.first);
                    coords[j.first] = (int)(it - order.begin());
                    coords_t[coords[j.first]] = j.first;
                }
            }
        }
        vector<clause> compressed(c.size());
        for (int i = 0; i < c.size(); i++)
            for (auto &j : c[i])
                compressed[i].push_back(make_pair(coords[j.first], j.second));

        values.assign(order.size(), -1);
        level.assign(order.size(), -1);
        level_time.assign(order.size(), -1);
        implications.resize(order.size());
        implications_t.resize(order.size());
        for (auto &i : compressed)
            add_clause(i);
    }

    bool solve() {
        if (solved) return ans;
        solved = true;

        decision_level = 0;
        level_time[decision_level] = -1;
        while (!unassigned.empty()) {
            int conflict = unit_propagate();
            if (conflict != -1) {
                if (decision_level == 0)
                    return ans = false;
                clause c = analyze_conflict(conflict);
                back_jump(c);
                add_clause(c);
            }
            else if (!unassigned.empty())
                make_new_decision();
        }
        return ans = true;
    }

private:
    int unit_propagate() {
        while (!units.empty()) {
            int c = *units.begin();
            literal l = *clause_unassigned_literals[c].begin();
            set_value(l);
            assignation_order.push_back(l.first);
            level[l.first] = decision_level;
            for (auto &i : clauses[c]) {
                if (i == l) continue;
                implications[i.first].insert(l.first);
                implications_t[l.first].insert(i.first);
            }
            if (!conflicts.empty())
                return *conflicts.begin();
        }
        return -1;
    }

    clause analyze_conflict(int conflict) {
        clause res;
        for (auto &i : clauses[conflict])
            implications[i.first].insert(-1); //edges to bottom

        vector<bool> is_pred(values.size());
        for (int i = (int)assignation_order.size() - 1; i >= level_time[decision_level]; i--) {
            int current_var = assignation_order[i];
            for (auto &j : implications[current_var])
                if (j == -1 || is_pred[j])
                    is_pred[current_var] = true;
        }

        int last_decision = assignation_order[level_time[decision_level]];
        int UIP = last_decision;
        for (int i = level_time[decision_level]; i < (int)assignation_order.size(); i++) {
            int current_var = assignation_order[i];
            if (is_pred[current_var]) {
                bool ok = true;
                set<int> used;
                queue<int> que;
                que.push(last_decision);
                while (!que.empty()) {
                    int v = que.front();
                    que.pop();
                    if (used.find(v) != used.end())
                        continue;
                    used.insert(v);
                    if (v == current_var) continue;
                    else if (v == -1) ok = false;
                    if(v != -1)
                        for (auto &j : implications[v])
                            que.push(j);
                }
                if (ok) UIP = current_var;
            }
        }

        set<int> B;
        queue<int> que;
        for (auto &j : implications[UIP])
            que.push(j);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            if (B.find(v) != B.end())
                continue;
            B.insert(v);
            if (v != -1)
                for (auto &j : implications[v])
                    que.push(j);
        }

        for (int v : assignation_order) {
            if (B.find(v) == B.end()) {
                for (auto &j : implications[v]) {
                    if (B.find(j) != B.end()) {
                        res.emplace_back(v, 1 - values[v]);
                        break;
                    }
                }
            }
        }
        for (auto &i : clauses[conflict])
            implications[i.first].erase(-1); //edges to bottom
        return res;
    }

    void back_jump(const clause &c) {
        pair<int, int> max_levels = make_pair(0, 0);
        for (auto &j : c) {
            if (level[j.first] < decision_level) {
                if (level[j.first] > max_levels.second) {
                    max_levels.first = max_levels.second;
                    max_levels.second = level[j.first];
                }
                else if (level[j.first] > max_levels.first)
                    max_levels.first = level[j.first];
            }
        }

        while ((int)assignation_order.size() - 1 > level_time[max_levels.first]) {
            int current_var = *assignation_order.rbegin();
            level[current_var] = -1;
            for (auto &i : implications_t[current_var])
                implications[i].erase(current_var);
            implications_t[current_var].clear();
            reset_value(current_var);
            assignation_order.pop_back();
        }
        decision_level = max_levels.first;
    }

    void make_new_decision() {
        int var = *unassigned.begin();
        set_value(make_pair(var, 0));
        assignation_order.push_back(var);
        ++decision_level;
        level[var] = decision_level;
        level_time[decision_level] = (int)assignation_order.size() - 1;
    }

    static literal get_opposite(literal l) {
        return make_pair(l.first, 1 - l.second);
    }

    void reset_value(int var) {
        unassigned.insert(var);
        literal l = make_pair(var, values[var]);
        values[var] = -1;
        for (auto &i : literal_clauses[l]) {
            --true_literals[i];
            clause_unassigned_literals[i].insert(l);
            if (clause_unassigned_literals[i].size() == 1)
                if (!true_literals[i])
                    units.insert(i);
        }
        for (auto &i : literal_clauses[get_opposite(l)]) {
            if (clause_unassigned_literals[i].empty()) {
                if (!true_literals[i]) {
                    units.insert(i);
                    conflicts.erase(i);
                }
            }
            else if (clause_unassigned_literals[i].size() == 1) {
                if (!true_literals[i])
                    units.erase(i);
            }
            --false_literals[i];
            clause_unassigned_literals[i].insert(get_opposite(l));
        }
    }

    void set_value(literal l) {
        unassigned.erase(l.first);
        values[l.first] = l.second;
        for (auto &i : literal_clauses[l]) {
            if (clause_unassigned_literals[i].size() == 1)
                if (!true_literals[i])
                    units.erase(i);
            ++true_literals[i];
            clause_unassigned_literals[i].erase(l);
        }
        for (auto &i : literal_clauses[get_opposite(l)]) {
            ++false_literals[i];
            clause_unassigned_literals[i].erase(get_opposite(l));
            if (clause_unassigned_literals[i].empty()) {
                if (!true_literals[i]) {
                    units.erase(i);
                    conflicts.insert(i);
                }
            }
            else if (clause_unassigned_literals[i].size() == 1) {
                if (!true_literals[i])
                    units.insert(i);
            }
        }
    }

    void add_clause(clause c) {
        true_literals.push_back(0);
        false_literals.push_back(0);
        clause_unassigned_literals.emplace_back();

        sort(c.begin(), c.end());
        c.erase(unique(c.begin(), c.end()), c.end());

        for (auto &i : c) {
            literal_clauses[i].push_back((int)clauses.size());
            if (values[i.first] == -1) {
                unassigned.insert(i.first);
                clause_unassigned_literals[(int)clauses.size()].insert(i);
            }
            else {
                if (values[i.first] == i.second)
                    ++true_literals[(int)clauses.size()];
                else ++false_literals[(int)clauses.size()];
            }
        }
        if (clause_unassigned_literals[(int)clauses.size()].size() == 1)
            units.insert((int)clauses.size());
        clauses.push_back(c);
    }

    void clear() {
        clauses.clear();
        literal_clauses.clear();
        true_literals.clear();
        false_literals.clear();
        clause_unassigned_literals.clear();
        values.clear();
        units.clear();
        conflicts.clear();
        unassigned.clear();
        level.clear();
        level_time.clear();
        assignation_order.clear();
        implications.clear();
        implications_t.clear();
    }

private:
    bool ans = false;
    bool solved = false;

    vector<clause> clauses;
    map<literal, vector<int>> literal_clauses;
    vector<int> true_literals, false_literals;
    vector<set<literal>> clause_unassigned_literals;

    vector<int> values;
    set<int> units, conflicts;
    set<int> unassigned;

    int decision_level = 0;
    vector<int> level, level_time;
    vector<int> assignation_order;
    vector<set<int>> implications, implications_t;

    map<int, int> coords, coords_t;
};

int main() {
    freopen("input.txt", "r", stdin);

    int n; cin >> n;
    vector<clause> clauses;
    for (int i = 0; i < n; i++) {
        int k; cin >> k;
        clause c;
        for (int j = 0; j < k; j++) {
            int literal; cin >> literal;
            if (literal > 0)
                c.push_back(make_pair(literal - 1, 1));
            else c.push_back(make_pair(-literal - 1, 0));
        }
        clauses.push_back(c);
    }
    solver s;
    s.fit(clauses);
    cout << (s.solve() ? "SAT" : "UNSAT") << endl;
    return 0;
}