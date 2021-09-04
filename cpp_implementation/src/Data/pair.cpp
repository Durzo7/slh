#include "pair.hpp"

pair::pair(unsigned short v0, unsigned short v1): m_v0(v0), m_v1(v1) {}

pair::pair(const pair& other): m_v0(other.m_v0), m_v1(other.m_v1) {}

pair& pair::operator=(const pair& other) {
    m_v0 = other.m_v0;
    m_v1 = other.m_v1;

    return *this;
}

bool pair::operator==(const pair& other) const {
    return (m_v0==other.m_v0 or m_v0==other.m_v1) and (m_v1==other.m_v0 or m_v1==other.m_v1);
}

bool pair::operator!=(const pair& other) const {
    return not (*this==other);
}

const unsigned short& pair::v0() const {
    return m_v0;
}

const unsigned short& pair::v1() const {
    return m_v1;
}

bool pair::is_valid() const {
    return m_v0!=m_v1;
}

std::size_t cantor_pairing_hash::operator()(const pair& p) const {
    std::size_t x = p.v0();
    std::size_t y = p.v1();

    if (x<y) {
        x = p.v1();
        y = p.v0();
    }

    return (x+y)*(x+y+1)/2+y;
}