/* ��������������Ҳ�����ڴ�����ͨ�������
 * QA Pass
 */

#ifndef TREENODE_CLASS
#define TREENODE_CLASS

#include <stdlib.h>

template <class T>
class BinSTree;

template <class T>
class TreeNode
{
   protected:
      // ָ�������Һ��ӵ�ָ��
      TreeNode<T> *left;
      TreeNode<T> *right;

   public:
      // ���г�Ա�������ⲿ�޸ĵ�����ֵ
      T data;
   
      // ���캯��
	  TreeNode (const T& item, TreeNode<T> *lptr = NULL,
                TreeNode<T> *rptr = NULL);
   
      // ����ָ����ĺ���
      TreeNode<T>* Left(void) const;
      TreeNode<T>* Right(void) const;

      friend class BinSTree<T>;
};

// ���캯������ʼ���������ݺ�ָ���򣬶��ڿ�����������ָ����ֵΪ NULL
template <class T>
TreeNode<T>::TreeNode (const T& item, TreeNode<T> *lptr,
    TreeNode<T> *rptr): data(item), left(lptr), right(rptr)
{}

// �����û���������
template <class T>
TreeNode<T>* TreeNode<T>::Left(void) const
{
   return left;
}

// �����û������Һ���
template <class T>
TreeNode<T>* TreeNode<T>::Right(void) const
{
   return right;
}

#endif // TREENODE_CLASS
