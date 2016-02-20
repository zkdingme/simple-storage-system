/* BTree.h

	BTree is the core of reference   
	In most occasions, we use BTree or BPlusTree to implement the database

*/
#ifndef _BTree_
#define _BTree_
#include<iostream>
#include"Reference.h"
using namespace std;

	/*

	M is the degree of each node
	we define the KEYMAX,KEYMIN, CHILD_MAX, CHILD_MIN

	*/

#define M 58
#define KEY_MAX  (2*M-1)
#define KEY_MIN  (M-1)
#define CHILD_MAX (KEY_MAX + 1)
#define CHILD_MIN (KEY_MIN + 1)
	/*

	Common Node, we add isleaf to make things easy 

	*/

template<typename T>
struct Node
{
	bool isLeaf;             
	int keyNum;             
	T keyValue[KEY_MAX];     
	Node<Refer> * pChild[CHILD_MAX]; 
	Node(bool b=true, int n=0)
		:isLeaf(b), keyNum(n) 
	{	
		for (int i = 0; i < CHILD_MAX; i++)
			pChild[i] = NULL;
	}
};
	/*

	Result is the return value of find()
	Pnode is the accurate node
	sequence is its index
	tag represents whether we find the key

	*/


template<class T>
struct Result
{
	Node<T>* Pnode;
	int sequence;
	bool tag;      //also contains the information of whether it can return with a right value
	Result(Node<T> * p = NULL, int seq = -1, bool sign = false) :Pnode(p), sequence(seq), tag(sign) {}
};
	/*

	BTree :

	1.     insert(...)
	2.     remove(...)
	3.     clear(...)
	4.     find(...)
	5.     contain(...)
	6.     traverse(...)

	The parameters are all about the key and reference

	*/
template<class T>
class BTREE
{
public:
	BTREE()
	{
		m_pRoot = NULL;  //An empty tree
	}

