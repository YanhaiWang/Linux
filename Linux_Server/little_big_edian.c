#include <stdio.h>

void func() {
    union {
        int val;
        char bytes;
    }test;
    test.val = 1;

    if(test.bytes == 1) 
        printf("litte edian\n");
    else
        printf("big edian\n");
}


int main() {
    func();
    return 0;
}