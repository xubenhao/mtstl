#ifndef _MYSTL_ARRAT_H
#define _MYSTL_ARRAY_H
#include "../base/allocate.h"
#include "../base/iter.h"
namespace mystl
{
// 关于迭代器需明确的几个方面：
// (1). 迭代器存在的意义
// 抽象出一套通用的访问方法－－泛型编程的要求
// 具体到数组的迭代器，由于元素指针已经具备需要实现的随机访问类型迭代器的所有访问接口．
// 所以，不必额外实现迭代器类型，直接元素指针作为迭代器即可．
// 此时，虽然迭代器类型的实现工作免了，但容器内部别名类型iterator，const_iterator不可免．
// 因为迭代器是抽象出来给容器外部用户看和使用的．
// (2). 有了常规迭代器，还要提供常量迭代器的意义
// 对于const修饰的容器类型，我们希望通过迭代器实现容器访问，但普通迭代器包含修改元素的方法．
// 此时，只能用常量迭代器．常量迭代器所有方法可保证不会修改元素和关联容器．

// 关于常量的容易混淆处说明：
// (1). 迭代器对象用const修饰，是在限制无法修改对象内字段．
// (2). 常量迭代器中常量，是在限制无法通过此类型迭代器去修改关联容器对象．
// (3). 容器对象用const修饰，是在限制无法修改此容器对象（无法修改容器对象各个成员）．
template<typename T, typename Alloc = mystl::allocator<T> >
class array
{
public:
    typedef Alloc                      				 allocator_type;
    typedef T      									 value_type;
    typedef T*         								 pointer;
    typedef const T*   								 const_pointer;
    typedef T&       								 reference;
    typedef const T& 								 const_reference;
    typedef uint32_t       							 size_type;
    typedef int32_t 								 difference_type;

    typedef T*                	 					 iterator;
    typedef const T*    	 						 const_iterator;
    typedef mystl::reverse_iterator<iterator>        reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;
private:
	T* m_pSource;
	size_type m_nSize;  // 大小
	size_type m_nCapacity; // 容量

public:
	// 1.类型行为控制
    // 构造与拷贝构造
	array() {
		m_pSource = nullptr;
		m_nSize = 0;
		m_nCapacity = 0;

		T* pTmp = nullptr;
		try
		{
			pTmp = allocator_type::allocate(16);
		}
		catch (...)
		{
			throw;
		}
		m_pSource = pTmp;
		m_nCapacity = 16;
	}
	explicit array(size_type n){
		m_pSource = nullptr;
		m_nSize = 0;
		m_nCapacity = 0;
		
		size_type cap = n > 16 ? n:16;
		size_type i = 0;
		T* pTmp = NULL;
		try
		{
			pTmp = allocator_type::allocate(cap);
			for(; i < cap; i++){
				allocator_type::construct(pTmp + i);
			}
		}
		catch (...)
		{
			if(i > 0){
				for(size_type k = 0; k < i - 1; k++){
					allocator_type::destroy(pTmp+k);
				}
				allocator_type::deallocate(pTmp);
			}
			throw;
		}

		m_pSource = pTmp;
		m_nSize = cap;
		m_nCapacity = cap;
	}
	array(size_type nInitialSize_, const T& nInitialValue_)
	{
		m_pSource = nullptr;
		m_nSize = 0;
		m_nCapacity = 0;

		T* pTmp = NULL;
		size_t i = 0;
		size_t cap = nInitialSize_ > 16 ? nInitialSize_:16;
		try
		{
			pTmp = allocator_type::allocate(cap);
			for(; i < cap; i++){
				allocator_type::construct(pTmp + i, nInitialValue_);
			}
		}
		catch (...)
		{
			if(i > 0){
				for(size_t k = 0; k < i - 1; k++){
					allocator_type::destroy(pTmp+k);
				}
				allocator_type::deallocate(pTmp);
			}
			throw;
		}

		m_pSource = pTmp;
		m_nSize = cap;
		m_nCapacity = cap;
	}
    array(const array& arrElements_){
		m_pSource = nullptr;
		m_nSize = 0;
		m_nCapacity = 0;

		T* pTmp = nullptr;
		size_t i = 0;
		try
		{
			pTmp = allocator_type::allocate(arrElements_.m_nCapacity);
			for(; i < arrElements_.m_nSize; i++){
				allocator_type::construct(pTmp + i, arrElements_[i]);
			}
		}
		catch (...)
		{
			if(i > 0){
				for(size_t k = 0; k < i - 1; k++){
					allocator_type::destroy(pTmp+k);
				}
				allocator_type::deallocate(pTmp);
			}
			throw;
		}

		m_pSource = pTmp;
		m_nSize = arrElements_.m_nSize;
		m_nCapacity = arrElements_.m_nCapacity;
	}
	array(array&& rhs):m_pSource(rhs.m_pSource), m_nCapacity(rhs.m_nCapacity), m_nSize(rhs.m_nSize){
		rhs.m_pSource = nullptr;
		rhs.m_nCapacity = 0;
		rhs.m_nSize = 0;
	}
	// 赋值与移动赋值
    array& operator=(const array& arrElements_){
		if(this == &arrElements_){
			return *this;
		}
		
		T* pTmp = nullptr;
		size_t i = 0;
		try
		{
			pTmp = allocator_type::allocate(arrElements_.m_nCapacity);
			for(; i < arrElements_.m_nSize; i++){
				allocator_type::construct(pTmp + i, arrElements_[i]);
			}
		}
		catch (...)
		{
			if(i > 0){
				for(size_t k = 0; k < i; k++){
					allocator_type::destroy(pTmp+k);
				}
				allocator_type::deallocate(pTmp);
			}
			throw;
		}

		this->~array();
		m_pSource = pTmp;
		m_nCapacity = arrElements_.m_nCapacity;
		m_nSize = arrElements_.m_nSize;
		return *this;
	}
	array& operator=(array&& arrElements_) noexcept{
		if(this == &arrElements_){
			return *this;
		}
		
		this->~array();
		m_pSource = arrElements_.m_pSource;
		m_nCapacity = arrElements_.m_nCapacity;
		m_nSize = arrElements_.m_nSize;
		arrElements_.m_pSource = nullptr;
		arrElements_.m_nCapacity = 0;
		arrElements_.m_nSize = 0;
		return *this;
	}
	// 析构
    ~array(){
		clear();
		if(m_pSource){
			allocator_type::deallocate(m_pSource);
		}
		m_pSource = nullptr;
		m_nSize = 0;
		m_nCapacity = 0;
	}

