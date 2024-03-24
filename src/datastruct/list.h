#ifndef _MYSTL_LIST_H
#define _MYSTL_LIST_H
#include "../base/allocate.h"
#include "../base/iter.h"
namespace mystl
{
template <class T>
struct list_node
{
public:
    list_node() :m_pPrev(this), m_pNext(this){}
    list_node(const T& v) :m_pPrev(this),m_pNext(this), m_nValue(v) {
    }
    list_node(T&& v) :m_pPrev(this),m_pNext(this),m_nValue(std::move(v)) {
    }
    void unlink() {
        m_pPrev = m_pNext = this;
    }
public:
    list_node* m_pPrev;  // 前一节点
    list_node* m_pNext;  // 下一节点
    T m_nValue;  // 数据域
};

template <class T, class Ptr, class Ref>
struct list_iterator
{
    typedef mystl::bidirectional_iterator_tag           iterator_category;
    typedef T                                           value_type;
    typedef Ptr                                         pointer;
    typedef Ref                                         reference;
public:
    list_node<T>* m_pNode;  // 指向当前节点
public:
    list_iterator():m_pNode(nullptr){}
    explicit list_iterator(list_node<T>* pNode) :m_pNode(pNode) {}
    list_iterator(const list_iterator& rhs) :m_pNode(rhs.m_pNode) {}
    
    reference operator*()  const { return m_pNode->m_nValue; }
    pointer   operator->() const { return &(operator*()); }
    list_iterator& operator++() {
        assert(m_pNode != nullptr);
        m_pNode = m_pNode->m_pNext;
        return *this;
    }
    list_iterator operator++(int) {
        list_iterator tmp = *this;
        ++*this;
        return tmp;
    }
    list_iterator& operator--() {
        assert(m_pNode != nullptr);
        m_pNode = m_pNode->m_pPrev;
        return *this;
    }
    list_iterator operator--(int) {
        list_iterator tmp = *this;
        --*this;
        return tmp;
    }
    bool operator==(const list_iterator& rhs) const { return m_pNode == rhs.m_pNode; }
    bool operator!=(const list_iterator& rhs) const { return m_pNode != rhs.m_pNode; }
};

template<typename T, typename NodeAlloc = mystl::allocator<mystl::list_node<T> > >
class list
{
    friend class list_iterator<T, T*, T&>;
    friend class list_iterator<T, const T*, const T&>;
public:
    typedef NodeAlloc                                allocator_type;
    typedef T                                        value_type;
    typedef T*                                       pointer;
    typedef const T*                                 const_pointer;
    typedef T&                                       reference;
    typedef const T&                                 const_reference;
    typedef uint32_t                                 size_type;
    typedef int32_t                                  difference_type;

    // 迭代器支持
    typedef list_iterator<T, T*, T&>                 iterator;
    typedef list_iterator<T, const T*, const T&>     const_iterator;
    typedef mystl::reverse_iterator<iterator>        reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;
private:
    // 为了模拟尾后概念：双向链表需是带哨兵节点的双向链表。
    list_node<T> m_stBase;// 哨兵节点
    size_type m_nSize;// 链表有效元素个数
public:
    // 行为控制
    list()
    {
        m_nSize = 0;
    }

    explicit list(size_type n){
        m_nSize = 0;
        try{
            for(size_type i = 0; i < n; i++){
                push_back(T());
            }
        }
        catch(...){
            clear();
            throw;
        }
    }

    list(size_type n, const T& val){
        m_nSize = 0;
        try{
            for(size_type i = 0; i < n; i++){
                push_back(val);
            }
        }
        catch(...){
            clear();
            throw;
        }
    }

