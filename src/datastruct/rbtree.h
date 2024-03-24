#ifndef MYSTL_RBTREE_H
#define MYSTL_RBTREE_H
#include "array.h"
namespace mystl {

template <class T>
struct rbtree_node;

enum rbtree_color{
	color_red,
	color_black,
	color_blackblack,
};

// 红黑树节点元素两种类型
// mtstl::pair<const int, int>形式，此形式下，pair的模板参数１必须是常量类型，模板参数２是非常量类型
// const int，此形式下，元素类型是常量类型
template<class T>
struct element_node{
    element_node* m_pPre;
    element_node* m_pNext;
    T m_nEle;
    rbtree_node<T>* m_pTreeNode = nullptr;
	element_node(rbtree_node<T>* pTreeNode, const T& nEle):m_nEle(nEle){
		m_pPre = this;
		m_pNext = this;
		m_pTreeNode = pTreeNode;
	}

	element_node(rbtree_node<T>* pTreeNode, T&& nEle):m_nEle(std::move(nEle)){
		m_pPre = this;
		m_pNext = this;
		m_pTreeNode = pTreeNode;
	}
};

template <class T>
struct rbtree_node {
	rbtree_node* m_pLeft = nullptr;
    rbtree_node* m_pRight = nullptr;
    rbtree_node* m_pParent = nullptr;
	element_node<T>* m_pEleList = nullptr;
	rbtree_color m_stColor = rbtree_color::color_red;
};

template <class T>
struct rbtree_ele_traits_imp {
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

template <class T1, class T2>
struct rbtree_ele_traits_imp<mystl::pair<T1,T2>> {
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

template <class T, class TreeNodeAlloc, class EleNodeAlloc>
class rbtree;
template <class T, class TreeNodeAlloc, class EleNodeAlloc>
struct rbtree_iterator;
template <class T, class TreeNodeAlloc, class EleNodeAlloc>
struct rbtree_const_iterator;

// 常量，非常量迭代器需单独定义．如果合并定义，无法区分set下，常量迭代器，非常量迭代器完全一致．
template <class T, class TreeNodeAlloc, class EleNodeAlloc>
struct rbtree_iterator {
    typedef mystl::bidirectional_iterator_tag  						    iterator_category;
    typedef T                               					        value_type;
	typedef T*                             					            pointer;
	typedef T&                             					            reference;
    typedef size_t                          					        size_type;
	typedef ptrdiff_t                      						        difference_type;

	typedef typename mystl::rbtree<T, TreeNodeAlloc, EleNodeAlloc>      container;
	typedef typename container::ele_node                                ele_node;
public:
    ele_node* m_pEleNode = nullptr;
	container* m_pRbTree = nullptr;
public:
	rbtree_iterator() = default;
	rbtree_iterator(ele_node* pNode, container* pRbTree) {
		m_pEleNode = pNode;
		m_pRbTree = pRbTree;
	}
	rbtree_iterator(const rbtree_iterator& rhs) {
		m_pEleNode = rhs.m_pEleNode;
		m_pRbTree = rhs.m_pRbTree;
	}
	
	rbtree_iterator& operator=(const rbtree_iterator& rhs) {
		if (this != &rhs) {
			m_pEleNode = rhs.m_pEleNode;
			m_pRbTree = rhs.m_pRbTree;
		}
		return *this;
	}
	
	// 哈希表通过迭代器解引用访问迭代器下面的元素时，
	// 对形式１，允许修改元素值，不允许修改元素键
	// 对形式２，不允许修改元素值/键
	reference operator*()  const { return m_pEleNode->m_nEle; }
	pointer   operator->() const { return &(operator*()); }
	rbtree_iterator& operator++() {
		m_pEleNode = m_pRbTree->get_next(m_pEleNode);
		return *this;
	}
	rbtree_iterator operator++(int) {
		rbtree_iterator tmp = *this;
		++*this;
		return tmp;
	}

	rbtree_iterator& operator--() {
		m_pEleNode = m_pRbTree->get_pre(m_pEleNode);
		return *this;
	}
	rbtree_iterator operator--(int) {
		rbtree_iterator tmp = *this;
		--*this;
		return tmp;
	}

	bool operator==(const rbtree_iterator& other) const { 
		return m_pEleNode == other.m_pEleNode && m_pRbTree == other.m_pRbTree; }
	bool operator!=(const rbtree_iterator& other) const { return !(*this == other); }
};

// 常量，非常量迭代器需单独定义．如果合并定义，无法区分set下，常量迭代器，非常量迭代器完全一致．
template <class T, class TreeNodeAlloc, class EleNodeAlloc>
struct rbtree_const_iterator {
    typedef mystl::bidirectional_iterator_tag  						    iterator_category;
    typedef T                               					        value_type;
	typedef const T*                             					    pointer;
	typedef const T&                             					    reference;
    typedef size_t                          					        size_type;
	typedef ptrdiff_t                      						        difference_type;

	typedef typename mystl::rbtree<T, TreeNodeAlloc, EleNodeAlloc>      container;
	typedef typename container::ele_node                                ele_node;
public:
    ele_node* m_pEleNode = nullptr;
	const container* m_pRbTree = nullptr;
public:
	rbtree_const_iterator() = default;
	rbtree_const_iterator(ele_node* pNode, const container* pRbTree) {
		m_pEleNode = pNode;
		m_pRbTree = pRbTree;
	}
	rbtree_const_iterator(const rbtree_const_iterator& rhs) {
		m_pEleNode = rhs.m_pEleNode;
		m_pRbTree = rhs.m_pRbTree;
	}
	
	rbtree_const_iterator& operator=(const rbtree_const_iterator& rhs) {
		if (this != &rhs) {
			m_pEleNode = rhs.m_pEleNode;
			m_pRbTree = rhs.m_pRbTree;
		}
		return *this;
	}
	
	// 哈希表通过迭代器解引用访问迭代器下面的元素时，
	// 对形式１，允许修改元素值，不允许修改元素键
	// 对形式２，不允许修改元素值/键
	// 这通过元素自身的类型中const修饰来实现
	reference operator*()  const { return m_pEleNode->m_nEle; }
	pointer   operator->() const { return &(operator*()); }
	rbtree_const_iterator& operator++() {
		m_pEleNode = m_pRbTree->get_next(m_pEleNode);
		return *this;
	}
	rbtree_const_iterator operator++(int) {
		rbtree_iterator tmp = *this;
		++*this;
		return tmp;
	}

	rbtree_const_iterator& operator--() {
		m_pEleNode = m_pRbTree->get_pre(m_pEleNode);
		return *this;
	}
	rbtree_const_iterator operator--(int) {
		rbtree_iterator tmp = *this;
		--*this;
		return tmp;
	}

