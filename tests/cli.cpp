#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Comp.h"

static void printTopK(const std::vector<int> &vals) {
    for (size_t i = 0; i < vals.size(); ++i) {
        std::cout << vals[i];
        if (i + 1 < vals.size()) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}

int main(int argc, char **argv) {
    std::istream *in = &std::cin;
    std::ifstream file;
    if (argc > 1) {
        file.open(argv[1]);
        if (file) {
            in = &file;
        }
    }

    Comp db;
    std::string line;
    while (std::getline(*in, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd == "createAccount") {
            std::string id;
            int bal;
            iss >> id >> bal;
            db.createAccount(id, bal);
        } else if (cmd == "getBalance") {
            std::string id;
            iss >> id;
            std::cout << db.getBalance(id) << "\n";
        } else if (cmd == "addTransaction") {
            std::string id;
            int delta;
            iss >> id >> delta;
            db.addTransaction(id, delta);
        } else if (cmd == "doesExist") {
            std::string id;
            iss >> id;
            std::cout << (db.doesExist(id) ? 1 : 0) << "\n";
        } else if (cmd == "deleteAccount" || cmd == "delete") {
            std::string id;
            iss >> id;
            db.deleteAccount(id);
        } else if (cmd == "databaseSize") {
            std::cout << db.databaseSize() << "\n";
        } else if (cmd == "getTopK") {
            int k;
            iss >> k;
            printTopK(db.getTopK(k));
        }
    }
    return 0;
}
