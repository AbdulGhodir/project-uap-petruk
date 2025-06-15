#ifndef LIB_H
#define LIB_H

#include <string>

enum WARNA {
    cyan,
    red,
    black,
    green,
    yellow,
    white,
    blue,
    magenta,
    reset
};

void cetakGaris(int panjang, WARNA warna = cyan, char simbol = '-');
void setWarnaFont(WARNA warna = cyan);
std::string jatuhTempo(const std::string& tanggalGadai, int jangkaWaktu);

#endif