



#define _NEW_OVERRIDE_IMPLEMENTATION_
#include "memleak.h"
#include <iostream>



std::unordered_map<void *, MemInfo> Memleak::memPool;
Memleak Memleak::memleak;







using std::cout;
using std::endl;


Memleak::Memleak() {
    cout << "Memleak Constructor" << endl;

}


Memleak::~Memleak() {
    cout << "Memleak destructor" << endl;
    cout << "------------------Leak Memory Info Begin------------------" << endl;
    for (auto it : memPool) {
        cout << "Leak Memory Address [" << it.first << "]";
        cout << " File [" << it.second.file << "]";
        cout << " Line [" << it.second.line << "]";
        cout << " Size [" << it.second.size << "]";
        cout << endl;
    }
    cout << "------------------Leak Memory Info End------------------" << endl;
}

void Memleak::setMemInfo(void *p, const char* file, size_t line, size_t size) {
    cout << "Malloc Address [" << p << "], ";
    cout << "Size [" << size << "]" << endl;
    
    memPool[p].file = file;
    memPool[p].line = line;
    memPool[p].size = size;
}


void Memleak::deleteMenInfo(void *p) {
    cout << "Free Address [" << p << "]" << endl;
    auto info = memPool.find(p);
    if (info != memPool.end()) {
        memPool.erase(p);
    }
}

void *operator new(size_t size, const char* file, size_t line) {
    if (size == 0) size = 1;
    void *p = malloc(size);
    if (p == nullptr) {
        cout << "ERROR NEW" << endl;
    } else {
        Memleak::setMemInfo(p, file, line, size);
    }
    return p;
}
void *operator new[](size_t size, const char* file, size_t line) {
    if (size == 0) size = 1;
    void *p = malloc(size);
    if (p == nullptr) {
        cout << "ERROR NEW[]" << endl;
    } else {
        Memleak::setMemInfo(p, file, line, size);
    }
    return p;    
}
void operator delete(void *p) {
    Memleak::deleteMenInfo(p);
    if (p) free(p);
}
void operator delete[](void *p) {
    Memleak::deleteMenInfo(p);
    if (p) free(p);
}
