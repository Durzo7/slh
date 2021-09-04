#include <algorithm>
#include <iostream>

#include "graph.hpp"

graph::graph(const unsigned short& n): 
    m_n(n),
    m_matrix(new boost::dynamic_bitset<>[n]),
    m_list(new std::vector<unsigned short>[n])
{
    for (unsigned short i = 0; i<n; i++) m_matrix[i] = boost::dynamic_bitset<>(n);
}

graph::~graph() {
    delete[] m_matrix;
    delete[] m_list;
}

bool graph::operator()(const unsigned short& v0, const unsigned short& v1) const {
    return m_matrix[v0][v1];
}

const boost::dynamic_bitset<>& graph::operator[](const unsigned short& v) const {
    return m_matrix[v];
}

const std::vector<unsigned short>& graph::get_neighbours(const unsigned short& v) const {
    return m_list[v];
}

const unsigned short& graph::size() const {
    return m_n;
}

void graph::connect(const unsigned short& v0, const unsigned short& v1) {
    if (v0!=v1 and v0<m_n and v1<m_n and not m_matrix[v0][v1]) {
        m_matrix[v0][v1] = 1;
        m_matrix[v1][v0] = 1;
        m_list[v0].push_back(v1);
        m_list[v1].push_back(v0);
    }
}