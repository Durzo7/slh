#pragma once

#include <boost/dynamic_bitset.hpp>
#include <vector>

class graph {
    private:
        const unsigned short m_n;
        boost::dynamic_bitset<>* const m_matrix;
        std::vector<unsigned short>* const m_list;

    public:
        graph(const unsigned short& n);
        ~graph();
        
        bool operator()(const unsigned short& v0,const unsigned short& v1) const;
        const boost::dynamic_bitset<>& operator[](const unsigned short& v) const;
        
        const std::vector<unsigned short>& get_neighbours(const unsigned short& v) const;
        const unsigned short& size() const;
        
        void connect(const unsigned short& v0, const unsigned short& v1);
};