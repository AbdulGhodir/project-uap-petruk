#include <iostream>
#include <vector>
#include "include/database.h"
#include "include/user.h"

namespace AUTH {
    void Login() {
        MyDatabase database("data/user.csv");
        std::string email, password;
        std::cout << "=== Selamat Datang di Pegadaian Rafly ===" << "\n";
        std::cout << "Silahkan Login Terlebih Dahulu" << "\n\n";
        std::cout << "Masukkan email anda: ";
        getline(std::cin, email);
        std::cout << "Masukkan password anda: ";
        getline(std::cin, password);
        std::cout << "\n\n";
        database.auth(email, password);
    }

    void Register() {
        MyDatabase database("data/user.csv");
        std::string username, password, confirmPassword, email, noTelp, noKtp = "null";
        std::vector<std::string> user;

        do {
            std::cout << "=== Selamat Datang di Pegadaian Rafly ===" << "\n";
            std::cout << "Silahkan Register Terlebih Dahulu" << "\n\n";
            std::cout << "Masukkan username anda: ";
            getline(std::cin, username);
            user.push_back(username);
            std::cout << "Masukkan email anda: ";
            getline(std::cin, email);
            user.push_back(email);
            std::cout << "Masukkan No. Telp anda: ";
            getline(std::cin, noTelp);
            user.push_back(noTelp);
            std::cout << "Masukkan password anda: ";
            getline(std::cin, password);
            user.push_back(password);
            std::cout << "Masukkan Confirm password anda: ";
            getline(std::cin, confirmPassword);
            user.push_back(noKtp);
            std::cout << "\n\n";
            if(password != confirmPassword) std::cout << "Password tidak sama!!" << "\n\n";
        } while(password != confirmPassword);
        database.writeData(user);
    }
}




int main() {
    MyDatabase database("data/user.csv");
    AUTH::Register();
    AUTH::Login();
    database.readData();
    // std::vector<std::vector<std::string>> data;
    // database.getData(data);
    // for(const auto& baris: data) {
    //     for(const auto& kolom: baris) {
    //         std::cout << kolom << " | ";
    //     }
    //     std::cout << "\n";
    // }
}