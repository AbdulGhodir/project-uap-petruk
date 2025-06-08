#include <map>
#include <vector>
#include <iostream>
using namespace std;


int main() {
    vector<map<string, string>> data = {
        {
            {"Nama", "Rafly"},
            {"NPM", "2417051049"}
        },
        {
            {"Nama", "Lili"},
            {"NPM", "2417051049"}
        }
    };

    for(const auto& row: data) {
        cout << "Nama: " << row.at("Nama") << "\n";
        cout << "NPM: " << row.at("NPM") << "\n";
    }
}