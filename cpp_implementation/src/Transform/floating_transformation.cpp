#include "floating_transformation.hpp"
#include "isomorphism.hpp"
#include "solver.hpp"

std::pair<unsigned short,std::shared_ptr<ordering>> floating_2flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap
) {
    const graph& G = solver::get_instance()->get_graph();
    const std::unordered_set<pair,cantor_pairing_hash>& gap_set = solver::get_instance()->get_gap_set();

    // Fordward
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->prev_of(a);

        if (x!=b and not gap_set.count(pair(y,b))) {
            unsigned short opt = 0;
            if (G(y,b)) ++opt;
            if (not G(a,b)) ++opt;

            std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
            isomorphism_gamma(n_ord,x,b);

            return {opt,n_ord};
        }
    }

    // Backward
    x = gap.v0();
    y = gap.v1();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->next_of(a);

        if (x!=b and not gap_set.count(pair(y,b))) {
            unsigned short opt = 0;
            if (G(y,b)) ++opt;
            if (not G(a,b)) ++opt;

            std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
            isomorphism_gamma(n_ord,b,x);

            return {opt,n_ord};
        }
    }

    return {0,std::shared_ptr<ordering>()};
}

std::pair<unsigned short,std::shared_ptr<ordering>> floating_3flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap
) {
    const graph& G = solver::get_instance()->get_graph();
    const std::unordered_set<pair,cantor_pairing_hash>& gap_set = solver::get_instance()->get_gap_set();

    // Forward
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short c = ord->prev_of(a);

        if (x!=c and ord->distance(a,y)>=2) {
            // (y,b) is a ladder, Symmetric
            for (unsigned short b : G.get_neighbours(y)) {
                if (ord->is_between(b,a,y)) {
                    unsigned short d = ord->next_of(b);

                    if (y!=d and not gap_set.count(pair(c,d))) {
                        unsigned short opt = 0;
                        if (G(c,d)) ++opt;
                        if (not G(a,c)) ++opt;
                        if (not G(b,d)) ++opt;

                        std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                        isomorphism_gamma(n_ord,x,c);
                        isomorphism_gamma(n_ord,d,y);

                        return {opt,n_ord};
                    }
                }
            }
            // (c,d) is a ladder
            for (unsigned short d : G.get_neighbours(c)) {
                if (ord->is_between(d,a,y)) {
                    unsigned short b = ord->prev_of(d);

                    if (a!=b and not gap_set.count(pair(y,b))) {
                        unsigned short opt = 0;
                        if (G(y,b)) ++opt;
                        if (not G(a,c)) ++opt;
                        if (not G(b,d)) ++opt;

                        std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                        isomorphism_gamma(n_ord,x,c);
                        isomorphism_gamma(n_ord,d,y);

                        return {opt,n_ord};
                    }
                }
            }
        }
    }

    // Backward
    x = gap.v0();
    y = gap.v1();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short c = ord->next_of(a);

        if (x!=c and ord->distance(y,a)>=2) {
            // (y,b) is a ladder, already tested
            // (c,d) is a ladder
            for (unsigned short d : G.get_neighbours(c)) {
                if (ord->is_between(d,y,a)) {
                    unsigned short b = ord->next_of(d);

                    if (a!=b and not gap_set.count(pair(y,b))) {
                        unsigned short opt = 0;
                        if (G(y,b)) ++opt;
                        if (not G(a,c)) ++opt;
                        if (not G(b,d)) ++opt;

                        std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                        isomorphism_gamma(n_ord,c,x);
                        isomorphism_gamma(n_ord,y,d);

                        return {opt,n_ord};
                    }
                }
            }
        }
    }

    return {0,std::shared_ptr<ordering>()};
}

