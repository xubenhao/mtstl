#ifndef MYSTL_HASHTABLE_H
#define MYSTL_HASHTABLE_H
#include "array.h"

namespace mystl {

// 哈希表节点元素两种类型
// mystl::pair<const int, int>形式，此形式下，pair的模板参数１必须是常量类型，模板参数２是非常量类型
// const int，此形式下，元素类型是常量类型
template <class T>
struct hashtable_node {
	hashtable_node* m_pNext;
	T m_nEle;
	hashtable_node() = default;
	hashtable_node(const T& nEle) 
		:m_pNext(nullptr), m_nEle(nEle) {}
	hashtable_node(T&& nEle) 
		:m_pNext(nullptr), m_nEle(std::move(nEle)) {}
	hashtable_node(const hashtable_node& node) 
		:m_pNext(node.m_pNext), m_nEle(node.m_nEle) {}
	hashtable_node(hashtable_node&& node) 
		:m_pNext(node.m_pNext), m_nEle(std::move(node.m_nEle)) {
	}
};

// 哈希表元素支持两种类型，参考上述
// 形式２
template <class T>
struct ht_ele_traits_imp {
	typedef T key_type;
	typedef T value_type;
	typedef T ele_type;
	static const key_type& get_key(const ele_type& nEle) {
		return nEle;
	}
	static const value_type& get_value(const ele_type& nEle) {
		return nEle;
	}
};

// 形式１
template <class T1, class T2>
struct ht_ele_traits_imp<mystl::pair<T1,T2>> {
	// 这样导出的类型是const修饰的类型
	typedef T1 									 key_type;
	typedef T2                           		 value_type;
	typedef mystl::pair<T1,T2>          	     ele_type;
	static const key_type& get_key(const ele_type& nEle) {
		return nEle.first;
	}
	static const value_type& get_value(const ele_type& nEle) {
		return nEle.second;
	}
};

template <class T, class Alloc>
class hashtable;

template <class T, class Alloc>
struct ht_iterator;
template <class T>
struct ht_local_iterator;

template <class T, class Alloc>
struct ht_const_iterator;
template <class T>
struct ht_const_local_iterator;

// 常量，非常量迭代器需单独定义．如果合并定义，无法区分set下，常量迭代器，非常量迭代器完全一致．哈希中函数重载将无法确定类型．
// 哈希表迭代器
template <class T, class Alloc>
struct ht_iterator {
    typedef mystl::forward_iterator_tag  						iterator_category;
    typedef T                               					value_type;
	typedef T*                             					    pointer;
	typedef T&                             					    reference;
    typedef size_t                          					size_type;
	typedef ptrdiff_t                      						difference_type;

	typedef typename mystl::hashtable<T, Alloc>  			    container;
	typedef typename container::node                            node;
public:
    node* m_pNode;
	container* m_pHt;
public:
	ht_iterator() = default;
	ht_iterator(node* pNode, container* pHt) {
		m_pNode = pNode;
		m_pHt = (pHt);
	}
	ht_iterator(const ht_iterator& rhs) {
		m_pNode = rhs.m_pNode;
		m_pHt = rhs.m_pHt;
	}
	
	ht_iterator& operator=(const ht_iterator& rhs) {
		if (this != &rhs) {
			m_pNode = rhs.m_pNode;
			m_pHt = rhs.m_pHt;
		}
		return *this;
	}
	
	// 哈希表通过迭代器解引用访问迭代器下面的元素时，
	// 对形式１，允许修改元素值，不允许修改元素键
	// 对形式２，不允许修改元素值/键
	// 这通过元素自身的类型中const修饰来实现
	reference operator*()  const { return m_pNode->m_nEle; }
	pointer   operator->() const { return &(operator*()); }
	ht_iterator& operator++() {
		m_pNode = m_pHt->get_next(m_pNode);
		return *this;
	}
	ht_iterator operator++(int) {
		ht_iterator tmp = *this;
		++*this;
		return tmp;
	}

	bool operator==(const ht_iterator& other) const { return m_pNode == other.m_pNode && m_pHt == other.m_pHt; }
	bool operator!=(const ht_iterator& other) const { return !(*this == other); }
};

template <class T, class Alloc>
struct ht_const_iterator {
    typedef mystl::forward_iterator_tag  						iterator_category;
    typedef T                               					value_type;
	typedef const T*                             				pointer;
	typedef const T&                             				reference;
    typedef size_t                          					size_type;
	typedef ptrdiff_t                      						difference_type;

