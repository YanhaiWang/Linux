#include<iostream>
#include<string>
using namespace std;

// 类对象作为类成员
class Phone{
public:
    Phone(string PName) {
        m_PName = PName;
        cout << "Phone构造函数调用" << endl;
    }

    ~Phone() {
        cout << "Phone析构函数调用" << endl;
    }

    string m_PName;
};


class Person{   
public:
    // Phone m_Phone = PName; 隐式转换
    Person(string name, string PName) : m_Name(name), m_Phone(PName) {
        cout << "Person构造函数调用" << endl;
    }

    ~Person() {
        cout << "Person析构函数调用" << endl;
    }

    string m_Name;
    Phone m_Phone;
};

// 当其他类对象作为本类成员，构造时先构造类对象，再构造自身，析构顺序相反
void test01() {
    Person p("张三", "三星");
    cout << p.m_Name << "拿着" << p.m_Phone.m_PName << endl;
}

int main() {
    test01();
    return 0;
}