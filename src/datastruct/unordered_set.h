#ifndef MYSTL_UNORDERED_SET_H_
#define MYSTL_UNORDERED_SET_H_
#include "hashtable.h"
namespace mystl
{
template <class Key, class Alloc = mystl::allocator<hashtable_node<const Key>>>
class unordered_set
{
private:
    typedef hashtable<const Key, Alloc> base_type;
    base_type m_stHt;

public:
    // 这样导出的是常量类型
    typedef typename base_type::key_type             key_type;
    typedef typename base_type::value_type           value_type;
    typedef typename base_type::ele_type             ele_type;
    typedef typename base_type::hash_fun             hash_fun;
    typedef typename base_type::key_fun              key_fun;

    typedef typename base_type::size_type            size_type;
    typedef typename base_type::difference_type      difference_type;
    typedef typename base_type::pointer              pointer;
    typedef typename base_type::const_pointer        const_pointer;
    typedef typename base_type::reference            reference;
    typedef typename base_type::const_reference      const_reference;

    typedef typename base_type::iterator             iterator;
    typedef typename base_type::local_iterator       local_iterator;
    typedef typename base_type::const_iterator       const_iterator;
    typedef typename base_type::const_local_iterator const_local_iterator;
public:
    // 类型行为控制
    unordered_set(size_type bucket_count = 100)
        :m_stHt(bucket_count) {
    }
    explicit unordered_set(size_type bucket_count, hash_fun fHash, key_fun fKey)
        :m_stHt(bucket_count, fHash, fKey) {
    }
    unordered_set(const unordered_set& rhs)
        :m_stHt(rhs.m_stHt) {
    }
    unordered_set(unordered_set&& rhs) noexcept
        :m_stHt(std::move(rhs.m_stHt)) {
    }
    unordered_set& operator=(const unordered_set& rhs) {
        m_stHt = rhs.m_stHt;
        return *this;
    }
    unordered_set& operator=(unordered_set&& rhs) {
        m_stHt = std::move(rhs.m_stHt);
        return *this;
    }
    ~unordered_set() = default;

    // 迭代器支持
    iterator begin() noexcept
    { return m_stHt.begin(); }
    const_iterator begin() const noexcept
    { return m_stHt.begin(); }
    iterator end() noexcept
    { return m_stHt.end(); }
    const_iterator end() const noexcept
    { return m_stHt.end(); }

    local_iterator begin(size_type n) noexcept
    { return m_stHt.begin(n); }
    const_local_iterator begin(size_type n) const noexcept
    { return m_stHt.begin(n); }
    local_iterator end(size_type n) noexcept
    { return m_stHt.end(n); }
    const_local_iterator end(size_type n) const noexcept
    { return m_stHt.end(n); }

    bool empty() const noexcept { return m_stHt.empty(); }
    size_type size() const noexcept { return m_stHt.size(); }
    pair<iterator, bool> insert(const ele_type& stEle)
    { 
        return m_stHt.insert_unique(stEle); 
    }
    pair<iterator, bool> insert(ele_type&& stEle)
    { 
        return m_stHt.insert_unique(std::move(stEle)); 
    }

    void erase(const_iterator it)
    { m_stHt.erase(it); }
    size_type erase(const key_type& key)
    { return m_stHt.erase_unique(key); }

    void clear()
    { m_stHt.clear(); }

    void swap(unordered_set& other) noexcept
    { m_stHt.swap(other.m_stHt); }

    value_type& at(const key_type& key)
    {
        iterator it = m_stHt.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }
    const value_type& at(const key_type& key) const
    {
        iterator it = m_stHt.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }

    value_type& operator[](const key_type& key)
    {
        iterator it = m_stHt.find(key);
        if(it.m_pNode == nullptr){
            pair<iterator,bool> ret = insert(key);
            if(ret.second == false){
                throw mystl::bad_paramter();
            }
            return ret.first->second;
        }
        return it->second;
    }

    size_type count(const key_type& key) const
    { return m_stHt.count(key); }

    iterator find(const key_type& key)
    { return m_stHt.find(key); }
    const_iterator find(const key_type& key)  const
    { return m_stHt.find(key); }

