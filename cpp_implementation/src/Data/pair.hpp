#pragma once

#include <cstddef>

class pair {
    private:
        unsigned short m_v0;
        unsigned short m_v1;

    public:
        pair(unsigned short v0, unsigned short v1);
        pair(const pair& other);

        pair& operator=(const pair& other);
        bool operator==(const pair& other) const;
        bool operator!=(const pair& other) const;

        const unsigned short& v0() const;
        const unsigned short& v1() const;

        bool is_valid() const;
};

class cantor_pairing_hash {
    public:
        std::size_t operator()(const pair& p) const;
};