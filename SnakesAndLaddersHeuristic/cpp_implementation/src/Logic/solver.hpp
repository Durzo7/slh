#pragma once

#include <vector>
#include <unordered_set>
#include <memory>

#include "graph.hpp"
#include "ordering.hpp"
#include "pair.hpp"

class solver {
    private:
        static solver* c_solv;

        solver(std::string filepath);
        ~solver();

        const graph& m_graph;
        std::vector<std::shared_ptr<ordering>> m_ord_stack;
        std::unordered_set<pair,cantor_pairing_hash> m_gap_set;
        unsigned short m_n_gaps;

        std::string m_stage;
        std::string m_elapsed;

    public:
        static solver* initialize(std::string filepath);
        static solver* get_instance();
        static void destroy();

        const graph& get_graph() const;
        const std::vector<std::shared_ptr<ordering>>& get_ordering_stack() const;
        const std::unordered_set<pair,cantor_pairing_hash>& get_gap_set() const;
        std::shared_ptr<ordering> get_ordering() const;
        const unsigned short& get_n_gaps() const;
        
        void clear();
        void add_ordering(std::shared_ptr<ordering> ord);
        std::shared_ptr<ordering> pop_ordering();
        void add_gap(const pair& gap);
        void set_n_gaps(const unsigned short& n);

        bool exit_condition(std::size_t exponent) const;

        bool solve();

        void write(std::string filepath) const;
};