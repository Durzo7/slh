#pragma once

#include <memory>

#include "ordering.hpp"
#include "pair.hpp"

bool closing_2opt_type1(std::shared_ptr<ordering> ord, const pair& gap);

bool closing_2opt_type2(std::shared_ptr<ordering> ord, const pair& gap);

bool closing_3opt(std::shared_ptr<ordering> ord, const pair& gap);