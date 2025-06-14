#include <iostream>
#include <vector>
#include <unordered_map>
#include "../include/lib.h"
#include "../include/database.h"
#include "../include/user.h"
#include "../include/menu.h"

using namespace std;

// Sebuah fungsi pembantu yang digunakan untuk menghitung taksiran harga
int taksiranHarga(int hargaBarang, int beratBarang) {
    if (beratBarang <= 5) {
        return hargaBarang * 0.9;
    } else if (beratBarang <= 10) {
        return hargaBarang * 0.85;
    } else if (beratBarang <= 20) {
        return hargaBarang * 0.80;
    } else if (beratBarang <= 50) {
        return hargaBarang * 0.70;
    } else {
        return hargaBarang * 0.60;
    }
}

// Fungsi helper untuk menghitung jatuh tempo
string jatuhTempo(const string& tanggalGadai, int jangkaWaktu) {
    vector<int> date;
    date.push_back(stoi(tanggalGadai.substr(0, 2)) + jangkaWaktu);
    date.push_back(stoi(tanggalGadai.substr(3, 4)));

    date[1] += date[0] / 13;
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

// Fungsi helper untuk mencari data barang berdasarkan ID
unordered_map<string, string> cariBarangById(const vector<unordered_map<string, string>>& dataBarang, const string& idBarang) {
    for (int i = 0; i < dataBarang.size(); i++) {
        auto it = dataBarang[i].find("id");
        if (it != dataBarang[i].end() && it->second == idBarang) {
            return dataBarang[i];
        }
    }
    return unordered_map<string, string>(); // Return empty map jika tidak ditemukan
}

void ajukanGadai() {
    MyDatabase barang("../data/barang.csv");
    MyDatabase transaksi("../data/transaksi.csv");
    MyDatabase penitipan("../data/penitipan.csv");
    
    unordered_map<string, string> tambahBarang;
    unordered_map<string, string> tambahTransaksi;
    unordered_map<string, string> tambahPenitipan;
    
    string namaBarang, idUser, tanggalGadai;
    int hargaBarang, beratBarang, jangkaWaktu;

    cout << "=== AJUKAN GADAI ===" << endl;
    MyDatabase user("../data/user.csv");
    vector<unordered_map<string, string>> dataUser;
    user.getData(dataUser);

    bool userDitemukan = false;
    unordered_map<string, string> foundUserData;

    do {
        cout << "Masukkan ID nasabah : ";
        cin >> idUser;

        userDitemukan = false;
        for (const auto& userData : dataUser) {
            if (userData.at("id") == idUser) {
                userDitemukan = true;
                foundUserData = userData;
                break;
            }
        }

        if (!userDitemukan) {
            cout << "Error: User dengan ID " << idUser << " tidak ditemukan!" << endl;
        }
    } while (!userDitemukan);

    cout << "Selamat Datang " << foundUserData["nama"] << endl;
    cout << "Masukkan nama barang yang ingin digadaikan : ";
    cin.ignore();
    getline(cin, namaBarang);
    cout << "Masukkan harga barang : ";
    cin >> hargaBarang;
    cout << "Masukkan berat barang (kg) : ";
    cin >> beratBarang;
    cout << "Masukkan tanggal gadai (format mm/yyyy, contoh: 06/2025) : ";
    cin >> tanggalGadai;
    cout << "Masukkan jangka waktu gadai (bulan) : ";
    cin >> jangkaWaktu;
    
    tambahBarang["idUser"] = idUser;
    tambahBarang["namaBarang"] = namaBarang;
    tambahBarang["hargaBarang"] = to_string(hargaBarang);
    tambahBarang["beratBarang"] = to_string(beratBarang);
    tambahBarang["statusBarang"] = "Berlangsung"; // Default status untuk pengajuan baru
    
    barang.writeData(tambahBarang);
    cout << "✓ Data barang berhasil disimpan!" << endl;
    
    vector<unordered_map<string, string>> dataBarang;
    barang.getData(dataBarang);
    
    string idBarangBaru = "";
    for (int i = dataBarang.size() - 1; i >= 0; i--) {
        if (dataBarang[i]["namaBarang"] == namaBarang && 
            dataBarang[i]["idUser"] == idUser &&
            dataBarang[i]["hargaBarang"] == to_string(hargaBarang)) {
            idBarangBaru = dataBarang[i]["id"];
            break;
        }
    }
    
    if (idBarangBaru.empty()) {
        cout << "Error: Gagal menemukan data barang yang baru ditambahkan!" << endl;
        return;
    }
    
    tambahTransaksi["idBarang"] = idBarangBaru;
    tambahTransaksi["idUser"] = idUser;
    tambahTransaksi["tanggalGadai"] = tanggalGadai;
    tambahTransaksi["jenisTransaksi"] = "Pengajuan"; // Default untuk nasabah
    tambahTransaksi["totalHarga"] = to_string(taksiranHarga(hargaBarang, beratBarang));
    
    transaksi.writeData(tambahTransaksi);
    cout << "✓ Data transaksi berhasil disimpan!" << endl;
    
    vector<unordered_map<string, string>> dataTransaksi;
    transaksi.getData(dataTransaksi);
    
    string idTransaksiBaru = "";
    for (int i = dataTransaksi.size() - 1; i >= 0; i--) {
        if (dataTransaksi[i]["idBarang"] == idBarangBaru && 
            dataTransaksi[i]["idUser"] == idUser &&
            dataTransaksi[i]["jenisTransaksi"] == "Pengajuan") {
            idTransaksiBaru = dataTransaksi[i]["id"];
            break;
        }
    }
    
    tambahPenitipan["idBarang"] = idBarangBaru;
    tambahPenitipan["tanggalGadai"] = tanggalGadai;
    tambahPenitipan["jatuhTempo"] = jatuhTempo(tanggalGadai, jangkaWaktu);
    
    penitipan.writeData(tambahPenitipan);
    cout << "✓ Data penitipan berhasil disimpan!" << endl;
    
    cout << "\n=== RINGKASAN PENGAJUAN GADAI ===" << endl;
    cout << "ID Transaksi    : " << idTransaksiBaru << endl;
    cout << "ID Barang       : " << idBarangBaru << endl;
    cout << "Nama Barang     : " << namaBarang << endl;
    cout << "Harga Barang    : Rp " << hargaBarang << endl;
    cout << "Berat Barang    : " << beratBarang << " kg" << endl;
    cout << "Taksiran Harga  : Rp " << taksiranHarga(hargaBarang, beratBarang) << endl;
    cout << "Tanggal Gadai   : " << tanggalGadai << endl;
    cout << "Jangka Waktu    : " << jangkaWaktu << " bulan" << endl;
    cout << "Jatuh Tempo     : " << jatuhTempo(tanggalGadai, jangkaWaktu) << endl;
    cout << "Status          : Menunggu Persetujuan Admin" << endl;
    cout << "\nPengajuan gadai berhasil disimpan ke semua database!" << endl;
}

void statusGadai() {
    int idTransaksi;
    cout << "=== STATUS GADAI BERDASARKAN ID TRANSAKSI ===" << endl;
    cout << "Masukkan ID Transaksi : ";
    cin >> idTransaksi;
    
    MyDatabase transaksi("../data/transaksi.csv");
    MyDatabase barang("../data/barang.csv");
    MyDatabase penitipan("../data/penitipan.csv");
    
    vector<unordered_map<string, string>> dataTransaksi;
    vector<unordered_map<string, string>> dataBarang;
    vector<unordered_map<string, string>> dataPenitipan;
    
    transaksi.getData(dataTransaksi);
    barang.getData(dataBarang);
    penitipan.getData(dataPenitipan);
    
    string idCari = to_string(idTransaksi);
    bool ditemukan = false;
    
    for (int i = 0; i < dataTransaksi.size(); i++) {
        if (dataTransaksi[i]["id"] == idCari) {
            ditemukan = true;
            string idBarangCari = dataTransaksi[i]["idBarang"];
            
            unordered_map<string, string> barangData = cariBarangById(dataBarang, idBarangCari);
            
            if (!barangData.empty()) {
                cout << "\n=== DETAIL TRANSAKSI GADAI ===" << endl;
                cout << "ID Transaksi        : " << dataTransaksi[i]["id"] << endl;
                cout << "ID Nasabah          : " << dataTransaksi[i]["idUser"] << endl;
                cout << "ID Barang           : " << dataTransaksi[i]["idBarang"] << endl;
                
                cout << "\n--- INFORMASI BARANG ---" << endl;
                cout << "Nama Barang         : " << barangData["namaBarang"] << endl;
                cout << "Harga Barang        : Rp " << barangData["hargaBarang"] << endl;
                cout << "Berat Barang        : " << barangData["beratBarang"] << " kg" << endl;
                cout << "Status Barang       : " << barangData["statusBarang"] << endl;
                
                cout << "\n--- INFORMASI TRANSAKSI ---" << endl;
                cout << "Jenis Transaksi     : " << dataTransaksi[i]["jenisTransaksi"] << endl;
                cout << "Tanggal Gadai       : " << dataTransaksi[i]["tanggalGadai"] << endl;
                cout << "Total Harga Gadai   : Rp " << dataTransaksi[i]["totalHarga"] << endl;
                
                string statusPengajuan;
                if (dataTransaksi[i]["jenisTransaksi"] == "Pengajuan") {
                    if (barangData["statusBarang"] == "Berlangsung") {
                        statusPengajuan = "DISETUJUI - Sedang Berlangsung";
                    } else if (barangData["statusBarang"] == "Tertebus") {
                        statusPengajuan = "SELESAI - Barang Telah Ditebus";
                    } else {
                        statusPengajuan = "MENUNGGU PERSETUJUAN";
                    }
                } else if (dataTransaksi[i]["jenisTransaksi"] == "Pelunasan") {
                    statusPengajuan = "PELUNASAN - Proses Pembayaran";
                } else {
                    statusPengajuan = "STATUS TIDAK DIKETAHUI";
                }
                cout << "Status Pengajuan    : " << statusPengajuan << endl;
                
                cout << "\n--- INFORMASI PENITIPAN ---" << endl;
                bool penitipanDitemukan = false;
                for (int k = 0; k < dataPenitipan.size(); k++) {
                    if (dataPenitipan[k]["idBarang"] == idBarangCari) {
                        cout << "Tanggal Mulai Gadai : " << dataPenitipan[k]["tanggalGadai"] << endl;
                        cout << "Jatuh Tempo         : " << dataPenitipan[k]["jatuhTempo"] << endl;
                        
                        string jatuhTempo = dataPenitipan[k]["jatuhTempo"];
                        string tanggalSekarang = dataPenitipan[k]["tanggalGadai"]; // Simulasi
                        cout << "Keterangan          : ";
                        if (barangData["statusBarang"] == "Berlangsung") {
                            cout << "Masih dalam masa gadai" << endl;
                        } else if (barangData["statusBarang"] == "Tertebus") {
                            cout << "Barang telah ditebus sebelum jatuh tempo" << endl;
                        }
                        penitipanDitemukan = true;
                        break;
                    }
                }
                
                if (!penitipanDitemukan) {
                    cout << "Data penitipan tidak ditemukan" << endl;
                }
                
                cout << "\n" << string(50, '=') << endl;
                
            } else {
                cout << "Error: Data barang tidak ditemukan untuk ID Barang: " << idBarangCari << endl;
            }
            break;
        }
    }
    
    if (!ditemukan) {
        cout << "\nTransaksi dengan ID " << idTransaksi << " tidak ditemukan!" << endl;
        cout << "Pastikan ID transaksi yang Anda masukkan benar." << endl;
    }
}

void riwayatGadai() {
    int iduser;
    cout << "=== RIWAYAT GADAI ===" << endl;
    cout << "Masukkan ID Nasabah anda : ";
    cin >> iduser;
    
    MyDatabase transaksi("../data/transaksi.csv");
    MyDatabase barang("../data/barang.csv");
    MyDatabase penitipan("../data/penitipan.csv");
    
    vector<unordered_map<string, string>> dataTransaksi;
    vector<unordered_map<string, string>> dataBarang;
    vector<unordered_map<string, string>> dataPenitipan;
    
    transaksi.getData(dataTransaksi);
    barang.getData(dataBarang);
    penitipan.getData(dataPenitipan);
    
    string idCari = to_string(iduser);
    bool ditemukan = false;
    
    cout << "\n=== RIWAYAT LENGKAP GADAI ANDA ===" << endl;
    
    for (int i = 0; i < dataTransaksi.size(); i++) {
        if (dataTransaksi[i]["idUser"] == idCari) {
            ditemukan = true;
            string idBarangCari = dataTransaksi[i]["idBarang"];
            
            unordered_map<string, string> barangData = cariBarangById(dataBarang, idBarangCari);
            
            if (!barangData.empty()) {
                cout << "\n--- Riwayat " << (i + 1) << " ---" << endl;
                cout << "ID Transaksi    : " << dataTransaksi[i]["id"] << endl;
                cout << "Nama Barang     : " << barangData["namaBarang"] << endl;
                cout << "Harga Barang    : Rp " << barangData["hargaBarang"] << endl;
                cout << "Berat Barang    : " << barangData["beratBarang"] << " kg" << endl;
                cout << "Status Barang   : " << barangData["statusBarang"] << endl;
                cout << "Jenis Transaksi : " << dataTransaksi[i]["jenisTransaksi"] << endl;
                cout << "Tanggal Gadai   : " << dataTransaksi[i]["tanggalGadai"] << endl;
                cout << "Total Harga     : Rp " << dataTransaksi[i]["totalHarga"] << endl;
                
                for (int k = 0; k < dataPenitipan.size(); k++) {
                    if (dataPenitipan[k]["idBarang"] == idBarangCari) {
                        cout << "Jatuh Tempo     : " << dataPenitipan[k]["jatuhTempo"] << endl;
                        break;
                    }
                }
                cout << "--------------------------------" << endl;
            }
        }
    }
    
    if (!ditemukan) {
        cout << "Tidak ada riwayat gadai untuk ID Nasabah: " << iduser << endl;
    }
}

// Keperluan Testing
// int main(){
//     ajukanGadai();
//     statusGadai();
//     riwayatGadai();
//     return 0;
// }