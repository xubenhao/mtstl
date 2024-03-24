#ifndef MYTINYSTL_QUEUE_H_
#define MYTINYSTL_QUEUE_H_
#include "array.h"
namespace mystl
{
template <class T, class Container = mystl::array<T> >
class queue
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
    queue() = default;
    explicit queue(size_type n) :m_stContainer(n)
    {
    }
    queue(size_type n, const value_type& value) :m_stContainer(n, value)
    {
    }

    queue(const Container& c) :m_stContainer(c)
    {
    }
    queue(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
        :m_stContainer(std::move(c))
    {
    }

    queue(const queue& rhs)
        :m_stContainer(rhs.m_stContainer)
    {
    }
    queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
        :m_stContainer(std::move(rhs.m_stContainer))
    {
    }

    queue& operator=(const queue& rhs)
    {
        m_stContainer = rhs.m_stContainer;
        return *this;
    }
    queue& operator=(queue&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value)
    {
        m_stContainer = std::move(rhs.m_stContainer);
        return *this;
    }

    ~queue() = default;

    // 访问元素相关操作
    reference       front()       { return m_stContainer.front(); }
    const_reference front() const { return m_stContainer.front(); }
    reference       back()        { return m_stContainer.back(); }
    const_reference back()  const { return m_stContainer.back(); }

    // 容量相关操作
    bool      empty() const noexcept { return m_stContainer.empty(); }
    size_type size()  const noexcept { return m_stContainer.size(); }

    void push(const value_type& value)
    { m_stContainer.push_back(value); }
    void push(value_type&& value)
    { m_stContainer.push_back(std::move(value)); }

    void pop()
    { m_stContainer.pop_front(); }

    void clear()
    {
        while (!empty())
            pop();
    }

    void swap(queue& rhs)
    { m_stContainer.swap(rhs.m_stContainer); }

    bool operator==(const queue& rhs){
        return m_stContainer == rhs.m_stContainer;
    }
    bool operator<(const queue& rhs){
        return m_stContainer < rhs.m_stContainer;
    }
};

template <class T, class Container>
bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
    return lhs.operator==(rhs);
}

template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
    return lhs.operator<(rhs);
}

template <class T, class Container>
bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
    return !(lhs < rhs);
}

}
#endif