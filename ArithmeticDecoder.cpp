#include "ArithmeticDecoder.h"
#include "FrequencyTable.h"

ArithmeticDecoder::ArithmeticDecoder(std::vector<uint8_t>& InData, FrequencyTable* InModel) 
{
    input = InData;
    model = InModel;
    for (int i = 0; i < 32; ++i)
        code = (code << 1) | ReadBit();
}

uint8_t ArithmeticDecoder::DecodeSymbol()
{
    uint64_t range = (uint64_t)high - low + 1;
    uint64_t scaled = ((uint64_t)(code - low + 1) * model->GetTotal() - 1) / range;
    uint8_t sym = model->FindSymbol(static_cast<uint32_t>(scaled));

    high = static_cast<uint32_t>(low + (range * model->GetHigh(sym)) / model->GetTotal() - 1);
    low = static_cast<uint32_t>(low + (range * model->GetLow(sym)) / model->GetTotal());

    while (true) {
        if (high < HALF) {
            //
        }
        else if (low >= HALF) {
            low -= HALF; 
            high -= HALF; 
            code -= HALF;
        }
        else if (low >= Q1 && high < Q3) {
            low -= Q1; 
            high -= Q1; 
            code -= Q1;
        }
        else {
            break;
        }
        low <<= 1;
        high = (high << 1) | 1;
        code = (code << 1) | ReadBit();
    }

    return sym;
}

uint8_t ArithmeticDecoder::ReadBit()
{
    if (bit_pos == 0) {
        if (byte_pos < input.size())
            current_byte = input[byte_pos++];
        else
            current_byte = 0;
        bit_pos = 8;
    }
    return (current_byte >> (--bit_pos)) & 1;
}
