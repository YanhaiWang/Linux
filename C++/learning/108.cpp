#include<iostream>
using namespace std;

// 拷贝构造函数调用时机

// 1、使用一个已经创建完毕的对象初始化一个新对象

// 2、值传递给函数传参

// 3、值传递返回局部对象

class Person{
public:
    Person() {
        cout << "Person默认构造函数调用" << endl;
    }

    Person(int age) {
        cout << "Person有参构造函数调用" << endl;
        m_Age = age;
    }

    Person(const Person & p) {
        cout << "Person拷贝构造函数调用" << endl;
        m_Age = p.m_Age;
    }

    ~Person() {
        cout << "Person析构函数调用" << endl;
    }

    int m_Age;
};

// 1、使用一个已经创建完毕的对象初始化一个新对象
void test01() {
    Person p1(20);
    Person p2(p1);
    cout << "p2的年龄为: " << p2.m_Age << endl;
}

// 2、值传递给函数传参  实参调用拷贝构造函数创建临时对象传给形参
void work(Person p) {

}

void test02() {
    Person p;
    work(p);
}

// 3、值传递返回局部对象
// 当一个函数返回一个对象实例，一个临时对象将被创建并通过调用拷贝构造函数把目标对象复制给这个临时对象。
// C++标准允许省略这些拷贝构造函数，即使这导致程序的不同行为，即使编译器把两个对象视作同一个具有副作用
Person work2() {
    Person p1;  
    cout << &p1 << endl;
    return p1;
}

void test03() {
    Person p = work2();
    cout << &p << endl;
}

int main() {
    // test01();
    // test02();
    test03();
    return 0;
}