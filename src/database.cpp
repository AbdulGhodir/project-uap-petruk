#include "../include/database.h"
#include "../include/user.h"
#include <fstream>
#include <sstream>
#include <iostream>

MyDatabase::MyDatabase(std::string namaFile) {
    this->namaFile = namaFile;
    this->reload();
}

void MyDatabase::reload() {
    std::ifstream myFile(this->namaFile);
    if(myFile.is_open()) {
        this->data.clear();
        std::string line;
        while (getline(myFile, line))
        {
            std::stringstream ss(line);
            std::vector<std::string> baris;
            std::string kolom;
            while (getline(ss, kolom, ','))
            {
                baris.push_back(kolom);
            }
            this->data.push_back(baris);
        }
        myFile.close();
    }
}

bool MyDatabase::is_empty() {
    return this->data.size() == 0;
}

MyDatabase& MyDatabase::auth(const std::string& email, const std::string& password) {
    this->reload();
    bool notfind = true;
    for(int baris = 0; baris < this->data.size(); baris++) {
        if(this->data.at(baris).at(2) == email && this->data.at(baris).at(4) == password) {
            user.id = this->data.at(baris).at(0);
            user.nama = this->data.at(baris).at(1);
            user.email = this->data.at(baris).at(2);
            user.noTelp = this->data.at(baris).at(3);
            notfind = false;
        }
    }
    if(notfind) std::cout << "Email/Password salah" << "\n\n";
}

MyDatabase& MyDatabase::getData(std::vector<std::vector<std::string>>& data) {
    this->reload();
    for(const auto& row: this->data) {
        std::vector<std::string> baris;
        for(const auto& column: row) {
            baris.push_back(column);
        }
        data.push_back(baris);
    }
}

MyDatabase& MyDatabase::writeData(const std::vector<std::string>& data) {
    std::ofstream myFile(this->namaFile, std::ios::app);
    if(myFile.is_open()) {
        myFile << "\n";
        myFile << this->data.size();
        myFile << ','; 
        int i = 1;
        for (const auto& kolom: data) {
            myFile << kolom;
            if(i != data.size()) {
                myFile << ',';
            }
            i++;
        }
        myFile.close();
    }
}

MyDatabase& MyDatabase::readData() {   
    this->reload();
    if(!this->is_empty()) {
        for(const auto& row: this->data) {
            for(const auto& kolom: row) {
                std::cout << kolom << " | ";
            }
            std::cout << "\n";
        }
    } else {
        for(const auto& row: this->data) {
            for(const auto& kolom: row) {
                std::cout << kolom << " | ";
            }
            std::cout << "\n";
        }
        std::cout << "Data masih kosong!!" << "\n";
    }
}

MyDatabase& MyDatabase::updateData(const std::string id,const std::vector<std::string>& data) {
    this->reload();
    int baris = 0;
    bool notfind = true;
    for(auto& row: this->data) {
        for(auto& column: row) {
            if(column == id) {
                for(auto i = 1; i < this->data.at(baris).size(); i++) {
                    this->data.at(baris).at(i) = data.at(i - 1);
                }
                notfind = false;
            }
            break;
        }
        baris++;
    }
    if(notfind) std::cout << "ID " << id << " tidak ditemukan" << "\n";
    std::ofstream myFile(this->namaFile);
    if(myFile.is_open()) {
        int kolom, baris;
        kolom = baris = 0;
        for(const auto& row: this->data) {
            kolom = 0;
            for(const auto& column: row) {
                myFile << column;
                if(kolom != this->data.at(baris).size() - 1) {
                    myFile << ",";
                }
                kolom++;
            }
            if(baris != this->data.size() - 1) {
                myFile << "\n";
            }
            baris++;
        }
    }
}

MyDatabase& MyDatabase::deleteData(const std::string id) {
    this->reload();
    int baris = 0;
    bool notfind = true;
    for(auto& row: this->data) {
        for(auto& column: row) {
            if(column == id) {
                this->data.erase(this->data.begin() + baris);
                notfind = false;
            }
            break;
        }
        baris++;
    }
    if(notfind) std::cout << "ID "<< id << " tidak ditemukan" << "\n";
    std::ofstream myFile(this->namaFile);
    if(myFile.is_open()) {
        int kolom, baris;
        kolom = baris = 0;
        for(const auto& row: this->data) {
            kolom = 0;
            for(const auto& column: row) {
                myFile << column;
                if(kolom != data.at(baris).size() - 1) {
                    myFile << ",";
                }
                kolom++;
            }
            if(baris != data.size() - 1) {
                myFile << "\n";
            }
            baris++;
        }
    }
}