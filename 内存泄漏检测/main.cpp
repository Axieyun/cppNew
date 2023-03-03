#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <string>
using namespace std;
  
class MyString {
private:
    
    size_t _length;  // 字符串的长度
    size_t _capcity; // 开辟的内存数组长度
    char *_date;     // 开辟的内存的首地址

public:
    MyString():_date(nullptr), _length(0), _capcity(0) {
        cout << "MyString constructor" << endl;
    }

    ~MyString() {
        if (_date != nullptr) delete[] _date;
        cout << "MyString destructor" << endl;
    }

    MyString(const char* s)
    : _length(strlen(s))
    , _capcity(_length + 1)
    , _date{new char(_capcity)} {
        strcpy(_date, s);
        cout << "char* constructor" << endl;
    }

    MyString(const MyString& s)
    : _length(s._length)
    , _capcity(s._capcity)
    , _date{new char(_capcity)} {
        strcpy(_date, s._date);
        cout << "copy constructor" << endl;
    }


public:
    // 不写这个的话，s = "223423"，会发生
    // s隐式转换为MyString类型，此时发生char *参数构造，生成个临时MyString类型数据
    // 然后，进行下面哪个MyString operator= 操作，最后析构 临时MyString类型数据。
    // 造成效率降低，增加内存消耗
    MyString& operator=(const char* s) {

        if (this->_date == s) return *this;

        cout << "char* operator= overloading" << endl;
        if (_date != nullptr) delete _date;

        _length = strlen(s);
        _capcity = _length + 1;
        _date = new char(_capcity);
        strcpy(_date, s);
        return *this;
    }

    MyString& operator=(const MyString& s) {
        if (this == &s) return *this;
        cout << "MyString operator= overloading" << endl;

        if (_date != nullptr) delete[] _date;

        _length = strlen(s._date);
        _capcity = _length + 1;
        _date = new char(_capcity);
        strcpy(_date, s._date);   
        return *this;
    }   


    MyString& operator+=(const char* s) {

        if (_date != nullptr) {
            size_t lenS = strlen(s);
            while (lenS + _length >= _capcity) _capcity <<= 1;
            
            char *p = (char *)realloc(_date, _capcity);
            if (p != nullptr) {
                _date = p;
            } else {
                p = (char *)malloc(_capcity);
                strncpy(p, _date, _length);
                delete[] _date; //释放原来数据
                _date = p;
            }
            strncpy(_date + _length, s, lenS);
            _length += lenS;
            _date[_length] = '\0';
        } else {
            _length = strlen(s);
            _date = new char(_length + 1);
            _capcity = _length + 1;
            strcpy(_date, s);
            _date[_length] = '\0';
        }

        return *this;
    }

    MyString& operator+=(const MyString& s) {
        return (*this) += s._date;
    }


    char& operator[](size_t ind) {
        return _date[ind];
    }

    MyString operator+(const char* s) {
        return MyString{(*this += s)};
    }

    MyString operator+(const MyString& s) {
        return MyString{(*this += s)};
    }

    friend ostream& operator<<(ostream& os, const MyString& s) {
        os << "string = " << s._date;
        return os;
    }


};

void text() {
    // 参数构造
    MyString s1{"123"}; 

    cout << s1 << endl;
    // 拷贝构造
    MyString s2 = s1;
    MyString s3{s2};

    cout << s2 << endl;
    cout << s3 << endl;

    s1 = "32dhjskdfhsj";
    cout << s1 << endl;
    s1 = "32dhjs674563453kdfhsj";
    cout << s1 << endl;
    s1 = s2;
    cout << s1 << endl;



    s1 += "234234";
    cout << s1 << endl;
    s1 += s2;
    cout << s1 << endl;   

    cout << s1 + s2 << endl; //会发生析构

    s1[5] = 'G';
    cout << s1 << endl;    
    s1 = s1;
    cout << s1 << endl;
    return  ; 
}

int main() {
    text();
    return 0;
}