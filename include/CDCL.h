//
// Created by arrias on 28.05.22.
//
#ifndef CDCL_SAT_SOLVER_CDCL_SOLVER_H
#define CDCL_SAT_SOLVER_CDCL_SOLVER_H

#include "Entities.h"
#include <map>
#include <set>

using std::map;
using std::set;

namespace time_ {
	double get_time(clock_t start);
}

namespace CDCL {
    struct SolverState {
        SolverState(Formula &f);

        void make_new_decision();

        void set_value(const Literal &l);

        void reset_value(int var);

        void add_clause(Clause &c);

        void back_jump(Clause &c);

        Clause analyze_conflict(int conflict);

        int unit_propagate();

        int all_variables_assigned() const;

        static const int U = -1;

        int decision_level;
        set<int> units;
        set<int> conflicts;
        set<int> unassigned;
        vector<int> values;
        vector<Clause> clauses;

        vector<int> level, level_time;
        vector<int> assignation_order;
        vector<set<int>> implications, implications_t;
        vector<int> true_literals;
        vector<int> false_literals;
        vector<set<Literal>> clause_unassigned_literals;

        map<Literal, vector<int>> literal_clauses;
    };

    class Solver : public SATSolver {
    public:
        virtual bool solve(Formula f) override;
    };
}


#endif //CDCL_SAT_SOLVER_CDCL_SOLVER_H