    list(const list& rhs)
    {
        m_nSize = 0;
        if(rhs.m_nSize == 0){
            return;
        }

        list_node<T>* pFirst = nullptr;
        list_node<T>* pTemp = nullptr; 
        iterator iter = rhs.begin();
        // 循环不变式：
        // 每次迭代开始，pFirst指向临时构造链表首个元素
        // 每次迭代开始，pTemp指向临时构建链表末个元素
        for(; iter != rhs.end(); iter++){
            list_node<T> *pNode = nullptr;
            try{
                pNode = allocator_type::allocate();
                allocator_type::construct(pNode, *iter);
            }
            catch(...){
                if(pNode){
                    allocator_type::deallocate(pNode);
                }

                // 对已经构造的局部链表进行释放
                list_node<T>* pCur = pFirst;
                while(pCur && pCur != pTemp){
                    list_node<T>* pNeedFree = pCur;
                    pCur = pCur->m_pNext;
                    allocator_type::destroy(pNeedFree);
                    allocator_type::deallocate(pNeedFree);
                }

                // 尾元素单独释放
                if(pCur){
                    allocator_type::destroy(pCur);
                    allocator_type::deallocate(pCur);
                }
                throw;
            }

            // 新元素插入临时链表
            if(pTemp){
                list_node<T>* pPrev = pTemp;
                list_node<T>* pNext = pPrev->m_pNext;
                // 前一元素&新元素关系
                pPrev->m_pNext = pNode;
                pNode->m_pPrev = pPrev;
                // 后一元素&新元素
                pNode->m_pNext = pNext;
                pNext->m_pPrev = pNode;
            }else{
                // no nothing
            }
            if(pFirst == nullptr){
                pFirst = pNode;
                pTemp = pFirst;
            }
            else{
                pTemp = pNode;
            }
        }

        // 临时链表合并到有效链表
        m_stBase.m_pNext = pFirst;
        pFirst->m_pPrev = m_stBase;

        m_stBase.m_pPrev = pTemp;
        pTemp->m_pNext = m_stBase;
        m_nSize = rhs.m_nSize;
    }

    list(list&& rhs) noexcept :m_stBase(rhs.m_stBase), m_nSize(rhs.m_nSize) {
        rhs.m_stBase.m_pNext = &rhs.m_stBase;
        rhs.m_stBase.m_pPrev = &rhs.m_stBase;
        rhs.m_nSize = 0;
    }

    list& operator=(const list& rhs) {
        if (this != &rhs) {
            
            list_node<T>* pFirst = nullptr;
            list_node<T>* pTemp = nullptr; 
            iterator iter = rhs.begin();
            for(; iter != rhs.end(); iter++){
                list_node<T> *pNode = nullptr;
                try{
                    pNode = allocator_type::allocate();
                    allocator_type::construct(pNode, *iter);
                }
                catch(...){
                    if(pNode){
                        allocator_type::deallocate(pNode);
                    }

                    list_node<T>* pCur = pFirst;
                    while(pCur && pCur != pTemp){
                        list_node<T>* pNeedFree = pCur;
                        pCur = pCur->m_pNext;
                        allocator_type::destroy(pNeedFree);
                        allocator_type::deallocate(pNeedFree);
                    }
                    if(pCur){
                        allocator_type::destroy(pCur);
                        allocator_type::deallocate(pCur);
                    }
                    throw;
                }

                // 新元素插入临时链表
                if(pTemp){
                    list_node<T>* pPrev = pTemp;
                    list_node<T>* pNext = pPrev->m_pNext;
                    // 前一元素&新元素关系
                    pPrev->m_pNext = pNode;
                    pNode->m_pPrev = pPrev;
                    // 后一元素&新元素
                    pNode->m_pNext = pNext;
                    pNext->m_pPrev = pNode;
                }else{
                    // no nothing
                }
                if(pFirst == nullptr){
                    pFirst = pNode;
                    pTemp = pFirst;
                }
                else{
                    pTemp = pNode;
                }
            }

            
            // 临时链表合并到有效链表
            m_stBase.m_pNext = pFirst;
            pFirst->m_pPrev = m_stBase;

            m_stBase.m_pPrev = pTemp;
            pTemp->m_pNext = m_stBase;
            m_nSize = rhs.m_nSize;
        }
        return *this;
    }

    list& operator=(list&& rhs) noexcept {
        if (this != &rhs) {
            this->~list();
            m_stBase = rhs.m_stBase;
            m_nSize = rhs.m_nSize;
            rhs.m_stBase.m_pNext = &rhs.m_stBase;
            rhs.m_stBase.m_pPrev = &rhs.m_stBase;
            rhs.m_nSize = 0;
        }
        return *this;
    }

    ~list() {
        clear();
    }

public:
    // 迭代器支持
    iterator begin() noexcept
    { return iterator(m_stBase.m_pNext); }
    iterator end() noexcept
    { return iterator(&m_stBase); }
    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }
    
