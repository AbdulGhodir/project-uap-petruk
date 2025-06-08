#include "../include/database.h"
#include "../include/user.h"
#include "../include/lib.h"
#include <fstream>
#include <sstream>
#include <iostream>

MyDatabase::MyDatabase(std::string namaFile) {
    this->namaFile = namaFile;
    this->reload();
}

void MyDatabase::reload() {
    try {
        std::ifstream myFile(this->namaFile);
        if(myFile.is_open()) {
            this->data.clear();
            this->header.clear();
            std::string lineOne;
            
            // MENGAMBIL HEADER SEBAGAI KEY
            if (getline(myFile, lineOne))
            {
                std::stringstream ss(lineOne);
                std::string kolom;
                while (getline(ss, kolom, ','))
                {
                    this->header.push_back(kolom);
                }
            }
            
            std::string line;
            while (getline(myFile, line))
            {
                std::stringstream ss(line);
                std::unordered_map<std::string, std::string> baris;
                std::string kolom;
                int iterasi = 0;

                while (getline(ss, kolom, ','))
                {
                    baris[this->header.at(iterasi)] = kolom;
                    iterasi++;
                }
                this->data.push_back(baris);
            }
            myFile.close();
        } else {
            throw 404;
        }
    } catch (int errorCode) {
        switch (errorCode)
        {
        case 404:
            std::cout << "\n" << "File tidak ditemukan!!" << "\n";
            break;
        default:
            std::cout << "\n" << "Ada sesuatu yang error!!" << "\n";
            break;
        }
    }
}

bool MyDatabase::is_empty() {
    return this->data.size() == 0;
}

bool MyDatabase::auth(const std::string& email, const std::string& password) {
    try {
        // CEK NAMA FILE HARUS USER.CSV
        if(this->namaFile != "data/user.csv") throw 505;
        
        // CEK EMAIL DAN PASSWORD
        this->reload();
        bool notfind = true;
        for(const auto& row: this->data) {
            if(row.at("email") == email && row.at("password") == password) {
                user.id = row.at("id");
                user.nama = row.at("nama");
                user.email = row.at("email");
                user.noTelp = row.at("noTelp");
                return true;
            }
        }

        // CEK KLO ENGGAK ADA LEMPAR 404
        if(notfind) throw 404;
        
    } catch(int errorCode) {
        switch (errorCode)
        {
        case 404:
            return false;
            break;
        default:
            setWarnaFont(red);
            std::cout << "\n" << "Method ini khusus user.csv!!" << "\n";
            setWarnaFont(reset);
            return false;
            break;
        }
    }
}

MyDatabase& MyDatabase::getData(std::vector<std::unordered_map<std::string, std::string>>& data) {
    this->reload();
    for(const auto& row: this->data) {
        std::unordered_map<std::string, std::string> baris;
        for(const auto& column: row) {
           baris[column.first] = column.second;
        }
        data.push_back(baris);
    }
}

bool MyDatabase::writeData(const std::unordered_map<std::string, std::string>& data) {
    try {
        std::ofstream myFile(this->namaFile, std::ios::app);
        if(myFile.is_open()) {
            myFile << "\n";
            myFile << this->data.size() + 1;
            myFile << ','; 
            int i = 0;
            for(const auto& row: this->header) {
                if(row != "id") {
                    myFile << data.at(row);
                    if(i != data.size() - 1) {
                        myFile << ',';
                    }
                    i++;
                }
            }
            myFile.close();
        }
        return true;
    } catch(...) {
        setWarnaFont(red);
        std::cout << "\n" << "Key index tidak ada dalam file database!" << "\n";
        setWarnaFont(reset);
        return false;
    }
}

MyDatabase& MyDatabase::readData() {   
    this->reload();
    std::cout << "Data Size: " << this->data.size() << "\n"
              << "Jenis Data: " << this->namaFile << "\n";
    if(!this->is_empty()) {
        for(const auto& row: this->data) {
            cetakGaris(40);
            for(int index = 0 ; index < row.size(); index++) {
                std::cout << this->header.at(index) << ": " << row.at(this->header.at(index));
                if(index != row.size() - 1) {
                    std::cout << "\n";
                } 
            }
            cetakGaris(40);
        }
    } else {
        setWarnaFont(red);
        std::cout <<"Data masih kosong!!" << "\n";
        setWarnaFont(reset);
    }
}