	bool operator==(const rbtree_const_iterator& other) const { 
		return m_pEleNode == other.m_pEleNode && m_pRbTree == other.m_pRbTree; }
	bool operator!=(const rbtree_const_iterator& other) const { return !(*this == other); }
};

// 哈希表
// 接收两种元素类型
// 形式１：mystl::pair<const int,int>，模板参数１是常量类型，模板参数２是非常量类型
// 形式２：const int，常量类型
template <class T, class TreeNodeAlloc = mystl::allocator<rbtree_node<T>>, class EleNodeAlloc = mystl::allocator<element_node<T>>>
class rbtree {
	// 对本身已经是有const修饰的类型再加const修饰是允许的，叠加后依然等同一个const的效果
	friend struct mystl::rbtree_iterator<T, TreeNodeAlloc, EleNodeAlloc>;
    friend struct mystl::rbtree_const_iterator<T, TreeNodeAlloc, EleNodeAlloc>;

public:
	typedef rbtree_ele_traits_imp<T> 				ele_traits;
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
  	typedef TreeNodeAlloc                           						treenode_allocator;
	typedef EleNodeAlloc                           							elenode_allocator;
	typedef T*                              								pointer;
	typedef const T*                        								const_pointer;
	typedef T&                              								reference;
	typedef const T&                        								const_reference;
	typedef size_t                          								size_type;
	typedef ptrdiff_t                       								difference_type;
	
	typedef mystl::rbtree_iterator<T,TreeNodeAlloc, EleNodeAlloc>           iterator;
    typedef mystl::rbtree_const_iterator<T,TreeNodeAlloc, EleNodeAlloc>     const_iterator;

	typedef rbtree_node<T>                                               	node;
	typedef element_node<T>                                                 ele_node;
	typedef int (*key_fun)(const key_type&, const key_type&);
private:
	static int keyfun(const key_type& nK1, const key_type& nK2){
		if(nK1 == nK1){
			return 0;
		}
		else if(nK1 < nK2){
			return -1;
		}
		else{
			return 1;
		}
	}

private:
	node*    	m_pRoot = nullptr;
	size_type 	m_nEleNum = 0;
	key_fun   	m_fKey;

public:
	// 1.类型行为控制
	explicit rbtree(key_fun fKey = &rbtree::keyfun) {
		m_fKey = fKey;
	}

	// 拷贝构造
	rbtree(const rbtree& rhs) {
		int nEleNum = 0;
		node* pTempRoot = nullptr;
		try{
			VisitForCons(rhs.m_pRoot, VForConsFun, nullptr, nEleNum, pTempRoot);
		}catch(...){
			clear(pTempRoot);
			throw;
		}
		m_pRoot = pTempRoot;
		m_nEleNum = nEleNum;
		m_fKey = rhs.m_fKey;
	}

	rbtree(rbtree&& rhs) noexcept {
		m_fKey = rhs.m_fKey;
		m_nEleNum = rhs.m_nEleNum;
		m_pRoot = rhs.m_pRoot;
		rhs.m_nEleNum = 0;
		rhs.m_pRoot = nullptr;
	}
	rbtree& operator=(const rbtree& rhs)
	{
		if(this == &rhs){
			return *this;
		}
		
		int nEleNum = 0;
		node* pTempRoot = nullptr;
		try{
			VisitForCons(rhs.m_pRoot, VForConsFun, nullptr, nEleNum, pTempRoot);
		}catch(...){
			clear(pTempRoot);
			throw;
		}

		clear(m_pRoot);
		m_pRoot = pTempRoot;
		m_nEleNum = nEleNum;
		m_fKey = rhs.m_fKey;
	}

	// 移动赋值
	rbtree& operator=(rbtree&& rhs){
		if(this == &rhs){
			return *this;
		}

		m_fKey = rhs.m_fKey;
		m_nEleNum = rhs.m_nEleNum;
		m_pRoot = rhs.m_pRoot;
		rhs.m_nEleNum = 0;
		rhs.m_pRoot = nullptr;
	}
	~rbtree() { 
		clear(m_pRoot); 
	}

	// 2.迭代器支持
	iterator begin() noexcept { 
		element_node* pNode = min();
		return iterator(pNode, this);
	}
	iterator end() noexcept { 
		return iterator(nullptr, this); 
	}

	const_iterator begin() const noexcept { 
		element_node* pNode = min();
		return const_iterator(pNode, this);
	}
	const_iterator end() const noexcept { 
		return const_iterator(nullptr, this); 
	}	

	// 3.增，删，查
	bool empty() const noexcept { return m_nEleNum == 0; }
	size_type size() const noexcept { return m_nEleNum; }

