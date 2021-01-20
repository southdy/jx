/* ����������
* 
*/

#ifndef BINARY_SEARCH_TREE_CLASS
#define BINARY_SEARCH_TREE_CLASS

#include <stdlib.h>
#include "KBinTreeNode.h"

template <class T>
class BinSTree
{
    private:
        // ָ����������ǰ����ָ��
        TreeNode<T> *root;
        TreeNode<T> *current;
        
        // �������������
        int size;
      
        // ���ڸ��ƹ��캯������ֵ�����
        TreeNode<T> *CopyTree(TreeNode<T> *t);
        
        // ����������������ֵ������� ClearList ����
        void DeleteTree(TreeNode<T> *t);

        // �ں��� Find �� Delete ��������λ��㼰��˫�������е�λ��
        TreeNode<T> *FindNode(const T& item, TreeNode<T>* & parent) const;

    public:
        // ���캯������������
        BinSTree(void);
        BinSTree(const BinSTree<T>& tree);
        ~BinSTree(void);
        
        // ��ֵ�����
        BinSTree<T>& operator= (const BinSTree<T>& rhs);
		
        
        // ��׼�ı�����
        bool Find(T& item);
        void Insert(const T& item);
        void Delete(const T& item);
        void ClearList(void);
        bool ListEmpty(void) const;
        int ListSize(void) const;
        
        // �������ⷽ��
        void Update(const T& item);
        TreeNode<T> *GetRoot(void) const;
};

// ������ t ��ʹ��洢�ڵ�ǰ������
template <class T>
TreeNode<T> *BinSTree<T>::CopyTree(TreeNode<T> *t)
{
    TreeNode<T> *newlptr, *newrptr, *newNode;
   
    // �������֧Ϊ�գ����� NULL
    if (t == NULL)
        return NULL;
        
    // ������ t �������������������� newlptr
    if (t->left != NULL) 
        newlptr = CopyTree(t->left);
    else
        newlptr = NULL;
 
    // ������ t �������������������� newrptr
    if (t->right != NULL) 
        newrptr = CopyTree(t->right);
    else
        newrptr = NULL;
 
    // Ϊ��ǰ��������洢������������ֵ��ָ����������������������ָ��
    newNode = new TreeNode<T>(t->data, newlptr, newrptr);
    return newNode;
}

// ɾ����ǰ����洢����
template <class T>
void BinSTree<T>::DeleteTree(TreeNode<T> *t)
{
    if (t != NULL)
    {
        DeleteTree(t->left);
        DeleteTree(t->right);
        delete t;
    }
}

// ������������������ҵ����򷵻ؽ���ַ��ָ����˫�׵�ָ�룻���򣬷��� NULL
template <class T>
TreeNode<T> *BinSTree<T>::FindNode(const T& item, TreeNode<T>* & parent) const
{   
    // ��ָ�� t �Ӹ���ʼ������
    TreeNode<T> *t = root;
    
    // ����˫��Ϊ NULL
    parent = NULL;
    
    // ������Ϊ�գ���ѭ������
    while(t != NULL)
    {
        // ���ҵ���ֵ�����˳�
        if (item == t->data)
            break;
        else 
        {
            // �޸�˫��ָ�룬���Ƶ���������������
            parent = t;
            if (item < t->data)
                t = t->left;
            else 
                t = t->right;
        }
    }
    
    // ����ָ�����ָ�룻��û�ҵ����򷵻� NULL
    return t;
}

// ���캯������ʼ�� root��current Ϊ�գ�size Ϊ 0
template <class T>
BinSTree<T>::BinSTree(void):root(NULL), current(NULL), size(0)
{}

// ���ƹ��캯��
template <class T>
BinSTree<T>::BinSTree(const BinSTree<T>& tree)
{
    // �� tree ���Ƶ���ǰ���󣬷��� current �� size
    root = CopyTree(tree.root);
    current = root;
    size = tree.size;
}

// ��������
template <class T>
BinSTree<T>::~BinSTree(void)
{
    ClearList();
}

// ɾ�����е����н��
template <class T>
void BinSTree<T>::ClearList(void)
{
    DeleteTree(root);
    root = current = NULL;
    size = 0;
}

// ��ֵ�����
template <class T>
BinSTree<T>& BinSTree<T>::operator= (const BinSTree<T>& rhs)
{
    // ���ܽ������Ƶ�����
    if (this == &rhs)
        return *this;
        
    // �����ǰ�������������Ƶ���ǰ����
    ClearList();
    root = CopyTree(rhs.root);
    
    // �� current ָ��ָ�� root ���������� size ֵ
    current = root;
    size = rhs.size;
    
    // ���ص�ǰ�����ָ��
    return *this;
}

