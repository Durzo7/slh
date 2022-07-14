#include "ordering_iterator.hpp"
#include "ordering.hpp"

ordering_iterator::ordering_iterator(const ordering& ord, const unsigned short& i, bool end): 
    m_ord(ord),
    m_i(i),
    m_end_flag(end)
{}

ordering_iterator::ordering_iterator(const ordering_iterator& other):
    m_ord(other.m_ord),
    m_i(other.m_i),
    m_end_flag(other.m_end_flag)
{}

const unsigned short& ordering_iterator::operator*() const {
    return m_ord.m_verts[m_i];
}

ordering_iterator& ordering_iterator::operator++() {
    return (*this)+=1;
}

ordering_iterator& ordering_iterator::operator--() {
    return (*this)-=1; 
}

ordering_iterator ordering_iterator::operator++(int) {
    ordering_iterator copy(*this);
    (*this)+=1;
    return copy;
}

ordering_iterator ordering_iterator::operator--(int) {
    ordering_iterator copy(*this);
    (*this)-=1;
    return copy;
}

bool ordering_iterator::operator==(const ordering_iterator& other) {
    if (m_i==other.m_i) {
        if (m_end_flag==other.m_end_flag) return true;
        m_end_flag = true;
    }

    return false;
}

ordering_iterator& ordering_iterator::operator=(const ordering_iterator & other) {
    m_i = other.m_i;
    m_end_flag = other.m_end_flag;
    
    return *this;
}

bool ordering_iterator::operator!=(const ordering_iterator& other) {
    return not (*this==other);
}

ordering_iterator ordering_iterator::operator+(const unsigned short& add) const {
    ordering_iterator copy(*this);
    copy += add;
    return copy;
}

ordering_iterator& ordering_iterator::operator+=(const unsigned short& add) {
    m_i = (m_i+add)%ordering::c_n;
    return *this;
}

ordering_iterator ordering_iterator::operator-(const unsigned short& add) const {
    ordering_iterator copy(*this);
    copy -= add;
    return copy;
}

ordering_iterator& ordering_iterator::operator-=(const unsigned short& add) {
    m_i = m_i<add ? ordering::c_n-(add-m_i)%ordering::c_n : m_i - add;
    return *this;
}

const unsigned short& ordering_iterator::get_index() const {
    return m_ord.m_indxs[m_i];
}