	// 插入新元素
	iterator insert_multi(const ele_type& stEle){
		if (m_pRoot == nullptr) {
			node* pNewNode = nullptr;
			try{
				pNewNode = treenode_allocator::allocate(1);
				treenode_allocator::construct(pNewNode);
			}catch(...){
				if(pNewNode){
					treenode_allocator::deallocate(pNewNode);
				}
				throw;
			}

			// 创建新的元素节点并插入到树节点下
			ele_node* pEleNode = nullptr;
			try{
				pEleNode = elenode_allocator::allocate(1);
				elenode_allocator::construct(pEleNode, pNewNode, stEle);
			}catch(...){
				if(pEleNode){
					elenode_allocator::deallocate(pEleNode);
				}
				treenode_allocator::destroy(pNewNode);
				treenode_allocator::deallocate(pNewNode);
				throw;
			}

			pNewNode->m_pEleList = pEleNode;
			m_pRoot = pNewNode;
			m_nEleNum++;
			return iterator(pEleNode, this);
		}

		node* pCur = m_pRoot;
		node* pParent = nullptr;
		while (pCur) {
			int nCmp = m_fKey(get_key(pCur->m_stEle), get_key(stEle));
			if (nCmp > 0) {
				pParent = pCur;
				pCur = pCur->m_pLeft;
			}
			else if(nCmp < 0){
				pParent = pCur;
				pCur = pCur->m_pRight;
			}
			else{
				// 构建新的元素节点，插入树节点
				// 创建新的元素节点并插入到树节点下
				ele_node* pEleNode = nullptr;
				try{
					pEleNode = elenode_allocator::allocate(1);
					elenode_allocator::construct(pEleNode, pNewNode, stEle);
				}catch(...){
					if(pEleNode){
						elenode_allocator::deallocate(pEleNode);
					}
					throw;
				}

				// 双向链表尾部插入
				pCur->m_pEleList->m_pPre = pEleNode;
				pEleNode->m_pNext = pCur->m_pEleList;

				ele_node* pLastNode = pCur->m_pEleList->m_pPre;
				pLastNode->m_pNext = pEleNode;
				pEleNode->m_pPre = pLastNode;
				m_nEleNum++;
				return iterator(pEleNode, this);
			}
		}

		// 构建新的树节点，元素节点
		node* pNewNode = nullptr;
		try{
			pNewNode = treenode_allocator::allocate(1);
			treenode_allocator::construct(pNewNode);
		}catch(...){
			if(pNewNode){
				treenode_allocator::deallocate(pNewNode);
			}
			throw;
		}

		// 创建新的元素节点并插入到树节点下
		ele_node* pEleNode = nullptr;
		try{
			pEleNode = elenode_allocator::allocate(1);
			elenode_allocator::construct(pEleNode, pNewNode, stEle);
		}catch(...){
			if(pEleNode){
				elenode_allocator::deallocate(pEleNode);
			}
			treenode_allocator::destroy(pNewNode);
			treenode_allocator::deallocate(pNewNode);
			throw;
		}

		pNewNode->m_pEleList = pEleNode;
		int nCmp2 = m_fKey(get_key(pParent->m_stEle), get_key(stEle));
		if(nCmp2 > 0)){
			pParent->m_pLeft = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		else{
			pParent->m_pRight = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		if (pParent->m_stColor == rbtree_color::color_red) {
			adjust_when_node_and_parent_is_red(pNewNode, pParent);
		}
		m_nEleNum++;
		return iterator(pEleNode, this);
	}

	iterator insert_multi(ele_type&& stEle){
		if (m_pRoot == nullptr) {
			node* pNewNode = nullptr;
			try{
				pNewNode = treenode_allocator::allocate(1);
				treenode_allocator::construct(pNewNode);
			}catch(...){
				if(pNewNode){
					treenode_allocator::deallocate(pNewNode);
				}
				throw;
			}

			// 创建新的元素节点并插入到树节点下
			ele_node* pEleNode = nullptr;
			try{
				pEleNode = elenode_allocator::allocate(1);
				elenode_allocator::construct(pEleNode, pNewNode, std::move(stEle));
			}catch(...){
				if(pEleNode){
					elenode_allocator::deallocate(pEleNode);
				}
				treenode_allocator::destroy(pNewNode);
				treenode_allocator::deallocate(pNewNode);
				throw;
			}

			pNewNode->m_pEleList = pEleNode;
			m_pRoot = pNewNode;
			m_nEleNum++;
			return iterator(pEleNode, this);
		}

		node* pCur = m_pRoot;
		node* pParent = nullptr;
		while (pCur) {
			int nCmp = m_fKey(get_key(pCur->m_stEle), get_key(stEle));
			if (nCmp > 0) {
				pParent = pCur;
				pCur = pCur->m_pLeft;
			}
			else if(nCmp < 0){
				pParent = pCur;
				pCur = pCur->m_pRight;
			}
			else{
				// 构建新的元素节点，插入树节点
				// 创建新的元素节点并插入到树节点下
				ele_node* pEleNode = nullptr;
				try{
					pEleNode = elenode_allocator::allocate(1);
					elenode_allocator::construct(pEleNode, pNewNode, std::move(stEle));
				}catch(...){
					if(pEleNode){
						elenode_allocator::deallocate(pEleNode);
					}
					throw;
				}

				// 双向链表尾部插入
				pCur->m_pEleList->m_pPre = pEleNode;
				pEleNode->m_pNext = pCur->m_pEleList;

				ele_node* pLastNode = pCur->m_pEleList->m_pPre;
				pLastNode->m_pNext = pEleNode;
				pEleNode->m_pPre = pLastNode;
				m_nEleNum++;
				return iterator(pEleNode, this);
			}
		}

		// 构建新的树节点，元素节点
		node* pNewNode = nullptr;
		try{
			pNewNode = treenode_allocator::allocate(1);
			treenode_allocator::construct(pNewNode);
		}catch(...){
			if(pNewNode){
				treenode_allocator::deallocate(pNewNode);
			}
			throw;
		}

		int nCmp2 = m_fKey(get_key(pParent->m_stEle), get_key(stEle));
		// 创建新的元素节点并插入到树节点下
		ele_node* pEleNode = nullptr;
		try{
			pEleNode = elenode_allocator::allocate(1);
			elenode_allocator::construct(pEleNode, pNewNode, std::move(stEle));
		}catch(...){
			if(pEleNode){
				elenode_allocator::deallocate(pEleNode);
			}
			treenode_allocator::destroy(pNewNode);
			treenode_allocator::deallocate(pNewNode);
			throw;
		}

		pNewNode->m_pEleList = pEleNode;
		if(nCmp2 > 0)){
			pParent->m_pLeft = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		else{
			pParent->m_pRight = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		if (pParent->m_stColor == rbtree_color::color_red) {
			adjust_when_node_and_parent_is_red(pNewNode, pParent);
		}
		m_nEleNum++;
		return iterator(pEleNode, this);
	}

	pair<iterator, bool> insert_unique(const ele_type& stEle){
		if (m_pRoot == nullptr) {
			node* pNewNode = nullptr;
			try{
				pNewNode = treenode_allocator::allocate(1);
				treenode_allocator::construct(pNewNode);
			}catch(...){
				if(pNewNode){
					treenode_allocator::deallocate(pNewNode);
				}
				throw;
			}

			// 创建新的元素节点并插入到树节点下
			ele_node* pEleNode = nullptr;
			try{
				pEleNode = elenode_allocator::allocate(1);
				elenode_allocator::construct(pEleNode, pNewNode, stEle);
			}catch(...){
				if(pEleNode){
					elenode_allocator::deallocate(pEleNode);
				}
				treenode_allocator::destroy(pNewNode);
				treenode_allocator::deallocate(pNewNode);
				throw;
			}

			pNewNode->m_pEleList = pEleNode;
			m_pRoot = pNewNode;
			m_nEleNum++;
			return mystl::pair(iterator(pEleNode, this), true);
		}

		node* pCur = m_pRoot;
		node* pParent = nullptr;
		while (pCur) {
			int nCmp = m_fKey(get_key(pCur->m_stEle), get_key(stEle));
			if (nCmp > 0) {
				pParent = pCur;
				pCur = pCur->m_pLeft;
			}
			else if(nCmp < 0){
				pParent = pCur;
				pCur = pCur->m_pRight;
			}
			else{
				return mystl::pair(iterator(nullptr, this), false);
			}
		}

		// 构建新的树节点，元素节点
		node* pNewNode = nullptr;
		try{
			pNewNode = treenode_allocator::allocate(1);
			treenode_allocator::construct(pNewNode);
		}catch(...){
			if(pNewNode){
				treenode_allocator::deallocate(pNewNode);
			}
			throw;
		}

		// 创建新的元素节点并插入到树节点下
		ele_node* pEleNode = nullptr;
		try{
			pEleNode = elenode_allocator::allocate(1);
			elenode_allocator::construct(pEleNode, pNewNode, stEle);
		}catch(...){
			if(pEleNode){
				elenode_allocator::deallocate(pEleNode);
			}
			treenode_allocator::destroy(pNewNode);
			treenode_allocator::deallocate(pNewNode);
			throw;
		}

		pNewNode->m_pEleList = pEleNode;
		int nCmp2 = m_fKey(get_key(pParent->m_stEle), get_key(stEle));
		if(nCmp2 > 0)){
			pParent->m_pLeft = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		else{
			pParent->m_pRight = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		if (pParent->m_stColor == rbtree_color::color_red) {
			adjust_when_node_and_parent_is_red(pNewNode, pParent);
		}
		m_nEleNum++;
		return mystl::pair(iterator(pEleNode, this), true);
	}
	pair<iterator, bool> insert_unique(ele_type&& stEle){
		if (m_pRoot == nullptr) {
			node* pNewNode = nullptr;
			try{
				pNewNode = treenode_allocator::allocate(1);
				treenode_allocator::construct(pNewNode);
			}catch(...){
				if(pNewNode){
					treenode_allocator::deallocate(pNewNode);
				}
				throw;
			}

			// 创建新的元素节点并插入到树节点下
			ele_node* pEleNode = nullptr;
			try{
				pEleNode = elenode_allocator::allocate(1);
				elenode_allocator::construct(pEleNode, pNewNode, std::move(stEle));
			}catch(...){
				if(pEleNode){
					elenode_allocator::deallocate(pEleNode);
				}
				treenode_allocator::destroy(pNewNode);
				treenode_allocator::deallocate(pNewNode);
				throw;
			}

			pNewNode->m_pEleList = pEleNode;
			m_pRoot = pNewNode;
			m_nEleNum++;
			return mystl::pair(iterator(pEleNode, this), true);
		}

		node* pCur = m_pRoot;
		node* pParent = nullptr;
		while (pCur) {
			int nCmp = m_fKey(get_key(pCur->m_stEle), get_key(stEle));
			if (nCmp > 0) {
				pParent = pCur;
				pCur = pCur->m_pLeft;
			}
			else if(nCmp < 0){
				pParent = pCur;
				pCur = pCur->m_pRight;
			}
			else{
				return mystl::pair(iterator(nullptr, this), false);
			}
		}

		// 构建新的树节点，元素节点
		node* pNewNode = nullptr;
		try{
			pNewNode = treenode_allocator::allocate(1);
			treenode_allocator::construct(pNewNode);
		}catch(...){
			if(pNewNode){
				treenode_allocator::deallocate(pNewNode);
			}
			throw;
		}

		// 创建新的元素节点并插入到树节点下
		int nCmp2 = m_fKey(get_key(pParent->m_stEle), get_key(stEle));
		ele_node* pEleNode = nullptr;
		try{
			pEleNode = elenode_allocator::allocate(1);
			elenode_allocator::construct(pEleNode, pNewNode, std::move(stEle));
		}catch(...){
			if(pEleNode){
				elenode_allocator::deallocate(pEleNode);
			}
			treenode_allocator::destroy(pNewNode);
			treenode_allocator::deallocate(pNewNode);
			throw;
		}

		pNewNode->m_pEleList = pEleNode;
		if(nCmp2 > 0)){
			pParent->m_pLeft = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		else{
			pParent->m_pRight = pNewNode;
			pNewNode->m_pParent = pParent;
		}
		if (pParent->m_stColor == rbtree_color::color_red) {
			adjust_when_node_and_parent_is_red(pNewNode, pParent);
		}
		m_nEleNum++;
		return mystl::pair(iterator(pEleNode, this), true);
	}

	// 移除已有元素
	void erase(const iterator& position){
		erase(position.m_pEleNode);
	}
	void erase(const const_iterator& position){
		erase(position.m_pEleNode);
	}
	size_type erase_multi(const key_type& key){
		mystl::array<ele_node*> stEleNodes;
		node* pNode = m_pRoot;
		while(pNode){
			int nCmp = m_fKey(get_key(pNode->m_pEleList->m_nEle), key);
			if(nCmp == 0){
				break;
			}
			else if(nCmp < 0){
				pNode = pNode->m_pRight;
			}
			else{
				pNode = pNode->m_pLeft;
			}
		}
		if(pNode == nullptr){
			return 0;
		}
		// 搜集树节点下所有元素节点
		ele_node* pEleNode = pNode->m_pEleList;
		while(pEleNode){
			stEleNodes.push_back(pEleNode);
			pEleNode = pEleNode->m_pNext;
		}
		for(size_type i = 0; i < stEleNodes.size(); i++){
			erase(stEleNodes[i]);
		}
		return stEleNodes.size();
	}
	size_type erase_unique(const key_type& key){
		mystl::array<ele_node*> stEleNodes;
		node* pNode = m_pRoot;
		while(pNode){
			int nCmp = m_fKey(get_key(pNode->m_pEleList->m_nEle), key);
			if(nCmp == 0){
				break;
			}
			else if(nCmp < 0){
				pNode = pNode->m_pRight;
			}
			else{
				pNode = pNode->m_pLeft;
			}
		}
		if(pNode == nullptr){
			return 0;
		}
		// 搜集树节点下所有元素节点
		ele_node* pEleNode = pNode->m_pEleList;
		while(pEleNode){
			stEleNodes.push_back(pEleNode);
			pEleNode = pEleNode->m_pNext;
		}
		erase(stEleNodes[0]);
		return 1;
	}

	void clear(node* pRoot = nullptr){
		if(pRoot == nullptr){
			pRoot = m_pRoot;
		}

		int nVisitNum = 0;
		VisitForClear(pRoot, VForClearFun, nVisitNum);
		m_nEleNum -= nVisitNum;
	}

	void swap(rbtree& rhs) noexcept{
		node*    	pRoot = m_pRoot;
		size_type 	nEleNum = m_nEleNum;
		key_fun   	fKey = m_fKey;
		m_fKey = rhs.m_fKey;
		m_nEleNum = rhs.m_nEleNum;
		m_pRoot = rhs.m_pRoot;
		rhs.m_nEleNum = nEleNum;
		rhs.m_pRoot = pRoot;
		rhs.m_fKey = fKey;
	}
	
	// 查找元素
	iterator find(const key_type& key)
	{
		node* pNode = m_pRoot;
		while(pNode){
			int nCmp = m_fKey(get_key(pNode->m_stEle), key);
			if(nCmp == 0){
				break;
			}
			else if(nCmp < 0){
				pNode = pNode->m_pRight;
			}
			else{
				pNode = pNode->m_pLeft;
			}
		}
		if(pNode){
			return iterator(pNode->m_pEleList, this);
		}
		else{
			return iterator(nullptr, this);
		}
	}
	const_iterator find(const key_type& key) const{
		node* pNode = m_pRoot;
		while(pNode){
			int nCmp = m_fKey(get_key(pNode->m_stEle), key);
			if(nCmp == 0){
				break;
			}
			else if(nCmp < 0){
				pNode = pNode->m_pRight;
			}
			else{
				pNode = pNode->m_pLeft;
			}
		}
		if(pNode){
			return const_iterator(pNode->m_pEleList, this);
		}
		else{
			return const_iterator(nullptr, this);
		}
	}
	pair<iterator, iterator> equal_range_multi(const key_type& key){
		node* pNode = m_pRoot;
		while(pNode){
			int nCmp = m_fKey(get_key(pNode->m_stEle), key);
			if(nCmp == 0){
				break;
			}
			else if(nCmp < 0){
				pNode = pNode->m_pRight;
			}
			else{
				pNode = pNode->m_pLeft;
			}
		}
		if(pNode){
			return mystl::pair<iterator,iterator>(iterator(pNode->m_pEleList, this), iterator(pNode->m_pEleList->m_pPre, this));
		}
		else{
			return mystl::pair<iterator, iterator>(iterator(nullptr, this), iterator(nullptr, this));
		}
	}
	pair<const_iterator, const_iterator> equal_range_multi(const key_type& key) const{
		node* pNode = m_pRoot;
		while(pNode){
			int nCmp = m_fKey(get_key(pNode->m_stEle), key);
			if(nCmp == 0){
				break;
			}
			else if(nCmp < 0){
				pNode = pNode->m_pRight;
			}
			else{
				pNode = pNode->m_pLeft;
			}
		}
		if(pNode){
			return mystl::pair<const_iterator,const_iterator>(const_iterator(pNode->m_pEleList, this), const_iterator(pNode->m_pEleList->m_pPre, this));
		}
		else{
			return mystl::pair<const_iterator, const_iterator>(const_iterator(nullptr, this), const_iterator(nullptr, this));
		}
	}
private:
	element_node* get_next(element_node* pNode) const{
		
		return nullptr;
	}
	element_node* get_pre(element_node* pNode) const{
		
		return nullptr;
	}

	typedef node* (*VisitForConsFun)(rbtree* pTree, node* pNode, node* pOutParent, int& nEleNum);
	static node* VForConsFun(rbtree* pTree, node* pNode, node* pOutParent, int& nEleNum){
		// 完成pNode及其下方所有元素节点的拷贝
		node* pNewNode = nullptr;
		try{
			pNewNode = treenode_allocator::allocate(1);
			treenode_allocator::construct(pNewNode);
		}catch(...){
			if(pNewNode){
				treenode_allocator::deallocate(pNewNode);
			}
			throw;
		}
		
		// 向新node依次添加元素节点
		ele_node* pEleNode = pNode->m_pEleList;
		int nEleNumIn = 0;
		while(pEleNode){
			// 创建新的元素节点并插入到树节点下
			ele_node* pEleNode = nullptr;
			try{
				pEleNode = elenode_allocator::allocate(1);
				elenode_allocator::construct(pEleNode, pNewNode, pEleNode->m_nEle);
			}catch(...){
				if(pEleNode){
					elenode_allocator::deallocate(pEleNode);
				}
				// 链表上元素释放
				ele_node* pTempEleNode = pNewNode->m_pEleList;
				while(pTempEleNode){
					ele_node*pFreeEle = pTempEleNode;
					pTempEleNode = pTempEleNode->m_pNext;
					elenode_allocator::destroy(pFreeEle);
					elenode_allocator::deallocate(pFreeEle);
				}
				// 树节点释放
				treenode_allocator::destroy(pNewNode);
				treenode_allocator::deallocate(pNewNode);
				throw;
			}
			
			// 新元素节点插入树节点
			if(pNewNode->m_pEleList == nullptr){
				pNewNode->m_pEleList = pEleNode;
			}
			else{
				// 双向链表尾部插入
				pNewNode->m_pEleList->m_pPre = pEleNode;
				pEleNode->m_pNext = pNewNode->m_pEleList;

				ele_node* pLastNode = pNewNode->m_pEleList->m_pPre;
				pLastNode->m_pNext = pEleNode;
				pEleNode->m_pPre = pLastNode;
			}

			pEleNode = pEleNode->m_pNext;
			nEleNumIn++;
		}

		// 需要建立新节点和其父节点的关系
		if(pOutParent == nullptr){
			return pNewNode;
		}
		else{
			// 左右孩子和pNode一致
			pNewNode->m_pParent = pOutParent;
			if(pNode->m_pParent->m_pLeft = pNode){
				pOutParent->m_pLeft = pNewNode;
			}
			else{
				pOutParent->m_pRight = pNewNode;
			}
		}
		nEleNum += nEleNumIn;
	}
	void VisitForCons(node* pRoot, VisitForConsFun pFun, node* pOutParent, int& nEleNum, node*&pTempRoot){
		if(pRoot == nullptr){
			return;
		}

		node* pNewParent = nullptr;
		try
		{
			pNewParent = pFun(this, pRoot, pOutParent, nEleNum);
		}
		catch(...)
		{
			throw;
		}
		
		if(pTempRoot == nullptr){
			pTempRoot = pNewParent;
		}
		VisitForCons(pRoot->m_pLeft, pFun, pNewParent, nEleNum, pTempRoot);
		VisitForCons(pRoot->m_pRight, pFun, pNewParent, nEleNum, pTempRoot);
	}

	ele_node* min(node* pRoot = nullptr){
		if(pRoot == nullptr){
			pRoot = m_pRoot;
		}

		if(pRoot == nullptr){
			return nullptr;
		}
		node* pNode = pRoot;
		while(pNode->m_pLeft){
			pNode = pNode->m_pLeft;
		}
		return pNode->m_pEleList;
	}
	ele_node* max(node* pRoot = nullptr){
		if(pRoot == nullptr){
			pRoot = m_pRoot;
		}
		if(pRoot == nullptr){
			return nullptr;
		}
		node* pNode = pRoot;
		while(pNode->m_pRight){
			pNode = pNode->m_pRight;
		}
		// 从元素列表选择最后一个元素返回
		return pNode->m_pEleList->m_pPre;
	}

    /*
        　|                            |
        Node                         Right
         /\                           /\
        /  \                         /  \
      Left Right       --->        Node RR
            /\                      /\
           /  \                    /  \
          RL  RR                 Left RL 
    */
	static node* left_rotate(node *pNode)
	{
		node *pRight = pNode->m_pRight;
		node *pLeft = pNode->m_pLeft;
		node *pRightLeft = pRight->m_pLeft;
		node *pRightRight = pRight->m_pRight;
		node *pParent = pNode->m_pParent;
		if(pParent){
			if (pParent->m_pLeft == pNode) {
				pParent->m_pLeft = pRight;
			}
			else {
				pParent->m_pRight = pRight;
			}
		}
		pRight->m_pParent = pParent;

		pRight->m_pLeft = pNode;
		pNode->m_pParent = pRight;

		pNode->m_pRight = pRightLeft;
		if (pRightLeft != nullptr) {
			pRightLeft->m_pParent = pNode;
		}
		return pRight;
	}

	/*
        　 |                       |
         Node                    Left
          /\                      /\
         /  \                    /  \                   
       Left Right       --->    LL  Node    
        /\                          /  \
       /  \                        LR   R
      LL  LR
	*/
	static node * right_rotate(node *pNode)
	{
		node *pLeft = pNode->m_pLeft;
		node *pRight = pNode->m_pRight;
		node *pLeftLeft = pLeft->m_pLeft;
		node *pLeftRight = pLeft->m_pRight;
		node *pParent = pNode->m_pParent;

		// 对pLeft完成关系调整
		// 父节点需要调整孩子
		if(pParent){
			if (pParent->m_pLeft == pNode) {
				pParent->m_pLeft = pLeft;
			}
			else {
				pParent->m_pRight = pLeft;
			}
		}
		pLeft->m_pParent = pParent;
		
		pLeft->m_pRight = pNode;
		// 对pNode完成关系调整
		pNode->m_pParent = pLeft;

		pNode->m_pLeft = pLeftRight;
		if (pLeftRight != nullptr) {
			pLeftRight->m_pParent = pNode;
		}
		return pLeft;
	}


	// pNode颜色是红色，其父节点颜色也是红色．所在的红黑树只有pNode和其父节点不满足约束．
	void adjust_when_node_and_parent_is_red(node*pNode, node* pParent){
		rbtree_color stColor = rbtree_color::color_red;
		node* pParentParent = pParent->m_pParent;
		while (true) {
			// 情况１
			if (pNode == pParent->m_pLeft && pParent == pParentParent->m_pLeft) {
				pNode = right_rotate(pParentParent);
				pNode->m_pLeft->m_stColor = (rbtree_color::color_black));
				// 迭代终止条件
				if(pNode->m_pParent == nullptr){
					m_pRoot->m_stColor = rbtree_color::color_black;
					break;
				}
				else if(pNode->m_pParent->m_stColor == rbtree_color::color_black){
					break;
				}
			}
			// 情况２
			else if (pNode == pParent->m_pLeft && pParent == pParentParent->m_pRight) {
				if(pParentParent->m_pLeft == nullptr){
					pParentParent = left_rotate(pParentParent);
					left_rotate(pParentParent->m_pLeft);
					pParentParent = right_rotate(pParentParent);
					pParentParent->m_stColor = rbtree_color::color_black;
					pParentParent->m_pLeft->m_stColor = rbtree_color::color_red;
					break;
				}
				else{
					right_rotate(pParent);
					pNode = left_rotate(pParentParent);
					pNode->m_pRight->m_stColor = (rbtree_color::color_black);
					if(pNode->m_pParent == nullptr){
						m_pRoot->m_stColor = rbtree_color::color_black;
						break;
					}
					else if(pNode->m_pParent->m_stColor == rbtree_color::color_black){
						break;
					}
				}
			}
			// 情况３
			else if (pNode == pParent->m_pRight && pParent == pParentParent->m_pLeft) {
				if(pParentParent->m_pRight == nullptr){
					left_rotate(pParent);
					pNode = right_rotate(pParentParent);
					pNode->m_stColor = rbtree_color::color_black;
					pNode->m_pRight->m_stColor = rbtree_color::color_red;
					break;
				} else{
					left_rotate(pParent);
					pNode = right_rotate(pParentParent);
					pNode->m_pLeft->m_stColor = (rbtree_color::color_black);
					if(pNode->m_pParent == nullptr){
						m_pRoot->m_stColor = rbtree_color::color_black;
						break;
					}
					else if(pNode->m_pParent->m_stColor == rbtree_color::color_black){
						break;
					}
				}
			}
			// 情况４
			else if (pNode == pParent->m_pRight && pParentParent->m_pRight == pParent) {
				pNode = left_rotate(pParentParent);
				pNode->m_pRight->m_stColor = (rbtree_color::color_black);
				if(pNode->m_pParent == nullptr){
					m_pRoot->m_stColor = rbtree_color::color_black;
					break;
				}
				else if(pNode->m_pParent->m_stColor == rbtree_color::color_black){
					break;
				}
			}
		}
	}

	typedef void (*VisitForClearFun)(node* pNode, int& nVisitNum);
	static void VForClearFun(node* pNode, int& nVisitNum){
		ele_node* pEleNode = pNode->m_pEleList;
		while(pEleNode){
			ele_node* pFreeEle = pEleNode;
			// 释放元素节点
			elenode_allocator::destroy(pFreeEle);
			elenode_allocator::deallocate(pFreeEle);
			pEleNode = pEleNode->m_pNext;
			nVisitNum++
		}
		// 一旦节点下所有元素节点被释放后，节点自身也需被释放
		treenode_allocator::destroy(pNode);
		treenode_allocator::deallocate(pNode);
	}
	void VisitForClear(node* pRoot, VisitForClearFun pFun, int& nEleNum){
		if(pRoot == nullptr){
			return;
		}

		if(nEleNum == 0){
			if(pRoot->m_pParent){
				if(pRoot->m_pParent->m_pLeft == pRoot){
					pRoot->m_pParent->m_pLeft = nullptr;
				}
				else{
					pRoot->m_pParent->m_pRight = nullptr;
				}
			}
			else{
				m_pRoot = nullptr;
			}
		}
		pFun(pRoot, nEleNum);
		VisitForClear(pRoot->m_pLeft, pFun, nEleNum);
		VisitForClear(pRoot->m_pRight, pFun, nEleNum);
	}


	void erase(ele_node* pEleNode){
		// 先在链式结构移除
		if(pEleNode->m_pNext != pEleNode){
			ele_node* pPre = pEleNode->m_pPre;
			ele_node* pNext = pEleNode->m_pNext;
			pPre->m_pNext = pNext;
			pNext->m_pPre = pPre;
			elenode_allocator::destroy(pEleNode);
			elenode_allocator::deallocate(pEleNode);
			return;
		}

		node *pNode = pEleNode->m_pTreeNode;
		// 当针对拥有唯一元素节点的树节点移除其元素时，转化为对树节点的移除
		if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr) {
			if (pNode->m_pParent == nullptr) {
				m_pRoot = nullptr;
				// 先移除元素节点，再移除树节点
				elenode_allocator::destroy(pNode->m_pEleList));
				elenode_allocator::deallocate(pNode->m_pEleList);
				treenode_allocator::destroy(pNode);
				treenode_allocator::deallocate(pNode);
				m_nEleNum--;
				return;
			}

			bool bNeedAdjust = pNode->m_stColor == rbtree_color::color_black;
			node* pParent = pNode->m_pParent;
			elenode_allocator::destroy(pNode->m_pEleList));
			elenode_allocator::deallocate(pNode->m_pEleList);
			treenode_allocator::destroy(pNode);
			treenode_allocator::deallocate(pNode);
			m_nEleNum--;
			if (pNode->m_pParent->m_pLeft == pNode) {
				pParent->m_pLeft = nullptr;
			}
			else {
				pParent->m_pRight = nullptr;
			}

			if (bNeedAdjust)
			{
				// 通过调节结束移除
				// pParent的某个颜色为黑的孩子被移除了．此孩子是叶子节点．
				adjust_for_black_height(pParent);
			}
		}
		else if (pNode->m_pLeft != nullptr)
		{
			ele_node *pMovingEleNode = max(pNode->m_pLeft);
			// 需要对树节点进行交换
			node* pMovingNode = pMovingEleNode->m_pTreeNode;
			// pNode是只有一个待移除元素节点的树节点
			// pMovingNode是另一个树节点
			// 1.记录pNode下元素节点
			ele_node* pDelEleNode = pNode->m_pEleList;
			// 2.将pMovingNode下各个元素节点转移到pNode节点下面
			pNode->m_pEleList = pMovingNode->m_pEleList;
			pMovingEleNode = pNode->m_pEleList;
			while(true){
				pMovingEleNode->m_pTreeNode = pNode;
				pMovingEleNode = pMovingEleNode->m_pNext;
				if(pMovingEleNode == pNode->m_pEleList){
					break;
				}
			}
			// 3.将记录的待移除节点转移到pMovingNode下
			pMovingNode->m_pEleList = pDelEleNode;
			pDelEleNode->m_pTreeNode = pMovingNode;
			// 3.转化为对pMovingNode中剩余那个元素节点的移除
			erase(pDelEleNode);
		}
		else
		{
			ele_node *pMovingEleNode = min(pNode->m_pRight);
			node* pMovingNode = pMovingEleNode->m_pTreeNode;
			pNode->m_stEle = pMovingNode->m_stEle;
			// pNode是只有一个待移除元素节点的树节点
			// pMovingNode是另一个树节点
			// 1.记录pNode下元素节点
			ele_node* pDelEleNode = pNode->m_pEleList;
			// 2.将pMovingNode下各个元素节点转移到pNode节点下面
			pNode->m_pEleList = pMovingNode->m_pEleList;
			pMovingEleNode = pNode->m_pEleList;
			while(true){
				pMovingEleNode->m_pTreeNode = pNode;
				pMovingEleNode = pMovingEleNode->m_pNext;
				if(pMovingEleNode == pNode->m_pEleList){
					break;
				}
			}
			// 3.将记录的待移除节点转移到pMovingNode下
			pMovingNode->m_pEleList = pDelEleNode;
			pDelEleNode->m_pTreeNode = pMovingNode;
			// 3.转化为对pMovingNode中剩余那个元素节点的移除
			erase(pDelEleNode);
		}
	}

