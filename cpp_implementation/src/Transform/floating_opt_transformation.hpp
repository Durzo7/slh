#pragma once

#include <memory>
#include <utility>
#include <set>

#include "ordering.hpp"
#include "pair.hpp"

std::pair<unsigned short,std::shared_ptr<ordering>> floating_2opt(
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_3opt(
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_4opt_type1(
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_4opt_type2(
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_5opt(
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);