#include<iostream>
using namespace std;

class Base{
public:
   
    static int m_A;

    static void func() {
        cout << "Base - static void func() 调用" << endl;
    } 

    static void func(int a) {
        cout << "Base - static void func(int a) 调用" << endl;
    } 
};

int Base::m_A = 100;

class Son : public Base{
public:

    static int m_A;

    static void func() {
        cout << "Son - static void func() 调用" << endl;
    } 
};

int Son::m_A = 200;

// 同名静态成员变量
void test01() {
    // 1、通过对象访问
    cout << "通过对象访问:" << endl;
    Son s;
    cout << "Son 的 m_A = " << s.m_A << endl;
    cout << "Base 的 m_A = " << s.Base::m_A << endl;

    // 2、通过类名访问
    cout << "通过类名访问:" << endl;
    cout << "Son 的 m_A = " << Son::m_A << endl;
    // 第一个::代表通过类名方式访问 第二个::代表父类作用域符访问
    cout << "Base 的 m_A = " << Son::Base::m_A << endl;

}

void test02() {
    // 1、通过对象访问
    cout << "通过对象访问:" << endl;
    Son s;
    s.func();
    s.Base::func();

    // 2、通过类名访问
    cout << "通过类名访问:" << endl;
    Son::func();
    Son::Base::func();

    // 如果子类中出现和父类同名的静态成员函数，子类的同名成员函数会隐藏掉父类的所有同名函数
    // 如果想访问父类的被隐藏的同名静态成员函数，需要加作用域访问
    Son::Base::func(100);
}

int main() {
    // test01();
    test02();
    return 0;
}