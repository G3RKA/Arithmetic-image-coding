#include "FrequencyTable.h"

FrequencyTable::FrequencyTable()
{
    freqs = std::vector<uint32_t>(256, 0);
    total = 0;
    width = 0;
    height = 0;
}

void FrequencyTable::BuildFromByteData(const std::vector<uint8_t>& data)
{
    for (uint8_t b : data) {
        freqs[b]++;
        total++;
    }
    cum_freqs.resize(257);
    cum_freqs[0] = 0;
    for (int i = 0; i < 256; ++i)
        cum_freqs[i + 1] = cum_freqs[i] + freqs[i];
}

bool FrequencyTable::BuildFromFile(std::string inputfile)
{
    std::ifstream in(inputfile);

    if (!in)
    {
        return false;
    }

    int i;
    uint32_t count;

    in >> width >> height;
    total = width*height*3;

    while (in >> i >> count) {
        freqs[i] = count;
    }

    cum_freqs.resize(257);
    cum_freqs[0] = 0;
    for (int i = 0; i < 256; ++i)
        cum_freqs[i + 1] = cum_freqs[i] + freqs[i];

    in.close();

    return true;
}

void FrequencyTable::SaveToFile(std::string outputfile, uint32_t w, uint32_t h)
{
    std::ofstream out(outputfile);

    out << w << " " << h << "\n";
    for (int i{ 0 }; i < 256; i++) {
        out << i << " " << freqs[i] << "\n";
    }

    out.close();
}

uint8_t FrequencyTable::FindSymbol(uint32_t scaled) const
{
    for (int i = 0; i < 256; ++i)
        if (cum_freqs[i] <= scaled && scaled < cum_freqs[i + 1])
            return i;
    throw std::runtime_error("Symbol not found");
}

uint32_t FrequencyTable::GetWidth() const
{
    return width;
}

uint32_t FrequencyTable::GetHeight() const
{
    return height;
}

uint32_t FrequencyTable::GetLow(uint8_t sym) const
{
    return cum_freqs[sym];
}

uint32_t FrequencyTable::GetHigh(uint8_t sym) const
{
    return cum_freqs[sym + 1];
}

uint32_t FrequencyTable::GetTotal() const
{
    return total;
}
