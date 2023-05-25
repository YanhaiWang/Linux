#include<iostream>
using namespace std;

class Person{
public:
    /*
    所有对象共享同一份数据
    在编译阶段分配内存，在全局变量区
    类内声明，类外初始化
    */
    static int m_A; // 类内声明
};

// 类外初始化  默认值为0
int Person::m_A;

void test01() {
    Person p;
    cout << p.m_A << endl;
    Person p2;
    p2.m_A = 200;
    cout << p.m_A << endl;
}

int main() {
    test01();
    return 0;
}