	typedef typename mystl::hashtable<T, Alloc>  			    container;
	typedef typename container::node                            node;
public:
    const node* m_pNode;
	container* m_pHt;
public:
	ht_const_iterator() = default;
	ht_const_iterator(node* pNode, const container* pHt) {
		m_pNode = pNode;
		m_pHt = pHt;
	}
	ht_const_iterator(const ht_const_iterator& rhs) {
		m_pNode = rhs.m_pNode;
		m_pHt = rhs.m_pHt;
	}
	
	ht_const_iterator& operator=(const ht_const_iterator& rhs) {
		if (this != &rhs) {
			m_pNode = rhs.m_pNode;
			m_pHt = rhs.m_pHt;
		}
		return *this;
	}
	
	// 哈希表通过迭代器解引用访问迭代器下面的元素时，
	// 对形式１，允许修改元素值，不允许修改元素键
	// 对形式２，不允许修改元素值/键
	// 这通过元素自身的类型中const修饰来实现
	reference operator*()  const { return m_pNode->m_nEle; }
	pointer   operator->() const { return &(operator*()); }
	ht_const_iterator& operator++() {
		m_pNode = m_pHt->get_next(m_pNode);
		return *this;
	}
	ht_const_iterator operator++(int) {
		ht_const_iterator tmp = *this;
		++*this;
		return tmp;
	}

	bool operator==(const ht_const_iterator& other) const { return m_pNode == other.m_pNode && m_pHt == other.m_pHt; }
	bool operator!=(const ht_const_iterator& other) const { return !(*this == other); }
};

template <class T>
struct ht_local_iterator {
    typedef mystl::forward_iterator_tag                         iterator_category;
	typedef T                                                   value_type;
	typedef T*                                                  pointer;
	typedef T&                                                  reference;
	typedef size_t                                              size_type;
	typedef ptrdiff_t                                           difference_type;

	typedef hashtable_node<T>                                   node;
public:
	node* m_pNode;
public:
    ht_local_iterator(node* pNode) :m_pNode(pNode) {
	}
	ht_local_iterator(const ht_local_iterator& rhs) :m_pNode(rhs.m_pNode) {
	}
	
	reference operator*()  const { return m_pNode->m_nEle; }
	pointer   operator->() const { return &(operator*()); }
	ht_local_iterator& operator++() {
		assert(m_pNode != nullptr);
		m_pNode = m_pNode->m_pNext;
		return *this;
	}
	ht_local_iterator operator++(int) {
		ht_local_iterator tmp(*this);
		++*this;
		return tmp;
	}
	bool operator==(const ht_local_iterator& other) const { return m_pNode == other.m_pNode; }
	bool operator!=(const ht_local_iterator& other) const { return !(*this == other); }
};

template <class T>
struct ht_const_local_iterator {
    typedef mystl::forward_iterator_tag                         iterator_category;
	typedef T                                                   value_type;
	typedef const T*                                            pointer;
	typedef const T&                                            reference;
	typedef size_t                                              size_type;
	typedef ptrdiff_t                                           difference_type;

	typedef hashtable_node<T>                                   node;
public:
	node* m_pNode;
public:
    ht_const_local_iterator(node* pNode) :m_pNode(pNode) {
	}
	ht_const_local_iterator(const ht_const_local_iterator& rhs) :m_pNode(rhs.m_pNode) {
	}
	
