#include "ArithmeticEncoder.h"
#include "FrequencyTable.h"

ArithmeticEncoder::ArithmeticEncoder(std::vector<uint8_t>& InData, FrequencyTable* InModel)
{
    model = InModel;
    ImageData = InData;
}

void ArithmeticEncoder::StartEncode()
{
    for (uint8_t b : ImageData)
        EncodeSymbol(b);
    FinishWork();
}

void ArithmeticEncoder::EncodeSymbol(uint8_t sym)
{
    uint64_t range = (uint64_t)high - low + 1;
    high = static_cast<uint32_t>(low + (range * model->GetHigh(sym)) / model->GetTotal() - 1);
    low = static_cast<uint32_t>(low + (range * model->GetLow(sym)) / model->GetTotal());

    while (true) {
        if (high < HALF) {
            OutputBit(0);
            ShiftBits(1);
        }
        else if (low >= HALF) {
            OutputBit(1);
            ShiftBits(0);
            low -= HALF;
            high -= HALF;
        }
        else if (low >= Q1 && high < Q3) {
            underflow++;
            low -= Q1;
            high -= Q1;
        }
        else {
            break;
        }
        low <<= 1;
        high = (high << 1) | 1;
    }
}

void ArithmeticEncoder::FinishWork()
{
    underflow++;
    if (low < Q1) {
        OutputBit(0); ShiftBits(1);
    }
    else {
        OutputBit(1); ShiftBits(0);
    }

    if (bit_pos > 0)
        bits.push_back(current_byte << (8 - bit_pos));
}

const std::vector<uint8_t>& ArithmeticEncoder::get_data() const
{
    return bits;
}

void ArithmeticEncoder::OutputBit(uint8_t bit)
{
    current_byte = (current_byte << 1) | bit;
    bit_pos++;
    if (bit_pos == 8) {
        bits.push_back(current_byte);
        current_byte = 0;
        bit_pos = 0;
    }
}

void ArithmeticEncoder::ShiftBits(uint8_t bit)
{
    while (underflow--)
    {
        OutputBit(bit);
    }
    underflow = 0;
}
