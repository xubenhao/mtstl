#include <stdio.h>
#include <iostream>
#include "mystl_config.h"
#include "heap.h"
void testForHeap(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForHeap\n");
    mystl::array<int> arr;
    arr.push_back(1);
    arr.push_back(3);
    arr.push_back(5);
    arr.push_back(11);
    arr.push_back(12);
    arr.push_back(4);
    arr.push_back(2);
    arr.push_back(6);
    arr.push_back(1);
    arr.push_back(0);

    mystl::heap<int> heap(arr);
    while(heap.empty() == false){
        printf("%d ", heap.top());
        heap.pop();
    }

    // 插入新元素
    heap.push(1);
    heap.push(3);
    heap.push(5);
    heap.push(6);
    heap.push(4);
    heap.push(2);
    while(heap.empty() == false){
        printf("%d ", heap.top());
        heap.pop();
    }
    printf("\nend\n");
}


void testForMaxHeap(){
    printf("ver_mar_%d,min_%d\n", MYSTL_VERSION_MAJOR, MYSTL_VERSION_MINOR);
    printf("testForHeap\n");
    mystl::array<int> arr;
    arr.push_back(1);
    arr.push_back(3);
    arr.push_back(5);
    arr.push_back(11);
    arr.push_back(12);
    arr.push_back(4);
    arr.push_back(2);
    arr.push_back(6);
    arr.push_back(1);
    arr.push_back(0);

    mystl::heap<int, false> heap(arr);
    while(heap.empty() == false){
        printf("%d ", heap.top());
        heap.pop();
    }

    // 插入新元素
    heap.push(1);
    heap.push(3);
    heap.push(5);
    heap.push(6);
    heap.push(4);
    heap.push(2);
    while(heap.empty() == false){
        printf("%d ", heap.top());
        heap.pop();
    }
    printf("\nend\n");
}

int main(){
    testForHeap();
    testForMaxHeap();
    return 0;
}