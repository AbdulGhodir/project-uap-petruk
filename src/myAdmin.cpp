#include <iostream>
#include <vector>
#include <unordered_map>
#include "../include/lib.h"
#include "../include/database.h"
#include "../include/user.h"
#include "../include/menu.h"

using namespace std;

string jatuhTempo(const string& tanggalGadai, int jangakWaktu) {
    vector<int> date;
    date.push_back(stoi(tanggalGadai.substr(0, 2)) + jangakWaktu);
    date.push_back(stoi(tanggalGadai.substr(3, 4)));

    date[1] +=date[0] / 13;
    date[0] %= 12;
    date[0] = date[0] == 0 ? 12 : date[0];

    string jatuhTempo;
    for (int i = 0; i < date.size(); i++) {
        if (date[i] < 10) {
            jatuhTempo += '0' + to_string(date[i]);
        } else {
            jatuhTempo += to_string(date[i]);
        }

        if (i != 1) {
            jatuhTempo += '/';
        }
    }
    return jatuhTempo;
}

int taksiranHarga(const string& hargaBarang, const string& beratBarang) {
    if (stoi(beratBarang) <= 5) {
        return stoi(hargaBarang) * 0.9;
    } else if (stoi(beratBarang) <= 10) {
        return stoi(hargaBarang) * 0.85;
    } else if (stoi(beratBarang) <= 20) {
        return stoi(hargaBarang) * 0.80;
    } else if (stoi(beratBarang) <= 50) {
        return stoi(hargaBarang) * 0.70;
    } else {
        return stoi(hargaBarang) * 0.60;
    }
}

void tambahDataGadai() {
    MyDatabase barang("data/barang.csv");
    MyDatabase transaksi("data/transaksi.csv");
    MyDatabase penitipan("data/penitipan.csv");
    
    unordered_map<string, string> tambahBarang;
    unordered_map<string, string> tambahTransaksi;
    unordered_map<string, string> tambahPenitipan;
    int statusBarang, jenisTransaksi, jangkaWaktu;
    
    cout << "Silahkan masukkan id nasabah : ";
    cin >> tambahBarang["idUser"];
    cout << "Silahkan masukkan nama barang yang akan digadaikan : ";
    cin >> tambahBarang["namaBarang"];
    cout << "Silahkan masukkan harga barang : ";
    cin >> tambahBarang["hargaBarang"];
    cout << "Silahkan masukkan berat barang : ";
    cin >> tambahBarang["beratBarang"];
    cout << "Silahkan pilih status gadai (1 = Berlangsung, 2 = Tertebus) : ";
    cin >> statusBarang;
    tambahBarang["statusBarang"] = (statusBarang == 1) ? "Berlangsung" : "Tertebus";
    
    barang.writeData(tambahBarang);

    vector<unordered_map<string, string>> dataBarang;
    barang.getData(dataBarang);

    int indeks;
    for (int i = 0; i < dataBarang.size(); i++) {
        if (dataBarang[i]["namaBarang"] == tambahBarang["namaBarang"]) {
            indeks = i;
        }
    }

    tambahTransaksi["idBarang"] = dataBarang[indeks]["id"];

    tambahTransaksi["idUser"] = tambahBarang["idUser"];
    cout << "Silahkan masukkan tanggal gadai barang (format mm/yyyy, ex : 06/2025) : ";
    cin >> tambahTransaksi["tanggalGadai"];
    cout << "Silahkan pilih jenis transaksi (1 = Pengajuan, 2 = Pelunasan) : ";
    cin >>  jenisTransaksi;

    tambahTransaksi["jenisTransaksi"] = (jenisTransaksi == 1) ? "Pengajuan" : "Pelunasan";
    tambahTransaksi["totalHarga"] = to_string(taksiranHarga(tambahBarang["hargaBarang"], tambahBarang["beratBarang"]));
    
    transaksi.writeData(tambahTransaksi);
    
    cout << "Silahkan masukkan jangka waktu gadai (bulan) : ";
    cin >> jangkaWaktu;
    tambahPenitipan["idBarang"] = tambahTransaksi["idBarang"];
    tambahPenitipan["tanggalGadai"] = tambahTransaksi["tanggalGadai"];
    tambahPenitipan["jatuhTempo"] = jatuhTempo(tambahTransaksi["tanggalGadai"], jangkaWaktu);

    penitipan.writeData(tambahPenitipan);
}

