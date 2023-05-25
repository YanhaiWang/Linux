#include<iostream>
using namespace std;

class Base {
public:
    int m_A;
protected:
    int m_B;
private:
    int m_C;
};

class Son : public Base{
public:
    int m_D;
};

void test01() {
    // 父类中所有非静态成员属性都会被子类继承
    // 父类中私有成员属性 被编译器隐藏 访问不到，但被继承了下来
    cout << "size of = " << sizeof(Son) << endl;
}

int main() {
    test01();
    return 0;
}