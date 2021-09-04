#include "transform.hpp"
#include "closing_transformation.hpp"
#include "floating_transformation.hpp"
#include "floating_opt_transformation.hpp"
#include "opening_transformation.hpp"
#include "solver.hpp"

bool get_closing(
    const unsigned short& tfrm,
    std::shared_ptr<ordering> ord,
    const pair& gap
) {
    switch (tfrm) {
        case 0: return closing_2opt_type1(ord,gap);
        case 1: return closing_2opt_type2(ord,gap);
        case 2: return closing_3opt(ord,gap);
    }

    return false;
}

std::pair<unsigned short,std::shared_ptr<ordering>> get_floating(
    const unsigned short& tfrm,
    std::shared_ptr<const ordering> ord,
    const pair& gap
) {
    switch (tfrm) {
        case 0: return floating_2flo(ord,gap);
        case 1: return floating_3flo(ord,gap);
        case 2: return floating_4flo_type1(ord,gap);
        case 3: return floating_4flo_type2(ord,gap);
        case 4: return floating_5flo(ord,gap);
    }

    return {0,std::shared_ptr<ordering>()};
}

std::pair<unsigned short,std::shared_ptr<ordering>> get_floating_opt(
    const unsigned short& tfrm,
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
) {
    switch (tfrm) {
        case 0: return floating_2opt(ord,gap,visited);
        case 1: return floating_3opt(ord,gap,visited);
        case 2: return floating_4opt_type1(ord,gap,visited);
        case 3: return floating_4opt_type2(ord,gap,visited);
        case 4: return floating_5opt(ord,gap,visited);
    }
    
    return {0,std::shared_ptr<ordering>()};
}

bool transform_close(std::shared_ptr<ordering> ord) {
    for (unsigned short tfrm = 0; tfrm<N_CLOSING_TRANSFORMATIONS; ++tfrm) {
        ordering_iterator itr = ord->begin();
        pair gap = ord->next_gap(&itr);
        
        while (gap.is_valid()) {
            if (get_closing(tfrm,ord,gap)) return true;
            gap = ord->next_gap(&itr);
        }
    }

    return false;
}

std::pair<unsigned short,std::shared_ptr<ordering>> transform_float(
    std::shared_ptr<const ordering> ord
) {
    for (unsigned short tfrm = 0; tfrm<N_FLOATING_TRANSFORMATIONS; ++tfrm) {
        ordering_iterator itr = ord->begin();
        pair gap = ord->next_gap(&itr);

        while (gap.is_valid()) {
            std::pair<unsigned short,std::shared_ptr<ordering>> n_ord_pair = get_floating(tfrm,ord,gap);
        
            if (n_ord_pair.second) {
                solver::get_instance()->add_gap(gap);
                return n_ord_pair;
            }
            
            gap = ord->next_gap(&itr);
        }
    }

    return {0,std::shared_ptr<ordering>()};
}

std::pair<unsigned short,std::shared_ptr<ordering>> transform_float_opt(
    std::shared_ptr<const ordering> ord,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
) {
    for (unsigned short tfrm = 0; tfrm<N_FLOATING_TRANSFORMATIONS; ++tfrm) {
        ordering_iterator itr = ord->begin();
        pair gap = ord->next_gap(&itr);

        while (gap.is_valid()) {
            std::pair<unsigned short,std::shared_ptr<ordering>> n_ord_pair = get_floating_opt(
                tfrm,
                ord,
                gap,
                visited
            );
        
            if (n_ord_pair.second) return n_ord_pair;

            gap = ord->next_gap(&itr);
        }
    }

    return {0,std::shared_ptr<ordering>()};
}

std::pair<short,std::shared_ptr<ordering>> transform_open(
    std::shared_ptr<const ordering> ord,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
) {
    ordering_iterator itr = ord->begin();
    pair gap = ord->next_gap(&itr);

    while (gap.is_valid()) {
        std::pair<short,std::shared_ptr<ordering>> n_ord_pair = opening_4flo(ord,gap,visited);

        if (n_ord_pair.second) return n_ord_pair;

        gap = ord->next_gap(&itr);
    }

    return {0,std::shared_ptr<ordering>()};
}