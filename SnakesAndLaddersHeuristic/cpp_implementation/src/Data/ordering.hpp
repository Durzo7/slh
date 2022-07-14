#pragma once

#include <memory>

#include "ordering_iterator.hpp"
#include "pair.hpp"

class solver;

class ordering {
    friend class ordering_iterator;

    private:
        static unsigned short c_n;
        
        unsigned short* m_verts;
        unsigned short* m_indxs;

    public:
        static void set_size(const unsigned short& n);
        static const unsigned short& size();

        ordering();
        ordering(const ordering& other);
        ~ordering();

        const unsigned short& operator[](const unsigned short& i) const;

        bool operator<(const ordering& other) const;

        ordering_iterator begin() const;
        ordering_iterator end() const;

        const unsigned short& index_of(const unsigned short& v) const;

        const unsigned short& prev_of(const unsigned short& v) const;
        const unsigned short& next_of(const unsigned short& v) const;

        int distance(const unsigned short& v0, const unsigned short& v1) const;
        bool is_between(
            const unsigned short& v,
            const unsigned short& v0,
            const unsigned short& v1
        ) const;

        pair next_gap(ordering_iterator* itr_ptr=NULL) const;
        unsigned short n_gaps() const;

        void reverse_segment(const unsigned short& v0, const unsigned short& v1);
        void swap_segments(
            const unsigned short& v00,
            const unsigned short& v01,
            const unsigned short& v10,
            const unsigned short& v11
        );

        void shuffle(unsigned seed);
};

class ordering_pointer_comparator {
    public:
        bool operator()(
            const std::shared_ptr<const ordering>& ord_ptr0,
            const std::shared_ptr<const ordering>& ord_ptr1
        ) const;
};