#include<iostream>
using namespace std;

// 基类
class BasePage{
public:
    void header() {
        cout << "首页、公开课、注册、登录" << endl;
    }

    void footer() {
        cout << "帮助中心、交流合作、站内信息" << endl;
    }

    void left() {
        cout << "Java、Python、C++" << endl;
    }
};

class Java:public BasePage{
public:
    void content() {
        cout << "Java 视频" << endl;
    } 
};

class Python:public BasePage{
    public:
    void content() {
        cout << "Python 视频" << endl;
    } 
};

class Cpp:public BasePage{
    public:
    void content() {
        cout << "C++ 视频" << endl;
    } 
};

void test01() {
    cout << "Java如下==========" << endl;
    Java ja;
    ja.header();
    ja.footer();
    ja.left();
    ja.content();
    cout << "Python如下==========" << endl;
    Python py;
    py.header();
    py.footer();
    py.left();
    py.content();
    cout << "C++如下==========" << endl;
    Cpp cp;
    cp.header();
    cp.footer();
    cp.left();
    cp.content();
}

main () {
    test01();
    return 0;
}