	reference operator*()  const { return m_pNode->m_nEle; }
	pointer   operator->() const { return &(operator*()); }
	ht_const_local_iterator& operator++() {
		assert(m_pNode != nullptr);
		m_pNode = m_pNode->m_pNext;
		return *this;
	}
	ht_const_local_iterator operator++(int) {
		ht_const_local_iterator tmp(*this);
		++*this;
		return tmp;
	}
	bool operator==(const ht_const_local_iterator& other) const { return m_pNode == other.m_pNode; }
	bool operator!=(const ht_const_local_iterator& other) const { return !(*this == other); }
};

#define PRIME_NUM 44
// 1. start with p = 101
// 2. p = next_prime(p * 1.7)
// 3. if p < (2 << 31), go to step 2, otherwise, go to step 4
// 4. end with p = prev_prime(2 << 31 - 1)
static constexpr size_t ht_prime_list[] = {
	101u, 173u, 263u, 397u, 599u, 907u, 1361u, 2053u, 3083u, 4637u, 6959u,
	10453u, 15683u, 23531u, 35311u, 52967u, 79451u, 119179u, 178781u, 268189u,
	402299u, 603457u, 905189u, 1357787u, 2036687u, 3055043u, 4582577u, 6873871u,
	10310819u, 15466229u, 23199347u, 34799021u, 52198537u, 78297827u, 117446801u,
	176170229u, 264255353u, 396383041u, 594574583u, 891861923u, 1337792887u,
	2006689337u, 3010034021u, 4294967291u,
};

inline size_t ht_next_prime(size_t n) {
	int32_t nIndex = 0;
    for(; nIndex < PRIME_NUM; nIndex++){
        if(ht_prime_list[nIndex] >= n){
            break;
        }
    }

    if(nIndex == PRIME_NUM){
        nIndex--;
    }

    return ht_prime_list[nIndex];
}

// 哈希表
// 接收两种元素类型
// 形式１：mystl::pair<const int,int>，模板参数１是常量类型，模板参数２是非常量类型
// 形式２：const int，常量类型
template <class T, class Alloc = mystl::allocator<hashtable_node<T>>>
class hashtable {
	// 对本身已经是有const修饰的类型再加const修饰是允许的，叠加后依然等同一个const的效果
	friend struct mystl::ht_iterator<T, Alloc>;
    friend struct mystl::ht_const_iterator<T, Alloc>;
    friend struct mystl::ht_local_iterator<T>;
    friend struct mystl::ht_const_local_iterator<T>;

public:
	typedef ht_ele_traits_imp<T> ele_traits;
	// 这里导出的必然是const修饰的类型
	typedef typename ele_traits::key_type          	key_type;
    typedef typename ele_traits::value_type         value_type;
    typedef typename ele_traits::ele_type           ele_type;
private:
	// 多重const修饰是允许的，效果等同一个const修饰
	typename ele_traits::key_type get_key(const ele_type& nEle) const {
        return ele_traits::get_key(nEle);
    }
    typename ele_traits::value_type get_value(const ele_type& nEle) const {
        return ele_traits::get_value(nEle);
    }
	
public:
  	typedef Alloc                           								node_allocator;
	typedef T*                              								pointer;
	typedef const T*                        								const_pointer;
	typedef T&                              								reference;
	typedef const T&                        								const_reference;
	typedef size_t                          								size_type;
	typedef ptrdiff_t                       								difference_type;
	
	typedef mystl::ht_iterator<T,Alloc>                                     iterator;
    typedef mystl::ht_const_iterator<T,Alloc>                               const_iterator;
	typedef mystl::ht_local_iterator<T>                                     local_iterator;
	typedef mystl::ht_const_local_iterator<T>                               const_local_iterator;

	typedef hashtable_node<T>                                               node;
	typedef mystl::array<node*>                                				bucket;
	typedef size_type (*hash_fun)(const key_type&, size_type);
	typedef bool (*key_fun)(const key_type&, const key_type&);
private:
	static size_type hashfun(const key_type& nkey, size_type nBucketSize){
		return nkey % nBucketSize;
	}	
	static bool keyfun(const key_type& nK1, const key_type& nK2){
		return nK1 == nK2;
	}

private:
	bucket    m_stBucket;
	size_type m_nBucketSize = 0;
	size_type m_nEleNum = 0;

	hash_fun  m_fHash;
	key_fun   m_fKey;
	float     m_nMaxLoadFactor = 100.0;

public:
	// 1.类型行为控制
	explicit hashtable(size_type nBucketSize, hash_fun fHash = &hashtable::hashfun, key_fun fKey = &hashtable::keyfun) {
		size_t nCount = ht_next_prime(nBucketSize);
		try {
			m_stBucket.reserve(nCount);
		}
		catch (...) {
			throw;
		}
		
		for(size_t i = 0; i < nCount; i++) {
			m_stBucket.push_back(nullptr);
		}
		m_nBucketSize = nCount;
		m_fHash = fHash;
		m_fKey = fKey;
	}

