#include <iostream>
#include <vector>
#include <unordered_map>
#include "../include/lib.h"
#include "../include/database.h"
#include "../include/user.h"
#include "../include/menu.h"

using namespace std;

int taksiranHarga(const string& hargaBarang, const string& beratBarang) {
    if (stoi(beratBarang) < 5) {
        return stoi(hargaBarang) * 0.7;
    } else if (stoi(beratBarang) <= 10) {
        return stoi(hargaBarang) * 0.75;
    } else if (stoi(beratBarang) <= 20) {
        return stoi(hargaBarang) * 0.8;
    } else if (stoi(beratBarang) <= 50) {
        return stoi(hargaBarang) * 0.85;
    } else {
        return stoi(hargaBarang) * 0.9;
    }
}

void tambahDataGadai() {
    MyDatabase barang("data/barang.csv");
    MyDatabase transaksi("data/transaksi.csv");
    MyDatabase penitipan("data/penitipan.csv");
    MyDatabase nasabah("data/user.csv");
    
    unordered_map<string, string> tambahBarang;
    unordered_map<string, string> tambahTransaksi;
    unordered_map<string, string> tambahPenitipan;
    vector<unordered_map<string, string>> dataNasabah;
    int statusBarang, jenisTransaksi, jangkaWaktu;
    
    cout << "==== Tambah Data Gadai ====\n";
    unordered_map<string, string> foundUserData;
    nasabah.getData(dataNasabah);
    string id;
    bool found = false;

    do {
        cout << "Silahkan masukkan ID nasabah : ";
        cin >> id;
        for (const auto& dataUser : dataNasabah) {
            if (dataUser.at("id") == id) {
                found = true;
                foundUserData = dataUser;
                break;
            }
        }

        if (found) {
            cout << "Apakah anda ingin menambahkan data nasabah dengan ID " << id << " dan nama " << foundUserData.at("nama") << "? (y/n) : ";
            char pilihan;
            cin >> pilihan;
            if (pilihan == 'n' || pilihan == 'N') {
                break;
            }
        }

        if (!found) {
            cout << "Nasabah dengan ID " << id << " tidak ditemukan" << endl;
            cout << "Apakah anda ingin tetap menambahkan data? (y/n) : ";
            char pilihan;
            cin >> pilihan;
            if (pilihan == 'y' || pilihan == 'Y') {
                found = true;
            }
        }
        
    } while (!found);

    tambahBarang["idUser"] = id;
    cout << "Silahkan masukkan nama barang yang akan digadaikan : ";
    cin.ignore();
    getline(cin, tambahBarang["namaBarang"]);
    cout << "Silahkan masukkan harga barang : ";
    cin >> tambahBarang["hargaBarang"];
    cout << "Silahkan masukkan berat barang (kg) : ";
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

    setWarnaFont(green);
    cout << "\n✓ Data berhasil ditambahkan\n" << endl;
    setWarnaFont(reset);

    cout << "====Rincian Data Gadai====\n";
    cout << "ID Nasabah      : " << tambahBarang["idUser"] << endl;
    cout << "Nama Nasabah    : " << foundUserData.at("nama") << endl;
    cout << "ID Barang       : " << dataBarang[indeks]["id"] << endl;
    cout << "Nama Barang     : " << tambahBarang["namaBarang"] << endl;
    cout << "Harga Barang    : " << "Rp " << tambahBarang["hargaBarang"] << endl;
    cout << "Berat Barang    : " << tambahBarang["beratBarang"] << " Kg" << endl;
    cout << "Status Barang   : " << tambahBarang["statusBarang"] << endl;
    cout << "Jenis Transaksi : " << tambahTransaksi["jenisTransaksi"] << endl;
    cout << "Taksiran Harga  : " << "Rp " << tambahTransaksi["totalHarga"] << endl;
    cout << "Tanggal Gadai   : " << tambahTransaksi["tanggalGadai"] << endl;
    cout << "Jangka Waktu    : " << jangkaWaktu << " Bulan" << endl;
    cout << "Jatuh Tempo     : " << tambahPenitipan["jatuhTempo"] << endl;

    cout << "\nApakah anda ingin menambahkan data lagi? (y/n) : ";
    char pilihan;
    cin >> pilihan;
    if (pilihan == 'y' || pilihan == 'Y') {
        tambahDataGadai();
    }
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
    MyDatabase barang("data/barang.csv");
    MyDatabase penitipan("data/penitipan.csv");
    MyDatabase transaksi("data/transaksi.csv");
    MyDatabase nasabah("data/user.csv");
    
    vector<unordered_map<string, string>> dataBarang;
    vector<unordered_map<string, string>> dataPenitipan;
    vector<unordered_map<string, string>> dataTransaksi;
    vector<unordered_map<string, string>> dataNasabah;

    barang.getData(dataBarang);
    penitipan.getData(dataPenitipan);
    transaksi.getData(dataTransaksi);
    nasabah.getData(dataNasabah);

    cout << "====== CARI DATA GADAI ======" << endl;
    cout << "1. Cari berdasarkan ID Barang" << endl;
    cout << "2. Cari berdasarkan Tanggal Transaksi" << endl;
    cout << "3. Cari berdasarkan ID Nasabah" << endl;
    cout << "4. Cari berdasarkan Nama Nasabah" << endl;
    cout << "Masukkan pilihan anda : ";
    int pilihan;
    cin >> pilihan;
    
    vector<unordered_map<string, string>> barangData;
    vector<unordered_map<string, string>> penitipanData;
    vector<unordered_map<string, string>> transaksiData;
    unordered_map<string, string> nasabahData;
    
    if (pilihan == 1) {
        barangData.clear();
        penitipanData.clear();
        transaksiData.clear();

        string idBarang;

        do {
            cout << "\nMasukkan ID Barang : ";
            cin >> idBarang;

            for (const auto& data : dataBarang) {
                if (data.at("id") == idBarang) {
                    barangData.push_back(data);
                    break;
                }
            }
            
            for (const auto& data : dataPenitipan) {
                if (data.at("idBarang") == idBarang) {
                    penitipanData.push_back(data);
                    break;
                }
            }
            
            for (const auto& data : dataTransaksi) {
                if (data.at("idBarang") == idBarang) {
                    transaksiData.push_back(data);
                    break;
                }
            }
            
            if (barangData.empty() || penitipanData.empty() || transaksiData.empty()) {
                setWarnaFont(red);
                cout << "\n" << "ID Barang " << idBarang << " tidak ditemukan" << "\n";
                setWarnaFont(reset);
            } else {
                for (const auto& data : dataNasabah) {
                    if (data.at("id") == barangData.at(0).at("idUser")) {
                        nasabahData = data;
                        break;
                    }
                }
            }
        } while(barangData.empty() || penitipanData.empty() || transaksiData.empty());

        cout << "\nBerikut adalah data barang dengan ID " << idBarang << " :";
        for (int i = 0; i < barangData.size(); i++) {
            cetakGaris(40, yellow, '-');
            cout << "ID Transaksi        : " << transaksiData.at(i).at("id") << endl;
            cout << "Nama Barang         : " << barangData.at(i).at("namaBarang") << endl;
            cout << "Berat Barang        : " << barangData.at(i).at("beratBarang") << " Kg"<< endl;
            cout << "Harga Barang        : " << "Rp "<< barangData.at(i).at("hargaBarang") << endl;
            cout << "Harga Taksiran      : " << "Rp " << transaksiData.at(i).at("totalHarga") << endl;
            cout << "Status Gadai Barang : " << barangData.at(i).at("statusBarang") << endl;
            cout << "Jenis Transaksi     : " << transaksiData.at(i).at("jenisTransaksi") << endl;
            cout << "Tanggal Gadai       : " << transaksiData.at(i).at("tanggalGadai") << endl;
            cout << "Jatuh Tempo         : " << penitipanData.at(i).at("jatuhTempo") << endl;
            setWarnaFont(blue);
            cout << "ID Nasabah          : " << nasabahData.at("id") << endl;
            cout << "Nama Nasabah        : " << nasabahData.at("nama");
            setWarnaFont(reset);
            cetakGaris(40, yellow, '=');
        }

    } else if (pilihan == 2) {
        barangData.clear();
        penitipanData.clear();
        transaksiData.clear();

        string tanggalTransaksi;

        do {
            cout << "\nMasukkan Tanggal Transaksi (format: mm/yyyy, ex: 09/2023) : ";
            cin >> tanggalTransaksi;

            for (const auto& data : dataTransaksi) {
                if (data.at("tanggalGadai") == tanggalTransaksi) {
                    transaksiData.push_back(data);
                }
            }

            for (const auto& data : dataBarang) {
                for (int i = 0; i < transaksiData.size(); i++) {
                    if (data.at("id") == transaksiData.at(i).at("idBarang")) {
                        barangData.push_back(data);
                    }
                }
            }
                
            for (const auto& data : dataPenitipan) {
                for (int i = 0; i < barangData.size(); i++) {
                    if (data.at("idBarang") == barangData.at(i).at("id")) {
                        penitipanData.push_back(data);
                    }
                }
            }

            if (barangData.empty() || penitipanData.empty() || transaksiData.empty()) {
                setWarnaFont(red);
                cout << "\n" << "Tidak ada transaksi pada " << tanggalTransaksi << " tidak ditemukan" << "\n";
                setWarnaFont(reset);
            } else {
                for (const auto& data : dataNasabah) {
                    if (data.at("id") == barangData.at(0).at("idUser")) {
                        nasabahData = data;
                        break;
                    }
                }
            }
        } while (transaksiData.empty() || barangData.empty() || penitipanData.empty());

        cout << "\nBerikut adalah data barang pada " << tanggalTransaksi << " :";
        for (int i = 0; i < barangData.size(); i++) {
            cetakGaris(40, yellow, '-');
            cout << "ID Transaksi        : " << transaksiData.at(i).at("id") << endl;
            cout << "Nama Barang         : " << barangData.at(i).at("namaBarang") << endl;
            cout << "Berat Barang        : " << barangData.at(i).at("beratBarang") << " Kg" << endl;
            cout << "Harga Barang        : " << "Rp " << barangData.at(i).at("hargaBarang") << endl;
            cout << "Harga Taksiran      : " << "Rp " << transaksiData.at(i).at("totalHarga") << endl;
            cout << "Status Gadai Barang : " << barangData.at(i).at("statusBarang") << endl;
            cout << "Jenis Transaksi     : " << transaksiData.at(i).at("jenisTransaksi") << endl;
            cout << "Tanggal Gadai       : " << transaksiData.at(i).at("tanggalGadai") << endl;
            cout << "Jatuh Tempo         : " << penitipanData.at(i).at("jatuhTempo") << endl;
            setWarnaFont(blue);
            cout << "ID Nasabah          : " << nasabahData.at("id") << endl;
            cout << "Nama Nasabah        : " << nasabahData.at("nama");
            setWarnaFont(reset);
            cetakGaris(40, yellow, '=');
        }

    } else if (pilihan == 3) {
        barangData.clear();
        penitipanData.clear();
        transaksiData.clear();
        
        string idNasabah;
        
        do {
            cout << "\nMasukkan ID Nasabah : ";
            cin >> idNasabah;

            for (const auto& data : dataNasabah) {
                if (data.at("id") == idNasabah) {
                    nasabahData = data;
                    break;
                }
            }

            if(nasabahData.empty()) {
                setWarnaFont(red);
                cout << "\n" << "ID Nasabah tidak ditemukan" << "\n";
                setWarnaFont(reset);
                continue;
            }

            for (const auto& data : dataBarang) {
                if (data.at("idUser") == nasabahData.at("id")) {
                    barangData.push_back(data);
                }
            }

            for (const auto& data : dataPenitipan) {
                for (int i = 0; i < barangData.size(); i++) {
                    if (data.at("idBarang") == barangData.at(i).at("id")) {
                        penitipanData.push_back(data);
                    }
                }
            }

            for (const auto& data : dataTransaksi) {
                for (int i = 0; i < barangData.size(); i++) {
                    if (data.at("idBarang") == barangData.at(i).at("id")) {
                        transaksiData.push_back(data);
                    }
                }
            }

            if (barangData.empty() || penitipanData.empty() || transaksiData.empty()) {
                setWarnaFont(red);
                cout << "\n" << "Nasabah dengan ID " << idNasabah << " Belum melakukan transaksi" << "\n";
                setWarnaFont(reset);
            }
        } while (nasabahData.empty() || barangData.empty() || penitipanData.empty() || transaksiData.empty());

        cout << "\nBerikut adalah data barang nasabah dengan ID " << idNasabah << " :";
        for (int i = 0; i < barangData.size(); i++) {
            cetakGaris(40, yellow, '-');
            cout << "ID Transaksi        : " << transaksiData.at(i).at("id") << endl;
            cout << "Nama Barang         : " << barangData.at(i).at("namaBarang") << endl;
            cout << "Berat Barang        : " << barangData.at(i).at("beratBarang") << " Kg" << endl;
            cout << "Harga Barang        : " << "Rp " << barangData.at(i).at("hargaBarang") << endl;
            cout << "Harga Taksiran      : " << "Rp " << transaksiData.at(i).at("totalHarga") << endl;
            cout << "Status Gadai Barang : " << barangData.at(i).at("statusBarang") << endl;
            cout << "Jenis Transaksi     : " << transaksiData.at(i).at("jenisTransaksi") << endl;
            cout << "Tanggal Gadai       : " << transaksiData.at(i).at("tanggalGadai") << endl;
            cout << "Jatuh Tempo         : " << penitipanData.at(i).at("jatuhTempo") << endl;
            setWarnaFont(blue);
            cout << "ID Nasabah          : " << nasabahData.at("id") << endl;
            cout << "Nama Nasabah        : " << nasabahData.at("nama");
            setWarnaFont(reset);
            cetakGaris(40, yellow, '=');
        }

    } else if (pilihan == 4) {
        string namaNasabah;
        do {
            cout << "\nMasukkan Nama Nasabah : ";
            cin >> namaNasabah;

            for (const auto& data : dataNasabah) {
                if (data.at("nama") == namaNasabah) {
                    nasabahData = data;
                    break;
                }
            }

            if(nasabahData.empty()) {
                setWarnaFont(red);
                cout << "\n" << "Nama Nasabah tidak ditemukan" << "\n";
                setWarnaFont(reset);
                continue;
            }

            for (const auto& data : dataBarang) {
                if (data.at("idUser") == nasabahData.at("id")) {
                    barangData.push_back(data);
                }
            }

            for (const auto& data : dataPenitipan) {
                for (int i = 0; i < barangData.size(); i++) {
                    if (data.at("idBarang") == barangData.at(i).at("id")) {
                        penitipanData.push_back(data);
                    }
                }
            }

            for (const auto& data : dataTransaksi) {
                for (int i = 0; i < barangData.size(); i++) {
                    if (data.at("idBarang") == barangData.at(i).at("id")) {
                        transaksiData.push_back(data);
                    }
                }
            }

            if (barangData.empty() || penitipanData.empty() || transaksiData.empty()) {
                setWarnaFont(red);
                cout << "\n" << "Nasabah dengan ID " << namaNasabah << " Belum melakukan transaksi" << "\n";
                setWarnaFont(reset);
            }
        } while (nasabahData.empty() || barangData.empty() || penitipanData.empty() || transaksiData.empty());

        cout << "\nBerikut adalah data barang nasabah dengan ID " << namaNasabah << " :";
        for (int i = 0; i < barangData.size(); i++) {
            cetakGaris(40, yellow, '-');
            cout << "ID Transaksi        : " << transaksiData.at(i).at("id") << endl;
            cout << "Nama Barang         : " << barangData.at(i).at("namaBarang") << endl;
            cout << "Berat Barang        : " << barangData.at(i).at("beratBarang") << " Kg" << endl;
            cout << "Harga Barang        : " << "Rp " << barangData.at(i).at("hargaBarang") << endl;
            cout << "Harga Taksiran      : " << "Rp " << transaksiData.at(i).at("totalHarga") << endl;
            cout << "Status Gadai Barang : " << barangData.at(i).at("statusBarang") << endl;
            cout << "Jenis Transaksi     : " << transaksiData.at(i).at("jenisTransaksi") << endl;
            cout << "Tanggal Gadai       : " << transaksiData.at(i).at("tanggalGadai") << endl;
            cout << "Jatuh Tempo         : " << penitipanData.at(i).at("jatuhTempo") << endl;
            setWarnaFont(blue);
            cout << "ID Nasabah          : " << nasabahData.at("id") << endl;
            cout << "Nama Nasabah        : " << nasabahData.at("nama");
            setWarnaFont(reset);
            cetakGaris(40, yellow, '=');
        }



    } else {
        setWarnaFont(red);
        cout << "\n" << "Pilihan tidak tersedia" << "\n";
        setWarnaFont(reset);
        return;
    }
}

void ubahStatusGadai() {
    
}

void hapusDataGadai() {
    
}

