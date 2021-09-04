#include "opening_transformation.hpp"
#include "isomorphism.hpp"
#include "solver.hpp"

std::pair<short,std::shared_ptr<ordering>> opening_4flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
) {
    const graph& G = solver::get_instance()->get_graph();

    // Forward
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->next_of(a);

        if (ord->distance(x,a)>=2 and ord->distance(b,y)>=2) {
            ordering_iterator itr = ordering_iterator(*ord,ord->index_of(x))+2;
            const ordering_iterator end(*ord,ord->index_of(a));

            for (; itr!=end; ++itr) {
                unsigned short c = *itr;

                for (unsigned short d : G.get_neighbours(c)) {
                    if (ord->is_between(d,b,y)) {
                        unsigned short e = ord->prev_of(c);
                        unsigned short f = ord->prev_of(d);

                        if (b!=f) {
                            std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                            isomorphism_kappa(n_ord,x,e,c,a,b,f,d,y);

                            if (not visited.count(n_ord)) {
                                short opt = -1;
                                if (G(b,e)) ++opt;
                                if (G(y,f)) ++opt;
                                if (not G(c,e)) ++opt;
                                if (not G(a,b)) ++opt;
                                if (not G(d,f)) ++opt;

                                return {opt,n_ord};
                            }
                        }
                    }
                }
            }
        } 
    }

    // Backward
    x = gap.v0();
    y = gap.v1();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->prev_of(a);

        if (ord->distance(a,x)>=2 and ord->distance(y,b)>=2) {
            ordering_iterator itr = ordering_iterator(*ord,ord->index_of(x))-2;
            const ordering_iterator end(*ord,ord->index_of(a));

            for (; itr!=end; --itr) {
                unsigned short c = *itr;

                for (unsigned short d : G.get_neighbours(c)) {
                    if (ord->is_between(d,y,b)) {
                        unsigned short e = ord->next_of(c);
                        unsigned short f = ord->next_of(d);

                        if (b!=f) {
                            std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                            isomorphism_kappa(n_ord,e,x,a,c,f,b,y,d);

                            if (not visited.count(n_ord)) {
                                short opt = -1;
                                if (G(b,e)) ++opt;
                                if (G(y,f)) ++opt;
                                if (not G(c,e)) ++opt;
                                if (not G(a,b)) ++opt;
                                if (not G(d,f)) ++opt;

                                return {opt,n_ord}; 
                            }
                        }
                    }
                }
            }
        }
    }

    return {0,std::shared_ptr<ordering>()};
}