	~BTREE()
	{
		clear();
	}
	/*

	bool insert(const T &key):
	recursive insert the key value

	*/
	bool insert(const T &key)    //insert new node
	{
		if (contain(key))  //check whether the key is already exist
		{
			return false;
		}
		else
		{
			
			if (m_pRoot == NULL)//whether it is an empty tree
			{
				m_pRoot = new Node<T>();
			}
			
			if (m_pRoot->keyNum == KEY_MAX) //whether the root is full
			{
				Node<T> *pNode = new Node<T>();  //create a new node
				pNode->isLeaf = false;
				pNode->pChild[0] = m_pRoot;
				splitChild(pNode, 0, m_pRoot);
				m_pRoot = pNode;  //updata the pointer of node
			}
			
			insertNonFull(m_pRoot, key);
			
			return true;
		}
	}
	/*
       
	bool remove(const T &key):
    based on 4 states, find the successor and delete
	
	*/
	bool remove(const T &key)    //delete the key
	{
		if (!(search(m_pRoot, key).tag))  //none existence
		{
			return false;
		}
		if (m_pRoot->keyNum == 1)
		{
			if (m_pRoot->isLeaf)
			{
				clear();
				return true;
			}
			else
			{
				Node<T> *pChild1 = m_pRoot->pChild[0];
				Node<T> *pChild2 = m_pRoot->pChild[1];
				if (pChild1->keyNum == KEY_MIN&&pChild2->keyNum == KEY_MIN)
				{
					mergeChild(m_pRoot, 0);
					deleteNode(m_pRoot);
					m_pRoot = pChild1;
				}
			}
		}
		recursive_remove(m_pRoot, key);
		return true;
	}
	/*
      
	void traverse()const:
	print all the key value
	we create this function to find errors more easily

	*/
	void traverse()const
	{
		traverseTree(m_pRoot);
	}
	/*
		
	bool contain(const T &key)const:
	use search(...)
		
	*/
	bool contain(const T &key)const   //check whether the tree is in this tree
	{
		return search(m_pRoot, key).tag;
	}
	/*

	Result<T> find(int & index)const:
	use search(...)
	recursive search
	
	*/
	Result<T> find(int & index)const
	{
		return search(m_pRoot, Refer(index, 0, 0));
	}
    /*
        
    void clear():
	recursive clear all the nodes

    */
	void clear()                      //clear all the tree
	{
		recursive_clear(m_pRoot);
		m_pRoot = NULL;
	}
private:
    /*

	void recursive_clear(Node<T>*pNode):
	recursive_clear the node
    
	*/
	void recursive_clear(Node<T>*pNode)
	{
		if (pNode != NULL)
		{
			if (!pNode->isLeaf)
			{
				for (int i = 0; i <= pNode->keyNum; ++i)
					recursive_clear(pNode->pChild[i]);
			}
			deleteNode(pNode);
		}
	}
    /*

	void deleteNode(Node<T> *&pNode):
	just delete node
    
	*/
	void deleteNode(Node<T> *&pNode)
	{
		if (pNode != NULL)
		{
			delete pNode;
			pNode = NULL;
		}
	}
    /*
	
	Result<T> search(Node<T> *pNode, const T &key)const:
	recursive search

    */
	Result<T> search(Node<T> *pNode, const T &key)const
	{
		if (pNode == NULL)  
		{
			return Result<T>(NULL, -1, false);
		}
		else
		{
			int i;
			for (i = 0; i<pNode->keyNum && key>*(pNode->keyValue + i); ++i)
			{
			}
			if (i<pNode->keyNum && key == pNode->keyValue[i])
			{
				return Result<T>(pNode, i, true);
			}
			else
			{
				if (pNode->isLeaf)   //check whether the node is leaf
				{
					return Result<T>(NULL, -1, false);
				}
				else
				{
					return search(pNode->pChild[i], key);
				}
			}
		}
	}
    /*

	void splitChild(Node<T> *pParent, int nChildIndex, Node<T> *pChild):
	put the middle element to the parent and split the children into two nodes
    
	*/
	void splitChild(Node<T> *pParent, int nChildIndex, Node<T> *pChild)
	{
		Node<T> *pRightNode = new Node<T>();
		pRightNode->isLeaf = pChild->isLeaf;
		pRightNode->keyNum = KEY_MIN;
		int i;
		for (i = 0; i<KEY_MIN; ++i)
		{
			pRightNode->keyValue[i] = pChild->keyValue[i + CHILD_MIN];
		}
		if (!pChild->isLeaf) 
		{
			for (i = 0; i<CHILD_MIN; ++i)
			{
				pRightNode->pChild[i] = pChild->pChild[i + CHILD_MIN];
			}
		}

		pChild->keyNum = KEY_MIN;  

		for (i = pParent->keyNum; i>nChildIndex; --i)
		{
			pParent->pChild[i + 1] = pParent->pChild[i];
			pParent->keyValue[i] = pParent->keyValue[i - 1];
		}
		++pParent->keyNum;  
		pParent->pChild[nChildIndex + 1] = pRightNode;  
		pParent->keyValue[nChildIndex] = pChild->keyValue[KEY_MIN];
	}
    /*

    void insertNonFull(Node<T> *pNode, const T &key):
	insert the element in the none full node

    */
	void insertNonFull(Node<T> *pNode, const T &key)
	{
		int i = pNode->keyNum;  
		if (pNode->isLeaf)     
		{
			while (i>0 && key<pNode->keyValue[i - 1])  
			{
				pNode->keyValue[i] = pNode->keyValue[i - 1]; 
				--i;
			}
			pNode->keyValue[i] = key;  
			++(pNode->keyNum); 
		}
		else
		{
			while (i>0 && key<pNode->keyValue[i - 1])
				--i;
			Node<T> *pChild = pNode->pChild[i];  
			if (pChild->keyNum == KEY_MAX)  
			{
				splitChild(pNode, i, pChild);
				if (key>pNode->keyValue[i])  
					pChild = pNode->pChild[i + 1];
			}
			insertNonFull(pChild, key);  
		}
	}
    /*

	void traverseTree(Node<T>*p)const:
	traverse the tree and display the infomation.
    
	*/
	void traverseTree(Node<T>*p)const
	{
		if (p == NULL)return;
		for (int i = 0; i < p->keyNum; i++)
		{
			cout << p->keyValue[i] << " ";
		}
		cout << endl;
		for (int i = 0; i <= p->keyNum; i++)
			traverseTree(p->pChild[i]);
	}
	/*

	void mergeChild(Node<T> *pParent, int index):
	when deleting, sometimes you need to merge two nodes in to one node 

	*/
	void mergeChild(Node<T> *pParent, int index)
	{
		Node<T> *pChild1 = pParent->pChild[index];
		Node<T> *pChild2 = pParent->pChild[index + 1];
		pChild1->keyNum = KEY_MAX;
		pChild1->keyValue[KEY_MIN] = pParent->keyValue[index];
		int i;
		for (i = 0; i<KEY_MIN; ++i)
		{
			pChild1->keyValue[i + KEY_MIN + 1] = pChild2->keyValue[i];
		}
		if (!pChild1->isLeaf)
		{
			for (i = 0; i<CHILD_MIN; ++i)
			{
				pChild1->pChild[i + CHILD_MIN] = pChild2->pChild[i];
			}
		}

		--pParent->keyNum;
		for (i = index; i<pParent->keyNum; ++i)
		{
			pParent->keyValue[i] = pParent->keyValue[i + 1];
			pParent->pChild[i + 1] = pParent->pChild[i + 2];
		}
		deleteNode(pChild2); 
	}

