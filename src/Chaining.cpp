#include "Chaining.h"
#include <cctype>
#include <cstdint>

namespace {
uint64_t tunedHash(const std::string &id) {
    const uint64_t alphaMul = 1315423911ULL;
    const uint64_t digitMul = 1099511628211ULL;
    uint64_t alpha = 0x9e3779b97f4a7c15ULL;
    uint64_t digit = 0x6a09e667f3bcc909ULL;

    for (unsigned char uc : id) {
        if (std::isalpha(uc)) {
            alpha = (alpha ^ static_cast<uint64_t>(std::tolower(uc))) * alphaMul;
        } else if (std::isdigit(uc)) {
            digit = (digit ^ static_cast<uint64_t>(uc - '0' + 1)) * digitMul;
        } else {
            alpha ^= static_cast<uint64_t>(uc);
            alpha *= alphaMul;
        }
    }

    uint64_t mixed = alpha ^ (digit << 1) ^ (alpha >> 13) ^ (digit >> 7);
    mixed ^= mixed >> 33;
    mixed *= 0xff51afd7ed558ccdULL;
    mixed ^= mixed >> 33;
    mixed *= 0xc4ceb9fe1a85ec53ULL;
    mixed ^= mixed >> 33;
    return mixed;
}
}

Chaining::Chaining(){
	for(int i=0;i<100019;i++){
		std::vector<Account> r;
		bankStorage2d.push_back(r);
	}
	size=0;
    collisionCount = 0;
    useTunedHash = true;
} 
void Chaining::createAccount(std::string id, int count) {
    int index = hash(id);
    Account newA;
    newA.id = id;
    newA.balance = count;
    collisionCount += bankStorage2d[index].size();
    bankStorage2d[index].push_back(newA);
    size++;
}

std::vector<int> Chaining::getTopK(int k) {
    std::vector<int> topBalances;
    int n=bankStorage2d.size();
    for(int i=0;i<n;i++){
    	int m=bankStorage2d[i].size();
    	for(int j=0;j<m;j++){
    		topBalances.push_back(bankStorage2d[i][j].balance);
		}
	}
    topBalances=sort(topBalances);
    if (k <=size) {
    	std::vector<int> ans;
        for(int i=0;i<k;i++){
        	ans.push_back(topBalances[size-1-i]);
		}
		return ans;
    }
    else{
    	std::vector<int> ans;
        for(int i=0;i<size;i++){
        	ans.push_back(topBalances[size-1-i]);
		}
		return ans;
	}
}

int Chaining::getBalance(std::string id) {
    int index = hash(id);
    int n=bankStorage2d[index].size();
    for (int i=0;i<n;i++) {
        if (i > 0) {
            collisionCount++;
        }
        if (bankStorage2d[index][i].id == id) {
            return bankStorage2d[index][i].balance;
        }
    }
    return -1;
}

void Chaining::addTransaction(std::string id, int count) {
    int index = hash(id);
    int n=bankStorage2d[index].size();
    for (int i=0;i<n;i++) {
        if (i > 0) {
            collisionCount++;
        }
        if (bankStorage2d[index][i].id == id) {
            bankStorage2d[index][i].balance += count;
            return;
        }
    }
    createAccount(id, count);
}

bool Chaining::doesExist(std::string id) {
    int index = hash(id);
    int n=bankStorage2d[index].size();
    for (int i=0;i<n;i++) {
        if (i > 0) {
            collisionCount++;
        }
        if (bankStorage2d[index][i].id == id) {
            return true;
        }
    }
    return false;
}

bool Chaining::deleteAccount(std::string id) {
    int index = hash(id);
    int n=bankStorage2d[index].size();
    for(int i=0;i<n;i++){
        if (i > 0) {
            collisionCount++;
        }
    	if(bankStorage2d[index][i].id==id){
    		bankStorage2d[index].erase(bankStorage2d[index].begin()+i);
    		size--;
    		return true;
		}
	}
    return false;
}
int Chaining::databaseSize() {
    return size;
}

int Chaining::hash(std::string id) {
    if (!useTunedHash) {
        int ha = 0;
        for (char c : id) {
            ha = (ha * 31 + c) % 100019;
        }
        return ha;
    }
    uint64_t mixed = tunedHash(id);
    return static_cast<int>(mixed % 100019ULL);
}

std::vector<int> Chaining::sort(std::vector<int> &v){
	int s=0;
	int e=v.size()-1;
	mergesort(v,s,e);
	return v;
}
void Chaining::merge2sort(std::vector<int> &a,int s,int e){
	int mid=s+(e-s)/2;
	int l1=mid-s+1;
	int l2=e-mid;
	std::vector<int> f1;
	std::vector<int> f2;
	int k=s;
	for(int i=0;i<l1;i++){
		f1.push_back(a[k++]);
	}
	k=mid+1;
	for(int i=0;i<l2;i++){
		f2.push_back(a[k++]);
	}
	int m=0;
	int n=0;
	int c=s;
	while(m<l1 && n<l2){
		if(f1[m]>f2[n]){
			a[c++]=f2[n++];
		}
		else{
			a[c++]=f1[m++];
		}
	}
	while(m<l1){
		a[c++]=f1[m++];
	}
	while(n<l2){
		a[c++]=f2[n++];
	}
	f1.clear();
	f2.clear();
}
void Chaining::mergesort(std::vector<int> &a,int s,int e){
	if(s>=e){
		return;
	}
	int mid=s+(e-s)/2;
	mergesort(a,s,mid);
	mergesort(a,mid+1,e);
	merge2sort(a,s,e); 
}
