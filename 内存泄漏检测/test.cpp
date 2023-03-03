#include <iostream>
#include <memory>
#include "memleak.h"
#include <malloc.h>


using std::cout;
using std::endl;


namespace master{


template<typename T>
class shared_ptr{

public:

    virtual ~shared_ptr() {
        if (_ptr) {
            if (--(*_ref) == 0) {
                delete _ptr;
                delete _ref;
                _ptr = nullptr;
                _ref = nullptr;
            }
        }
    }
    shared_ptr() : _ptr(nullptr), _ref( new size_t{0} ) { 
        //没有管理的内存，计数为0
    }

    shared_ptr(T* ptr) : _ptr(ptr), _ref( new size_t{0} ) { //默认为0
        if (_ptr != nullptr) ++(*_ref); //指针不为空才有管理的内存，计数+1
    }
    shared_ptr(const shared_ptr<T>& sp) : _ptr(sp._ptr), _ref( sp._ref ) {
        if (this == &sp) return ;
        if (_ptr != nullptr) ++(*_ref); //指针不为空才有管理的内存，计数+1
    }
    shared_ptr(shared_ptr<T>&& sp) : _ptr(sp._ptr), _ref( sp._ref ) {
        sp._ptr = nullptr; // 右值对象，置空
        sp._ref = nullptr;
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& sp) {
        if (this == &sp) return *this;
        this->reset();
        _ref = sp._ref;
        _ptr = sp._ptr;
        if (_ptr) ++(*_ref); //如果别人有管理的内存，计数才能+1
        return *this;
    }

    shared_ptr<T>& operator=(shared_ptr<T>&& sp) {
        if (this == &sp) return *this;
        this->reset();
        _ref = std::forward<shared_ptr<T>>(sp)._ref;
        _ptr = std::forward<shared_ptr<T>>(sp)._ptr;
        sp._ptr = nullptr;
        sp._ref = nullptr;
        return *this;
    }

public:

    operator bool() const { return _ptr != nullptr; }
    T* operator->() const { return _ptr; }
    T& operator*() const { return *_ptr; }


public:
    inline size_t use_count() const { return _ref == nullptr ? 0 : *_ref; }

    inline T* get() const { return _ptr; }

    inline void swap(shared_ptr& sp) {
        if (this == &sp) return ;
        std::swap(_ptr, sp._ptr);
        std::swap(_ref, sp._ref);
        return ;
    }

    inline bool unique() const { return _ref == nullptr ? false : (*_ref) == 1; }

    void reset(T* t = nullptr) {
        //把自己干掉
        if (_ptr != nullptr) {
            if (--(*_ref) == 0) {
                delete _ptr;
                delete _ref;
            }
            _ptr = nullptr;
            _ref = nullptr;
        }
        if (t != nullptr) {
            _ptr = t;
            _ref = new size_t{1};
        }
    }





private:
    T* _ptr;
    size_t* _ref;
};


template<typename T, typename... ARGS>
inline shared_ptr<T> make_shared(ARGS&&... args) {
    return shared_ptr<T> { new T{std::forward<ARGS>(args)...} };
}
template<typename T>
inline shared_ptr<T> make_shared() {
    return shared_ptr<T> { new T{} };
}

};


struct A{
    int _a;

    A() : _a(0) {
        std::cout << "A default constructor " << this << std::endl;
    }
    A(int a) : _a(a) {
        std::cout << "A constructor int " << this << endl;
    }
    ~A() {
        std::cout << "A destructor " << this << std::endl;
    }
};

void test() {
    std::shared_ptr<A> p;
    std::shared_ptr<A> p1{new A{}};
    std::shared_ptr<A> p2(p1);
    // p1.get();
    std::cout << p2.use_count() << std::endl;//2
    std::cout << p1.use_count() << std::endl;//2
    p1.swap(p2);
    p1.reset();
    // p1.unique();
    // p1.use_count();
    std::cout << p2.use_count() << std::endl;
    std::cout << p1.use_count() << std::endl;
    //p2.reset();
    p = std::move(p2);
    //p = p2;
    std::cout << p2.use_count() << std::endl;
    std::cout << p.use_count() << std::endl;
}

void test1() {
    master::shared_ptr<A> p;
    master::shared_ptr<A> p1{new A{}};
    master::shared_ptr<A> p2(p1);
    // p1.get();
    std::cout << p2.use_count() << std::endl;//2
    std::cout << p1.use_count() << std::endl;//2
    p1.swap(p2);
    p1.reset();
    // p1.unique();
    // p1.use_count();
    std::cout << p2.use_count() << std::endl;
    std::cout << p1.use_count() << std::endl;
    //p2.reset();
    p = std::move(p2);
    //p = p2;
    std::cout << p2.use_count() << std::endl;
    std::cout << p.use_count() << std::endl;
}

int main(int argc, const char** argv) {
    //test1();
    master::shared_ptr<A> p6 = master::make_shared<A>(1);
    master::shared_ptr<A> p7 = master::make_shared<A>();

    // master::shared_ptr<int> p3, p4;
    // master::shared_ptr<int> p5{new int{3}};
    // p3 = p5;
    // cout << p3.use_count() << endl;
    // cout << p4.use_count() << endl;
    // cout << p5.use_count() << endl;

    cout << p6.use_count() << endl;
    cout << p7.use_count() << endl;

    p6 = p7;
    cout << p6.use_count() << endl;
    cout << p7.use_count() << endl;   

    p6.reset();
    cout << p6.use_count() << endl;
    cout << p7.use_count() << endl;   

    p7.reset();

    cout << p6.use_count() << endl;
    cout << p7.use_count() << endl; 

    return 0;
}