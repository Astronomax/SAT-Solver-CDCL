#include "CDCL.h"
#include <iostream>
#include "Dimacs_parser.h"

#include <fstream>
#include <sstream>
#include <iomanip>

const std::string sats_inputs_file_name = "../sats_inputs.txt";
const std::string unsats_inputs_file_name = "../unsats_inputs.txt";
const std::string measurements_file_name = "../measurements.txt";
const std::string bad_inputs_file_name = "../bad_inputs.txt";

class measurement {
public:
    measurement(std::string input_name_, size_t clause_ct_, size_t literals_ct_, double time_, bool is_sat_) : 
        input_name(input_name_),
        clause_ct(clause_ct_), 
        literals_ct(literals_ct_),
        time(time_),
        is_sat(is_sat_) {};
    
    void write_in_file(std::ofstream &file) {
        file<<std::fixed<<std::setprecision(2)<<input_name<<" "<<clause_ct<<" "<<literals_ct<<" "<<time<<" "<<(int)is_sat<<"\n";
        //std::cout<<std::fixed<<std::setprecision(2)<<input_name<<" "<<clause_ct<<" "<<literals_ct<<" "<<time<<" "<<(int)is_sat<<"\n";
    }

    std::string get_input_name() const { return input_name; }

private:
    std::string input_name;
    size_t clause_ct, literals_ct;
    double time = 0;
    bool is_sat = 0;
};


inline size_t get_clauses_ct(Formula &f) {
    return f.get_clauses().size();
}

inline size_t get_literals_ct(Formula &f) {
    size_t ct = 0;
    for(auto &clause : f.get_clauses())
        ct += clause.get_literals().size();
    return ct;
}

std::string get_till_space(std::string line, size_t &it) {
    std::string ret="";
    for(;it<line.size();++it) {
        if(line[it]==' ') { ++it; break; };
        ret+=line[it];
    }
    return ret;
}

measurement parse_line(std::string line) {
    std::string input_name = "";
    size_t clause_ct, literals_ct;
    double time = 0;
    bool is_sat = 0;

    size_t it=0;
    input_name = get_till_space(line, it);
    clause_ct = std::stoi(get_till_space(line, it));
    literals_ct = std::stoi(get_till_space(line, it));
    time = std::stod(get_till_space(line, it));
    is_sat = (get_till_space(line, it)[0]=='1');

    return measurement(input_name, clause_ct, literals_ct, time, is_sat);
}


void get_measured(std::string measurements_fname, std::vector<measurement> &v) {
    std::ifstream in(measurements_fname);
    for (std::string line; std::getline(in, line);)
        v.push_back(parse_line(line));
    in.close();
}

bool measure_inputs(std::string inputs_fname, std::string output_fname, bool is_sats_inputs) {
    bool ret=0;
    
    std::vector<measurement> done;
    //get_measured("../queens_measurements.txt", done);
    get_measured(measurements_file_name, done);

    std::map<std::string, bool> done_names;
    for(auto &meas : done)
        done_names[meas.get_input_name()] = 1;

    std::ifstream in(inputs_fname);
    int ct=0;
    for (std::string line; std::getline(in, line);) {
        line.erase(line.begin());
        std::string input_fname = (is_sats_inputs?"../sats":"../unsats")+line;
        //std::string input_fname = (is_sats_inputs?"../sats":"../queenTests")+line;

        if(done_names.find(input_fname)!=done_names.end())
            continue;
        
        std::ifstream input(input_fname);
        
        //
        Formula formula = dimacs_parser::parse(input);
        //        
        clock_t start = clock();
        //
        CDCL::Solver s;
        s.solve(formula);
        //
        double _time = time_::get_time(start);
        //

        input.close();

        measurement cur = measurement(input_fname, get_clauses_ct(formula), get_literals_ct(formula), _time, is_sats_inputs);
        done.push_back(cur);
        done_names[input_fname] = 1;
        ret=1;

        ++ct;
        if(ct==5) break;
    }
    in.close();

    std::ofstream out(output_fname);    
    for(auto &meas : done)
        meas.write_in_file(out);
    out.close();

    return ret;
}

void unique_measurements(std::string measurements_fname) {
    std::vector<measurement> done,done_unique;
    get_measured(measurements_fname, done);

    std::map<std::string, bool> done_names;
    for(auto &meas : done) {
        if(!done_names[meas.get_input_name()])
            done_unique.push_back(meas);
        done_names[meas.get_input_name()] = 1;
    }

    std::ofstream out(measurements_fname);    
    for(auto &meas : done)
        meas.write_in_file(out);
    out.close();
}

int main(int argc, char *argv[]) {
    //unique_measurements(measurements_file_name);
    while(true) {
        int c=0;
        c+=measure_inputs(unsats_inputs_file_name, measurements_file_name, 0);
        c+=measure_inputs(sats_inputs_file_name, measurements_file_name, 1);
        if(!c)
            break;
    }
    //while(true)
    //    measure_inputs("../queens_inputs.txt", "../queens_measurements.txt", 0);
    return 0;
}