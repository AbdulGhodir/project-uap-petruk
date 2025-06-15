#ifndef LIB_H
#define LIB_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

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
void merge(std::vector<std::unordered_map<std::string, std::string>>& data, int begin, int middle, int end, bool descending, std::string key);
void mergeSort(std::vector<std::unordered_map<std::string, std::string>>& data, int left, int right, bool descending, std::string key);

#endif