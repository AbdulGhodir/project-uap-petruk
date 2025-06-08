#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <unordered_map>

class MyDatabase {
    private:
        std::string namaFile;
        std::vector<std::string> header;
        std::vector<std::unordered_map<std::string, std::string>> data;
        
    public:
        MyDatabase(std::string namaFile);
        bool writeData(const std::unordered_map<std::string, std::string>& data);
        MyDatabase& readData();
        MyDatabase& getData(std::vector<std::unordered_map<std::string, std::string>>& data);
        bool auth(const std::string& email, const std::string& password);
        MyDatabase& updateData(const std::string id, const std::unordered_map<std::string, std::string>& data);
        MyDatabase& deleteData(const std::string id);
        bool is_empty();
        void reload();
};


#endif