#include <stdio.h>
#include <iostream>
#include "mystl_config.h"
#include "queue.h"
#include "list.h"
class A1{
public:
    A1(){
        i = 0;
        pInt = new int(i);
        printf("A1()_%d_%x_%d\n", i, pInt, *pInt);
    }
    A1(int k){
        i = k;
        pInt = new int(i);
        printf("A1(int)_%d_%x_%d\n", i, pInt, *pInt);
    }
    A1(const A1& a){
        i = a.i;
        pInt = new int(a.i);
        printf("A1(A1&)_%d_%x_%d\n", i, pInt, *pInt);
    }
    A1(A1&& a){
        i = a.i;
        if(pInt)
            delete pInt;
        pInt = a.pInt;
        a.i = 0;
        a.pInt = nullptr;
        printf("A1(A1&&)_%d_%x_%d\n", i, pInt, *pInt);
    }
    A1& operator=(const A1&a){
        i = a.i;
        *pInt = *(a.pInt);
        printf("operator=(const A1&)_%d_%x_%d\n", i, pInt, *pInt);
    }
    A1& operator=(A1&&a){
        i = a.i;
        if(pInt)
            delete pInt;
        pInt = a.pInt;
        a.i = 0;
        a.pInt = nullptr;
        printf("operator=(A1&&)_%d_%x_%d\n", i, pInt, *pInt);
    }
    ~A1(){
        if(pInt) {
            printf("~A1()_%d_%x_%d\n", i, pInt, *pInt);
        }
        else{
            printf("~A1()_%d_null_null\n", i);
        }
        if(pInt){
            delete pInt;
            pInt = nullptr;
        }
    }
public:
    int i = 10;
    int* pInt = nullptr;
};

void testForArray(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForInt\n");
    mystl::queue<int, mystl::array<int>> arr1;
    for(int i = 0; i < 20; i++){
        arr1.push(i);
    }
    
    while(!arr1.empty()){
        printf("%d ", arr1.front());
        arr1.pop();
    }
    printf("\nend\n");
}

void testForList(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForList\n");
    mystl::queue<int, mystl::list<int>> arr1;
    for(int i = 0; i < 20; i++){
        arr1.push(i);
    }
    
    while(!arr1.empty()){
        printf("%d ", arr1.front());
        arr1.pop();
    }
    printf("\nend\n");
}

int main(){
    testForArray();
    testForList();
    return 0;
}