	// 拷贝构造
	hashtable(const hashtable& rhs) {
		bucket stTmpBucket;
		try{
			stTmpBucket.reserve(rhs.m_nBucketSize);
		}
		catch(...){
			throw;
		}
		
		for(size_t i = 0; i < rhs.m_nBucketSize; i++){
			stTmpBucket.push_back(nullptr);
		}

		for (size_type i = 0; i < rhs.m_nBucketSize; ++i) {
			node* pSource = rhs.m_stBucket[i];
			if(pSource == nullptr){
				continue;
			}

			node* pCopy = nullptr;
			node* pCopyPre = nullptr;
			while(pSource){
				node* pSourceCur = pSource;
				pSource = pSource->m_pNext;

				try
				{
					pCopy = node_allocator::allocate(1);
					// 元素可能是常量类型－－常量类型执行构造是允许的
					node_allocator::construct(pCopy, pSourceCur->m_nEle);
				}
				catch(...)
				{
					if(pCopy){
						node_allocator::deallocate(pCopy);
					}
					
					for(size_type k = 0; k <= i; k++){
						node* pTmp = stTmpBucket[k];
						while(pTmp){
							node* pFree = pTmp;
							pTmp = pTmp->m_pNext;
							node_allocator::destroy(pFree);
							node_allocator::deallocate(pFree);
						}
					}
					throw;
				}

				if(pCopyPre == nullptr){
					stTmpBucket[i] = pCopy;
				}
				else{
					pCopyPre->m_pNext = pCopy;
				}

				pCopyPre = pCopy;
			}
		}
			
		// 拷贝
		m_stBucket = std::move(stTmpBucket);
		m_nBucketSize = rhs.m_nBucketSize;
		m_nEleNum = rhs.m_nEleNum;

		m_fHash = rhs.m_fHash;
		m_fKey = rhs.m_fKey;
		m_nMaxLoadFactor = rhs.m_nMaxLoadFactor;

	}
	hashtable(hashtable&& rhs) noexcept {
		// 拷贝
		m_stBucket = std::move(rhs.m_stBucket);
		m_nBucketSize = rhs.m_nBucketSize;
		m_nEleNum = rhs.m_nEleNum;

		m_fHash = rhs.m_fHash;
		m_fKey = rhs.m_fKey;
		m_nMaxLoadFactor = rhs.m_nMaxLoadFactor;

		rhs.m_nBucketSize = 0;
		rhs.m_nEleNum = 0;
	}
	hashtable& operator=(const hashtable& rhs)
	{
		if(this == &rhs){
			return *this;
		}

		bucket stTmpBucket;
		try{
			stTmpBucket.reserve(rhs.m_nBucketSize);
		}
		catch(...){
			throw;
		}
		
		for(size_t i = 0; i < rhs.m_nBucketSize; i++){
			stTmpBucket.push_back(nullptr);
		}

		for (size_type i = 0; i < rhs.m_nBucketSize; ++i) {
			node* pSource = rhs.m_stBucket[i];
			if(pSource == nullptr){
				continue;
			}

			node* pCopy = nullptr;
			node* pCopyPre = nullptr;
			while(pSource){
				node* pSourceCur = pSource;
				pSource = pSource->m_pNext;

				try
				{
					pCopy = node_allocator::allocate(1);
					// 元素类型可能是常量类型，对常量类型执行构造是允许的
					node_allocator::construct(pCopy, pSourceCur->m_nEle);
				}
				catch(...)
				{
					if(pCopy){
						node_allocator::deallocate(pCopy);
					}
					
					for(size_type k = 0; k <= i; k++){
						node* pTmp = stTmpBucket[k];
						while(pTmp){
							node* pFree = pTmp;
							pTmp = pTmp->m_pNext;
							node_allocator::destroy(pFree);
							node_allocator::deallocate(pFree);
						}
					}
					throw;
				}

				if(pCopyPre == nullptr){
					stTmpBucket[i] = pCopy;
				}
				else{
					pCopyPre->m_pNext = pCopy;
				}

				pCopyPre = pCopy;
			}
		}
		
		// 先释放自身
		clear();

		// 拷贝
		m_stBucket = std::move(stTmpBucket);
		m_nBucketSize = rhs.m_nBucketSize;
		m_nEleNum = rhs.m_nEleNum;

		m_fHash = rhs.m_fHash;
		m_fKey = rhs.m_fKey;
		m_nMaxLoadFactor = rhs.m_nMaxLoadFactor;

	}
	// 移动赋值
	hashtable& operator=(hashtable&& rhs){
		if(this == &rhs){
			return *this;
		}

		clear();

		// 拷贝
		m_stBucket = std::move(rhs.m_stBucket);
		m_nBucketSize = rhs.m_nBucketSize;
		m_nEleNum = rhs.m_nEleNum;

		m_fHash = rhs.m_fHash;
		m_fKey = rhs.m_fKey;
		m_nMaxLoadFactor = rhs.m_nMaxLoadFactor;

		rhs.m_nBucketSize = 0;
		rhs.m_nEleNum = 0;
	}
	~hashtable() { 
		// 释放持有的节点
		clear(); 
	}

