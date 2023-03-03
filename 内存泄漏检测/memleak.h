
#ifndef _MEMLEAK_H_
#define _MEMLEAK_H_

#include <string>
#include <unordered_map>


struct MemInfo {
    std::string file;
    size_t line;
    size_t size;
};





class Memleak {

public:
    Memleak();
    ~Memleak();


    static void deleteMenInfo(void *p);
    static void setMemInfo(void *p, const char* file, size_t line, size_t size);
    


private:
    static std::unordered_map<void *, MemInfo> memPool;
    static Memleak memleak;
};

void *operator new(size_t size, const char* file, size_t line);
void *operator new[](size_t size, const char* file, size_t line);
void operator delete(void *);
void operator delete[](void *);



// 定义宏，不希望new宏在memleak.cpp文件展开，而在其他文件展开
#ifndef _NEW_OVERRIDE_IMPLEMENTATION_
    #define new new(__FILE__, __LINE__)
#endif

#endif 
