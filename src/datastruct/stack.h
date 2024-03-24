#ifndef _MYSTL_STACK_H_
#define _MYSTL_STACK_H_
#include "array.h"
namespace mystl
{
template <class T, class Container = mystl::array<T> >
class stack
{
public:
    typedef Container                           container_type;
    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;
private:
    container_type m_stContainer;
public:
    stack() = default;
    stack(const Container& c)
        :m_stContainer(c)
    {
    }
    stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
        :m_stContainer(std::move(c))
    {
    }

    stack(const stack& rhs)
        :m_stContainer(rhs.m_stContainer)
    {
    }
    stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
        :m_stContainer(std::move(rhs.m_stContainer))
    {
    }

    stack& operator=(const stack& rhs)
    {
        m_stContainer = rhs.m_stContainer;
        return *this;
    }
    stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value)
    {
        m_stContainer = std::move(rhs.m_stContainer);
        return *this;
    }

    ~stack() = default;
    // 仅仅查看
    reference       top()       {
        if(empty()){
            throw mystl::container_empty("container is empty");
        }
        else{
            auto iter = m_stContainer.end();
            return *(--iter);
        }
    }
    const_reference top() const {
        if(empty()){
            throw mystl::container_empty();
        }
        else{
            auto iter = m_stContainer.end();
            return *(--iter);
        }
    }

    // 容量相关操作
    bool      empty() const noexcept { return m_stContainer.empty(); }
    size_type size()  const noexcept { return m_stContainer.size(); }

    void push(const value_type& value)
    { m_stContainer.push_back(value); }
    void push(value_type&& value)
    { m_stContainer.push_back(std::move(value)); }
    void pop()
    { m_stContainer.pop_back(); }

    void clear()
    {
        while (!empty())
            pop();
    }

    void swap(stack& rhs) 
    { m_stContainer.swap(rhs.m_stContainer); }

    bool operator==(const stack& rhs){
        return m_stContainer == rhs.m_stContainer;
    }
    bool operator<(const stack& rhs){
        return m_stContainer < rhs.m_stContainer;
    }
};

template <class T, class Container>
bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.operator==(rhs);
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.operator<(rhs);
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return !(lhs < rhs);
}
}
#endif