	// 2.迭代器支持
	iterator begin() noexcept { 
		for (size_type n = 0; n < m_nBucketSize; ++n) {
			if (m_stBucket[n])  
				return iterator(m_stBucket[n], this);
		}
		return iterator(nullptr, this);
	}
	iterator end() noexcept { 
		return iterator(nullptr, this); 
	}

	const_iterator begin() const noexcept { 
		for (size_type n = 0; n < m_nBucketSize; ++n) {
			if (m_stBucket[n])  
				return const_iterator(m_stBucket[n], this);
		}
		return const_iterator(nullptr, this);
	}
	const_iterator end() const noexcept { 
		return const_iterator(nullptr, this); 
	}	

	local_iterator begin(size_type n) noexcept {
		return local_iterator(m_stBucket[n]);
	}
	local_iterator end(size_type n) noexcept {
		return local_iterator(nullptr);
	}
	const_local_iterator begin(size_type n) const noexcept {
		return const_local_iterator(m_stBucket[n]);
	}
	const_local_iterator end(size_type n) const noexcept {
		return const_local_iterator(nullptr);
	}
	
	// 3.增，删，查
	bool empty() const noexcept { return m_nEleNum == 0; }
	size_type size() const noexcept { return m_nEleNum; }

	iterator insert_multi_noresize(const ele_type& stEle){
		// 获取元素的键，计算哈希值
		node* pNew = nullptr;
		try
		{
			pNew = node_allocator::allocate(1);
			// 元素类型可能是常量类型，对常量类型执行构造是允许的
			node_allocator::construct(pNew, stEle);
		}
		catch(...)
		{
			if(pNew){
				node_allocator::deallocate(pNew);
			}
			throw;
		}
		
		size_type nIndex = m_fHash(get_key(pNew->m_nEle), m_nBucketSize);
		node* pPrev = nullptr;
		node* pNode = m_stBucket[nIndex];
		while(pNode) {
			if (m_fKey(get_key(pNode->m_nEle), get_key(pNew->m_nEle))){
				break;
			}
			else {
				pPrev = pNode;
				pNode = pNode->m_pNext;
			}
		}
		
		if(pNode) {
			if(pPrev == nullptr){
				m_stBucket[nIndex] = pNew;
				pNew->m_pNext = pNode;
			}
			else{
				pPrev->m_pNext = pNew;
				pNew->m_pNext = pNode;
			}
		}
		else {
			pNew->m_pNext = m_stBucket[nIndex];
			m_stBucket[nIndex] = pNew;
		}
		
		m_nEleNum++;
		return iterator(pNew, this);
	}