MyDatabase& MyDatabase::updateData(const std::string id,const std::unordered_map<std::string, std::string>& data) {
    try {
        // AMBIL DATA DARI DATABASE FILE KE DATA VECTOR
        this->reload();

        // UBAH DATA BERDASARKAN ID
        bool notfind = true;
        for(auto& row: this->data) {
            if(row.at("id") == id) {
                for(const auto& kolom: data) {
                    row.at(kolom.first) = kolom.second;
                }
                notfind = false;
            }
        }

        // CEK ID ADA ATAU TIDAK
        if(notfind) throw 404;
        
        // SIMPAN DATA KE FILE DATABASE
        std::ofstream myFile(this->namaFile);
        if(myFile.is_open()) {
            int kolom, baris;
            kolom = baris = 0;
            for(const auto& header: this->header) {
                myFile << header;
                if(kolom != this->header.size() - 1) {
                    myFile << ',';
                }
                kolom++;
            }
            myFile << "\n";

            for(const auto& row: this->data) {
                for(int index = 0; index < row.size(); index++) {
                    myFile << row.at(this->header.at(index));
                    if(index != row.size() - 1) {
                        myFile << ',';
                    } 
                }
                if(baris != this->data.size() - 1) {
                    myFile << "\n";
                }
                baris++;
            }
            myFile.close();
        }

    } catch(int errorCode) {
        switch (errorCode)
        {
        case 404:
            setWarnaFont(red);
            std::cout << "\n" << "ID "<< id << " tidak ditemukan" <<"\n";
            setWarnaFont(reset);
            break;
        
        default:
            setWarnaFont(red);
            std::cout << "\n" << "Ada kesalahan program" << "\n"; 
            setWarnaFont(reset);
            break;
        }
    }
}

MyDatabase& MyDatabase::deleteData(const std::string id) {
    try {
        // RELOAD DATA DARI FILE DATABASE CSV
        this->reload();
        bool notfind = true;
        
        // MENGHAPUS DATA BERDASARKAN ID
        std::vector<std::unordered_map<std::string, std::string>>::iterator it;
        for(it = this->data.begin(); it != this->data.end();) {
            if((*it).at("id") == id) {
                this->data.erase(it);
                notfind = false;
            } else {
                ++it;
            }
        }

        // CEK DATA DITEMUKAN ATAU TIDAK
        if(notfind) throw 404;

        // RESET ID
        for(int index = 0; index < this->data.size(); index++) {
            this->data.at(index).at("id") = std::to_string(index + 1);
        }
        
        // SIMPAN DATA KE FILE DATABASE
        std::ofstream myFile(this->namaFile);
        if(myFile.is_open()) {
            int kolom, baris;
            kolom = baris = 0;
            for(const auto& header: this->header) {
                myFile << header;
                if(kolom != this->header.size() - 1) {
                    myFile << ',';
                }
                kolom++;
            }
            myFile << "\n";

            for(const auto& row: this->data) {
                for(int index = 0; index < row.size(); index++) {
                    myFile << row.at(this->header.at(index));
                    if(index != row.size() - 1) {
                        myFile << ',';
                    } 
                }
                if(baris != this->data.size() - 1) {
                    myFile << "\n";
                }
                baris++;
            }
            myFile.close();
        }

    } catch(int errorCode) {
        switch (errorCode)
        {
        case 404:
            setWarnaFont(red);
            std::cout << "\n" << "ID "<< id << " tidak ditemukan" << "\n";
            setWarnaFont(reset);
            break;
        
        default:
            setWarnaFont(red);
            std::cout << "\n" <<"Ada kesalahan program" << "\n";
            setWarnaFont(reset);
            break;
        }
    }
}