// ���������� item�����ҵ����򽫽�����ݸ��� item
template <class T>
bool BinSTree<T>::Find(T& item)
{
    // ʹ�� FindNode������Ҫ parent ����
    TreeNode<T> *parent;

    // ���������� item����ƥ��Ľ�㸳�� current
    current = FindNode(item, parent);
    
    // ���ҵ��������ݸ��� item ������ True
    if (current != NULL)
    {
        item = current->data;
        return true;
    }
    else
    	// ������û�ҵ� item������ False
        return false;
}

// ָʾ���Ƿ�Ϊ��
template <class T>
bool BinSTree<T>::ListEmpty(void) const
{
    return (size == 0);
}

// �������е����������
template <class T>
int BinSTree<T>::ListSize(void) const
{
    return size;
}

// ���������в����������Ԫ���ظ������������Ԫ��
template <class T>
void BinSTree<T>::Insert(const T& item)
{
    // t Ϊ���������еĵ�ǰ��㣬parent Ϊǰһ���
    TreeNode<T> *parent = NULL;

	current = FindNode(item, parent);
    
	if (current != NULL)
		current->data = item;
	else
	{
		// �����µ�Ҷ�ӽ��
		TreeNode<T> *newNode = new TreeNode<T>(item,NULL,NULL);
		
		// �� parent Ϊ NULL��������Ϊ��������
		if (parent == NULL)
			root = newNode;
        
		// �� item < parent->data��������Ϊ���Ӳ���        
		else if (item < parent->data)                   
			parent->left = newNode;
        
		else
			// �� item >= parent->data����Ϊ�Һ��Ӳ���     
			parent->right = newNode;
        
		// current ��ֵΪ�½��ĵ�ַ���� size �� 1
		current = newNode;
		size++;
	}
}

// ��� item �����У�����ɾ��
template <class T>
void BinSTree<T>::Delete(const T& item)
{
    // DNodePtr = ָ��ɾ����� D ��ָ��
    // PNodePtr = ָ����� D ��˫�׽ڵ� P ��ָ��
    // RNodePtr = ָ���滻 D �Ľ�� R ��ָ��
    TreeNode<T> *DNodePtr, *PNodePtr, *RNodePtr;
    
    // ��������ֵΪ item �Ľ�㣬������ý���˫�׽���ָ��
    if ((DNodePtr = FindNode (item, PNodePtr)) == NULL)
        return;
    
    // ��� D ��һ��ָ��Ϊ NULL�����滻���Ϊ����һ֦��ĳһ���
    if (DNodePtr->right == NULL)
        RNodePtr = DNodePtr->left;
    else if (DNodePtr->left == NULL)
        RNodePtr = DNodePtr->right;
        
    // DNodePtr ������ָ�����Ϊ NULL
    else
    {
        // Ѱ�Ҳ�ж�� D ���滻��㡣�ӽ�� D ����������ʼ��������ֵС�� D ������ֵ��
        // ���ֵ�����ý������жϿ�
        
        // PofRNodePtr = ָ���滻���˫�׵�ָ��
        TreeNode<T> *PofRNodePtr = DNodePtr;
        
        // ��һ�ֿ��ܵ��滻Ϊ D ������
        RNodePtr = DNodePtr->left;
    
        // �� D �����ӵ����������������������ֵ������¼��ǰ��㼰��˫�׽���
        // ָ�룬������ǽ��ҵ��滻���
        while(RNodePtr->right != NULL)
        {
            PofRNodePtr = RNodePtr;
            RNodePtr = RNodePtr->right;
        }
        
        if (PofRNodePtr == DNodePtr)
            // ��ɾ����������Ϊ�滻��㣬�� D ������������ R
            RNodePtr->right = DNodePtr->right;
        else
        {
            // �������������ƶ���һ����㣬������ɾ���滻��㣬����������������˫��
            PofRNodePtr->right = RNodePtr->left;
            
            // ���滻������ DNodePtr
            RNodePtr->left = DNodePtr->left;
            RNodePtr->right = DNodePtr->right;
        }
    }

    // ��ɵ�˫�׽������ӡ�ɾ������㣬�����¸���ֵ
    if (PNodePtr == NULL)
        root = RNodePtr;
        
    // �� R ���� P ����ȷһ֦��
    else if (DNodePtr->data < PNodePtr->data)
        PNodePtr->left = RNodePtr;
    else
        PNodePtr->right = RNodePtr;
        
    // �ͷű�ɾ����ڴ沢�����Ĵ�С�� 1
    delete DNodePtr;
    size--;
}

// ����ǰ����Ѷ���������ֵ���������ֵ��ȣ��򽫽��ֵ���� item�����򣬽� item ���뵽����
template <class T>
void BinSTree<T>::Update(const T& item)
{   
    if (current != NULL && current->data == item)
            current->data = item;
    else
        Insert(item);
}

// ���ظ����ĵ�ַ
template <class T>
TreeNode<T> *BinSTree<T>::GetRoot(void) const
{
    return root;
}

#endif  // BINARY_SEARCH_TREE_CLASS
