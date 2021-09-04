#include "stage.hpp"
#include "solver.hpp"
#include "transform.hpp"
#include "opening_transformation.hpp"

#include <iostream>
#include <vector>
#include <utility>
#include <set>

void stage0() {
    solver* solv = solver::get_instance();
    std::shared_ptr<ordering> ord = solv->get_ordering();

    while (transform_close(ord));
    
    solv->set_n_gaps(ord->n_gaps());
    //std::cout << solv->get_n_gaps() << std::endl;
}

void stage1() {
    solver* solv = solver::get_instance();
    std::vector<std::shared_ptr<ordering>> aux_ord_stack(solv->get_ordering_stack());
    unsigned short n_gaps = solv->get_n_gaps();

    while (not solv->exit_condition(2) and not aux_ord_stack.empty()) {
        std::pair<unsigned short,std::shared_ptr<ordering>> n_ord_pair = transform_float(aux_ord_stack.back());
        
        if (n_ord_pair.second) {
            solv->add_ordering(n_ord_pair.second);

            if (n_ord_pair.first) {
                n_gaps -= n_ord_pair.first;
                //std::cout << n_gaps << std::endl;

                if (n_gaps==0) {
                    solv->set_n_gaps(0);
                    return;
                }
                solv->clear();
                aux_ord_stack.clear();
            }
            aux_ord_stack.push_back(n_ord_pair.second);
        }
        else aux_ord_stack.pop_back();
    }
    solv->set_n_gaps(n_gaps);
}

bool stage2() {
    //std::cout << "Stage 2" << std::endl;
    solver* solv = solver::get_instance();
    std::shared_ptr<ordering> ord = solv->pop_ordering();
    pair gap = ord->next_gap();
    const unsigned short n_gaps = solv->get_n_gaps();
    
    std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator> visited;

    std::pair<short,std::shared_ptr<ordering>> n_ord_pair = opening_4flo(ord,gap,visited);

    while (n_ord_pair.second) {
        solv->add_ordering(n_ord_pair.second);
        solv->clear();
        solv->add_gap(gap);
        solv->set_n_gaps(n_gaps-n_ord_pair.first);

        stage1();

        if (n_gaps>solv->get_n_gaps()) {
            if (solv->get_n_gaps()==0) return false;
            solv->clear();
            return true;
        }
        visited.insert(n_ord_pair.second);
        n_ord_pair = opening_4flo(ord,gap,visited);
    }
    solv->add_ordering(ord);
    solv->set_n_gaps(n_gaps);

    return false;
}

bool stage3() {
    //std::cout << "Stage 3" << std::endl;
    solver* solv = solver::get_instance();
    std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator> visited(
        solv->get_ordering_stack().begin(),
        solv->get_ordering_stack().end()
    );
    std::pair<short,std::shared_ptr<ordering>> n_ord_pair;

    while (not solv->exit_condition(3)) {
        n_ord_pair = transform_open(solv->get_ordering_stack().back(),visited);
        if (not n_ord_pair.second) break;

        std::vector<std::pair<unsigned short,std::shared_ptr<ordering>>> aux_ord_pair_stack;

        solv->add_ordering(n_ord_pair.second);
        visited.insert(n_ord_pair.second);
        aux_ord_pair_stack.push_back(std::make_pair(n_ord_pair.second->n_gaps(),n_ord_pair.second));

        while (not solv->exit_condition(3) and not aux_ord_pair_stack.empty()) {
            n_ord_pair = transform_float_opt(aux_ord_pair_stack.back().second,visited);

            if (n_ord_pair.second) {
                solv->add_ordering(n_ord_pair.second);
                visited.insert(n_ord_pair.second);
                unsigned short n_gaps = aux_ord_pair_stack.back().first-n_ord_pair.first;
                aux_ord_pair_stack.push_back(std::make_pair(n_gaps,n_ord_pair.second));
            }
            else if (aux_ord_pair_stack.back().first<solv->get_n_gaps()) {
                //std::cout << aux_ord_pair_stack.back().first << std::endl;
                solv->set_n_gaps(aux_ord_pair_stack.back().first);
                if (aux_ord_pair_stack.back().first==0) return false;
                solv->clear();
                return true;
            }
            else aux_ord_pair_stack.pop_back();
        }
    }

    return false;
}