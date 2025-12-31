#include "LinearProbing.h"
#include <algorithm>
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

LinearProbing::LinearProbing(){
	for(int i=0;i<199999;i++){
		Account acc;
		acc.id="";
		acc.balance=0;
		bankStorage1d.push_back(acc);
	}
	size=0;
    collisionCount = 0;
    useTunedHash = true;
}
void LinearProbing::createAccount(std::string id, int count) {
    int index = hash(id);
        while (bankStorage1d[index].id!="" && bankStorage1d[index].id!="-1") {
            collisionCount++;
            index = (index + 1) % 199999;
        }
         bankStorage1d[index].id =id;
         bankStorage1d[index].balance=count;
         size++;
}

std::vector<int> LinearProbing::getTopK(int k) {
    std::vector<int> topBalances;
    int n=bankStorage1d.size();
    for(int i=0;i<n;i++){
    	if(bankStorage1d[i].id!="" && bankStorage1d[i].id!="-1"){
        		topBalances.push_back(bankStorage1d[i].balance);
		}
	}
    topBalances=sort(topBalances);
    if (k <= size) {
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

int LinearProbing::getBalance(std::string id) {
    int index = hash(id);
        while (bankStorage1d[index].id!="") {
            if (bankStorage1d[index].id == id) {
                return bankStorage1d[index].balance;
            }
            collisionCount++;
            index = (index + 1) % 199999;
        }
        return -1;
    
}

void LinearProbing::addTransaction(std::string id, int count) {
    int index = hash(id);
    while(bankStorage1d[index].id!=""){
        if (bankStorage1d[index].id == id) {
                bankStorage1d[index].balance+=count;
                return;
        }
        collisionCount++;
        index = (index + 1) % 199999;
    }
    createAccount(id,count);
}

bool LinearProbing::doesExist(std::string id) {
    int index = hash(id);
    while(bankStorage1d[index].id!=""){
        if (bankStorage1d[index].id == id) {
                return true;
        }
        collisionCount++;
        index = (index + 1) % 199999;
    }
    return false;
}

bool LinearProbing::deleteAccount(std::string id) {
    int index = hash(id);
    while(bankStorage1d[index].id!=""){
    	if (bankStorage1d[index].id!="-1" && bankStorage1d[index].id == id) {
                bankStorage1d[index].id="-1";
                bankStorage1d[index].balance=0;
                size--;
                return true;
        }
        collisionCount++;
        index = (index + 1) % 199999;
	}
    return false;
}
int LinearProbing::databaseSize() {
	return size;
}

int LinearProbing::hash(std::string id) {
    if (!useTunedHash) {
        int ha = 5381;
        for (char c : id) {
            ha = (ha * 31 + c) % 199999;
        }
        return ha;
    }
    uint64_t mixed = tunedHash(id);
    return static_cast<int>(mixed % 199999ULL);
}
std::vector<int> LinearProbing::sort(std::vector<int> &v){
	int s=0;
	int e=v.size()-1;
	mergesort(v,s,e);
	return v;
}
void LinearProbing::merge2sort(std::vector<int> &a,int s,int e){
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
void LinearProbing::mergesort(std::vector<int> &a,int s,int e){
	if(s>=e){
		return;
	}
	int mid=s+(e-s)/2;
	mergesort(a,s,mid);
	mergesort(a,mid+1,e);
	merge2sort(a,s,e); 
}
