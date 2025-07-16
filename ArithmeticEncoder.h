#pragma once
#include <vector>

struct FrequencyTable;

class ArithmeticEncoder
{
public:
    ArithmeticEncoder(std::vector<uint8_t>&, FrequencyTable*);

    void StartEncode();

    void EncodeSymbol(uint8_t);

    void FinishWork();

    const std::vector<uint8_t>& get_data() const;

private:
    static const uint32_t TOP = 0xFFFFFFFF;
    static const uint32_t HALF = TOP / 2 + 1;
    static const uint32_t Q1 = HALF / 2;
    static const uint32_t Q3 = Q1 * 3;

    uint32_t low = 0;
    uint32_t high = TOP;
    int bit_pos = 0;
    uint32_t underflow = 0;
    std::vector<uint8_t> bits;
    uint8_t current_byte = 0;

    FrequencyTable* model;
    std::vector<uint8_t> ImageData;

    void OutputBit(uint8_t);
    void ShiftBits(uint8_t);
};