std::pair<unsigned short,std::shared_ptr<ordering>> floating_4flo_type1(
    std::shared_ptr<const ordering> ord,
    const pair& gap
) {
    const graph& G = solver::get_instance()->get_graph();
    const std::unordered_set<pair,cantor_pairing_hash>& gap_set = solver::get_instance()->get_gap_set();

    // Symmetric
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->next_of(a);

        if (G(y,b) and ord->distance(x,a)>=2 and ord->distance(b,y)>=2) {
            ordering_iterator itr = ordering_iterator(*ord,ord->index_of(x))+2;
            const ordering_iterator end(*ord,ord->index_of(a));

            for (; itr!=end; ++itr) {
                unsigned short c = *itr;

                for (unsigned short d :G.get_neighbours(c)) {
                    if (ord->is_between(d,b,y)) {
                        unsigned short e = ord->prev_of(c);
                        unsigned short f = ord->next_of(d);

                        if (y!=f and not gap_set.count(pair(e,f))) {
                            unsigned short opt = 0;
                            if (G(e,f)) ++opt;
                            if (not G(c,e)) ++opt;
                            if (not G(a,b)) ++opt;
                            if (not G(d,f)) ++opt;

                            std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                            isomorphism_gamma(n_ord,x,e);
                            isomorphism_gamma(n_ord,c,a);
                            isomorphism_gamma(n_ord,b,d);
                            isomorphism_gamma(n_ord,f,y);

                            return {opt,n_ord};
                        }
                    }
                }
            }
        }
    }

    return {0,std::shared_ptr<ordering>()};
}

