
#include <stdio.h>
#include <iostream>

#include "mystl_config.h"
#include "unordered_set.h"
#include "unordered_map.h"

class A{
public:
    A(){
        printf("A()_%x\n", this);
    }
    A(const A&){
        printf("A(const A&)_%x\n", this);
    }
    A(A&&){
        printf("A(A&&)_%x\n", this);
    }
    A& operator=(const A&){
        printf("operator=(const A&)_%x\n", this);
    }
    A& operator=(A&&){
        printf("operator=(A&&)_%x\n", this);
    }
    ~A(){
        printf("~A()\n");
    }
};

void MapTest(){
    printf("MapTest\n");
    mystl::unordered_map<int, int> stMap;
    for(int i = 0; i < 100000; i++){
        auto ret = stMap.insert(i, i);
        if(ret.second == false){
            printf("i_%d,err\n", i);
            break;
        }
    }

    for(int i = 0; i < 100000; i++){
         auto ret = stMap.find(i);
         if((*ret).first != i || (*ret).second != i){
             printf("print_i_%d, fir_%d,sec_%d\n", i, (*ret).first, (*ret).second);
         }
    }

    for(int i = 0; i < 100; i++){
        stMap.erase(i);
        auto ret = stMap.find(i);
        if(ret != stMap.end()){
            printf("erase %d err\n", i);
            break;
        }
    }

    auto ret = stMap.find(1000);
    (*ret).second = 10;

    auto ret2 = stMap.insert(110000, 110000);
    printf("ret2.second_%d\n", ret2.second); 
    ret2 = stMap.insert(110000, 110000);
    printf("ret2.second_%d\n", ret2.second);
}

void MulMapTest(){
    printf("MulMapTest\n");
    mystl::unordered_mulmap<int, int> stMap;
    for(int i = 0; i < 100000; i++){
        stMap.insert(i, i);
    }

    for(int i = 0; i < 100000; i++){
         auto ret = stMap.find(i);
         if((*ret).first != i || (*ret).second != i){
             printf("print_i_%d, fir_%d,sec_%d\n", i, (*ret).first, (*ret).second);
         }
    }

    for(int i = 0; i < 100; i++){
        stMap.erase(i);
        auto ret = stMap.find(i);
        if(ret != stMap.end()){
            printf("erase %d err\n", i);
            break;
        }
    }

    auto ret = stMap.find(1000);
    (*ret).second = 10;

    stMap.insert(110000, 110000);
    stMap.insert(110000, 110001);
    stMap.insert(110000, 110002);
    stMap.insert(110000, 110003);
    auto ret2 = stMap.find_range(110000);
    for(auto iter = ret2.first; true; iter++){
        printf("key_%d,value_%d\n", iter->first, iter->second);
        if(iter == ret2.second){
            break;
        }
    }
}

void SetTest(){
    printf("SetTest\n");
    mystl::unordered_set<int> stMap;
    for(int i = 0; i < 100000; i++){
        auto ret = stMap.insert(i);
        if(ret.second == false){
            printf("i_%d,err\n", i);
            break;
        }
    }

    for(int i = 0; i < 100000; i++){
         auto ret = stMap.find(i);
         if((*ret) != i){
             printf("print_i_%d, ele_%d\n", i, (*ret));
         }
    }

    for(int i = 0; i < 100; i++){
        stMap.erase(i);
        auto ret = stMap.find(i);
        if(ret != stMap.end()){
            printf("erase %d err\n", i);
            break;
        }
    }

    auto ret = stMap.find(1000);
    //(*ret) = 10;

    auto ret2 = stMap.insert(110000);
    printf("ret2.second_%d\n", ret2.second); 
    ret2 = stMap.insert(110000);
    printf("ret2.second_%d\n", ret2.second);
}

