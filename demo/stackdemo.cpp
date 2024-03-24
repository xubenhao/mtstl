#include <stdio.h>
#include <iostream>
#include "mystl_config.h"
#include "stack.h"
#include "list.h"
void testForArray(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForArray\n");
    mystl::stack<int, mystl::array<int>> arr1;
    for(int i = 0; i < 20; i++){
        arr1.push(i);
    }
    
    while(!arr1.empty()){
        printf("%d ", arr1.top());
        arr1.pop();
    }
    printf("\nend\n");
}

void testForList(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForList\n");
    mystl::stack<int, mystl::list<int>> arr1;
    for(int i = 0; i < 20; i++){
        arr1.push(i);
    }
    
    while(!arr1.empty()){
        printf("%d ", arr1.top());
        arr1.pop();
    }
    printf("\nend\n");
}

int main(){
    testForArray();
    testForList();
    return 0;
}