#pragma once

#include "ordering.hpp"

void isomorphism_gamma(std::shared_ptr<ordering> ord, const unsigned short& x, const unsigned short& b);

void isomorphism_kappa (
    std::shared_ptr<ordering> ord,
    const unsigned short& x,
    const unsigned short& e,
    const unsigned short& c,
    const unsigned short& a,
    const unsigned short& b,
    const unsigned short& f,
    const unsigned short& d,
    const unsigned short& y
);  