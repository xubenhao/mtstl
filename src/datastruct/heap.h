#ifndef _MYSTL_HEAP_H_
#define _MYSTL_HEAP_H_
#include "array.h"
namespace mystl
{
template <class T,bool isMin = true>
class heap
{
public:
    typedef T                   value_type;
    typedef uint32_t            size_type;
    typedef T&                  reference;
    typedef const T&            const_reference;
private:
    mystl::array<T> m_stContainer;
public:
    heap() = default;
    heap(const mystl::array<T>& c)
        :m_stContainer(c)
    {
        build();
    }
    heap(mystl::array<T>&& c)
        :m_stContainer(std::move(c))
    {
        build();
    }

    heap(const heap& rhs)
        :m_stContainer(rhs.m_stContainer)
    {
        build();
    }
    heap(heap&& rhs)
        :m_stContainer(std::move(rhs.m_stContainer))
    {
        build();
    }

    heap& operator=(const heap& rhs)
    {
        m_stContainer = rhs.m_stContainer;
        return *this;
    }
    heap& operator=(heap&& rhs)
    {
        m_stContainer = std::move(rhs.m_stContainer);
        return *this;
    }

    ~heap() = default;
    // 仅仅查看
    reference       top()       {
        if(empty()){
            throw mystl::container_empty("container is empty");
        }
        else{
            auto iter = m_stContainer.begin();
            return *(iter);
        }
    }
    const_reference top() const {
        if(empty()){
            throw mystl::container_empty();
        }
        else{
            auto iter = m_stContainer.begin();
            return *(iter);
        }
    }

    // 容量相关操作
    bool      empty() const noexcept { return m_stContainer.empty(); }
    size_type size()  const noexcept { return m_stContainer.size(); }

    void push(const value_type& value)
    { 
        m_stContainer.push_back(value);
        adjust();
    }
    void push(value_type&& value)
    { 
        m_stContainer.push_back(std::move(value));
        adjust();
    }
    void pop()
    { 
        // 移除首个元素
        if(m_stContainer.size() <= 1){
            m_stContainer.pop_back();    
        }
        else{
            // 交换首元素，尾元素
            size_type nLIndex = m_stContainer.size() - 1;
            T nEle = m_stContainer[0];
            m_stContainer[0] = m_stContainer[nLIndex];
            m_stContainer[nLIndex] = nEle;
            m_stContainer.pop_back();

            size_type nIndex = 0;
            while(true){
                size_type nLIndex = 2 * nIndex + 1;
                size_type nRIndex = 2 * nIndex + 2;
                size_type nTarIndex = nIndex;
                // 若nIndex的左孩子，右孩子皆不存在，则迭代结束＝＝左孩子不存在
                if(nLIndex >= size()){
                    break;
                }
                if(isMin){
                    // 确定三者中最小元素位置
                    if(nRIndex < size() && m_stContainer[nRIndex] < m_stContainer[nTarIndex]){
                        nTarIndex = nRIndex;
                    }
                    if(nLIndex < size() && m_stContainer[nLIndex] < m_stContainer[nTarIndex]){
                        nTarIndex = nLIndex;
                    }
                    if(nTarIndex != nIndex){
                        // 交换
                        T nEle = m_stContainer[nIndex];
                        m_stContainer[nIndex] = m_stContainer[nTarIndex];
                        m_stContainer[nTarIndex] = nEle;
                        // 此时nTarIndex位置元素变大了．堆中唯有nTarIndex和其左右孩子可能不满足约束．
                        nIndex = nTarIndex;
                    }
                    else{
                        break;
                    }
                }
                else{
                    // 确定三者中最小元素位置
                    if(nRIndex < size() && m_stContainer[nRIndex] > m_stContainer[nTarIndex]){
                        nTarIndex = nRIndex;
                    }
                    if(nLIndex < size() && m_stContainer[nLIndex] > m_stContainer[nTarIndex]){
                        nTarIndex = nLIndex;
                    }
                    if(nTarIndex != nIndex){
                        // 交换
                        T nEle = m_stContainer[nIndex];
                        m_stContainer[nIndex] = m_stContainer[nTarIndex];
                        m_stContainer[nTarIndex] = nEle;
                        // 此时nTarIndex位置元素变大了．堆中唯有nTarIndex和其左右孩子可能不满足约束．
                        nIndex = nTarIndex;
                    }
                    else{
                        break;
                    }
                }
            }
            
        }
    }

    void clear()
    {
        m_stContainer.clear();
    }

    void swap(heap& rhs) 
    { m_stContainer.swap(rhs.m_stContainer); }

