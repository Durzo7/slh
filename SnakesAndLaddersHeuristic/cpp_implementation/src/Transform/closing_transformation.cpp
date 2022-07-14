#include "closing_transformation.hpp"
#include "isomorphism.hpp"
#include "solver.hpp"

bool closing_2opt_type1(std::shared_ptr<ordering> ord, const pair& gap) {
    const graph& G = solver::get_instance()->get_graph();

    // Forward
    unsigned short x = gap.v1();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->prev_of(a);

        if (x!=b and not G(a,b)) {
            isomorphism_gamma(ord,x,b);
            return true;
        }
    }

    // Backward
    x = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->next_of(a);

        if (x!=b and not G(a,b)) {
            isomorphism_gamma(ord,b,x);
            return true;
        }
    }

    return false;
}

bool closing_2opt_type2(std::shared_ptr<ordering> ord, const pair& gap) {
    const graph& G = solver::get_instance()->get_graph();

    // Symmetric
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->prev_of(a);

        if (x!=b and G(y,b)) {
            isomorphism_gamma(ord,x,b);
            return true;
        }
    }

    return false;
}

bool closing_3opt(std::shared_ptr<ordering> ord, const pair& gap) {
    const graph& G = solver::get_instance()->get_graph();

    // Symmetric
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short c = ord->prev_of(a);

        if (x!=c and ord->distance(a,y)>=2) {
            for (unsigned short b : G.get_neighbours(y)) {
                if (ord->is_between(b,a,y)) {
                    unsigned short d = ord->next_of(b);

                    if (y!=d and G(c,d)) {
                        isomorphism_gamma(ord,x,c);
                        isomorphism_gamma(ord,d,y);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}