	iterator insert_multi_noresize(ele_type&& stEle){
		// 获取元素的键，计算哈希值
		node* pNew = nullptr;
		try
		{
			pNew = node_allocator::allocate(1);
			// 元素类型可能是常量类型，对常量类型执行构造是允许的
			node_allocator::construct(pNew, std::move(stEle));
		}
		catch(...)
		{
			if(pNew){
				node_allocator::deallocate(pNew);
			}
			throw;
		}
		
		size_type nIndex = m_fHash(get_key(pNew->m_nEle), m_nBucketSize);
		node* pPrev = nullptr;
		node* pNode = m_stBucket[nIndex];
		while(pNode) {
			if (m_fKey(get_key(pNode->m_nEle), get_key(pNew->m_nEle))){
				break;
			}
			else {
				pPrev = pNode;
				pNode = pNode->m_pNext;
			}
		}
		
		if(pNode) {
			if(pPrev == nullptr){
				m_stBucket[nIndex] = pNew;
				pNew->m_pNext = pNode;
			}
			else {
				pPrev->m_pNext = pNew;
				pNew->m_pNext = pNode;
			}
		}else {
			pNew->m_pNext = m_stBucket[nIndex];
			m_stBucket[nIndex] = pNew;
		}
		
		m_nEleNum++;
		return iterator(pNew, this);
	}

	pair<iterator, bool> insert_unique_noresize(const ele_type& stEle){
		size_type nIndex = m_fHash(get_key(stEle), m_nBucketSize);
		node* pPrev = nullptr;
		node* pNode = m_stBucket[nIndex];
		while(pNode) {
			if (m_fKey(get_key(pNode->m_nEle), get_key(stEle))){
				break;
			}
			else {
				pPrev = pNode;
				pNode = pNode->m_pNext;
			}
		}
		
		if(pNode) {
			return mystl::pair<iterator, bool>(iterator(nullptr, this), false);
		} else {
			node* pNew = nullptr;
			try
			{
				pNew = node_allocator::allocate(1);
				node_allocator::construct(pNew, stEle);
			}
			catch(...)
			{
				if(pNew){
					node_allocator::deallocate(pNew);
				}
				throw;
			}

			pNew->m_pNext = m_stBucket[nIndex];
			m_stBucket[nIndex] = pNew;
			m_nEleNum++;
			return mystl::pair<iterator, bool>(iterator(pNew, this), true);
		}
	}

	pair<iterator, bool> insert_unique_noresize(ele_type&& stEle){
		size_type nIndex = m_fHash(get_key(stEle), m_nBucketSize);
		node* pPrev = nullptr;
		node* pNode = m_stBucket[nIndex];
		while(pNode) {
			if (m_fKey(get_key(pNode->m_nEle), get_key(stEle))){
				break;
			}
			else {
				pPrev = pNode;
				pNode = pNode->m_pNext;
			}
		}
		
		if(pNode) {
			return mystl::pair<iterator, bool>(iterator(nullptr, this), false);
		} else {
			node* pNew = nullptr;
			try
			{
				pNew = node_allocator::allocate(1);
				// 元素类型可能是常量类型，对常量类型执行构造是允许的
				node_allocator::construct(pNew, std::move(stEle));
			}
			catch(...)
			{
				if(pNew){
					node_allocator::deallocate(pNew);
				}
				throw;
			}

			pNew->m_pNext = m_stBucket[nIndex];
			m_stBucket[nIndex] = pNew;
			m_nEleNum++;
			return mystl::pair<iterator, bool>(iterator(pNew, this), true);
		}
	}

	iterator insert_multi(const ele_type& stEle) {
		rehash_if_need(1);
		return insert_multi_noresize(stEle);
	}
	iterator insert_multi(ele_type&& stEle)
	{ 
		rehash_if_need(1);
		return insert_multi_noresize(std::move(stEle)); 
	}

	pair<iterator, bool> insert_unique(const ele_type& stEle) {
		rehash_if_need(1);
		return insert_unique_noresize(stEle);
	}
	pair<iterator, bool> insert_unique(ele_type&& stEle) {
		rehash_if_need(1);
		return insert_unique_noresize(std::move(stEle));
	}

