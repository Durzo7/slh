#pragma once

#include <memory>
#include <utility>
#include <set>

#include "ordering.hpp"
#include "pair.hpp"

std::pair<short,std::shared_ptr<ordering>> opening_4flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap,
    const std::set<std::shared_ptr<const ordering>,ordering_pointer_comparator>& visited
);