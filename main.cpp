#include <iostream>
#include <vector>
#include "include/database.h"
#include "include/user.h"
#include "include/lib.h"
#include "include/menu.h"

    void Login() {
        MyDatabase database("data/user.csv");
        std::string email, password;

        std::cin.ignore();
        cetakGaris(40, green);
        setWarnaFont(blue);
        std::cout << "Silahkan Login Terlebih Dahulu" << "\n\n";
        setWarnaFont(reset);
        std::cout << "Masukkan email anda: ";
        getline(std::cin, email);
        std::cout << "Masukkan password anda: ";
        getline(std::cin, password);
        std::cout << "\n";
        
        int respon = database.auth(email, password);
        if(respon) {
            setWarnaFont(green);
            std::cout << "\n" << "Login berhasil";
            setWarnaFont(reset);
            menu(user.role);
        } else {
            setWarnaFont(red);
            std::cout << "\n" << "Email \\ Password salah";
            setWarnaFont(reset);
        }
        cetakGaris(40, green);

    }

    void Register() {
        MyDatabase database("data/user.csv");
        std::string username, password, confirmPassword, email, noTelp, noKtp = "null";
        std::unordered_map<std::string, std::string> user;

        cetakGaris(40, magenta);
        do {
            std::cin.ignore();
            setWarnaFont(blue);
            std::cout << "Silahkan Register Terlebih Dahulu" << "\n\n";
            setWarnaFont(reset);
            std::cout << "Masukkan username anda: ";
            getline(std::cin, username);
            std::cout << "Masukkan email anda: ";
            getline(std::cin, email);
            std::cout << "Masukkan No. Telp anda: ";
            getline(std::cin, noTelp);
            std::cout << "Masukkan password anda: ";
            getline(std::cin, password);
            std::cout << "Masukkan Confirm password anda: ";
            getline(std::cin, confirmPassword);

            user["role"] = "user";
            user["nama"] = username;
            user["email"] = email;
            user["noTelp"] = noTelp;
            user["password"] = password;
            user["noKtp"] = noKtp;
            if(password != confirmPassword) {
                setWarnaFont(red);
                std::cout << "\n" << "Password tidak sama!!" << "\n";
                setWarnaFont(reset);
            }
        } while(password != confirmPassword);

        int respon = database.writeData(user);
        if(respon) {
            setWarnaFont(green);
            std::cout << "\n" << "Register berhasil";
            setWarnaFont(reset);
        } else {
           setWarnaFont(red);
           std::cout << "\n" << "Register Gagal!";
           setWarnaFont(reset);
        }
        cetakGaris(40, magenta);
    }

int main() {
    
    int pilihan;
    do {
        cetakGaris(40, yellow);
        std::cout << "   SELAMAT DATANG DI SISTEM PEGADAIAN";
        cetakGaris(40, yellow);
        std::cout << "1. Login" << "\n"
        << "2. Register" << "\n\n";

        std::cout << "Masukkan pilhan anda: ";
        std::cin >> pilihan;
        switch (pilihan)
        {
        case 1:
            Login();
            break;
        case 2:
            Register();
            break;
        default:
            setWarnaFont(red);
            std::cout << "Pilihan tidak valid!!\n\n";
            setWarnaFont(reset);
            break;
        }
    } while(pilihan != 1);
    
}