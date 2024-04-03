//
// Created by qwe on 15.03.24.
//

#ifndef CPP2_S21_CONTAINERS_2_SRC_AVLTREE_AVLTREE_H_
#define CPP2_S21_CONTAINERS_2_SRC_AVLTREE_AVLTREE_H_

#include <iostream>
#include <algorithm>

template <typename Key, typename Value>
class AVLTree {
 private:
  struct Node {
    Key key;
    Value value;
    Node *left;
    Node *right;
    Node *parent;
    int height;
  };



 public:
  class Iterator;
  class ConstIterator;

  using reference = Value&;
  using const_reference = const Value&;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using size_type = size_t;

  class Iterator{
   public:

    Node* iter_node;
    Node* iter_past_node;
    Iterator() : iter_node(nullptr), iter_past_node(nullptr) {};
    Iterator(Node* node, Node* past_node = nullptr) : iter_node(node), iter_past_node(past_node) {};
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);
    reference operator*();
    bool operator==(const iterator& it);
    bool operator!=(const iterator& it);




    Node* MoveForward(Node* node);
    Node* MoveBack(Node* node);
  };
  class ConstIterator : public Iterator {
   public:
    ConstIterator() : Iterator() {};
    ConstIterator(Node* node, Node* past_node = nullptr) : Iterator(node, past_node) {};
    const_reference operator*() const { return Iterator::operator*(); };
  };


  Node *root;
  // Constructor
  AVLTree() : root(nullptr) {}
  // Destructor
  ~AVLTree() { clear(root); }
  //TODO конструктор копирования
  //TODO конструкторы и присвоения

  void clear(Node *node);
  int getHeight(Node *node);
  int bFactor(Node *node);
  void setHeight(Node *node);
  void leftRotate(Node *node);
  void rightRotate(Node *node);
  void balance(Node *node);
  bool insert(const Key &key, const Value &value);
  Node *find(const Key &key);
  static Node *findMax(Node *node);
  static Node *findMin(Node *node);
  bool remove(const Key &key);
  void swap(AVLTree& other);
  void merge(AVLTree& other);
  size_type size();
  size_type max_size();


};


