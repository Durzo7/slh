#pragma once

#include <memory>
#include <utility>

#include "ordering.hpp"
#include "pair.hpp"

std::pair<unsigned short,std::shared_ptr<ordering>> floating_2flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_3flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_4flo_type1(
    std::shared_ptr<const ordering> ord,
    const pair& gap
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_4flo_type2(
    std::shared_ptr<const ordering> ord,
    const pair& gap
);

std::pair<unsigned short,std::shared_ptr<ordering>> floating_5flo(
    std::shared_ptr<const ordering> ord,
    const pair& gap
);