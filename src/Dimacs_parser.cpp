#include "Dimacs_parser.h"
#include "CDCL.h"

#include <fstream>
#include <functional>
#include <sstream>

void read_from_stream(std::istringstream &stream, std::string &buffer, const std::string &target) {
    stream >> buffer;
    if(buffer != target)
        throw std::runtime_error("Expected \"" + target + "\", got \"" + buffer + "\" instead");
}

Formula dimacs_parser::parse(std::ifstream &input_file) {
    Formula formula;
    int number_of_variables, number_of_clauses;
    for (std::string line; std::getline(input_file, line);) {
        if (!line.empty() && line[0] == 'p') {
            std::istringstream stream(line);
            std::string str;
            read_from_stream(stream, str, "p");
            read_from_stream(stream, str, "cnf");
            stream >> number_of_variables >> number_of_clauses;
            break;
        }
    }
    int read_clauses = 0;
    for (std::string line; std::getline(input_file, line) && read_clauses < number_of_clauses;) {
        int variable;
        Clause clause;
        std::istringstream stream(line);
        while (!stream.eof() && stream >> variable) {
            if (variable == 0) break;
            else {
                Literal literal(abs(variable), variable > 0);
                if(literal.num > number_of_variables)
                    throw std::runtime_error("The variable number is greater than the one specified in the header");
                clause.add_literal(literal);
            }
        }
        formula.add_clause(clause);
        ++read_clauses;
    }
    if(read_clauses < number_of_clauses)
        throw std::runtime_error("The number of clauses lower than the one specified in the header");
    return formula;
}
