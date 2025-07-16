#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

// Библиотека
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Структура таблицы вероятности
#include "FrequencyTable.h"
//  Класс кодировщик
#include "ArithmeticEncoder.h"
//  Класс декодировщик
#include "ArithmeticDecoder.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");

    int number;
    int width, height;

    string image_name;

    cout << "Меню кодирования:" << endl;
    cout << "1. Кодирование" << endl;
    cout << "2. Декодирование" << endl;
    cout << "Выберите пункт: ";
    cin >> number;
    
    if (number == 1)
    {
        cout << "Введите название изображения для кодирования :";
        cin >> image_name;

        // Загрузка изображения в трёх каналах
        int channels;
        uint8_t* img = stbi_load((image_name + ".bmp").c_str(), &width, &height, &channels, 3);
        if (!img) {
            cerr << "Ошибка загрузки.\n";
            return 1;
        }

        // Перенос данных и освобождение памяти
        size_t size = width * height * channels;
        vector<uint8_t> data(img, img + size);
        stbi_image_free(img);

        // Построение таблицы вероятности
        FrequencyTable freq;
        freq.BuildFromByteData(data);
        
        // Кодирование
        ArithmeticEncoder encoder(data, &freq);
        encoder.StartEncode();

        // Запись ключа кодирования в бинарный файл
        vector<uint8_t> encoded = encoder.get_data();
        ofstream out("encoded_" + image_name +".bin", ios::binary);
        out.write(reinterpret_cast<char*>(encoded.data()), encoded.size());

        // Сохранение таблицы вероятности
        freq.SaveToFile("encoded_" + image_name + ".txt", width, height);

        cout << "Изображения закодировано" << endl;
        cout << "Файл ключа кодирования: " << "encoded_" + image_name + ".bin" << endl;
        cout << "Файл таблицы вероятности: " << "encoded_" + image_name + ".txt" << endl;
    }
    else if (number == 2)
    {
        cout << "Введите название изображения для декодирования :";
        cin >> image_name;

        // Загрузка таблицы вероятности
        FrequencyTable freqDecode;
        bool freq_loaded = freqDecode.BuildFromFile("encoded_" + image_name + ".txt");

        if (!freq_loaded) {
            cerr << "Ошибка загрузки.\n";
            return 2;
        }

        ifstream in("encoded_" + image_name + ".bin", ios::binary | ios::ate);

        if (!in) {
            cerr << "Ошибка загрузки.\n";
            return 3;
        }

        // Получение размера файла и чтение
        streampos encode_size = in.tellg();
        in.seekg(0, ios::beg);

        vector<uint8_t> encoded_code(encode_size);
        in.read(reinterpret_cast<char*>(encoded_code.data()), encode_size);

        // Декодирование
        ArithmeticDecoder decoder(encoded_code, &freqDecode);
        width = freqDecode.GetWidth();
        height = freqDecode.GetHeight();
        vector<uint8_t> decoded;
        decoded.reserve(width * height * 3);
        for (size_t i = 0; i < width * height * 3; ++i)
            decoded.push_back(decoder.DecodeSymbol());

        // Запись изображения
        stbi_write_bmp(("decoded_" + image_name + ".bmp").c_str(), width, height, 3, decoded.data());

        cout << "Изображения декодировано" << endl;
        cout << "Файл изображения: " << "decoded_" + image_name + ".bmp" << endl;
    }

    return 0;
}