    const_iterator begin() const noexcept
    { return const_iterator(m_stBase.m_pNext); }
    const_iterator end() const noexcept
    { return const_iterator(&m_stBase); }
    const_reverse_iterator rend() const noexcept
    { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept
    { return reverse_iterator(end()); }
    
    // 容量相关操作
    bool empty() const noexcept
    { return m_nSize == 0; }
    size_type size() const noexcept
    { return m_nSize; }

    // 元素访问
    reference front(){
        if(m_nSize == 0){
            throw mystl::container_empty();
        }
        return m_stBase.m_pNext->m_nValue;
    }
    const_reference front() const{
        if(m_nSize == 0){
            throw mystl::container_empty();
        }
        return m_stBase.m_pNext->m_nValue;
    }
    reference back(){
        if(m_nSize == 0){
            throw mystl::container_empty();
        }
        return m_stBase.m_pPrev->m_nValue;
    }
    const_reference back() const{
        if(m_nSize == 0){
            throw mystl::container_empty();
        }
        return m_stBase.m_pPrev->m_nValue;
    }
    
    void push_front(const value_type& value) {
        insert(begin(), value);
    }
    void push_front(value_type&& value) {
        insert(begin(), std::move(value));
    }
    void push_back(const value_type& value) {
        insert(end(), value);
    }
    void push_back(value_type&& value) {
        insert(end(), std::move(value));
    }

    void pop_front() {
        if(m_nSize > 0){
            erase(begin());
        }
    }
    void pop_back() {
        if(m_nSize > 0){
            erase(--end());
        }
    }

    // 插入
    void insert(const iterator& pos, const value_type& value) {
        list_node<T>* pNode = pos.m_pNode;
        insert(pNode, value);
    }
    void insert(const iterator& pos, value_type&& value) {
        list_node<T>* pNode = pos.m_pNode;
        insert(pNode, std::move(value));
    }
    void insert(const const_iterator& pos, const value_type& value) {
        list_node<T>* pNode = pos.m_pNode;
        insert(pNode, value);
    }
    void insert(const const_iterator& pos, value_type&& value) {
        list_node<T>* pNode = pos.m_pNode;
        insert(pNode, std::move(value));
    }

    void erase(const iterator& pos){
        list_node<T>* pNode = pos.m_pNode;
        erase(pNode);
    }
    void erase(const const_iterator& pos){
        list_node<T>* pNode = pos.m_pNode;
        erase(pNode);
    }
    
    void     clear(){
        // 移除所有有效元素
        list_node<T>* pCur = m_stBase.m_pNext;
        while(pCur != &m_stBase){
            list_node<T>* pTmp = pCur;
            pCur = pCur->m_pNext;
            allocator_type::destroy(pTmp);
            allocator_type::deallocate(pTmp);
        }

        m_stBase.m_pNext = &m_stBase;
        m_stBase.m_pPrev = &m_stBase;
        m_nSize = 0;
    }

    void     swap(list& rhs) noexcept {
        list_node<T> stTemp = m_stBase;
        size_type nSize = m_nSize;
        m_stBase = rhs.m_stBase;
        m_nSize = rhs.m_nSize;
        rhs.m_stBase = stTemp;
        rhs.m_nSize = nSize;
    }
private:
    void insert(list_node<T>* pNode, const value_type& nValue){
        if(pNode == nullptr){
            throw mystl::bad_paramter();
        }

        list_node<T>* pNew = nullptr;
        try{
            pNew = allocator_type::allocate();
            allocator_type::construct(pNew, nValue);
        }
        catch(...){
            if(pNew){
                allocator_type::deallocate(pNew);
            }
            throw;
        }

        list_node<T>* pPrev = pNode->m_pPrev;
        list_node<T>* pNext = pNode;
        // 前一元素&新元素关系
        pPrev->m_pNext = pNew;
        pNew->m_pPrev = pPrev;
        // 新元素&后一元素
        pNew->m_pNext = pNext;
        pNext->m_pPrev = pNew;
        m_nSize++;
    }
    void insert(list_node<T>* pNode, value_type&& nValue){
        if(pNode == nullptr){
            throw mystl::bad_paramter();
        }

        list_node<T>* pNew = nullptr;
        try{
            pNew = allocator_type::allocate();
            allocator_type::construct(pNew, std::move(nValue));
        }
        catch(...){
            if(pNew){
                allocator_type::deallocate(pNew);
            }
            throw;
        }

        list_node<T>* pPrev = pNode->m_pPrev;
        list_node<T>* pNext = pNode;
        // 前一元素&新元素关系
        pPrev->m_pNext = pNew;
        pNew->m_pPrev = pPrev;
        // 新元素&后一元素
        pNew->m_pNext = pNext;
        pNext->m_pPrev = pNew;
        m_nSize++;
    }
    void erase(list_node<T>* pNode){
        if(pNode == nullptr){
            throw mystl::bad_paramter();
        }

        list_node<T>* pPrev = pNode->m_pPrev;
        list_node<T>* pNext = pNode->m_pNext;
        pPrev->m_pNext = pNext;
        pNext->m_pPrev = pPrev;
        allocator_type::destroy(pNode);
        allocator_type::deallocate(pNode);
        m_nSize--;
    }
};
}
#endif