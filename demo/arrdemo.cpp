#include <stdio.h>
#include <iostream>
#include "mystl_config.h"
#include "array.h"

class NonA1{
public:
    int i;
};

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
void testForInt(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForInt\n");
    mystl::array<int> arr1;
    for(int i = 0; i < 20; i++){
        arr1.push_back(i);
    }
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        printf("%d\t", *iter);
    }
    printf("\nsize_%d,cap_%d\n", arr1.size(), arr1.capacity());
    auto iter2 = arr1.begin();
    for(; iter2 != arr1.end(); iter2++){
        if(*iter2 == 5){
            break;
        }
    }
    printf("%d\n", *iter2);
    arr1.erase(iter2);
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        printf("%d\t", *iter);
    }
    printf("\nend\n");
}
void testForPointer(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForPointer\n");
    mystl::array<int*> arr1;
    for(int i = 0; i < 20; i++){
        int* pInt = new int(i);
        arr1.push_back(pInt);
    }
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        printf("*%x=%d\t", *iter, **iter);
    }
    printf("\nsize_%d,cap_%d\n", arr1.size(), arr1.capacity());
    auto iter2 = arr1.begin();
    for(; iter2 != arr1.end(); iter2++){
        if(**iter2 == 5){
            break;
        }
    }
    printf("%x=%d\n", *iter2, **iter2);
    arr1.erase(iter2);
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        printf("%x=%d\t", *iter,**iter);
    }
    printf("\nend\n");
}
void testForNonA(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForNonA\n");
    mystl::array<NonA1> arr1;
    for(int i = 0; i < 20; i++){
        arr1.push_back(NonA1());
    }
    int k = 10;
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        (*iter).i = k++;
        printf("%d\t", (*iter).i);
    }
    printf("\nsize_%d,cap_%d\n", arr1.size(), arr1.capacity());
    auto iter2 = arr1.begin();
    for(; iter2 != arr1.end(); iter2++){
        if((*iter2).i == 15){
            break;
        }
    }
    printf("%d\n", (*iter2).i);
    arr1.erase(iter2);
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        printf("%d\t", (*iter).i);
    }
    printf("\nend\n");
}
void testForA1(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForA1\n");
    mystl::array<A1> arr1;
    for(int i = 0; i < 20; i++){
        arr1.push_back(A1(i));
    }
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        printf("%d-%d\t", (*iter).i, *((*iter).pInt));
    }
    printf("\nsize_%d,cap_%d\n", arr1.size(), arr1.capacity());
    auto iter2 = arr1.begin();
    for(; iter2 != arr1.end(); iter2++){
        if((*iter2).i == 5){
            break;
        }
    }
    printf("%d-%d\n", (*iter2).i, *((*iter2).pInt));
    arr1.erase(iter2);
    for(auto iter = arr1.begin(); iter != arr1.end(); iter++){
        printf("%d=%d\t", (*iter).i, *((*iter).pInt));
    }
    printf("\nend\n");
}
int main(){
    testForInt();
    testForPointer();
    testForNonA();
    testForA1();
    return 0;
}