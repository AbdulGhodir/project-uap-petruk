#ifndef USER_H
#define USER_H
#include <string>
struct USER
{
    std::string id = "null";
    std::string nama = "null";
    std::string noTelp = "null";
    std::string email = "null"; 
    std::string noKtp = "null";   
};

extern USER user;


#endif