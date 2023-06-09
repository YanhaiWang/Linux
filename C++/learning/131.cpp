#include<iostream>
using namespace std;

class Base{
public:
    Base() {
        m_A = 100;
    }

    void func() {
        cout << "Base - func() 调用" << endl;
    }

    void func(int a) {
        cout << "Base - func(int a) 调用" << endl;
    }

    int m_A;
};

class Son : public Base{
public:
    Son() {
        m_A = 200;
    }

    void func() {
        cout << "Son - func() 调用" << endl;
    }

    int m_A;
};

void test01() {
    Son s;
    cout << "Son 的 m_A = " << s.m_A << endl;
    cout << "Base 的 m_A = " << s.Base::m_A << endl;
}

void test02() {
    Son s;
    s.func();
    s.Base::func();
    // 如果子类中出现和父类同名的成员函数，子类的同名成员函数会隐藏掉父类的所有同名函数
    // 如果想访问父类的被隐藏的同名成员函数，需要加作用域访问
    s.Base::func(100);
}

int main() {
    // test01();
    test02();
    return 0;
}