void listDataIdentitas() {
    MyDatabase nasabah("data/user.csv");
    vector<unordered_map<string, string>> dataNasabah;
    nasabah.getData(dataNasabah);

    cout << "Silahkan Pilih Data yang Ingin Ditampilkan :\n";
    cout << "1. Semua Data Identitas\n";
    cout << "2. Data Identitas Berdasarkan ID Nasabah\n";
    cout << "3. Data Identitas Berdasarkan Nama Nasabah\n";
    cout << "Masukkan Pilihan Anda : ";

    int pilihan;
    cin >> pilihan;
    cout << endl;   

    if (pilihan == 1) {
        int ke = 1;

        for (int i = 0; i < dataNasabah.size(); i++) {
            if (dataNasabah[i]["role"] != "user") continue;
            
            cout << "\nNasabah ke-" << ke;
            cetakGaris(12, yellow, '-');    
            cout << "ID Nasabah         : " << dataNasabah[i]["id"] << endl;
            cout << "Nama Nasabah       : " << dataNasabah[i]["nama"] << endl;
            cout << "E-Mail Nasabah     : " << dataNasabah[i]["email"] << endl;
            cout << "No Telepon Nasabah : " << dataNasabah[i]["noTelp"] << endl;
            cout << "Password Nasabah   : " << dataNasabah[i]["password"] << endl;
            cout << "NO KTP Nasabah     : " << dataNasabah[i]["noKtp"];
            cetakGaris(40, yellow, '=');
            ke++;
        }
    } else if (pilihan == 2) {

        string id;
        cout << "Masukkan ID Nasabah : ";
        cin >> id;
        
        int indeks;
        bool ada = false;
        for (int i = 0 ; i < dataNasabah.size() ; i++){
            if (dataNasabah[i]["id"] == id) {
                indeks = i;
                ada = true;
                break;
            }
        }
        
        if (ada == false){
            setWarnaFont(red);
            cout << "\n" << "ID Nasabah " << id << " tidak ditemukan" << "\n";
            setWarnaFont(reset);
            return;
        }

        cout << "Berikut adalah data nasabah dengan id " << id << " :";
        cetakGaris(40, yellow, '-');
        cout << "ID Nasabah         : " << dataNasabah[indeks]["id"] << endl;
        cout << "Nama Nasabah       : " << dataNasabah[indeks]["nama"] << endl;
        cout << "E-Mail Nasabah     : " << dataNasabah[indeks]["email"] << endl;
        cout << "No Telepon Nasabah : " << dataNasabah[indeks]["noTelp"] << endl;
        cout << "Password Nasabah   : " << dataNasabah[indeks]["password"] << endl;
        cout << "NO KTP Nasabah     : " << dataNasabah[indeks]["noKtp"];
        cetakGaris(40, yellow, '=');

    } else if (pilihan == 3) {

        string namaNasabah;
        cout << "Masukkan Nama Nasabah : ";
        cin >> namaNasabah;
        
        int indeks;
        bool ada = false;
        for (int i = 0 ; i < dataNasabah.size() ; i++){
            if (dataNasabah[i]["nama"] == namaNasabah) {
                indeks = i;
                ada = true;
                break;
            }
        }
        
        if (ada == false){
            setWarnaFont(red);
            cout << "\n" << "Nama Nasabah " << namaNasabah << " tidak ditemukan" << "\n";
            setWarnaFont(reset);
            return;
        }

        cout << "Berikut adalah data nasabah dengan nama " << namaNasabah << " :";
        cetakGaris(40, yellow, '-');
        cout << "ID Nasabah         : " << dataNasabah[indeks]["id"] << endl;
        cout << "Nama Nasabah       : " << dataNasabah[indeks]["nama"] << endl;
        cout << "E-Mail Nasabah     : " << dataNasabah[indeks]["email"] << endl;
        cout << "No Telepon Nasabah : " << dataNasabah[indeks]["noTelp"] << endl;
        cout << "Password Nasabah   : " << dataNasabah[indeks]["password"] << endl;
        cout << "NO KTP Nasabah     : " << dataNasabah[indeks]["noKtp"];
        cetakGaris(40, yellow, '=');
    } else {
        setWarnaFont(red);
        cout << "\n" << "Pilihan tidak tersedia" << "\n";
        setWarnaFont(reset);
        return;
    }
}

void listDataGadai() {
    cout << "LIHAT DATA GADAI NASABAH";
    
}

void cariDataGadai() {
    cout << "CARI DATA GADAI BERDASARKAN NAMA NASABAH";
}

void ubahStatusGadai() {
    
}

void hapusDataGadai() {
    
}