	// 动态哈希表
	void rehash_if_need(size_type nAddNum){
		if (static_cast<float>(m_nEleNum + nAddNum) <= (float)m_nBucketSize * m_nMaxLoadFactor){
			return;
		}

		// 需要寻找一个更大的桶容量
		size_type nTarBucketSize = (m_nEleNum + nAddNum) / m_nMaxLoadFactor;
		nTarBucketSize = ht_next_prime(nTarBucketSize);
		if(nTarBucketSize == m_nBucketSize){
			return;
		}

		// 元素转移到新桶
		bucket stTmpBucket;
		try{
			stTmpBucket.reserve(nTarBucketSize);
		}
		catch(...){
			throw;
		}
		
		for(size_t i = 0; i < nTarBucketSize; i++){
			stTmpBucket.push_back(nullptr);
		}

		for (size_type i = 0; i < m_nBucketSize; ++i) {
			node* pSource = m_stBucket[i];
			if(pSource == nullptr){
				continue;
			}

			node* pCopy = nullptr;
			while(pSource){
				node* pSourceCur = pSource;
				pSource = pSource->m_pNext;

				try
				{
					pCopy = node_allocator::allocate(1);
					// 元素类型可能是常量类型，对常量类型执行构造是允许的
					node_allocator::construct(pCopy, pSourceCur->m_nEle);
				}
				catch(...)
				{
					if(pCopy){
						node_allocator::deallocate(pCopy);
					}
					
					for(size_type k = 0; k <= nTarBucketSize; k++){
						node* pTmp = stTmpBucket[k];
						while(pTmp){
							node* pFree = pTmp;
							pTmp = pTmp->m_pNext;
							node_allocator::destroy(pFree);
							node_allocator::deallocate(pFree);
						}
					}
					throw;
				}

				size_type nIndex = m_fHash(get_key(pCopy->m_nEle), nTarBucketSize);
				node* pTmp = stTmpBucket[nIndex];
				node* pPreTmp = nullptr;
				// 寻找stEle最后一次出现位置的前一位置
				while(pTmp){
					if(m_fKey(get_key(pTmp->m_nEle), get_key(pCopy->m_nEle))){
						pPreTmp = pTmp;
						pTmp = pTmp->m_pNext;
						while(pTmp && m_fKey(get_key(pTmp->m_nEle), get_key(pCopy->m_nEle))){
							pPreTmp = pTmp;
							pTmp = pTmp->m_pNext;
						}

						break;
					}
					else{
						pTmp = pTmp->m_pNext;
					}
				}
				
				if(pPreTmp == nullptr){
					pCopy->m_pNext = stTmpBucket[nIndex];
					stTmpBucket[nIndex] = pCopy;
				}
				else{
					pCopy->m_pNext = pPreTmp->m_pNext;
					pPreTmp->m_pNext = pCopy;
				}
			}
		}
		
		// 先释放自身
		size_t nEleNum = m_nEleNum;
		clear();

		m_stBucket = std::move(stTmpBucket);
		m_nBucketSize = nTarBucketSize;
		m_nEleNum = nEleNum;
	}
	
	void erase(const iterator& position){
		node* pNode = position.m_pNode;
		erase(pNode);
	}
	void erase(const const_iterator& position){
		node* pNode = position.m_pNode;
		erase(pNode);
	}
	size_type erase_multi(const key_type& key){
		mystl::array<node*> stNodes;
		size_type nIndex = m_fHash(key, m_nBucketSize);
		node* pNode = m_stBucket[nIndex];
		while(pNode){
			if(m_fKey(get_key(pNode->m_nEle), key)){
				stNodes.push_back(pNode);
			}

			pNode = pNode->m_pNext;
		}

		for(size_type i = 0; i < stNodes.size(); i++){
			erase(stNodes[i]);
		}
	}
	size_type erase_unique(const key_type& key){
		mystl::array<node*> stNodes;
		size_type nIndex = m_fHash(key, m_nBucketSize);
		node* pNode = m_stBucket[nIndex];
		while(pNode){
			if(m_fKey(get_key(pNode->m_nEle), key)){
				stNodes.push_back(pNode);
				break;
			}

			pNode = pNode->m_pNext;
		}

		for(size_type i = 0; i < stNodes.size(); i++){
			erase(stNodes[i]);
		}
	}

	void clear(){
		for(size_type k = 0; k < m_nBucketSize; k++){
			node* pTmp = m_stBucket[k];
			while(pTmp){
				node* pFree = pTmp;
				pTmp = pTmp->m_pNext;
				node_allocator::destroy(pFree);
				node_allocator::deallocate(pFree);
			}
			m_stBucket[k] = nullptr;
		}

		m_nEleNum = 0;
	}

