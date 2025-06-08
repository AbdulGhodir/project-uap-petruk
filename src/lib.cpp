#include "../include/lib.h"
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

