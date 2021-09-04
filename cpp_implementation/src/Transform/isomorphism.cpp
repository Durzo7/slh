#include "isomorphism.hpp"

void isomorphism_gamma(std::shared_ptr<ordering> ord, const unsigned short& x, const unsigned short& b) {
    ord->reverse_segment(x,b);
}

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
) {
    ord->swap_segments(b,f,d,y);
    ord->reverse_segment(x,e);
    ord->reverse_segment(c,a);
    ord->reverse_segment(b,f);
}