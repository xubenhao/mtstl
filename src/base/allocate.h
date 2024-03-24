#ifndef MYSTL_ALLOCATOR_H_
#define MYSTL_ALLOCATOR_H_
// 用于获取c++标准库一些基础特性的支持
#include "std.h"
#include "exception.h"
namespace mystl
{
template <class T>
class allocator;
template <class T>
class allocator
{
public:
    // 类型萃取
    typedef T            value_type;// 值
    typedef T*           pointer;// 指针
    typedef const T*     const_pointer;
    typedef T&           reference;// 引用
    typedef const T&     const_reference;
    typedef size_t       size_type;// 对象尺寸，计数
    typedef ptrdiff_t    difference_type;// 对象指针差值

public:
    // 空间申请
    static T*   allocate(){
        T* pRes = NULL;
        try{
            pRes = static_cast<T*>(::operator new(sizeof(T)));
        }
        catch(...){
            throw mystl::bad_alloc("operator new throw exception");
        }

        if(pRes == NULL){
            throw mystl::bad_alloc("operator new return null");
        }
        return pRes;
    }
    static T*   allocate(size_type n){
        if (n == 0){
            throw mystl::bad_paramter();
        }
        T* pRes = NULL;
        try{
            pRes = static_cast<T*>(::operator new(n * sizeof(T)));
        }
        catch(...){
            throw mystl::bad_alloc();
        }
        if(pRes == NULL){
            throw mystl::bad_alloc();
        }
        return pRes;
    }
    // 空间释放
    static void deallocate(T* ptr){
        if (ptr == nullptr)
            return;
        ::operator delete(ptr);// ::operator delete用于空间释放
    }
    static void deallocate(T* ptr, size_type n){
        if (ptr == nullptr)
            return;
        ::operator delete(ptr);
    }
    // 可变模板参数&完美转发
    template <class... Args>
    static void construct(T* ptr, Args&& ...args){
        ::new ((void*)ptr) T(std::forward<Args>(args)...);// // 利用placenew在利用对象起始位置完成对象普通构造
    }
    
    static void destroy(T* first, T* last)
    {
        for (; first != last; ++first)
            destroy(first);
    }
    static void destroy(T* ptr)
    {
        if(ptr){
            ptr->~T();
        }
    }
};
}
#endif