	// 循环不变式：pNode节点颜色是黑黑．只有pNode自身不满足红黑树约束．
	void adjust_for_black_black(node* pNode){
		// 循环不变式：pNode节点颜色是黑黑．只有pNode自身不满足红黑树约束．
		node* pParent = pNode->m_pParent;
		while (true) {
			if (pNode == pParent->m_pLeft 
				&& pParent->m_stColor == rbtree_color::color_black 
				&& pParent->m_pRight->m_nColor == rbtree_color::color_black) {
				if (pParent->m_pRight->m_pLeft->m_stColor == rbtree_color::color_black) {
					pParent = left_rotate(pParent);
					pParent->m_stColor = rbtree_color::color_blackblack;
					pParent->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_black;
					pParent->m_pLeft->m_stColor = rbtree_color::color_red;
					pNode = pParent;
					if(pNode->m_pParent == nullptr){
						m_pRoot->m_stColor = rbtree_color::color_black;
						return;
					}
				}
				else if (pParent->m_pRight->m_pRight->m_stColor == rbtree_color::color_red) {
					pParent = left_rotate(pParent);
					left_rotate(pParent->m_pLeft);
					pParent->m_pLeft->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_black;
					pParent->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_red;
					pParent->m_pLeft->m_stColor = rbtree_color::color_black;
					pParent->m_pRight->m_stColor = rbtree_color::color_black;
					return;
				}
				else {
					right_rotate(pParent->m_pRight);
					pParent = left_rotate(pParent);
					pParent->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_black;
					pParent->m_stColor = rbtree_color::color_black;
					return;
				}
			}
			else if (pNode == pParent->m_pLeft
				&& pParent->m_stColor == rbtree_color::color_black 
				&& pParent->m_pRight->m_stColor == rbtree_color::color_red) {
				pParent = left_rotate(pParent);
				if (pParent->m_pLeft->m_pRight->m_pLeft->m_stColor == rbtree_color::color_black) {
					pParent->m_stColor = rbtree_color::color_black;
					left_rotate(pParent->m_pLeft);
					pParent->m_pLeft->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_black;
					pParent->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_red;
					return;
				}
				else {
					right_rotate(pParent->m_pLeft->m_pRight);
					left_rotate(pParent->m_pLeft);
					pParent->m_stColor = rbtree_color::color_black;
					pParent->m_pLeft->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_black;
					return;
				}
			}
			else if (pNode == pParent->m_pLeft && pParent->m_stColor == rbtree_color::color_red) {
				pParent = left_rotate(pParent);
				pParent->m_stColor = rbtree_color::color_black;
				pParent->m_pLeft->m_stColor = rbtree_color::color_red;
				pParent->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_black;
				if (pParent->m_pLeft->m_pRight->m_stColor == rbtree_color::color_red) {
					adjust_when_node_and_parent_is_red(pParent->m_pLeft->m_pRight, pParent->m_pLeft);
				}
				return;
			}

			else if (pNode == pParent->m_pRight 
				&& pParent->m_stColor == rbtree_color::color_black 
				&& pParent->m_pLeft->m_stColor == rbtree_color::color_black) {
				if (pParent->m_pLeft->m_pRight->m_stColor == rbtree_color::color_black) {
					pParent = right_rotate(pParent);
					pParent->m_pRight->m_pRight->m_stColor = rbtree_color::color_black;
					pParent->m_pRight->m_stColor = rbtree_color::color_red;
					pParent->m_stColor = rbtree_color::color_blackblack;
					pNode = pParent;
					if(pNode->m_pParent == nullptr){
						m_pRoot->m_stColor = rbtree_color::color_black;
						return;
					}
				}
				else {
					left_rotate(pParent->m_pLeft);
					pParent = right_rotate(pParent);
					pParent->m_stColor = rbtree_color::color_black;
					pParent->m_pRight->m_pRight->m_stColor = rbtree_color::color_black;
					return;
				}
			}
			else if (pNode == pParent->m_pRight && pParent->m_stColor == rbtree_color::color_black 
				&& pParent->m_pLeft->m_stColor == rbtree_color::color_red) {
				if (pParent->m_pLeft->m_pRight->m_pRight->m_stColor == rbtree_color::color_black) {
					pParent = right_rotate(pParent);
					right_rotate(pParent->m_pRight);
					pParent->m_pRight->m_pRight->m_pRight->m_stColor = rbtree_color::color_black;
					pParent->m_pRight->m_pRight->m_stColor = rbtree_color::color_red;
					pParent->m_stColor = rbtree_color::color_black;
					return;
				}
				else {
					pParent = right_rotate(pParent);
					left_rotate(pParent->m_pRight->m_pLeft);
					right_rotate(pParent->m_pRight);
					pParent->m_stColor = rbtree_color::color_black;
					pParent->m_pRight->m_pRight->m_pRight->m_stColor = rbtree_color::color_black;
					return;
				}
			}
			else if (pNode == pParent->m_pRight && pParent->m_stColor == rbtree_color::color_red) {
				pParent = right_rotate(pParent);
				pParent->m_pRight->m_pRight->m_stColor = rbtree_color::color_black;
				if (pParent->m_pRight->m_pLeft->m_stColor == rbtree_color::color_red) {
					adjust_when_node_and_parent_is_red(pParent->m_pRight->m_pLeft, pParent->m_pRight);
				}
				return;
			}
		}
	}