    void reserve(size_type count)                
    { m_stHt.rehash_if_need(count); }
};


template <class Key, class Alloc = mystl::allocator<hashtable_node<const Key>>>
class unordered_mulset
{
private:
    typedef hashtable<const Key, Alloc> base_type;
    base_type m_stHt;

public:
    // 导出类型是常量类型
    typedef typename base_type::key_type             key_type;
    typedef typename base_type::value_type           value_type;
    typedef typename base_type::ele_type             ele_type;
    typedef typename base_type::hash_fun             hash_fun;
    typedef typename base_type::key_fun              key_fun;

    typedef typename base_type::size_type            size_type;
    typedef typename base_type::difference_type      difference_type;
    typedef typename base_type::pointer              pointer;
    typedef typename base_type::const_pointer        const_pointer;
    typedef typename base_type::reference            reference;
    typedef typename base_type::const_reference      const_reference;

    typedef typename base_type::iterator             iterator;
    typedef typename base_type::local_iterator       local_iterator;
    typedef typename base_type::const_iterator       const_iterator;
    typedef typename base_type::const_local_iterator const_local_iterator;
public:
    // 类型行为控制
    unordered_mulset(size_type bucket_count = 100)
        :m_stHt(bucket_count) {
    }
    explicit unordered_mulset(size_type bucket_count, hash_fun fHash, key_fun fKey)
        :m_stHt(bucket_count, fHash, fKey) {
    }
    unordered_mulset(const unordered_mulset& rhs)
        :m_stHt(rhs.m_stHt) {
    }
    unordered_mulset(unordered_mulset&& rhs) noexcept
        :m_stHt(std::move(rhs.m_stHt)) {
    }
    unordered_mulset& operator=(const unordered_mulset& rhs) {
        m_stHt = rhs.m_stHt;
        return *this;
    }
    unordered_mulset& operator=(unordered_mulset&& rhs) {
        m_stHt = std::move(rhs.m_stHt);
        return *this;
    }
    ~unordered_mulset() = default;

    // 迭代器支持
    iterator begin() noexcept
    { return m_stHt.begin(); }
    iterator end() noexcept
    { return m_stHt.end(); }
    const_iterator begin() const noexcept
    { return m_stHt.begin(); }
    const_iterator end() const noexcept
    { return m_stHt.end(); }

    local_iterator begin(size_type n) noexcept
    { return m_stHt.begin(n); }
    local_iterator end(size_type n) noexcept
    { return m_stHt.end(n); }
    const_local_iterator begin(size_type n) const noexcept
    { return m_stHt.begin(n); }
    const_local_iterator end(size_type n) const noexcept
    { return m_stHt.end(n); }

    bool empty() const noexcept { return m_stHt.empty(); }
    size_type size() const noexcept { return m_stHt.size(); }
    iterator insert(const ele_type& stEle)
    { 
        return m_stHt.insert_multi(stEle); 
    }
    iterator insert(ele_type&& stEle)
    { 
        return m_stHt.insert_multi(std::move(stEle)); 
    }

    void erase(const iterator& it)
    { m_stHt.erase(it); }
    void erase(const const_iterator& it)
    { m_stHt.erase(it); }
    size_type erase(const key_type& key)
    { return m_stHt.erase_multi(key); }

    void clear()
    { m_stHt.clear(); }

    void swap(unordered_mulset& other) noexcept
    { m_stHt.swap(other.m_stHt); }

    value_type& at(const key_type& key)
    {
        iterator it = m_stHt.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }
    const value_type& at(const key_type& key) const
    {
        iterator it = m_stHt.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }

    value_type& operator[](const key_type& key)
    {
        iterator it = m_stHt.find(key);
        if(it.m_pNode == nullptr){
            pair<iterator,bool> ret = insert(key);
            if(ret.second == false){
                throw mystl::bad_paramter();
            }
            return ret.first->second;
        }
        return it->second;
    }

    size_type count(const key_type& key) const
    { return m_stHt.count(key); }

    iterator find(const key_type& key)
    { return m_stHt.find(key); }
    pair<iterator, iterator> find_range(const key_type& key) {
        return m_stHt.equal_range_multi(key);
    }
    const_iterator find(const key_type& key)  const
    { return m_stHt.find(key); }
    pair<const_iterator, const_iterator> find_range(const key_type& key) const{
        return m_stHt.equal_range_multi(key);
    }

    void reserve(size_type count)                
    { m_stHt.rehash_if_need(count); }
};
}
#endif