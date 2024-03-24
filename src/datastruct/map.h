#ifndef MYSTL_MAP_H_
#define MYSTL_MAP_H_
#include "rbtree.h"
namespace mystl
{
template <class Key, class Value, 
    class TreeNodeAlloc = mystl::allocator<rbtree_node<mystl::pair<const Key, Value>>>, 
    class EleNodeAlloc = mystl::allocator<element_node<mystl::pair<const Key, Value>>>>
class map
{
private:
    typedef rbtree<mystl::pair<const Key, Value>, TreeNodeAlloc, EleNodeAlloc> base_type;
    base_type m_stRbTree;

public:
    // 这样导出的是常量类型
    typedef typename base_type::key_type             key_type;
    typedef typename base_type::value_type           value_type;
    typedef typename base_type::ele_type             ele_type;
    typedef typename base_type::key_fun              key_fun;

    typedef typename base_type::size_type            size_type;
    typedef typename base_type::difference_type      difference_type;
    typedef typename base_type::pointer              pointer;
    typedef typename base_type::const_pointer        const_pointer;
    typedef typename base_type::reference            reference;
    typedef typename base_type::const_reference      const_reference;

    typedef typename base_type::iterator             iterator;
    typedef typename base_type::const_iterator       const_iterator;
public:
    // 类型行为控制
    map() {
    }
    explicit map(key_fun fKey)
        :m_stRbTree(fKey) {
    }
    map(const map& rhs)
        :m_stRbTree(rhs.m_stRbTree) {
    }
    map(map&& rhs) noexcept
        :m_stRbTree(std::move(rhs.m_stRbTree)) {
    }
    map& operator=(const map& rhs) {
        m_stRbTree = rhs.m_stRbTree;
        return *this;
    }
    map& operator=(map&& rhs) {
        m_stRbTree = std::move(rhs.m_stRbTree);
        return *this;
    }
    ~map() = default;

    // 迭代器支持
    iterator begin() noexcept
    { return m_stRbTree.begin(); }
    const_iterator begin() const noexcept
    { return m_stRbTree.begin(); }
    iterator end() noexcept
    { return m_stRbTree.end(); }
    const_iterator end() const noexcept
    { return m_stRbTree.end(); }

    bool empty() const noexcept { return m_stRbTree.empty(); }
    size_type size() const noexcept { return m_stRbTree.size(); }
    pair<iterator, bool> insert(const ele_type& stEle)
    { return m_stRbTree.insert_unique(stEle); }
    pair<iterator, bool> insert(ele_type&& stEle)
    { return m_stRbTree.insert_unique(std::move(stEle)); }

    pair<iterator, bool> insert(const Key& nKey, const Value& nValue){
        ele_type stEle(nKey, nValue);
        return insert(std::move(stEle));
    }
    
    void erase(iterator it)
    { m_stRbTree.erase(it); }
    size_type erase(const key_type& key)
    { return m_stRbTree.erase_unique(key); }

    void clear()
    { m_stRbTree.clear(); }

    void swap(map& other) noexcept
    { m_stRbTree.swap(other.m_stRbTree); }

    value_type& at(const key_type& key)
    {
        iterator it = m_stRbTree.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }
    const value_type& at(const key_type& key) const
    {
        iterator it = m_stRbTree.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }

    value_type& operator[](const key_type& key)
    {
        iterator it = m_stRbTree.find(key);
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
    { return m_stRbTree.count(key); }

    iterator find(const key_type& key)
    { return m_stRbTree.find(key); }
    const_iterator find(const key_type& key)  const
    { return m_stRbTree.find(key); }
};


template <class Key, class Value, 
    class TreeNodeAlloc = mystl::allocator<rbtree_node<mystl::pair<const Key, Value>>>, 
    class EleNodeAlloc = mystl::allocator<element_node<mystl::pair<const Key, Value>>>>
class mulmap
{
private:
    typedef rbtree<mystl::pair<const Key, Value>, TreeNodeAlloc, EleNodeAlloc> base_type;
    base_type m_stRbTree;

public:
    // 这样导出的是常量类型
    typedef typename base_type::key_type             key_type;
    typedef typename base_type::value_type           value_type;
    typedef typename base_type::ele_type             ele_type;
    typedef typename base_type::key_fun              key_fun;

    typedef typename base_type::size_type            size_type;
    typedef typename base_type::difference_type      difference_type;
    typedef typename base_type::pointer              pointer;
    typedef typename base_type::const_pointer        const_pointer;
    typedef typename base_type::reference            reference;
    typedef typename base_type::const_reference      const_reference;

    typedef typename base_type::iterator             iterator;
    typedef typename base_type::const_iterator       const_iterator;

public:
    // 类型行为控制
    mulmap() {
    }
    explicit mulmap(key_fun fKey)
        :m_stRbTree(fKey) {
    }
    mulmap(const mulmap& rhs)
        :m_stRbTree(rhs.m_stRbTree) {
    }
    mulmap(mulmap&& rhs) noexcept
        :m_stRbTree(std::move(rhs.m_stRbTree)) {
    }
    mulmap& operator=(const mulmap& rhs) {
        m_stRbTree = rhs.m_stRbTree;
        return *this;
    }
    mulmap& operator=(mulmap&& rhs) {
        m_stRbTree = std::move(rhs.m_stRbTree);
        return *this;
    }
    ~mulmap() = default;

    // 迭代器支持
    iterator begin() noexcept
    { return m_stRbTree.begin(); }
    const_iterator begin() const noexcept
    { return m_stRbTree.begin(); }
    iterator end() noexcept
    { return m_stRbTree.end(); }
    const_iterator end() const noexcept
    { return m_stRbTree.end(); }

    bool empty() const noexcept { return m_stRbTree.empty(); }
    size_type size() const noexcept { return m_stRbTree.size(); }
    iterator insert(const ele_type& stEle)
    { return m_stRbTree.insert_multi(stEle); }
    iterator insert(ele_type&& stEle)
    { return m_stRbTree.insert_multi(std::move(stEle)); }

    iterator insert(const Key& nKey, const Value& nValue)
    {
        ele_type stEle(nKey, nValue); 
        return m_stRbTree.insert_multi(std::move(stEle)); 
    }

    void erase(iterator it)
    { m_stRbTree.erase(it); }
    size_type erase(const key_type& key)
    { return m_stRbTree.erase_multi(key); }

    void clear()
    { m_stRbTree.clear(); }

    void swap(mulmap& other) noexcept
    { m_stRbTree.swap(other.m_stRbTree); }

    value_type& at(const key_type& key)
    {
        iterator it = m_stRbTree.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }
    const value_type& at(const key_type& key) const
    {
        iterator it = m_stRbTree.find(key);
        if(it.m_pNode == nullptr){
            throw mystl::bad_paramter();
        }
        return it->second;
    }

    value_type& operator[](const key_type& key)
    {
        iterator it = m_stRbTree.find(key);
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
    { return m_stRbTree.count(key); }

    iterator find(const key_type& key)
    { return m_stRbTree.find(key); }
    const_iterator find(const key_type& key)  const
    { return m_stRbTree.find(key); }
    pair<iterator, iterator> find_range(const key_type& key) {
        return m_stRbTree.equal_range_multi(key);
    }
    pair<const_iterator, const_iterator> find_range(const key_type& key) const {
        return m_stRbTree.equal_range_multi(key);
    }
};
}
#endif