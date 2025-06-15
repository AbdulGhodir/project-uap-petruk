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

    cetakGaris(45, yellow);
    cout << "         LIHAT DATA IDENTITAS NASABAH";
    cetakGaris(45, yellow);

    cout << "\nSilahkan Pilih Data yang Ingin Ditampilkan :\n";
    cout << "1. Semua Data Identitas\n";
    cout << "2. Data Identitas Berdasarkan ID Nasabah\n";
    cout << "3. Data Identitas Berdasarkan Nama Nasabah\n";
    cout << "Masukkan Pilihan Anda: ";

    int pilihan;
    cin >> pilihan;
    cout << endl;   

    if (pilihan == 1) {
        int ke = 1;

        cetakGaris(45, yellow, '=');
        cout << "          List Data Identitas Nasabah";
        cetakGaris(45, yellow, '=');

        for (int i = 0; i < dataNasabah.size(); i++) {
            if (dataNasabah[i]["role"] != "user") continue;
            
            cout << "\nNasabah ke-" << ke;
            cetakGaris(12, yellow, '-');    
            cout << "ID Nasabah         : " << dataNasabah[i]["id"] << endl;
            cout << "Nama Nasabah       : " << dataNasabah[i]["nama"] << endl;
            cout << "E-Mail Nasabah     : " << dataNasabah[i]["email"] << endl;
            cout << "No Telepon Nasabah : " << dataNasabah[i]["noTelp"] << endl;
            cout << "Password Nasabah   : " << dataNasabah[i]["password"] << endl;
            cout << "NO KTP Nasabah     : " << dataNasabah[i]["noKtp"] << endl;
            cetakGaris(45, yellow, '=');
            ke++;
        }
    } else if (pilihan == 2) {

        string id;
        cout << "Masukkan ID Nasabah: ";
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

        cout << "Berikut adalah data nasabah dengan ID " << id << ":";
        cetakGaris(45, yellow, '-');
        cout << "ID Nasabah         : " << dataNasabah[indeks]["id"] << endl;
        cout << "Nama Nasabah       : " << dataNasabah[indeks]["nama"] << endl;
        cout << "E-Mail Nasabah     : " << dataNasabah[indeks]["email"] << endl;
        cout << "No Telepon Nasabah : " << dataNasabah[indeks]["noTelp"] << endl;
        cout << "Password Nasabah   : " << dataNasabah[indeks]["password"] << endl;
        cout << "NO KTP Nasabah     : " << dataNasabah[indeks]["noKtp"];
        cetakGaris(45, yellow, '=');

    } else if (pilihan == 3) {

        string namaNasabah;
        cout << "Masukkan Nama Nasabah: ";
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

        cout << "Berikut adalah data nasabah dengan nama " << namaNasabah << ":";
        cetakGaris(45, yellow, '-');
        cout << "ID Nasabah         : " << dataNasabah[indeks]["id"] << endl;
        cout << "Nama Nasabah       : " << dataNasabah[indeks]["nama"] << endl;
        cout << "E-Mail Nasabah     : " << dataNasabah[indeks]["email"] << endl;
        cout << "No Telepon Nasabah : " << dataNasabah[indeks]["noTelp"] << endl;
        cout << "Password Nasabah   : " << dataNasabah[indeks]["password"] << endl;
        cout << "NO KTP Nasabah     : " << dataNasabah[indeks]["noKtp"];
        cetakGaris(45, yellow, '=');
    } else {
        setWarnaFont(red);
        cout << "\n" << "Pilihan tidak tersedia" << "\n";
        setWarnaFont(reset);
        return;
    }
}