	void swap(hashtable& rhs) noexcept{
		m_stBucket.swap(rhs.m_stBucket);
		m_nBucketSize = rhs.m_nBucketSize;
		m_nEleNum = rhs.m_nEleNum;

		m_fHash = rhs.m_fHash;
		m_fKey = rhs.m_fKey;
		m_nMaxLoadFactor = rhs.m_nMaxLoadFactor;
	}
	
	size_type count(const key_type& key) const{
		size_type nCount = 0;
		size_type nIndex = m_fHash(key, m_nBucketSize);
		node* pNode = m_stBucket[nIndex];
		while(pNode){
			if(m_fKey(get_key(pNode->m_nEle), key)){
				nCount++;
			}

			pNode = pNode->m_pNext;
		}

		return nCount;
	}
	iterator find(const key_type& key)
	{
		size_type nIndex = m_fHash(key, m_nBucketSize);
		node* pNode = m_stBucket[nIndex];
		node* pTmp = nullptr;
		while(pNode){
			if(m_fKey(get_key(pNode->m_nEle), key)){
				pTmp = pNode;
				break;
			}

			pNode = pNode->m_pNext;
		}

		return iterator(pTmp, this);
	}
	const_iterator find(const key_type& key) const{
		size_type nIndex = m_fHash(key, m_nBucketSize);
		node* pNode = m_stBucket[nIndex];
		node* pTmp = nullptr;
		while(pNode){
			if(m_fKey(get_key(pNode->m_nEle), key)){
				pTmp = pNode;
				break;
			}

			pNode = pNode->m_pNext;
		}

		return const_iterator(pTmp, this);
	}
	pair<iterator, iterator> equal_range_multi(const key_type& key){
		mystl::array<node*> stNodes;
		size_type nIndex = m_fHash(key, m_nBucketSize);
		node* pNode = m_stBucket[nIndex];
		while(pNode){
			if(m_fKey(get_key(pNode->m_nEle), key)){
				stNodes.push_back(pNode);
			}

			pNode = pNode->m_pNext;
		}
		
		size_type nSize = stNodes.size();
		if(nSize > 0){
			return mystl::pair<iterator,iterator>(iterator(stNodes[0], this), iterator(stNodes[nSize - 1], this));
		}
		else{
			return mystl::pair<iterator,iterator>(iterator(nullptr, this), iterator(nullptr, this));
		}
	}
	pair<const_iterator, const_iterator> equal_range_multi(const key_type& key) const{
		mystl::array<node*> stNodes;
		size_type nIndex = m_fHash(key, m_nBucketSize);
		node* pNode = m_stBucket[nIndex];
		while(pNode){
			if(m_fKey(get_key(pNode->m_nEle), key)){
				stNodes.push_back(pNode);
			}

			pNode = pNode->m_pNext;
		}
		
		size_type nSize = stNodes.size();
		if(nSize > 0){
			return mystl::pair<const_iterator,const_iterator>(const_iterator(stNodes[0], this), const_iterator(stNodes[nSize - 1], this));
		}
		else{
			return mystl::pair<const_iterator,const_iterator>(const_iterator(nullptr, this), const_iterator(nullptr, this));
		}
	}

	void erase(node* pNode){
		if(pNode == nullptr){
			return;
		}

		size_type nIndex = m_fHash(get_key(pNode->m_nEle), m_nBucketSize);
		node* pPre = nullptr;
		node* pCur = m_stBucket[nIndex];
		while (pCur) {
			if(pCur == pNode){
				break;
			}

			pPre = pCur;
			pCur = pCur->m_pNext;
		}

		if(pCur == nullptr){
			return;
		}

		if(pPre == nullptr){
			m_stBucket[nIndex] = nullptr;
		}
		else{
			pPre->m_pNext = pNode->m_pNext;
		}
		node_allocator::destroy(pNode);
		node_allocator::deallocate(pNode);
		m_nEleNum--;	
	}
private:
	node* get_next(node* pNode) const{
		if(pNode->m_pNext){
			return pNode->m_pNext;
		}
		
		size_type nIndex = m_fHash(get_key(pNode->m_nEle), m_nBucketSize);
		for(size_type k = nIndex + 1; k < m_nBucketSize; k++){
			if(m_stBucket[k]){
				return m_stBucket[k];
			}
		} 
		return nullptr;
	}
};

}
#endif