void MulSetTest(){
    printf("MulSetTest\n");
    mystl::unordered_mulset<int> stMap;
    for(int i = 0; i < 100000; i++){
        stMap.insert(i);
    }

    for(int i = 0; i < 100000; i++){
         auto ret = stMap.find(i);
         if((*ret) != i){
             printf("print_i_%d, ele_%d\n", i, (*ret));
         }
    }

    for(int i = 0; i < 100; i++){
        stMap.erase(i);
        auto ret = stMap.find(i);
        if(ret != stMap.end()){
            printf("erase %d err\n", i);
            break;
        }
    }

    auto ret = stMap.find(1000);
    //(*ret) = 10;

    stMap.insert(110000);
    stMap.insert(110000);
    stMap.insert(110000);
    stMap.insert(110000);
    auto ret2 = stMap.find_range(110000);
    for(auto iter = ret2.first; true; iter++){
        printf("key_%d\n", *iter);
        if(iter == ret2.second){
            break;
        }
    }
}

void TestMapClass(){
    printf("TestMapClass\n");
    mystl::unordered_map<int, A> stMap;
    for(int i = 0; i < 2; i++){
        auto ret = stMap.insert(i, A());
        if(ret.second == false){
            printf("i_%d,err\n", i);
            break;
        }
    }

    for(int i = 0; i < 2; i++){
         auto ret = stMap.find(i);
         if((*ret).first != i){
             printf("print_i_%d, fir_%d\n", i, (*ret).first);
         }
    }

    for(int i = 0; i < 2; i++){
        stMap.erase(i);
        auto ret = stMap.find(i);
        if(ret != stMap.end()){
            printf("erase %d err\n", i);
            break;
        }
    }

    //auto ret = stMap.find(1000);
    //(*ret).second = A();

    auto ret2 = stMap.insert(110000, A());
    printf("ret2.second_%d\n", ret2.second); 
    ret2 = stMap.insert(110000, A());
    printf("ret2.second_%d\n", ret2.second);
}

int main(){
    MapTest();
    MulMapTest();
    SetTest();
    MulSetTest();
    TestMapClass();
    return 0;
}


/*#include <map>
#include <set>
template <class T>
struct ele_traits {
	typedef T                           	key_type;
	typedef T                               value_type;
	typedef T          	                    ele_type;
	static const key_type& get_key(const ele_type& nEle) {
		return nEle.first;
	}
	static const value_type& get_value(const ele_type& nEle) {
		return nEle.second;
	}
};
template <class T1, class T2>
struct ele_traits<std::pair<T1,T2>> {
	typedef T1                                   key_type;
	typedef T2                           		 value_type;
	typedef mystl::pair<T1,T2>          	     ele_type;
	static const key_type& get_key(const ele_type& nEle) {
		return nEle.first;
	}
	static const value_type& get_value(const ele_type& nEle) {
		return nEle.second;
	}
};

class A{
public:
    void fun(){
        printf("fun\n");
    }
    void fun() const{
        printf("fun const\n");
    }
};

class B{
public:
    void funB(){
        a.fun();
    }
    void funB() const{
        a.fun();
    }
private:
    A a;
};

template <class T>
class TA{
public:
    typedef T        Ele;
    typedef const T* Ptr;
    typedef const T& Ref;

};

int main(){
    //MapTest();
    //MulMapTest();
    //SetTest();
    //MulSetTest();
    //TestMapClass();
    ele_traits<int>::key_type i = 10;
    i = 11;
    ele_traits<int>::value_type v = 10;
    v = 11;
    ele_traits<int>::ele_type e = 10;
    e = 11;
    ele_traits<const int> t2;
    ele_traits<const int>::key_type ci = 10;
    //ci = 11;
    ele_traits<const int>::value_type vi = 10;
    //vi = 11;
    ele_traits<const int>::ele_type ei = 10;
    //ei = 11;
    //ele_traits<std::pair<int, int>> t3;
    const ele_traits<const int>::ele_type &cei = ei;
    const int* pi = &ei;
    typedef const int CINT;
    const CINT* cpi = &ei;
    cpi = &ei;
    B b;
    const B cb;
    b.funB();
    cb.funB();
    TA<int>::Ele t_i = 10;
    TA<int>::Ptr t_p = &t_i;
    TA<int>::Ref t_r = 10;
    
    TA<const int>::Ele t_i = 10;
    TA<const int>::Ptr t_p = &t_i;
    TA<const int>::Ref t_r = 10;
    
    return 0;
}*/