template <typename Key, typename Value>
int AVLTree<Key, Value>::getHeight(Node *node) {
  if (node == nullptr) {
    return 0;
  }
  return node->height;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::clear(Node *node) {
  if (node) {
    clear(node->left);
    clear(node->right);
    delete node;
  }
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::bFactor(Node *node) {
  return getHeight(node->right) - getHeight(node->left);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::setHeight(Node *node) {
  node->height = std::max(getHeight(node->right), getHeight(node->left)) + 1;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::leftRotate(Node *node) {
  Node *newParentNode = node->right;
  node->right = newParentNode->left;
  if (newParentNode->left != nullptr) {
    newParentNode->left->parent = node;
  }
  newParentNode->parent = node->parent;
  if (node->parent == nullptr) {
    root = newParentNode;
  } else if (node == node->parent->left) {
    node->parent->left = newParentNode;
  } else {
    node->parent->right = newParentNode;
  }
  newParentNode->left = node;
  node->parent = newParentNode;

  setHeight(node);
  setHeight(newParentNode);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::rightRotate(Node *node) {
  Node *newParentNode = node->left;
  node->left = newParentNode->right;
  if (newParentNode->right != nullptr) {
    newParentNode->right->parent = node;
  }
  newParentNode->parent = node->parent;
  if (node->parent == nullptr) {
    root = newParentNode;
  } else if (node == node->parent->right) {
    node->parent->right = newParentNode;
  } else {
    node->parent->left = newParentNode;
  }
  newParentNode->right = node;
  node->parent = newParentNode;

  setHeight(node);
  setHeight(newParentNode);
}


template <typename Key, typename Value>
void AVLTree<Key, Value>::balance(Node *node) {
  while (node != nullptr) {
    setHeight(node);
    int balanceFactor = bFactor(node);
    if (balanceFactor == -2) {
      if (bFactor(node->left) > 0) {
        leftRotate(node->left);
      }
      rightRotate(node);
    } else if (balanceFactor == 2) {
      if (bFactor(node->right) < 0) {
        rightRotate(node->right);
      }
      leftRotate(node);
    }
    node = node->parent;
  }
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::insert(const Key &key, const Value &value) {
  if (root == nullptr) {
    Node *newNode = new Node{key, value, nullptr, nullptr, nullptr, 1};
    root = newNode;
    return true;
  }

  Node *current = root;
  Node *parent = nullptr;

  while (current) {
    parent = current;
    if (key < current->key) {
      current = current->left;
    } else if (key > current->key) {
      current = current->right;
    } else {
      return false;
    }
  }

  Node *newNode = new Node{key, value, nullptr, nullptr, parent, 1};
  if (key < parent->key) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }

  balance(newNode);

  return true;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node *AVLTree<Key, Value>::find(const Key &key) {
  Node *current = root;
  while (current != nullptr) {
    if (key < current->key) {
      current = current->left;
    } else if (key > current->key) {
      current = current->right;
    } else {
      return current;
    }
  }
  return nullptr;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node *AVLTree<Key, Value>::findMax(Node *node) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->right == nullptr) {
    return node;
  }
  return findMax(node->right);
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node *AVLTree<Key, Value>::findMin(Node *node) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->left == nullptr) {
    return node;
  }
  return findMin(node->left);
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::remove(const Key &key) {
  Node *remove = find(key);
  if (remove == nullptr) {
    return false;
  }

  if (remove->left == nullptr && remove->right == nullptr) {
    if (remove->parent) {
      if (remove == remove->parent->left) {
        remove->parent->left = nullptr;
      } else {
        remove->parent->right = nullptr;
      }
    } else {
      root = nullptr;
    }

    delete remove;
    return true;
  }

  if (remove->left == nullptr && remove->right != nullptr) {
    if (remove == remove->parent->left) {
      remove->parent->left = remove->right;
    } else {
      remove->parent->right = remove->right;
    }
    delete remove;
    return true;
  } else if (remove->left != nullptr && remove->right == nullptr) {
    if (remove == remove->parent->left) {
      remove->parent->left = remove->left;
    } else {
      remove->parent->right = remove->left;
    }
    delete remove;
    return true;
  }

  if (remove->left != nullptr && remove->right != nullptr) {
    Node *exchange = findMax(remove->left);

    remove->key = exchange->key;
    remove->value = exchange->value;

    if (exchange == exchange->parent->left) {
      exchange->parent->left = exchange->left;
    } else {
      exchange->parent->right = exchange->left;
    }

    if (exchange->left != nullptr) {
      exchange->left->parent = exchange->parent;
    }

    delete exchange;
    balance(remove->left);
  }

  balance(remove->parent);

  return true;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::Iterator::MoveForward(AVLTree::Node* node){
  if (node->right != nullptr){
      return findMin(node->right);
  }

  if (node->parent->left == node){
    return node->parent;
  }

  Node *current = node;
  while ( current->parent != nullptr && node->key > current->parent->key ){
    current = current->parent;
  }

  if (node->key > current->key && current->parent == nullptr ){
    return node;
  }

  current = current->parent;
  return current;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::Iterator::MoveBack(AVLTree::Node* node){
  if (node->left != nullptr){
    return findMax(node->left);
  }

  if (node->parent->right == node){
    return node->parent;
  }

  Node *current = node;
  while ( current->parent != nullptr && node->key < current->parent->key ){
    current = current->parent;
  }

  if (node->key < current->key && current->parent == nullptr ){
    return node;
  }

  current = current->parent;
  return current;

}

template<typename Key, typename Value>
void AVLTree<Key, Value>::swap(AVLTree &other){
  std::swap(root, other.root);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::merge(AVLTree &other){
  if (other.root == nullptr){
    return;
  } else if (root == nullptr){
    root = other.root;
    return;
  }

  iterator it = other.findMin(other.root);

  for ( ; it != other.findMax(other.root); it++ ){

    Node *current = root;
    Node *parent = nullptr;

    while (current) {
      parent = current;
      if (it.iter_node->key < current->key) {
        current = current->left;
      } else if (it.iter_node->key > current->key) {
        current = current->right;
      } else {
          delete it.iter_node; // разобраться как точно удалять
          break;
      }
    }

    if (it.iter_node == nullptr){
      continue;
    }

    if ( it.iter_node->key < parent->key ) {
       parent->left = it.iter_node;
      it.iter_node->parent = parent->left;

    } else {
      parent->right = it.iter_node;
      it.iter_node->parent = parent->right;
    }

    balance(it.iter_node);

  }
}




template <typename Key, typename Value>
typename AVLTree<Key, Value>::Iterator& AVLTree<Key, Value>::Iterator::operator++() {
  iter_node = MoveForward(iter_node);
  return *this;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Iterator AVLTree<Key, Value>::Iterator::operator++(int) {
  iterator temp = *this;
  iter_node = MoveForward(iter_node);
  return temp;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Iterator& AVLTree<Key, Value>::Iterator::operator--() {
  iter_node = MoveBack(iter_node);
  return *this;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::Iterator AVLTree<Key, Value>::Iterator::operator--(int) {
  iterator temp = *this;
  iter_node = MoveBack(iter_node);
  return temp;
}

template <typename Key, typename Value>
typename AVLTree<Key, Value>::reference AVLTree<Key, Value>::Iterator::operator*() {
  if (iter_node == nullptr) {
    throw std::runtime_error("Dereferencing end() iterator");
  }
  return iter_node->value;
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::Iterator::operator==(const Iterator& it) {
  return iter_node == it.iter_node && iter_past_node == it.iter_past_node;
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::Iterator::operator!=(const Iterator& it) {
  return iter_node != it.iter_node || iter_past_node != it.iter_past_node;
}

template <typename Key, typename Value>
size_t AVLTree<Key, Value>::size(){
  if (node == nullptr) {
    return 0;
  }
  return 1 + size(node->left) + size(node->right);
}

template <typename Key, typename Value>
size_t AVLTree<Key, Value>::max_size(){
  return std::numeric_limits<size_type>::max();
}


#endif  // CPP2_S21_CONTAINERS_2_SRC_AVLTREE_AVLTREE_H_