	// 2.迭代器支持
    iterator               begin()         noexcept
    { return  iterator(m_pSource); }
	iterator               end()           noexcept
    { return iterator(m_pSource+m_nSize); }
    reverse_iterator       rbegin()        noexcept
    { return reverse_iterator(end()); }
	reverse_iterator       rend()          noexcept
    { return reverse_iterator(begin()); }
    

    const_iterator         begin()   const noexcept
    { return const_iterator(m_pSource); }
    const_iterator         end()     const noexcept
    { return const_iterator(m_pSource+m_nSize); }
    const_reverse_iterator rbegin()  const noexcept
    { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()    const noexcept
    { return const_reverse_iterator(begin()); }

	// 2.对外接口集合
	bool      empty()    const noexcept
    { return m_nSize == 0; }
    size_type size()     const noexcept
    { return m_nSize; }
    size_type capacity() const noexcept
    { return m_nCapacity; }

	// 提供容量保证
    void      reserve(size_type n){
		if(n <= m_nCapacity){
			return;
		}

		T* pTmp = nullptr;
		try
		{
			pTmp = allocator_type::allocate(n);
		}
		catch (...)
		{
			throw;
		}

		for(size_type i = 0; i < m_nSize; i++){
			allocator_type::construct(pTmp + i, std::move(m_pSource[i]));
		}
		
		size_type nSize = m_nSize;
		this->~array();
		m_pSource = pTmp;
		m_nCapacity = n;
		m_nSize = nSize;
	}
	// 释放多余空间
    void      shrink_to_fit()
	{
		if(m_nSize > 16 && m_nSize < m_nCapacity){
			T* pTmp = nullptr;
			try
			{
				pTmp = allocator_type::allocate(m_nSize);
			}
			catch (...)
			{
				throw;
			}

			for(size_type i = 0; i < m_nSize; i++){
				allocator_type::construct(pTmp + i, std::move(m_pSource[i]));
			}
			
			size_type nSize = m_nSize;
			this->~array();
			m_pSource = pTmp;
			m_nCapacity = nSize;
			m_nSize = nSize;
		}
	}
	// 下标访问
	reference operator[](size_type n)
    {
        assert(n < size());
        return *(m_pSource + n);
    }
    const_reference operator[](size_type n) const
    {
        assert(n < size());
        return *(m_pSource + n);
    }

	// 元素访问
	reference front(){
		if(m_nSize == 0){
			throw mystl::container_empty();
		}
		return *m_pSource;
	}
	const_reference front() const{
		if(m_nSize == 0){
			throw mystl::container_empty();
		}
		return *m_pSource;
	}
	reference back(){
		if(m_nSize == 0){
			throw mystl::container_empty();
		}
		return *(m_pSource+m_nSize-1);
	}
	const_reference back() const{
		if(m_nSize == 0){
			throw mystl::container_empty();
		}
		return *(m_pSource+m_nSize-1);
	}

	// 插入
	void push_front(const value_type& value){
		insert(begin(), value);
	}
	void push_front(value_type&& value){
		insert(begin(), value);
	}
	void push_back(const value_type& value)
	{
		insert(end(), value);
	}
    void push_back(value_type&& value)
    {
		insert(end(), std::move(value));
	}

	void pop_front(){
		if(m_nSize > 0){
			erase(begin());
		}	
	}
	void pop_back()
    {
		if(m_nSize > 0){
			erase(end() - 1);
		}
	}
	
	// 在pos代表位置插入新值
	// 迭代器对象作为容器方法的参数：
	// (1).对容器来说，其了解迭代器内部实现
	// (2).对容器来说，迭代器只是起指示位置的作用
	// 迭代器存在的意义是对容器外部提供一致性访问容器的方法，以便实现泛型编程．
	// 容器内部访问自身，没必要使用迭代器的方法绕一圈．
	void insert(const iterator& pos, const value_type& value){
		insert(pos - m_pSource, value);
	}
	void insert(const const_iterator& pos, const value_type& value){
		insert(pos - m_pSource, value);
	}
	void insert(const iterator& pos, value_type&& value){
		insert(pos - m_pSource, std::move(value));
	}
    void insert(const const_iterator& pos, value_type&& value){
		insert(pos - m_pSource, std::move(value));
	}
	// 移除
	void erase(const iterator& pos){
		erase(pos - m_pSource);
	}
	void erase(const const_iterator& pos){
		erase(pos - m_pSource);
	}
	
    void     clear() {
		for(size_t k = 0; k < m_nSize; k++){
			allocator_type::destroy(m_pSource+k);
		}
		m_nSize = 0;
	}

	// 交换
	void     swap(array& rhs) noexcept{
		if(this != &rhs){
			T* pSource = rhs.m_pSource;
			size_type nSize = rhs.m_nSize;
			size_type nCap = rhs.m_nCapacity;
			rhs.m_pSource = m_pSource;
			rhs.m_nSize = m_nSize;
			rhs.m_nCapacity = m_nCapacity;
			m_pSource = pSource;
			m_nSize = nSize;
			m_nCapacity = nCap;
		}
	}
private:
	void insert(size_type pos, const value_type& value){
		if(pos > m_nSize){
			throw mystl::bad_paramter();
		}
		size_type nOff = pos;
		if(m_nSize == m_nCapacity){
			size_type cap = m_nCapacity+m_nCapacity/2;
			cap = cap > 16 ? cap : 16;
			reserve(cap);
		}
		allocator_type::construct(m_pSource+m_nSize);
		if(m_nSize > 0){
			for(long long off = m_nSize-1; off >= nOff; off--){
				*(m_pSource+off+1) = std::move(*(m_pSource+off));
			}
		}
		
		*(m_pSource+nOff) = value;
		m_nSize++;
	}
	void insert(size_type pos, value_type&& value){
		if(pos > m_nSize){
			throw mystl::bad_paramter();
		}
		size_type nOff = pos;
		if(m_nSize == m_nCapacity){
			size_type cap = m_nCapacity+m_nCapacity/2;
			cap = cap > 16 ? cap : 16;
			reserve(cap);
		}
		allocator_type::construct(m_pSource+m_nSize);
		if(m_nSize > 0){
			for(long long off = m_nSize-1; off >= nOff; off--){
				*(m_pSource+off+1) = std::move(*(m_pSource+off));
			}
		}
		
		*(m_pSource+nOff) = std::move(value);
		m_nSize++;
	}
	void erase(size_type pos){
		if(pos >= m_nSize){
			throw mystl::bad_paramter();
		}
		// 前移
		for(T* pCur = m_pSource+pos+1; pCur < m_pSource+m_nSize; pCur++){
			*(pCur-1) = std::move(*pCur);
		}
		// 析构
		allocator_type::destroy(m_pSource+m_nSize-1);
		m_nSize--;
	}
};
}
#endif

