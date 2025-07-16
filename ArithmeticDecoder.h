#pragma once
#include <vector>

struct FrequencyTable;

class ArithmeticDecoder
{
public:
    ArithmeticDecoder(std::vector<uint8_t>&, FrequencyTable*);

    uint8_t DecodeSymbol();

private:
    static const uint32_t TOP = 0xFFFFFFFF;
    static const uint32_t HALF = TOP / 2 + 1;
    static const uint32_t Q1 = HALF / 2;
    static const uint32_t Q3 = Q1 * 3;

    std::vector<uint8_t> input;
    size_t byte_pos = 0;
    uint8_t current_byte = 0;
    int bit_pos = 0;

    uint32_t code = 0;
    uint32_t low = 0;
    uint32_t high = TOP;

    FrequencyTable* model;

    uint8_t ReadBit();
};