	// pNode的某个孩子被移除了．移除的孩子是叶子节点．移除节点的颜色是黑色．
	void adjust_for_black_height(node *pNode)
	{
		if (pNode == nullptr) {
			return;
		}

		if (pNode->m_pRight != nullptr) 
		{
			if (pNode->m_stColor == rbtree_color::color_red)
			{
				node *pTemp = left_rotate(pNode);
				if (pTemp->m_pLeft->m_pRight != nullptr)
				{
					pNode = pTemp->m_pLeft->m_pRight;
					adjust_when_node_and_parent_is_red(pNode, pNode->m_pParent);
				}
				return;
			}

			// pNode->m_nColor == Color::BLACK
			if (pNode->m_pRight->m_stColor == rbtree_color::color_red)
			{
				node *pTemp = left_rotate(pNode);
				left_rotate(pTemp->m_pLeft);
				pTemp->m_stColor = rbtree_color::color_black;
				pTemp->m_pLeft->m_pLeft->m_stColor = rbtree_color::color_red;
				if (pTemp->m_pLeft->m_pLeft->m_pRight != nullptr)
				{
					pNode = pTemp->m_pLeft->m_pLeft->m_pRight;
					adjust_when_node_and_parent_is_red(pNode, pNode->m_pParent);
				}
				return;
			}

			// pNode->m_pRightChild->m_nColor == Color::BLACK
			node *pTemp = left_rotate(pNode);
			if (pTemp->m_pRight != nullptr)
			{
				pTemp->m_pRight->m_stColor = rbtree_color::color_black;
				return;
			}
			
			// pTemp->m_pRightChild ＝= nullptr
			if (pTemp->m_pLeft->m_pRight != nullptr)
			{
				left_rotate(pTemp->m_pLeft);
				pTemp = right_rotate(pTemp);
				pTemp->m_stColor = rbtree_color::color_black;
				return;
			}

			// pTemp->m_pLeftChild->m_pRightChild == nullptr
			pTemp->m_pLeft->m_stColor = rbtree_color::color_red;
			pTemp->m_stColor = rbtree_color::color_blackblack;
			pNode = pTemp;
			node *pParent = pNode->m_pParent;
			if(pParent == nullptr){
				m_pRoot->m_stColor = rbtree_color::color_black;
				return;
			}

			adjust_for_black_black(pNode);
		}
		else
		{
			if (pNode->m_stColor == rbtree_color::color_red)
			{
				node *pTemp = right_rotate(pNode);
				if (pTemp->m_pRight->m_pLeft != nullptr)
				{
					adjust_when_node_and_parent_is_red(pTemp->m_pRight->m_pLeft, pTemp->m_pRight);
				}
				return;
			}

			// pNode->m_nColor == Color::BLACK
			if (pNode->m_pLeft->m_stColor == rbtree_color::color_red)
			{
				node *pTemp = right_rotate(pNode);
				right_rotate(pTemp->m_pRight);
				pTemp->m_stColor = rbtree_color::color_black;
				pTemp->m_pRight->m_pRight->m_stColor = rbtree_color::color_red;
				if (pTemp->m_pRight->m_pRight->m_pLeft != nullptr)
				{
					adjust_when_node_and_parent_is_red(pTemp->m_pRight->m_pRight->m_pLeft, pTemp->m_pRight->m_pRight);
				}
				return;
			}

			// pNode->m_pLeftChild->m_nColor == Color::BLACK
			node *pTemp = right_rotate(pNode);
			if (pTemp->m_pLeft)
			{
				pTemp->m_pLeft->m_stColor = rbtree_color::color_black;
				return;
			}
			
			// pTemp->m_pLeftChild == null
			if (pTemp->m_pRight->m_pLeft)
			{
				right_rotate(pTemp->m_pRight);
				pTemp = left_rotate(pTemp);
				pTemp->m_stColor = rbtree_color::color_black;
				return;
			}

			// pTemp->m_pRightChild->m_pLeftChild == null
			pTemp->m_pRight->m_stColor = rbtree_color::color_red;
			pTemp->m_stColor = rbtree_color::color_blackblack;
			adjust_for_black_black(pTemp);
		}
	}

};

}
#endif