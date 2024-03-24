#ifndef MYSTL_ITER_H_
#define MYSTL_ITER_H_
#include "std.h"
namespace mystl
{
template <class T, T v>
struct m_integral_constant 
{
    static constexpr T value = v;
};

template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;

typedef m_bool_constant<true>  m_true_type;
typedef m_bool_constant<false> m_false_type;

template <class T1, class T2>
struct pair
{
    typedef T1    first_type;
    typedef T2    second_type;
    first_type first;   
    second_type second; 
    pair(const T1& key, const T2& value):first(key),second(value){
    }
    
    pair(const pair& rhs) = default;
    pair(pair&& rhs) = default;
    pair& operator=(const pair& rhs)
    {
        if (this != &rhs)
        {
            first = rhs.first;
            second = rhs.second;
        }
        return *this;
    }

    pair& operator=(pair&& rhs)
    {
        if (this != &rhs)
        {
            first = std::move(rhs.first);
            second = std::move(rhs.second);
        }
        return *this;
    }

    ~pair() = default;
};

template <class T>
struct is_pair : mystl::m_false_type {};

template <class T1, class T2>
struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type {};

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator
{
    typedef Category                             iterator_category;
    typedef T                                    value_type;
    typedef Pointer                              pointer;
    typedef Reference                            reference;
    typedef Distance                             difference_type;
};

template <class T>
struct has_iterator_cat
{
private:
    struct two { char a; char b; };

    template <class U>
    static two test(...);
    template <class U>
    static char test(typename U::iterator_category* = 0);
public:
    // T包含内置iterator_category类别定义时value为true。否则，为false。
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
    typedef typename Iterator::difference_type   difference_type;
};

template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true>
    : public iterator_traits_impl<Iterator, std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value || std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
{
};

// 当Iterator包含内置iterator_category类别时，通过集成获得Iterator的iterator_category，...，difference_type等迭代器内置类别
template <class Iterator>
struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

template <class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag           iterator_category;
    typedef T                                    value_type;
    typedef T*                                   pointer;
    typedef T&                                   reference;
    typedef ptrdiff_t                            difference_type;
};

template <class T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag           iterator_category;
    typedef T                                    value_type;
    typedef const T*                             pointer;
    typedef const T&                             reference;
    typedef ptrdiff_t                            difference_type;
};

// T包含内置iterator_category类别时为true
// 利用模板特化匹配规则
// bool为true时，匹配这里。这样在T的内置iterator_category类别可隐式转换为U时为m_bool_constant<true>。否则为m_bool_constant<false>
// T不含内置iterator_category类别时为false
// 利用模板特化匹配规则，匹配到has_iterator_cat_of<T, U, false>
template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
    : public m_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category,U>::value>
{
};

template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

// 当且仅当Iter含内置iterator_category类别，且此类别可隐式转换为input_iterator_tag时，会继承m_bool_constant<true>。
// 否则，继承m_bool_constant<false>
template <class Iter>
struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value || is_output_iterator<Iterator>::value>
{
};

// 模板函数
// 返回一个Iterator内置iterator_category的实例。若Iterator不含内置iterator_category将引发编译时报错。
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

// 模板函数
// 返回一个指针，指向Iterator内置difference_type类型
template <class Iterator>
typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 模板函数
// 返回一个指针，指向Iterator内置value_type类型
template <class Iterator>
typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 模板函数
// 计算first和last间的距离。
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
        ++first;
        ++n;
    }
    return n;
}

// 模板函数
// 计算first和last间的距离。
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type distance_dispatch(RandomIter first, RandomIter last, random_access_iterator_tag)
{
    return last - first;
}

// 模板函数
// 计算first和last间的距离，逻辑上等价于last - first。
// 利用模板函数重载可选择效率高的方式执行
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
{
    return distance_dispatch(first, last, iterator_category(first));
}

// 模板函数
// 对i执行n次前向迭代
template <class InputIterator, class Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
{
    while (n--)
        ++i;
}

// 模板函数
// 对i执行n次迭代（n可以为负数）
template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0)
        while (n--)  ++i;
    else
        while (n++)  --i;
}

// 模板函数
// 对i执行n次迭代（n可以为负数）
template <class RandomIter, class Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
{
    i += n;
}

// 模板函数
// 对i执行n次迭代
// 利用模板函数重载采用高效版本实现，且结合参数能力提供能力相匹配的功能
template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n)
{
    advance_dispatch(i, n, iterator_category(i));
}

template <class Iterator>
class reverse_iterator
{
private:
  Iterator current;  // 记录对应的正向迭代器

public:
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type        value_type;
    typedef typename iterator_traits<Iterator>::difference_type   difference_type;
    typedef typename iterator_traits<Iterator>::pointer           pointer;
    typedef typename iterator_traits<Iterator>::reference         reference;
    typedef Iterator                                              iterator_type;
    typedef reverse_iterator<Iterator>                            self;

public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type i) :current(i) {}
    reverse_iterator(const self& rhs) :current(rhs.current) {}

public:
    iterator_type base() const
    { return current; }

    reference operator*() const
    { // 实际对应正向迭代器的前一个位置
        auto tmp = current;
        return *--tmp;
    }
    pointer operator->() const
    {
        return &(operator*());
    }

    // 前进(++)变为后退(--)
    self& operator++()
    {
        --current;
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        --current;
        return tmp;
    }
    // 后退(--)变为前进(++)
    self& operator--()
    {
        ++current;
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator+=(difference_type n)
    {
        current -= n;
        return *this;
    }
    self operator+(difference_type n) const
    {
        return self(current - n);
    }
    self& operator-=(difference_type n)
    {
        current += n;
        return *this;
    }
    self operator-(difference_type n) const
    {
        return self(current + n);
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }
};

// 重载 operator-
template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return rhs.base() - lhs.base();
}

// 重载比较操作符
template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return lhs.base() == rhs.base();
}

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return rhs.base() < lhs.base();
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return !(lhs == rhs);
}

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return rhs < lhs;
}

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return !(rhs < lhs);
}

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
{
    return !(lhs < rhs);
}
}
#endif