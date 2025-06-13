#include <cctype>
#include <iostream>
#include "../include/lib.h"
#include "../include/menu.h"
using namespace std;

void menuNasabah() {
    cetakGaris(40, yellow);
    cout << "   SELAMAT DATANG DI SISTEM PEGADAIAN\n";
    cout << "       Aman, Cepat, dan Terpercya";
    cetakGaris(40, yellow);

    cout << "Silahkan Pilih Menu Berikut :\n";
    cout << "1. Anjukan Gadai Baru\n";
    cout << "2. Lihat Status Gadai Saya\n";
    cout << "3. Lihat Riwayat Gadai Saya\n";
    cout << "4. Keluar\n";
    cout << "Masukkan Pilihan Anda : ";

    int pilihan;
    cin >> pilihan;

    switch (pilihan) {
        case 1:
            ajukanGadai();
            break;
        case 2:
            statusGadai();
            break;
        case 3:
            riwayatGadai();
            break;
        case 4:
            cout << "Keluar";
            break;
        default:
            cout << "Masukkan opsi yang sesuai!!!";
            break;
    }
}

void menuAdmin() {
    cetakGaris(40, yellow);
    cout << "   SELAMAT DATANG DI SISTEM PEGADAIAN\n";
    cout << "       Aman, Cepat, dan Terpercya";
    cetakGaris(40, yellow);

    cout << "Silahkan Pilih Menu Berikut :\n";
    cout << "1. Tambah Data Gadai\n";
    cout << "2. Lihat Semua Data Identitas Nasabah\n";
    cout << "3. Lihat Semua Data Gadai Nasabah\n";
    cout << "4. Cari Data  Berdasarkan Nama Nasabah\n";
    cout << "5. Ubah Status Gadai\n";
    cout << "6. Hapus Data Gadai\n";
    cout << "7. Keluar\n";
    cout << "Masukkan Pilihan Anda : ";

    int pilihan;
    cin >> pilihan;

    switch (pilihan) {
        case 1:
            tambahDataGadai();
            break;
        case 2:
            lihatDataIdentitas();
            break;
        case 3:
            lihatDataGadai();
            break;
        case 4:
            cariDataGadai();
            break;
        case 5:
            ubahStatusGadai();
            break;
        case 6:
            hapusDataGadai();
            break;
        case 7:
            cout << "Keluar";
            break;
        default:
            cout << "Masukkan opsi yang sesuai!!!";
            break;
    }
}

void menu(string role) {  
    if (role == "admin") {
        menuAdmin();
    } else {
        menuNasabah();
    }
}