#ifndef INCLUDE_CHAR_SET_HPP
#define INCLUDE_CHAR_SET_HPP

#include <stdint.h>

class char_set {
  private:
    int get_index(char c)
    {
        return ((uint8_t)c & 0xC0) >> 6;
    }

    int get_bit(char c)
    {
        return c & 0x3F;
    }

  public:
    uint64_t set[4];

    char_set() : set{0, 0, 0, 0} { };

    void add(char v)
    {
        int index = get_index(v);
        int bit = get_bit(v);

        this->set[index] |= (uint64_t)1 << bit;
    }

    void add_range(char start, char end)
    {
        int index_start = get_index(start);
        int bit_start = get_bit(start);

        int index_end = get_index(end);
        int bit_end = get_bit(end);

        while (index_start < index_end)
        {
            for (; bit_start < 64; bit_start++)
                this->set[index_start] |= (uint64_t)1 << bit_start;

            bit_start = 0;
            index_start++;
        }

        for (; bit_start <= bit_end; bit_start++)
            this->set[index_start] |= (uint64_t)1 << bit_start;
    }

    void remove(char v)
    {
        int index = ((uint8_t)v & 0xC0) >> 6;
        int bit = v & 0x3F;

        this->set[index] &= ~((uint64_t)1 << bit);
    }

    bool has(char v)
    {
        int index = ((uint8_t)v & 0xC0) >> 6;
        int bit = v & 0x3F;

        return !!(this->set[index] & ((uint64_t)1 << bit));
    }

    void set_union(char_set *set2)
    {
        this->set[0] |= set2->set[0];
        this->set[1] |= set2->set[1];
        this->set[2] |= set2->set[2];
        this->set[3] |= set2->set[3];
    }

    void set_intersect(char_set *set2)
    {
        this->set[0] &= set2->set[0];
        this->set[1] &= set2->set[1];
        this->set[2] &= set2->set[2];
        this->set[3] &= set2->set[3];
    }

    bool operator!=(char_set &set2)
    {
        return this->set[0] != set2.set[0]
            || this->set[1] != set2.set[1]
            || this->set[2] != set2.set[2]
            || this->set[3] != set2.set[3];
    }

    std::string to_string()
    {
        std::string str = "";
        int i, first = 1;
        char hex[20];

        str += "{ ";
        for (i = 0; i < 256; i++) {
            int index = get_index(i);
            int bit = get_bit(i);
            if (this->set[index] & ((uint64_t)1 << bit)) {
                if (!first)
                    str += ", ";
                else
                    first = 0;

                str += (char)i;
                str += "(" + std::to_string(i) + ")";
            }
        }
        str += " }";
        return str;
    }
};

#endif
