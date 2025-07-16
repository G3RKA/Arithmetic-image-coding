#pragma once
#include <vector>
#include <string>
#include <fstream>

struct FrequencyTable {
    FrequencyTable();

    void BuildFromByteData(const std::vector<uint8_t>&);

    bool BuildFromFile(std::string);

    void SaveToFile(std::string, uint32_t, uint32_t);

    uint8_t FindSymbol(uint32_t) const;

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetLow(uint8_t) const;
    uint32_t GetHigh(uint8_t) const;
    uint32_t GetTotal() const;

private:
    std::vector<uint32_t> freqs;
    std::vector<uint32_t> cum_freqs;
    uint32_t total;
    uint32_t width, height;
};

