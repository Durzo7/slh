#pragma once

#include <memory>
#include <utility>
#include <set>

#include "ordering.hpp"

#define N_CLOSING_TRANSFORMATIONS 3
#define N_FLOATING_TRANSFORMATIONS 5

bool get_closing(
    const unsigned short& trans,
    std::shared_ptr<ordering> ord,
    const pair& gap
);

std::pair<unsigned short,std::shared_ptr<ordering>> get_floating(
    const unsigned short& trans,
    std::shared_ptr<const ordering> ord,
    const pair& gap
);

std::pair<unsigned short,std::shared_ptr<ordering>> get_floating_opt(
    const unsigned short& trans,
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);

bool transform_close(std::shared_ptr<ordering> ord);

std::pair<unsigned short,std::shared_ptr<ordering>> transform_float(
    std::shared_ptr<const ordering> ord
);

std::pair<unsigned short,std::shared_ptr<ordering>> transform_float_opt(
    std::shared_ptr<const ordering> ord,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);

std::pair<short,std::shared_ptr<ordering>> transform_open(
    std::shared_ptr<const ordering> ord,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);