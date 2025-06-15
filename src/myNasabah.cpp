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
        return hargaBarang * 0.7;
    } else if (beratBarang <= 10) {
        return hargaBarang * 0.75;
    } else if (beratBarang <= 20) {
        return hargaBarang * 0.8;
    } else if (beratBarang <= 50) {
        return hargaBarang * 0.85;
    } else {
        return hargaBarang * 0.9;
    }
}

void knapsack(const vector<string>& namaBarang, const vector<string>& beratBarang, const vector<string>& hargaBarang, int kapasitas) {
    int n = namaBarang.size();
    vector<int> berat(n), harga(n);

    // Konversi string ke int
    for (int i = 0; i < n; ++i) {
        berat[i] = stoi(beratBarang[i]);
        harga[i] = stoi(hargaBarang[i]);
    }

    vector<vector<int>> dp(n + 1, vector<int>(kapasitas + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= kapasitas; ++w) {
            if (berat[i - 1] <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - berat[i - 1]] + harga[i - 1]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    cout << "=== HASIL Rekomendasi ===" << endl;
    cout << "\nTotal harga maksimal yang bisa diperoleh : Rp " << dp[n][kapasitas] << endl;

    int w = kapasitas;
    cout << "Barang yang dipilih :\n";
    for (int i = n; i > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            cout << "- " << namaBarang[i - 1]
                 << " (berat: " << berat[i - 1]
                 << " kg, Harga : Rp " << harga[i - 1] << ")\n";
            w -= berat[i - 1];
        }
    }
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
    MyDatabase barang("data/barang.csv");
    MyDatabase transaksi("data/transaksi.csv");
    MyDatabase penitipan("data/penitipan.csv");
    
    unordered_map<string, string> tambahBarang;
    unordered_map<string, string> tambahTransaksi;
    unordered_map<string, string> tambahPenitipan;
    
    string namaBarang, tanggalGadai;
    int hargaBarang, beratBarang, jangkaWaktu;

    cout << "Sebelum anda mengajukan gadai, apakah anda ingin melihat rekomendasi barang untuk gadai ? (y/n) : ";
    char pilihan;
    cin >> pilihan;

    if (pilihan == 'y' || pilihan == 'Y') {
        cout << "=== REKOMENDASI BARANG ===" << endl;
        cout << "Masukkan jumlah barang yang ingin digadaikan : ";

        vector<string> Barang, Harga, Berat;
        int jumlahBarang;
        cin >> jumlahBarang;

        string barang, berat, harga;
        for (int i = 0; i < jumlahBarang; i++) {
            cout << "Masukkan nama barang ke-" << i + 1 << " : ";
            cin.ignore();
            getline(cin, barang);
            cout << "Masukkan harga barang ke-" << i + 1 << " (Rp) : ";
            cin >> harga;
            cout << "Masukkan berat barang ke-" << i + 1 << " (Kg) : ";
            cin >> berat;

            Barang.push_back(barang);
            Harga.push_back(to_string(taksiranHarga(stoi(harga), stoi(berat))));
            Berat.push_back(berat);
        }
        knapsack(Barang, Berat, Harga, 50);
    }

    cout << "\n=== AJUKAN GADAI ===" << endl;
    MyDatabase MyUser("data/user.csv");
    vector<unordered_map<string, string>> dataUser;
    MyUser.getData(dataUser);

    bool userDitemukan = false;
    unordered_map<string, string> foundUserData;

    // do {
    //     cout << "Masukkan ID nasabah : ";
    //     cin >> idUser;

    //     userDitemukan = false;
    //     for (const auto& userData : dataUser) {
    //         if (userData.at("id") == idUser) {
    //             userDitemukan = true;
    //             foundUserData = userData;
    //             break;
    //         }
    //     }

    //     if (!userDitemukan) {
    //         cout << "Error: User dengan ID " << idUser << " tidak ditemukan!" << endl;
    //     }
    // } while (!userDitemukan);

    
    cout << "Selamat Datang " << user.nama << endl;
    
    int jumlahBarang;
    cout << "Masukkan jumlah barang yang ingin digadaikan : ";
    cin >> jumlahBarang;
    for (int i = 0; i < jumlahBarang; i++) {
        cout << "Barang ke-" << i + 1 << endl;
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
        
        tambahBarang["idUser"] = user.id;
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
                dataBarang[i]["idUser"] == user.id &&
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
        tambahTransaksi["idUser"] = user.id;
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
                dataTransaksi[i]["idUser"] == user.id &&
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
        
        cout << "\n=== RINGKASAN PENGAJUAN GADAI BARANG KE " << i + 1 << " ===" << endl;
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
}

void statusGadai() {
    int idTransaksi;
    cout << "=== STATUS GADAI BERDASARKAN ID TRANSAKSI ===" << endl;
    cout << "Masukkan ID Transaksi : ";
    cin >> idTransaksi;
    
    MyDatabase transaksi("data/transaksi.csv");
    MyDatabase barang("data/barang.csv");
    MyDatabase penitipan("data/penitipan.csv");
    
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
    
    MyDatabase transaksi("data/transaksi.csv");
    MyDatabase barang("data/barang.csv");
    MyDatabase penitipan("data/penitipan.csv");
    
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