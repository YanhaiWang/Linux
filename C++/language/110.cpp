#include<iostream>
using namespace std;

// 深拷贝 浅拷贝

class Person{
public:
    Person() {
        cout << "Person的默认构造函数调用" << endl;
    }

    Person(int age, int height) {
        m_Age = age;
        m_height = new int(height);
        cout << "Person有参构造函数调用" << endl;
    }

    // 自己实现拷贝构造函数 解决浅拷贝问题
    Person(const Person & p) {
        cout << "Person拷贝构造函数调用" << endl;
        m_Age = p.m_Age;  
        // 编译器默认实现的
        // m_height = p.m_height;
        // 深拷贝
        m_height = new int(*p.m_height);
    }

    ~Person() {
        // 析构函数 将堆区开辟的数据做释放
        if(m_height != nullptr) {
            delete m_height;
            m_height = nullptr;  // 防止野指针出现
        }
        cout << "Person析构函数调用" << endl;
    }

    int m_Age;
    int *m_height;
};


void test01() {
    Person p(18, 160);
    cout << "p的年龄为: " << p.m_Age << " p的身高为: " << *p.m_height << endl;

    Person p2(p);
    cout << "p2的年龄为: " << p2.m_Age << " p2的身高为: " << *p2.m_height << endl;
}

int main() {
    test01();
    return 0;
}