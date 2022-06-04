# cdcl-sat-solver
[![C++ CI](https://github.com/Astronomax/cdcl-sat-solver/actions/workflows/cmake.yml/badge.svg)](https://github.com/Astronomax/cdcl-sat-solver/actions/workflows/cmake.yml)

SAT-solver based on [CDCL](https://en.wikipedia.org/wiki/Conflict-driven_clause_learning) algorithm.  

Decision heuristics:
*   DLIS
*   VSIDS (zChaff)
## Usage
```shell
./solveSAT -i input.cnf -t -l -v 
```

## Flags
```
[-i / --input  ] -- input dimacs file
[-t / --time   ] -- show solve time
[-l / --learned] -- show learned clauses (for unsats it's like proof)
[-v / --values ] -- show interpetation if SAT 
```

## Output
```shell
unsatisfiable.

learned clauses:
0 
0 1 
-1 2 
-2 3 
0 -3 

done in 0 s
```

```shell
satisfiable.

values are:
x_1 = 1
x_2 = 1
x_3 = 1
x_4 = 1

learned clauses:
0 
0 1 
-1 2 
-2 3 

done in 0 s
```
