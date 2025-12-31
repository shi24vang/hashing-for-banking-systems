#ifndef DOUBLEHASHING_H
#define DOUBLEHASHING_H

#include "BaseClass.h"

class DoubleHashing : public BaseClass {
public:
    DoubleHashing();
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;

    long long getCollisionCount() const { return collisionCount; }
    void resetCollisionCount() { collisionCount = 0; }
    void setUseTunedHash(bool v) { useTunedHash = v; }

private:
    int secondaryHash(const std::string &id);
    void merge2sort(std::vector<int> &a, int s, int e);
    void mergesort(std::vector<int> &a, int s, int e);
    std::vector<int> sort(std::vector<int> &v);

    int size;
    long long collisionCount;
    bool useTunedHash;
};

#endif // DOUBLEHASHING_H
