#ifndef MYSTL_EXCEPTION_H
#define MYSTL_EXCEPTION_H
#include "std.h"
namespace mystl
{
class exception
{
public:
    exception(){m_pInfo = "";}
    exception(const char*pInfo){
        m_pInfo = pInfo;
    }
    ~exception(){
    }
    const char* get_info(){
        return m_pInfo;
    }
private:
    const char* m_pInfo;
};

class bad_alloc:public exception
{
public:
    bad_alloc(){}
    bad_alloc(const char* pInfo):exception(pInfo){}
};

class bad_paramter:public exception
{
public:
    bad_paramter(){}
};

class container_empty:public exception
{
public:
    container_empty(){}
    container_empty(const char* pInfo):exception(pInfo){}
};
}

#endif