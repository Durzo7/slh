#include <iostream>
#include <string>

#include "solver.hpp"

int main(int argc, char *argv[]) {
    if (argc!=3) {
        std::cout << "Usage: ./slh <seed> <filepath>" << std::endl;
        return 1;
    }

    unsigned seed = std::atol(argv[1]);
    std::string filepath = argv[2];

    solver* solv = solver::initialize(filepath);
    if (seed) solv->get_ordering()->shuffle(unsigned(seed));

    solv->solve();

    solv->write(filepath.substr(filepath.find_last_of('/')+1)+".sol");

    solver::destroy();

    return 0;
}