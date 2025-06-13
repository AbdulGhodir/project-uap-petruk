#ifndef MENU_H
#define MENU_H

#include <string>
using namespace std;

void menuNasabah();
void menuAdmin();
void menu(string role);

// Admin
void tambahDataGadai();
void lihatDataIdentitas();
void lihatDataGadai();
void cariDataGadai();
void ubahStatusGadai();
void hapusDataGadai();

// Nasabah
void ajukanGadai();
void statusGadai();
void riwayatGadai();

#endif