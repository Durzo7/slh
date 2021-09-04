#pragma once

class ordering;

class ordering_iterator {
    friend class ordering;

    private:
        unsigned short m_i;
        const ordering& m_ord;
        bool m_end_flag;

    public:
        ordering_iterator(const ordering& ord, const unsigned short& i, bool end_flag=false);
        ordering_iterator(const ordering_iterator& other);

        // Deference
        const unsigned short& operator*() const;

        // Increment and Decrement
        ordering_iterator& operator++();
        ordering_iterator& operator--();
        ordering_iterator operator++(int);
        ordering_iterator operator--(int);

        // Direct assigment
        ordering_iterator& operator=(const ordering_iterator& other);

        // Comparison
        bool operator==(const ordering_iterator& other);
        bool operator!=(const ordering_iterator& other);

        // Displacement
        ordering_iterator operator+(const unsigned short& add) const;
        ordering_iterator& operator+=(const unsigned short& add);
        ordering_iterator operator-(const unsigned short& add) const;
        ordering_iterator& operator-=(const unsigned short& add);

        const unsigned short& get_index() const;
};