	/*

	void recursive_remove(Node<T> *pNode, const T &key):
	remove all the nodes 
	
	*/
	void recursive_remove(Node<T> *pNode, const T &key)
	{
		int i = 0;
		while (i<pNode->keyNum&&key>pNode->keyValue[i])
			++i;
		if (i<pNode->keyNum&&key == pNode->keyValue[i])
		{
			if (pNode->isLeaf)
			{
				--pNode->keyNum;
				for (; i<pNode->keyNum; ++i)
				{
					pNode->keyValue[i] = pNode->keyValue[i + 1];
				}
				return;
			}
			else
			{
				Node<T> *pChildPrev = pNode->pChild[i];
				Node<T> *pChildNext = pNode->pChild[i + 1];
				if (pChildPrev->keyNum >= CHILD_MIN)
				{
					T prevKey = getPredecessor(pChildPrev); 
					recursive_remove(pChildPrev, prevKey);
					pNode->keyValue[i] = prevKey;     
					return;
				}
				else if (pChildNext->keyNum >= CHILD_MIN)
				{
					T nextKey = getSuccessor(pChildNext);
					recursive_remove(pChildNext, nextKey);
					pNode->keyValue[i] = nextKey;   
					return;
				}
				else
				{
					mergeChild(pNode, i);
					recursive_remove(pChildPrev, key);
				}
			}
		}
		else
		{
			Node<T> *pChildNode = pNode->pChild[i];
			if (pChildNode == NULL)return;
			if (pChildNode->keyNum == KEY_MIN)
			{
				Node<T> *pLeft = i>0 ? pNode->pChild[i - 1] : NULL;
				Node<T> *pRight = i<pNode->keyNum ? pNode->pChild[i + 1] : NULL;
				int j;
				if (pLeft&&pLeft->keyNum >= CHILD_MIN)
				{
					for (j = pChildNode->keyNum; j>0; --j)
					{
						pChildNode->keyValue[j] = pChildNode->keyValue[j - 1];
					}
					pChildNode->keyValue[0] = pNode->keyValue[i - 1];

					if (!pLeft->isLeaf)
					{
						for (j = pChildNode->keyNum + 1; j>0; --j) 
						{
							pChildNode->pChild[j] = pChildNode->pChild[j - 1];
						}
						pChildNode->pChild[0] = pLeft->pChild[pLeft->keyNum];
					}
					++pChildNode->keyNum;
					pNode->keyValue[i] = pLeft->keyValue[pLeft->keyNum - 1];
					--pLeft->keyNum;
				}
				else if (pRight&&pRight->keyNum >= CHILD_MIN)
				{
					
					pChildNode->keyValue[pChildNode->keyNum] = pNode->keyValue[i];
					++pChildNode->keyNum;
					pNode->keyValue[i] = pRight->keyValue[0];
					--pRight->keyNum;
					for (j = 0; j<pRight->keyNum; ++j)
					{
						pRight->keyValue[j] = pRight->keyValue[j + 1];
					}
					if (!pRight->isLeaf)
					{
						pChildNode->pChild[pChildNode->keyNum] = pRight->pChild[0];
						for (j = 0; j <= pRight->keyNum; ++j)
						{
							pRight->pChild[j] = pRight->pChild[j + 1];
						}
					}
				}
				else if (pLeft)
				{
					mergeChild(pNode, i - 1);
					pChildNode = pLeft;
				}
				else if (pRight)
				{
					mergeChild(pNode, i);

				}
			}
			recursive_remove(pChildNode, key);
		}
	}
	/*
	
	T getPredecessor(Node<T>*pNode):
	get the predeccessor data
	
	*/
	T getPredecessor(Node<T>*pNode)
	{
		while (!pNode->isLeaf)
		{
			pNode = pNode->pChild[pNode->keyNum];
		}
		return pNode->keyValue[pNode->keyNum - 1];
	}

	/*
	
	T getSuccessor(Node<T> *pNode):
	get the getsuccessor data

	*/
	T getSuccessor(Node<T> *pNode)
	{
		while (!pNode->isLeaf)
		{
			pNode = pNode->pChild[0];
		}
		return pNode->keyValue[0];
	}

public:
	/*

	root
	
	*/
	Node<T> * m_pRoot;  
};
#endif