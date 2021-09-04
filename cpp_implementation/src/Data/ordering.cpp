#include <cstring>
#include <numeric>
#include <random>
#include <algorithm>

#include "ordering.hpp"
#include "solver.hpp"

unsigned short ordering::c_n;

void ordering::set_size(const unsigned short& n) {
    c_n = n;
}

const unsigned short& ordering::size() {
    return c_n;
}

ordering::ordering(): m_verts(new unsigned short[c_n]), m_indxs(new unsigned short[c_n]) {
    std::iota(m_verts,m_verts+c_n,0);
    std::memcpy(m_indxs,m_verts,sizeof(unsigned short)*c_n);
}

ordering::ordering(const ordering& other): 
    m_verts(new unsigned short[c_n]),
    m_indxs(new unsigned short[c_n])
{
    std::memcpy(m_verts,other.m_verts,sizeof(unsigned short)*c_n);
    std::memcpy(m_indxs,other.m_indxs,sizeof(unsigned short)*c_n);
}

ordering::~ordering() {
    delete[] m_verts;
    delete[] m_indxs;
}

const unsigned short& ordering::operator[](const unsigned short& i) const {
    return m_verts[i];
}

bool ordering::operator<(const ordering& other) const {
    ordering_iterator itr0(*this,m_indxs[0]);
    ordering_iterator itr1(other,other.m_indxs[0]);
    
    bool dir0 = next_of(0)<prev_of(0);
    bool dir1 = other.next_of(0)<other.prev_of(0);

    dir0 ? ++itr0 : --itr0;
    dir1 ? ++itr1 : --itr1;

    for (unsigned short i = 1; i<c_n; ++i) {
        if (*itr0<*itr1) return true;
        if (*itr0>*itr1) return false;
        
        dir0 ? ++itr0 : --itr0;
        dir1 ? ++itr1 : --itr1;
    }

    return false;
}

ordering_iterator ordering::begin() const {
    return ordering_iterator(*this,0,false);
}

ordering_iterator ordering::end() const {
    return ordering_iterator(*this,0,true);
}

const unsigned short& ordering::index_of(const unsigned short& v) const {
    return m_indxs[v];
}

const unsigned short& ordering::prev_of(const unsigned short& v) const {
    return m_verts[m_indxs[v] ? m_indxs[v]-1 : c_n-1];
}

const unsigned short& ordering::next_of(const unsigned short& v) const {
    return m_verts[(m_indxs[v]+1)%c_n];
}

int ordering::distance(const unsigned short& v0, const unsigned short& v1) const {
    const unsigned short& i0 = m_indxs[v0];
    const unsigned short& i1 = m_indxs[v1];

    return i0>i1 ? c_n-(i0-i1+1) : i1-i0-1;
}

bool ordering::is_between(
    const unsigned short& v,
    const unsigned short& v0,
    const unsigned short& v1
) const {

    const unsigned short& i = m_indxs[v];
    const unsigned short& i0 = m_indxs[v0];
    const unsigned short& i1 = m_indxs[v1];

    if (i0<i1 and i>i0 and i<i1) return true;
    if (i0>i1 and (i>i0 or i<i1)) return true;

    return false;
}

pair ordering::next_gap(ordering_iterator* itr_ptr) const {
    const graph& G = solver::get_instance()->get_graph();

    ordering_iterator itr = begin();
    if (not itr_ptr) itr_ptr = &itr;

    unsigned short prev, curr = **itr_ptr;

    const ordering_iterator e = end();
    
    while (*itr_ptr!=e) {
        prev = curr;
        curr = *++*itr_ptr;
        if (not G(prev,curr)) return pair(prev,curr);
    }
    
    return pair(0,0);
}

unsigned short ordering::n_gaps() const {
    unsigned short n = 0;
    ordering_iterator itr = begin();
    
    while (next_gap(&itr).is_valid()) ++n;

    return n;
}

void ordering::reverse_segment(const unsigned short& v0, const unsigned short& v1) {
    ordering_iterator itr0(*this,m_indxs[v0]);
    ordering_iterator itr1(*this,m_indxs[v1]);

    unsigned short n_itr = distance(v0,v1)/2+1;

    for (; n_itr>0; --n_itr, ++itr0, --itr1) {
        m_indxs[*itr0] = itr1.m_i;
        m_indxs[*itr1] = itr0.m_i;

        unsigned short aux = *itr0;
        m_verts[itr0.m_i] = *itr1;
        m_verts[itr1.m_i] = aux;
    }
}

void ordering::swap_segments(
    const unsigned short& v00,
    const unsigned short& v01,
    const unsigned short& v10,
    const unsigned short& v11
) {
    unsigned short* new_verts = new unsigned short[c_n];
    unsigned short i = 0;

    // Second segment
    ordering_iterator itr(*this,m_indxs[v10]);
    ordering_iterator e(*this,m_indxs[v11]); ++e;
    for (; itr!=e; ++itr, ++i) new_verts[i] = *itr;
    

    // Intermediate segment
    itr = ++ordering_iterator(*this,m_indxs[v01]);
    e = ordering_iterator(*this,m_indxs[v10]);
    for (; itr!=e; ++itr, ++i) new_verts[i] = *itr;

    // First segment
    itr = ordering_iterator(*this,m_indxs[v00]);
    e = ++ordering_iterator(*this,m_indxs[v01]);
    for (; itr!=e; ++itr, ++i) new_verts[i] = *itr;

    // Final segment
    itr = ++ordering_iterator(*this,m_indxs[v11]);
    e = ordering_iterator(*this,m_indxs[v00]);
    for (; itr!=e; ++itr, ++i) new_verts[i] = *itr;

    delete[] m_verts;
    m_verts = new_verts;
    
    for (unsigned short i = 0; i<c_n; ++i) m_indxs[m_verts[i]] = i;
}

void ordering::shuffle(unsigned seed) {
    std::shuffle(m_verts,m_verts+c_n,std::default_random_engine(seed));
    for (unsigned short i = 0; i<c_n; ++i) m_indxs[m_verts[i]] = i;
}

bool ordering_pointer_comparator::operator()(
    const std::shared_ptr<const ordering>& ord_ptr0,
    const std::shared_ptr<const ordering>& ord_ptr1
) const {
    return *ord_ptr0<*ord_ptr1;
}