#include<iostream>
using namespace std;

// 继承方式

class Base{
public:
    int m_A;
protected:
    int m_B;
private:
    int m_C;
};

// 公有继承
class Son1 : public Base{
public:
    void func() {
        m_A = 10; // 父类中的公共成员 子类依然可以访问
        m_B = 10; // 父类中的保护成员 子类依然可以访问
        // m_C = 10; // 父类中的私有成员 子类不可访问
    } 
};

// 保护继承
class Son2 : protected Base{
public:
    void func() {
        m_A = 100; // 父类中的公共成员 到子类变为保护成员
        m_B = 100; // 父类中的保护成员 到子类变为保护成员
        // m_C = 100; // 父类中的私有成员 子类访问不到
    }    
};

// 私有继承
class Son3 : private Base{
public:
    void func() {
        m_A = 100; // 父类中的公共成员 到子类变为私有成员
        m_B = 100; // 父类中的保护成员 到子类变为私有成员
        // m_C = 100; // 父类中的私有成员 子类访问不到
    }    
};

class GrandSon3 : public Son3{
public:
    void func() {
        // m_A = 1000; // 到了Son3中 m_A变为私有，即使是子类也访问不到
        // m_B = 1000; // 到了Son3中 m_B变为私有，即使是子类也访问不到
    }
};

void test01() {
    Son1 s;
    s.m_A = 100;
    // s.m_B = 100; // 在 Son1中 m_B是保护成员 类外访问不到
}

void test02() {
    Son2 s;
    // s.m_A = 100;   // 在 Son2中 m_A变为保护成员 类外访问不到
    // s.m_B = 100; // 在 Son2中 m_B是保护成员 类外访问不到
}

void test03() {
    Son3 s;
    //  s.m_A = 100;   // 在 Son3中 m_A变为私有成员 类外访问不到
    //  s.m_B = 100; // 在 Son3中 m_B变为私有成员 类外访问不到
}