    bool operator==(const heap& rhs){
        return m_stContainer == rhs.m_stContainer;
    }
    bool operator<(const heap& rhs){
        return m_stContainer < rhs.m_stContainer;
    }
private:
    void build(){
        //size_type nSize = size();
        // 建堆
        for(int32_t nIndex = size() - 1; nIndex >= 0; nIndex--){
            // 对nIndex
            // 迭代到nIndex时，其左右孩子为根子树内所有节点皆满足堆的性质．
            // 只有nIndex和其左右孩子可能不满足堆的性质．
            size_type nTmpIndex = nIndex;
            while(true){
                size_type nLIndex = 2 * nTmpIndex + 1;
                size_type nRIndex = 2 * nTmpIndex + 2;
                size_type nTarIndex = nTmpIndex;
                // 若nIndex的左孩子，右孩子皆不存在，则迭代结束＝＝左孩子不存在
                if(nLIndex >= size()){
                    break;
                }
                if(isMin){
                    // 确定三者中最小元素位置
                    if(nRIndex < size() && m_stContainer[nRIndex] < m_stContainer[nTarIndex]){
                        nTarIndex = nRIndex;
                    }
                    if(nLIndex < size() && m_stContainer[nLIndex] < m_stContainer[nTarIndex]){
                        nTarIndex = nLIndex;
                    }
                    if(nTarIndex != nTmpIndex){
                        // 交换
                        T nEle = m_stContainer[nTmpIndex];
                        m_stContainer[nTmpIndex] = m_stContainer[nTarIndex];
                        m_stContainer[nTarIndex] = nEle;
                        // 此时nTarIndex位置元素变大了．堆中唯有nTarIndex和其左右孩子可能不满足约束．
                        nTmpIndex = nTarIndex;
                    }
                    else{
                        break;
                    }
                }
                else{
                    // 确定三者中最小元素位置
                    if(nRIndex < size() && m_stContainer[nRIndex] > m_stContainer[nTarIndex]){
                        nTarIndex = nRIndex;
                    }
                    if(nLIndex < size() && m_stContainer[nLIndex] > m_stContainer[nTarIndex]){
                        nTarIndex = nLIndex;
                    }
                    if(nTarIndex != nTmpIndex){
                        // 交换
                        T nEle = m_stContainer[nTmpIndex];
                        m_stContainer[nTmpIndex] = m_stContainer[nTarIndex];
                        m_stContainer[nTarIndex] = nEle;
                        // 此时nTarIndex位置元素变大了．堆中唯有nTarIndex和其左右孩子可能不满足约束．
                        nTmpIndex = nTarIndex;
                    }
                    else{
                        break;
                    }
                }
            }
        }
    }
    void adjust(){
        // 此时只有尾元素和其父可能不满足堆的性质
        size_type nNewIndex = m_stContainer.size() - 1;
        // 假设父节点索引为nIndex，则左孩子索引2*nIndex+1，右孩子索引2*nIndex+2.
        size_type nPIndex;
        while(nNewIndex != 0){// 迭代停止条件检测
            if(nNewIndex % 2 == 0){
                nPIndex = nNewIndex / 2 + 1;
            }
            else{
                nPIndex = nNewIndex / 2;
            }

            // 考察最小堆
            if(isMin){
                if(m_stContainer[nPIndex] <= m_stContainer[nNewIndex]){
                    // 无需调节．堆中所有节点均已满足性质．
                    break;
                }
                else{
                    // 交换父子上元素
                    T nEle = m_stContainer[nPIndex];
                    m_stContainer[nPIndex] = m_stContainer[nNewIndex];
                    m_stContainer[nNewIndex] = nEle;
                    // 此时唯有nPIndex和其父节点可能不满足堆的性质
                    nNewIndex = nPIndex;
                }
            }
            else{
                if(m_stContainer[nPIndex] >= m_stContainer[nNewIndex]){
                    // 无需调节．堆中所有节点均已满足性质．
                    break;
                }
                else{
                    // 交换父子上元素
                    T nEle = m_stContainer[nPIndex];
                    m_stContainer[nPIndex] = m_stContainer[nNewIndex];
                    m_stContainer[nNewIndex] = nEle;
                    // 此时唯有nPIndex和其父节点可能不满足堆的性质
                    nNewIndex = nPIndex;
                }
            }
        }
    }
};

template <class T>
bool operator==(const heap<T>& lhs, const heap<T>& rhs)
{
    return lhs.operator==(rhs);
}

template <class T>
bool operator<(const heap<T>& lhs, const heap<T>& rhs)
{
    return lhs.operator<(rhs);
}

template <class T>
bool operator!=(const heap<T>& lhs, const heap<T>& rhs)
{
    return !(lhs == rhs);
}

template <class T>
bool operator>(const heap<T>& lhs, const heap<T>& rhs)
{
    return rhs < lhs;
}

template <class T>
bool operator<=(const heap<T>& lhs, const heap<T>& rhs)
{
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const heap<T>& lhs, const heap<T>& rhs)
{
    return !(lhs < rhs);
}
}
#endif