std::pair<unsigned short,std::shared_ptr<ordering>> floating_4flo_type2(
    std::shared_ptr<const ordering> ord,
    const pair& gap
) {
    const graph& G = solver::get_instance()->get_graph();
    const std::unordered_set<pair,cantor_pairing_hash>& gap_set = solver::get_instance()->get_gap_set();

    // Forward
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short b = ord->next_of(a);

        if (ord->distance(x,a)>=2 and ord->distance(b,y)>=2) {
            // (b,e) is a ladder
            for (unsigned short e : G.get_neighbours(b)) {
                if (ord->is_between(e,x,a)) {
                    unsigned short c = ord->next_of(e);

                    if (a!=c) {
                        for (unsigned short d : G.get_neighbours(c)) {
                            if (ord->is_between(d,b,y)) {
                                unsigned short f = ord->prev_of(d);

                                if (b!=f and not gap_set.count(pair(y,f))) {
                                    unsigned short opt = 0;
                                    if (G(y,f)) ++opt;
                                    if (not G(c,e)) ++opt;
                                    if (not G(a,b)) ++opt;
                                    if (not G(d,f)) ++opt;

                                    std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                                    isomorphism_kappa(n_ord,x,e,c,a,b,f,d,y);

                                    return {opt,n_ord};
                                }
                            }
                        }
                    }
                }
            }
            // (y,f) is a ladder
            for (unsigned short f : G.get_neighbours(y)) {
                if (ord->is_between(f,b,y)) {
                    unsigned short d = ord->next_of(f);

                    if (y!=d) {
                        for (unsigned short c : G.get_neighbours(d)) {
                            if (ord->is_between(c,x,a)) {
                                unsigned short e = ord->prev_of(c);

                                if (x!=e and not gap_set.count(pair(b,e))) {
                                    unsigned short opt = 0;
                                    if (G(b,e)) ++opt;
                                    if (not G(c,e)) ++opt;
                                    if (not G(a,b)) ++opt;
                                    if (not G(d,f)) ++opt;

                                    std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                                    isomorphism_kappa(n_ord,x,e,c,a,b,f,d,y);

                                    return {opt,n_ord};
                                }
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

        if (ord->distance(a,x)>=2 and ord->distance(y,b)) {
            // (b,e) is a ladder
            for (unsigned short e : G.get_neighbours(b)) {
                if (ord->is_between(e,a,x)) {
                    unsigned short c = ord->prev_of(e);

                    if (a!=c) {
                        for (unsigned short d : G.get_neighbours(c)) {
                            if (ord->is_between(d,y,b)) {
                                unsigned short f = ord->next_of(d);

                                if (b!=f and not gap_set.count(pair(y,f))) {
                                    unsigned short opt = 0;
                                    if (G(y,f)) ++opt;
                                    if (not G(c,e)) ++opt;
                                    if (not G(a,b)) ++opt;
                                    if (not G(d,f)) ++opt;

                                    std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                                    isomorphism_kappa(n_ord,e,x,a,c,f,b,y,d);

                                    return {opt,n_ord};
                                }
                            }
                        }
                    }
                }
            }
            // (y,f) is a ladder
            for(unsigned short f : G.get_neighbours(y)) {
                if (ord->is_between(f,y,b)) {
                    unsigned short d = ord->prev_of(f);

                    if (y!=d) {
                        for (unsigned short c : G.get_neighbours(d)) {
                            if (ord->is_between(c,a,x)) {
                                unsigned short e = ord->next_of(c);

                                if (x!=e and not gap_set.count(pair(b,e))) {
                                    unsigned short opt = 0;
                                    if (G(b,e)) ++opt;
                                    if (not G(c,e)) ++opt;
                                    if (not G(a,b)) ++opt;
                                    if (not G(d,f)) ++opt;

                                    std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                                    isomorphism_kappa(n_ord,e,x,a,c,f,b,y,d);

                                    return {opt,n_ord};
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return {0,std::shared_ptr<ordering>()};
}

std::pair<unsigned short,std::shared_ptr<ordering>> floating_5flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap
) {
    const graph& G = solver::get_instance()->get_graph();
    const std::unordered_set<pair,cantor_pairing_hash>& gap_set = solver::get_instance()->get_gap_set();

    // Forward
    unsigned short x = gap.v1();
    unsigned short y = gap.v0();

    for (unsigned short a : G.get_neighbours(x)) {
        unsigned short f = ord->next_of(a);

        if (ord->distance(x,a)>=2 and ord->distance(f,y)>=5) {
            for (unsigned short b : G.get_neighbours(y)) {
                if (ord->is_between(b,f,y) and ord->distance(b,y)>=3) {
                    unsigned short g = ord->prev_of(b);

                    if (f!=g) {
                        for (unsigned short e : G.get_neighbours(f)) {
                            if (ord->is_between(e,x,a)) {
                                unsigned short c = ord->next_of(e);

                                if (a!=c) {
                                    for (unsigned short d : G.get_neighbours(c)) {
                                        if (ord->is_between(d,b,y)) {
                                            unsigned short j = ord->prev_of(d);
                                            unsigned short h = ord->next_of(d);

                                            if (b!=j and y!=h and not gap_set.count(pair(g,h))) {
                                                unsigned short opt = 0;
                                                if (G(g,h)) ++opt;
                                                if (not G(c,e)) ++opt;
                                                if (not G(a,f)) ++opt;
                                                if (not G(b,g)) ++opt;
                                                if (not G(d,h)) ++opt;

                                                std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                                                isomorphism_kappa(n_ord,x,e,c,a,f,j,d,y);
                                                isomorphism_gamma(n_ord,j,b);
                                                isomorphism_gamma(n_ord,g,d);

                                                return {opt,n_ord};
                                            }
                                        }
                                    }
                                }
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
        unsigned short f = ord->prev_of(a);

        if (ord->distance(a,x)>=2 and ord->distance(y,f)>=5) {
            for (unsigned short b : G.get_neighbours(y)) {
                if (ord->is_between(b,y,f) and ord->distance(y,b)>=3) {
                    unsigned short g = ord->next_of(b);

                    if (f!=g) {
                        for (unsigned short e : G.get_neighbours(f)) {
                            if (ord->is_between(e,a,x)) {
                                unsigned short c = ord->prev_of(e);

                                if (a!=c) {
                                    for (unsigned short d : G.get_neighbours(c)) {
                                        if (ord->is_between(d,y,b)) {
                                            unsigned short j = ord->next_of(d);
                                            unsigned short h = ord->prev_of(d);

                                            if (b!=j and y!=h and not gap_set.count(pair(g,h))) {
                                                unsigned short opt = 0;
                                                if (G(g,h)) ++opt;
                                                if (not G(c,e)) ++opt;
                                                if (not G(a,f)) ++opt;
                                                if (not G(b,g)) ++opt;
                                                if (not G(d,h)) ++opt;

                                                std::shared_ptr<ordering> n_ord = std::make_shared<ordering>(*ord);
                                                isomorphism_kappa(n_ord,e,x,a,c,j,f,y,d);
                                                isomorphism_gamma(n_ord,b,j);
                                                isomorphism_gamma(n_ord,d,g);

                                                return {opt,n_ord};
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return {0,std::shared_ptr<ordering>()};
}