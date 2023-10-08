#include "Bench_parser.h"
#include "CDCL.h"

#include <fstream>
#include <functional>
#include <sstream>
#include <regex>
#include <cassert>
#include <iostream>

void read_from_stream(std::istringstream &stream, std::string &buffer, const std::string &target) {
    stream >> buffer;
    if(buffer != target)
        throw std::runtime_error("Expected \"" + target + "\", got \"" + buffer + "\" instead");
}

Gate* get_or_create(Circuit &circuit, const std::string &gate_name) {
    auto it = circuit.gate_by_name.find(gate_name);
    Gate *gate;
    if(it != circuit.gate_by_name.end()) {
        gate = it->second;
    } else {
        gate = new Gate();
        circuit.gate_by_name[gate_name] = gate;
    }
    return gate;
}

Circuit bench_parser::parse(std::ifstream &input_file) {
    Circuit circuit;

    enum ParsingState {
        INPUT,
        OUTPUT,
        EDGES,
        END
    };

    ParsingState state = ParsingState::INPUT;

    for (std::string line; std::getline(input_file, line);) {
        if(line.empty() || line[0] == '#')
            continue; //ignore comments

        switch (state) {
            case ParsingState::INPUT: {
                std::regex r("INPUT\\((\\w*)\\)");
                std::smatch m;
                if(std::regex_match(line, m, r)) {
                    auto gate_name =  m[1].str();
                    auto gate = get_or_create(circuit, gate_name);
                    gate->m_op = Op::INPUT;
                    circuit.inputs.push_back(gate);
                    break;
                } else {
                    state = ParsingState::OUTPUT;
                }
            }
            case ParsingState::OUTPUT: {
                std::regex r("OUTPUT\\((\\w*)\\)");
                std::smatch m;
                assert(std::regex_match(line, m, r)); //in our circuits there is exactly one output
                auto gate_name =  m[1].str();
                auto gate = get_or_create(circuit, gate_name);
                circuit.output = gate;
                state = ParsingState::EDGES; //in our circuits there is exactly one output
                break;
            }
            case ParsingState::EDGES: {
                std::vector<std::pair<Op, std::regex>> ops_to_regex = {
                    {Op::AND, std::regex("(\\w*)\\s*=\\s*AND\\((\\w*),\\s*(\\w*)\\)")},
                    {Op::OR, std::regex("(\\w*)\\s*=\\s*OR\\((\\w*),\\s*(\\w*)\\)")},
                    {Op::NOT, std::regex("(\\w*)\\s*=\\s*NOT\\((\\w*)\\)")},
                    {Op::NAND, std::regex("(\\w*)\\s*=\\s*NAND\\((\\w*),\\s*(\\w*)\\)")},
                    {Op::NOR, std::regex("(\\w*)\\s*=\\s*NOR\\((\\w*),\\s*(\\w*)\\)")},
                    {Op::XOR, std::regex("(\\w*)\\s*=\\s*XOR\\((\\w*),\\s*(\\w*)\\)")},
                    {Op::NXOR, std::regex("(\\w*)\\s*=\\s*NXOR\\((\\w*),\\s*(\\w*)\\)")},
                    {Op::BUFF, std::regex("(\\w*)\\s*=\\s*BUFF\\((\\w*)\\)")}
                };

                std::smatch m;
                bool is_one_of = false;
                for(auto &p : ops_to_regex) {
                    if(std::regex_match(line, m, p.second)) {
                        Gate* child = get_or_create(circuit, m[1].str());
                        std::vector<Gate*> ancestors;
                        for(int i = 2; i < m.size(); i++)
                            ancestors.push_back(get_or_create(circuit, m[i].str()));
                        for(auto &ancestor : ancestors) {
                            ancestor->links.push_back(child);
                            child->links_t.push_back(ancestor);
                        }
                        child->m_op = p.first;
                        is_one_of = true;
                        break;
                    }
                }
                if(is_one_of)
                    break;
                else
                    state = ParsingState::END;
            }
            case ParsingState::END: {
                break;
            }
        }
    }
    return circuit;
}
