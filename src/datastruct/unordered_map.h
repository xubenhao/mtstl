#ifndef MYSTL_UNORDERED_MAP_H_
#define MYSTL_UNORDERED_MAP_H_
#include "hashtable.h"
namespace mystl
{
template <class Key, class Value, class Alloc = mystl::allocator<hashtable_node<mystl::pair<const Key, Value>>>>
class unordered_map
{
private:
    typedef hashtable<mystl::pair<const Key, Value>, Alloc> base_type;
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
    typedef typename base_type::const_iterator       const_iterator;
    typedef typename base_type::local_iterator       local_iterator;
    typedef typename base_type::const_local_iterator const_local_iterator;

public:
    // 类型行为控制
    unordered_map(size_type bucket_count = 100)
        :m_stHt(bucket_count) {
    }
    explicit unordered_map(size_type bucket_count, hash_fun fHash, key_fun fKey)
        :m_stHt(bucket_count, fHash, fKey) {
    }
    unordered_map(const unordered_map& rhs)
        :m_stHt(rhs.m_stHt) {
    }
    unordered_map(unordered_map&& rhs) noexcept
        :m_stHt(std::move(rhs.m_stHt)) {
    }
    unordered_map& operator=(const unordered_map& rhs) {
        m_stHt = rhs.m_stHt;
        return *this;
    }
    unordered_map& operator=(unordered_map&& rhs) {
        m_stHt = std::move(rhs.m_stHt);
        return *this;
    }
    ~unordered_map() = default;

    // 迭代器支持
    iterator begin() noexcept
    { return m_stHt.begin(); }
    const_iterator begin() const noexcept
    { return m_stHt.begin(); }
    iterator end() noexcept
    { return m_stHt.end(); }
    const_iterator end() const noexcept
    { return m_stHt.end(); }

    local_iterator       begin(size_type n)        noexcept
    { return m_stHt.begin(n); }
    const_local_iterator begin(size_type n)  const noexcept
    { return m_stHt.begin(n); }
    local_iterator       end(size_type n)          noexcept
    { return m_stHt.end(n); }
    const_local_iterator end(size_type n)    const noexcept
    { return m_stHt.end(n); }

    bool empty() const noexcept { return m_stHt.empty(); }
    size_type size() const noexcept { return m_stHt.size(); }
    pair<iterator, bool> insert(const ele_type& stEle)
    { return m_stHt.insert_unique(stEle); }
    pair<iterator, bool> insert(ele_type&& stEle)
    { return m_stHt.insert_unique(std::move(stEle)); }

    pair<iterator, bool> insert(const Key& nKey, const Value& nValue){
        ele_type stEle(nKey, nValue);
        return insert(std::move(stEle));
    }
    
    void erase(iterator it)
    { m_stHt.erase(it); }
    size_type erase(const key_type& key)
    { return m_stHt.erase_unique(key); }

    void clear()
    { m_stHt.clear(); }

    void swap(unordered_map& other) noexcept
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
            pair<iterator,bool> ret = insert(mystl::pair<const Key, Value>(key, value_type()));
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


template <class Key, class Value, class Alloc = mystl::allocator<hashtable_node<mystl::pair<const Key, Value>>>>
class unordered_mulmap
{
private:
    typedef hashtable<mystl::pair<const Key, Value>, Alloc> base_type;
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
    typedef typename base_type::const_iterator       const_iterator;
    typedef typename base_type::local_iterator       local_iterator;
    typedef typename base_type::const_local_iterator const_local_iterator;

public:
    // 类型行为控制
    unordered_mulmap(size_type bucket_count = 100)
        :m_stHt(bucket_count) {
    }
    explicit unordered_mulmap(size_type bucket_count, hash_fun fHash, key_fun fKey)
        :m_stHt(bucket_count, fHash, fKey) {
    }
    unordered_mulmap(const unordered_mulmap& rhs)
        :m_stHt(rhs.m_stHt) {
    }
    unordered_mulmap(unordered_mulmap&& rhs) noexcept
        :m_stHt(std::move(rhs.m_stHt)) {
    }
    unordered_mulmap& operator=(const unordered_mulmap& rhs) {
        m_stHt = rhs.m_stHt;
        return *this;
    }
    unordered_mulmap& operator=(unordered_mulmap&& rhs) {
        m_stHt = std::move(rhs.m_stHt);
        return *this;
    }
    ~unordered_mulmap() = default;

    // 迭代器支持
    iterator begin() noexcept
    { return m_stHt.begin(); }
    const_iterator begin() const noexcept
    { return m_stHt.begin(); }
    iterator end() noexcept
    { return m_stHt.end(); }
    const_iterator end() const noexcept
    { return m_stHt.end(); }

    local_iterator       begin(size_type n)        noexcept
    { return m_stHt.begin(n); }
    const_local_iterator begin(size_type n)  const noexcept
    { return m_stHt.begin(n); }
    local_iterator       end(size_type n)          noexcept
    { return m_stHt.end(n); }
    const_local_iterator end(size_type n)    const noexcept
    { return m_stHt.end(n); }

    bool empty() const noexcept { return m_stHt.empty(); }
    size_type size() const noexcept { return m_stHt.size(); }
    iterator insert(const ele_type& stEle)
    { return m_stHt.insert_multi(stEle); }
    iterator insert(ele_type&& stEle)
    { return m_stHt.insert_multi(std::move(stEle)); }

    iterator insert(const Key& nKey, const Value& nValue)
    {
        ele_type stEle(nKey, nValue); 
        return m_stHt.insert_multi(std::move(stEle)); 
    }

    void erase(iterator it)
    { m_stHt.erase(it); }
    size_type erase(const key_type& key)
    { return m_stHt.erase_multi(key); }

    void clear()
    { m_stHt.clear(); }

    void swap(unordered_mulmap& other) noexcept
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
            pair<iterator,bool> ret = insert(mystl::pair<const Key, Value>(key, value_type()));
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
    pair<iterator, iterator> find_range(const key_type& key) {
        return m_stHt.equal_range_multi(key);
    }
    pair<const_iterator, const_iterator> find_range(const key_type& key) const {
        return m_stHt.equal_range_multi(key);
    }

    void reserve(size_type count)                
    { m_stHt.rehash_if_need(count); }
};
}
#endif