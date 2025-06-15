#include "../include/lib.h"
#include <vector>
#include <iostream>

void cetakGaris(int panjang, WARNA warna, char simbol) {
    std::cout << "\n";
    switch (warna)
    {
    case cyan:
        std::cout << "\033[36m";
        break;
    case red:
        std::cout << "\033[31m";
        break;
    case blue:
        std::cout << "\033[34m";
        break;
    case white:
        std::cout << "\033[37m";
        break;
    case magenta:
        std::cout << "\033[35m";
        break;
    case green:
        std::cout << "\033[32m";
        break;
    case black:
        std::cout << "\033[30m";
        break;
    case yellow:
        std::cout << "\033[33m";
        break;
    default:
        std::cout << "\033[36m";
        break;
    }
    
    for(int i = 1; i <= panjang; i++) {
        std::cout << simbol;
    }
    std::cout << "\033[0m" << "\n";
}

void setWarnaFont(WARNA warna) {
    switch (warna)
    {
    case cyan:
        std::cout << "\033[36m";
        break;
    case red:
        std::cout << "\033[31m";
        break;
    case blue:
        std::cout << "\033[34m";
        break;
    case white:
        std::cout << "\033[37m";
        break;
    case magenta:
        std::cout << "\033[35m";
        break;
    case green:
        std::cout << "\033[32m";
        break;
    case black:
        std::cout << "\033[30m";
        break;
    case yellow:
        std::cout << "\033[33m";
        break;
    case reset:
        std::cout << "\033[0m";
        break;
    default:
        std::cout << "\033[36m";
        break;
    }
}


std::string jatuhTempo(const std::string& tanggalGadai, int jangkaWaktu) {
    std::vector<int> date;
    date.push_back(stoi(tanggalGadai.substr(0, 2)) + jangkaWaktu);
    date.push_back(stoi(tanggalGadai.substr(3, 4)));

    date[1] += date[0] / 13;
    date[0] %= 12;
    date[0] = date[0] == 0 ? 12 : date[0];

    std::string jatuhTempo;
    for (int i = 0; i < date.size(); i++) {
        if (date[i] < 10) {
            jatuhTempo += '0' + std::to_string(date[i]);
        } else {
            jatuhTempo += std::to_string(date[i]);
        }

        if (i != 1) {
            jatuhTempo += '/';
        }
    }
    return jatuhTempo;
}