#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
class MyDatabase {
    private:
        std::string namaFile;
        std::vector<std::vector<std::string>> data;
        
    public:
        MyDatabase(std::string namaFile);
        MyDatabase& writeData(const std::vector<std::string>& data);
        MyDatabase& readData();
        MyDatabase& getData(std::vector<std::vector<std::string>>& data);
        MyDatabase& auth(const std::string& email, const std::string& password);
        MyDatabase& updateData(const std::string id, const std::vector<std::string>& data);
        MyDatabase& deleteData(const std::string id);
        bool is_empty();
        void reload();
};


#endif