void listDataGadai() {
    MyDatabase barang("data/barang.csv");
    MyDatabase penitipan("data/penitipan.csv");
    MyDatabase transaksi("data/transaksi.csv");
    MyDatabase nasabah("data/user.csv");

    vector<unordered_map<string, string>> lihatBarang;
    vector<unordered_map<string, string>> lihatPenitipan;
    vector<unordered_map<string, string>> lihatTransaksi;
    vector<unordered_map<string, string>> lihatNasabah;

    barang.getData(lihatBarang);
    penitipan.getData(lihatPenitipan);
    transaksi.getData(lihatTransaksi);
    nasabah.getData(lihatNasabah);

    cetakGaris(45, yellow, '-');
    cout << "           List Data Gadai Nasabah";
    cetakGaris(45, yellow, '-');
    cout << endl;

    for (int i = 0 ; i < lihatBarang.size() ; i++){
        cout << "Nama Barang            : " << lihatBarang[i]["namaBarang"] << endl;
        cout << "ID Barang              : " << lihatBarang[i]["id"] << endl;
        cout << "ID Transaksi           : " << lihatTransaksi[i]["id"] << endl;
        cout << "ID Nasabah             : " << lihatBarang[i]["idUser"] << endl;

        for (int i = 0 ; i < lihatNasabah.size() ; i++){
            if (lihatBarang[i]["idUser"] == lihatNasabah[i]["id"]){
                cout << "Nama Nasabah           : " << lihatNasabah[i]["nama"] << endl;
                break;
            }
        }
        cout << "Berat Barang           : " << lihatBarang[i]["beratBarang"] << " kg" << endl;
        cout << "Harga Barang           : " << "Rp " << lihatBarang[i]["hargaBarang"] << endl;
        cout << "Tanggal Penggadaian    : " << lihatPenitipan[i]["tanggalGadai"] << endl;
        cout << "Tanggal Jatuh Tempo    : " << lihatPenitipan[i]["jatuhTempo"] << endl;
        cout << "Jenis Transaksi        : " << lihatTransaksi[i]["jenisTransaksi"] << endl;
        cout << "Status Gadai           : " << lihatBarang[i]["statusBarang"];
        cetakGaris(45, yellow, '=');
    }

}

void cariDataGadai() {
    cout << "CARI DATA GADAI BERDASARKAN NAMA NASABAH";
}

void ubahStatusGadai() {
    //cetakGaris(40, yellow);
    setWarnaFont(yellow);
    cout << "========== ";
    setWarnaFont(reset);
    cout <<"UBAH STATUS GADAI ";
    setWarnaFont(yellow);
    cout << "===========\n";
    setWarnaFont(reset);
    //cetakGaris(40, yellow);

    cout << "\nCari ID Barang yang ingin diubah: ";
    string cariID;
    cin >> cariID;

    MyDatabase barang("data/barang.csv");
    vector<unordered_map<string, string>> lihatBarang;
    barang.getData(lihatBarang);

    string statusBarang;
    bool ada = false;
    for (int i = 0 ; i < lihatBarang.size() ; i++){
        if (lihatBarang[i]["id"] == cariID){
            cout << "Nama Barang    : " << lihatBarang[i]["namaBarang"] << endl;
            cout << "Harga Barang   : " << lihatBarang[i]["hargaBarang"] << endl;
            cout << "Status Gadai   : " << lihatBarang[i]["statusBarang"] << endl;

            cetakGaris(40, yellow, '-');
            cout << "\nSilahkan update status gadai:" << endl;
            cout << "1. Berlangsung\n2. Tertebus\n";
            cout << "Masukkan Pilihan: ";
            cin >> statusBarang;
            
            ada = true;

            if (statusBarang == "1"){
                lihatBarang[i]["statusBarang"] = "Berlangsung";
            } else if (statusBarang == "2"){
                lihatBarang[i]["statusBarang"] = "Tertebus";
            } else {
                setWarnaFont(red);
                cout << "Pilihan tidak tersedia" << endl;
                setWarnaFont(reset);
                break;
            }

            cout << "Status Gadai Barang Dengan ID  " << cariID << " adalah: ";
            setWarnaFont(green);
            cout << lihatBarang[i]["statusBarang"] << endl;
            setWarnaFont(reset);
            
            barang.updateData(cariID, lihatBarang[i]);
            break;
        } 
    } 

    if (ada == false){
                setWarnaFont(red);
                cout << "Pilihan tidak tersedia" << endl;
                setWarnaFont(reset);
    }

}

void hapusDataGadai() {
    
}

