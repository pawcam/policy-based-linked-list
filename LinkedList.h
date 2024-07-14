#ifndef LINKED_LIST_CMAKE_BUILD_DEBUG_LINKEDLIST_H_
#define LINKED_LIST_CMAKE_BUILD_DEBUG_LINKEDLIST_H_
#include <cstddef>
#include <type_traits>
#include <utility>

template<typename T>
struct Node
{
  Node *pNext;
  Node *pPrev;
  T& m_data;

  explicit Node(T& data) : m_data(data), pNext(nullptr), pPrev(nullptr) {}

  Node& operator=(const Node& node)
  {
    pNext = node.pNext;
    pPrev = node.pPrev;
    m_data = node.pData;

    return *this;
  }

  Node& operator==(const Node& node)
  {
    return (pNext == node.pNext) && (pPrev == node.pPrev) && (m_data == node.m_data);
  }
};

template <typename T>
struct HeapStorage
{
  template <typename... Args>
  static T* allocate(Args&&... args)
  {
    return new T(std::forward<Args>(args)...);
  }

  static void deallocate(T* p)
  {
    delete p;
  }
};

template <typename T>
struct StackStorage
{
  template <typename... Args>
  static T* allocate(Args&&... args)
  {
    return new (&storage) T(std::forward<Args>(args)...);
  }

  static void deallocate(T* p)
  {
    delete p;
  }

 private:
  static typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
};

// Inheriting policy classes adds additional flexibility without confining virtual methods
// extremely open-ended customization of policy classes independent of each other
// Allows us to call methods without specifying name of specific StoragePolicy class (HeapStorage, StackStorage)
template<typename T
         , template <typename> class StoragePolicy = HeapStorage
         >
class LinkedList : public StoragePolicy<Node<T>> {
  using TStoragePolicy = StoragePolicy<Node<T>>;

public:
  void insert_back(T& data)
  {
    if (pTail)
    { // At least 1 item in the list
      auto tmp = pTail;
//      pTail = new Node<T>(data);
      pTail = TStoragePolicy::allocate(data);
      tmp->pNext = pTail;
      pTail->pPrev = tmp;
    }
    else
    {
//      pTail = new Node<T>(data);
      pTail = TStoragePolicy::allocate(data);
      pHead = pTail;
    }

    ++m_size;
  }

  void insert_front(T& data)
  {
    if (pHead)
    { // At least 1 item in the list
      auto tmp = pHead;
//      pHead = new Node<T>(data);
      pHead = TStoragePolicy::allocate(data);
      tmp->pPrev = pHead;
      pHead->pNext = tmp;
    }
    else
    {
//      pHead = new Node<T>(data);
      pHead = TStoragePolicy::allocate(data);
      pTail = pHead;
    }

    ++m_size;
  }

  bool remove(T& data)
  {
    if (empty())
    {
      return false;
    }

    auto bLastElement = (pHead == pTail);
    if (pHead->m_data == data)
    {
      auto tmp = pHead;
      pHead = pHead->pNext;
      if (pHead) {
        pHead->pPrev = nullptr;
      }

      if (bLastElement) {
        pTail = nullptr;
      }

      // delete tmp;
      TStoragePolicy::deallocate(tmp);
      --m_size;

      return true;
    }
    else if(pTail->m_data == data)
    {
      auto tmp = pTail;
      pTail = pTail->pPrev;
      if (pTail) {
        pTail->pNext = nullptr;
      }

      if (bLastElement) {
          pHead = nullptr;
      }

      // delete tmp;
      TStoragePolicy::deallocate(tmp);
      --m_size;

      return true;
    }

    if (auto pFoundNode = find(data); pFoundNode)
    {
      auto tmp = pFoundNode;
      pFoundNode->pPrev->pNext = pFoundNode->pNext;
      pFoundNode->pNext->pPrev = pFoundNode->pPrev;

      // delete tmp;
      TStoragePolicy::deallocate(tmp);
      --m_size;

      return true;
    }

    return false;
  }

  Node<T>* find(T& data) const
  {
    if (empty())
    {
      return nullptr;
    }

    if (pHead->m_data == data)
    {
      return pHead;
    }
    else if (pTail->m_data == data)
    {
      return pTail;
    }

    // Find the node by value comparison
    auto tmp = pHead;
    while (tmp->pNext)
    {
      if (tmp->pNext->m_data == data)
      {
        return tmp->pNext;
      }
      tmp = tmp->pNext;
    }

    return nullptr;
  }

  [[nodiscard]] Node<T>* getPrev(Node<T>* node) const
  {
    return node->pPrev;
  }

  [[nodiscard]] Node<T>* getNext(Node<T>* node) const
  {
    return node->pNext;
  }

  [[nodiscard]] Node<T>* head() const
  {
    return pHead;
  }

  [[nodiscard]] Node<T>*tail() const
  {
    return pTail;
  }

  [[nodiscard]] size_t size() const
  {
    return m_size;
  }

  [[nodiscard]] bool empty() const
  {
    return (pHead == nullptr) && (pTail == nullptr);
  }

 private:
  Node<T> *pHead{nullptr};
  Node<T> *pTail{nullptr};

  size_t m_size{0};
};

#endif//LINKED_LIST_CMAKE_BUILD_DEBUG_LINKEDLIST_H_
