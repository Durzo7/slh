#include <cmath>
#include <iostream>
#include <chrono>
#include <fstream>

#include "solver.hpp"
#include "parse.hpp"
#include "stage.hpp"

solver* solver::c_solv;

solver* solver::initialize(std::string filepath) {
    c_solv = new solver(filepath);
    return c_solv;
}

solver* solver::get_instance() {
    return c_solv;
}

void solver::destroy() {
    delete c_solv;
}

solver::solver(std::string filepath): m_graph(load(filepath)) {
    ordering::set_size(m_graph.size());
    std::shared_ptr<ordering> n_ord = std::make_shared<ordering>();
    m_ord_stack.push_back(n_ord);
    m_stage = "-";
    m_elapsed = "-";
}

solver::~solver() {
    delete &m_graph;
}

const graph& solver::get_graph() const {
    return m_graph;
}

const std::vector<std::shared_ptr<ordering>>& solver::get_ordering_stack() const {
    return m_ord_stack;
}

const std::unordered_set<pair,cantor_pairing_hash>& solver::get_gap_set() const {
    return m_gap_set;
}

std::shared_ptr<ordering> solver::get_ordering() const {
    return m_ord_stack.back();
}

const unsigned short& solver::get_n_gaps() const {
    return m_n_gaps;
}

void solver::clear() {
    std::shared_ptr<ordering> ord = m_ord_stack.back();
    m_ord_stack.clear();
    m_ord_stack.push_back(ord);
    m_gap_set.clear();
}

void solver::add_ordering(std::shared_ptr<ordering> ord) {
    m_ord_stack.push_back(ord);
}

std::shared_ptr<ordering> solver::pop_ordering() {
    std::shared_ptr<ordering> ord = m_ord_stack.back();
    m_ord_stack.pop_back();
    return ord;
}

void solver::add_gap(const pair& gap) {
    m_gap_set.insert(gap);
}

void solver::set_n_gaps(const unsigned short& n) {
    m_n_gaps = n;
}

bool solver::exit_condition(std::size_t exponent) const {
    return m_ord_stack.size()>std::pow((size_t)m_graph.size(),exponent);
}

bool solver::solve() {
    auto start = std::chrono::steady_clock::now();

    m_stage = "0";
    stage0();
    
    if (m_n_gaps>0) {
        m_stage = "1";
        stage1();
    }

    if (m_n_gaps>0) m_stage = "2";
    while (m_n_gaps>0 and stage2()) stage1();
    
    if (m_n_gaps>0) m_stage = "3";
    while (m_n_gaps>0 and stage3()) {
        stage1();
        while (m_n_gaps>0 and stage2()) stage1();
    }

    auto end = std::chrono::steady_clock::now();
    m_elapsed = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    //std::cout << "Elapsed time: "
    //<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    //<< " ms" << std::endl;

    return m_n_gaps==0;
}

void solver::write(std::string filepath) const {
    std::ofstream file(filepath);

    file << "Stage: " << m_stage << std::endl;
    file << "Elapsed Time: " << m_elapsed << "ms" << std::endl;

    for (unsigned short v : *m_ord_stack.back()) file << v+1 << ' ';
    file << std::endl;

    file.close();
}