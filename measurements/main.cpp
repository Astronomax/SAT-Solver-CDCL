#include "CDCL.h"
#include <iostream>
#include "Dimacs_parser.h"

#include <fstream>
#include <sstream>
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;

namespace time_ {
	clock_t start_time;
	double get_time() {
		return (double)(clock() - start_time) / CLOCKS_PER_SEC;
	}
}

const std::string sats_input_file_names = "../sats_input_file_names.txt";
const std::string unsats_input_file_names = "../unsats_input_file_names.txt";

int main(int argc, char *argv[]) {
    std::ifstream in(sats_input_file_names);
    map<int, std::vector<double>> time_sats;
    int it=0;
    for (std::string line; std::getline(in, line);) {
        ++it; if(it>0) break;
        line.erase(line.begin());
        std::ifstream input("../sats"+line);

        time_::start_time = clock();
        //
        Formula formula = dimacs_parser::parse(input);
        CDCL::Solver s;
        s.solve(formula);
        //
        double _time = time_::get_time();
        time_sats[formula.get_clauses().size()].push_back(_time);

        std::cout<<"../sats"+line<<" pokazal "<<_time<<" na "<<formula.get_clauses().size()<<"\n";
    }

    in = std::ifstream(unsats_input_file_names);
    map<int, std::vector<double>> time_unsats;
    it=0;
    for (std::string line; std::getline(in, line);) {
        std::getline(in, line);
        ++it; if(it>3) break;
        line.erase(line.begin());
        std::ifstream input("../unsats"+line);
        //std::cout<<"../unsats"+line<<"\n\n";

        time_::start_time = clock();
        
        Formula formula = dimacs_parser::parse(input);
        std::cout<<"../unsats"+line<<" pokazal "<<0<<" na "<<formula.get_clauses().size()<<"\n";
        CDCL::Solver s;
        s.solve(formula);
        
        double _time = time_    ::get_time();
        time_unsats[formula.get_clauses().size()].push_back(_time);
        std::cout<<"../unsats"+line<<" pokazal "<<_time<<" na "<<formula.get_clauses().size()<<"\n";
    }

    for(auto &el : time_sats) {
        double sum=0,ct=el.second.size();
        for(double el : el.second)
            sum+=el;

        std::cout<<std::fixed<<std::setprecision(3)<<el.first<<" "<<sum/ct<<"\n";
    }
    std::cout<<"\n\n";
    for(auto &el : time_unsats) {
        double sum=0,ct=el.second.size();
        for(double el : el.second)
            sum+=el;

        std::cout<<std::fixed<<std::setprecision(3)<<el.first<<" "<<sum/ct<<"\n";
    }

    return 0;
}