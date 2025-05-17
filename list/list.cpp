
namespace lasd {

/* ************************************************************************** */

// Specific constructors (Move)
template <typename Data>
List<Data>::Node::Node(Data && dat) noexcept {
  std::swap(element, dat);
}

template<typename Data>
List<Data>::Node::Node(Node && nod) noexcept {
  std::swap(element, nod.element);
  std::swap(next, nod.next);
}

//Node Destructor
template<typename Data>
List<Data>::Node::~Node() {
  delete next;
}

//Comparison operators
template<typename Data>
bool List<Data>::Node::operator==(const Node & nod) const noexcept {
    if (element != nod.element) {
        return false;
    }
    if (next == nullptr && nod.next == nullptr) {
        return true;
    }
    if (next != nullptr && nod.next != nullptr) {
        return *next == *nod.next;
    }
    return false;
}

template <typename Data>
bool List<Data>::Node::operator!=(const Node & other) const noexcept {
  return !(*this == other);
}

// Specific member functions
template<typename Data>
typename List<Data>::Node * List<Data>::Node::Clone(Node * tail) {
  if (next == nullptr) {
    return tail;
  } else {
    Node * node = new Node(element);
    node->next = next->Clone(tail);
    return node;
  }
}

// Specific constructor
template <typename Data>
List<Data>::List(const TraversableContainer<Data> & container) {
  container.Traverse([this](const Data & dat) { InsertAtBack(dat); });
}

template <typename Data>
List<Data>::List(MappableContainer<Data> && container) {
  container.Map([this](Data & dat) { InsertAtBack(std::move(dat)); });
}

// Copy constructor
template<typename Data>
List<Data>::List(const List<Data> & lst) {
  if (lst.tail != nullptr) {
    tail = new Node(*lst.tail);
    head = lst.head->Clone(tail);
    size = lst.size;
  }
}
// Move constructor
template<typename Data>
List<Data>::List(List<Data> && lst) noexcept {
  std::swap(head, lst.head);
  std::swap(tail, lst.tail);
  std::swap(size, lst.size);
}

// List Destructor
template<typename Data>
List<Data>::~List() {
  delete head;
}

template<typename Data>
List<Data> & List<Data>::operator=(const List<Data> & lst) {
  if (size <= lst.size) {
    if (tail == nullptr) {
      List<Data> * tmplst = new List<Data>(lst);
      std::swap(*tmplst, *this);
      delete tmplst;
    } else {
      Node * ocur = lst.head;
      for (Node * ncur = head; ncur != nullptr; ncur = ncur->next, ocur = ocur->next) {
        ncur->element = ocur->element;
      }
      if (ocur != nullptr) {
        Node * newtail = new Node(*lst.tail);
        tail->next = ocur->Clone(newtail);
        tail = newtail;
      }
    }
  } else {
    if (lst.tail == nullptr) {
      delete head;
      head = tail = nullptr;
    } else {
      Node * ncur = head;
      for (Node * ocur = lst.head; ocur != nullptr; ocur = ocur->next, tail = ncur, ncur = ncur->next) {
        ncur->element = ocur->element;
      }
      delete ncur;
      tail->next = nullptr;
    }
  }
  size = lst.size;
  return *this;
}

// Move assignment
template<typename Data>
List<Data> & List<Data>::operator=(List<Data> && lst) noexcept {
  std::swap(head, lst.head);
  std::swap(tail, lst.tail);
  std::swap(size, lst.size);
  return *this;
}

// Comparison operators
template<typename Data>
inline bool List<Data>::operator==(const List<Data> & lst) const noexcept {
    if (size != lst.size) {
        return false;
    }
    if (head == nullptr && lst.head == nullptr) {
        return true;
    }
    if (head != nullptr && lst.head != nullptr) {
        return *head == *lst.head;
    }
    return false;
}

template<typename Data>
inline bool List<Data>::operator!=(const List<Data> & lst) const noexcept {
  return !(*this == lst);
}

// Specific member functions
template <typename Data>
void List<Data>::InsertAtFront(const Data & dat) {
  Node * newNode = new Node(dat);
  newNode->next = head;
  head = newNode;
  if (tail == nullptr) tail = head;
  ++size;
}

template <typename Data>
void List<Data>::InsertAtFront(Data && dat) {
  Node * newNode = new Node(std::move(dat));
  newNode->next = head;
  head = newNode;
  if (tail == nullptr) tail = head;
  ++size;
}

template<typename Data>
void List<Data>::RemoveFromFront() {
  if (head != nullptr) {
    Node * front = head;
    if (tail == head) {
      head = tail = nullptr;
    } else {
      head = head->next;
    }
    --size;
    front->next = nullptr;
    delete front;
  } else {
    throw std::length_error("Access to an empty list.");
  }
}

template<typename Data>
Data List<Data>::FrontNRemove() {
  if (head != nullptr) {
    Node * front = head;
    if (tail == head) {
      head = tail = nullptr;
    } else {
      head = head->next;
    }
    --size;
    front->next = nullptr;
    Data dat(std::move(front->element));
    delete front;
    return dat;
  } else {
    throw std::length_error("Access to an empty list.");
  }
}

//da ricontrollare
template<typename Data>
void List<Data>::InsertAtBack(const Data & dat) {
  Node * end = new Node(dat);
  if (tail == nullptr) {
    head = end;
  } else {
    tail->next = end;
  }
  tail = end;
  ++size;
}

template<typename Data>
void List<Data>::InsertAtBack(Data && dat) {
  Node * end = new Node(std::move(dat));
  if (tail == nullptr) {
    head = end;
  } else {
    tail->next = end;
  }
  tail = end;
  ++size;
}

template <typename Data>
void List<Data>::RemoveFromBack() {
  if (size == 0) throw std::length_error("List is empty");
  if (size == 1) {
    delete head;
    head = tail = nullptr;
    size = 0;
    return;
  }
  Node * curr = head;
  while (curr->next != tail) {
    curr = curr->next;
  }
  delete tail;
  tail = curr;
  tail->next = nullptr;
  --size;
}

template <typename Data>
Data List<Data>::BackNRemove() {
  if (size == 0) throw std::length_error("List is empty");
  if (size == 1) {
    Data result = std::move(head->element);
    delete head;
    head = tail = nullptr;
    size = 0;
    return result;
  }
  Node * curr = head;
  while (curr->next != tail) {
    curr = curr->next;
  }
  Data result = std::move(tail->element);
  delete tail;
  tail = curr;
  tail->next = nullptr;
  --size;
  return result;
}

// Specific member functions (inherited from MutableLinearContainer)
template <typename Data>
Data & List<Data>::operator[](ulong index) {
  if (index >= size) throw std::out_of_range("Index out of range");
  Node * curr = head;
  for (ulong i = 0; i < index; ++i) {
    curr = curr->next;
  }
  return curr->element;
}

template <typename Data>
Data & List<Data>::Front() {
  if (size == 0) throw std::length_error("List is empty");
  return head->element;
}

template <typename Data>
Data & List<Data>::Back() {
  if (size == 0) throw std::length_error("List is empty");
  return tail->element;
}

// Specific member functions (inherited from LinearContainer)
template<typename Data>
const Data & List<Data>::operator[](const ulong index) const {
  if (index < size) {
    Node * cur = head;
    for (ulong idx = 0; idx < index; ++idx, cur = cur->next) {}
    return cur->element;
  } else {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; list size " + std::to_string(size) + ".");
  }
}

template<typename Data>
const Data & List<Data>::Front() const {
  if (head != nullptr) {
    return head->element;
  } else {
    throw std::length_error("Access to an empty list.");
  }
}

template<typename Data>
const Data & List<Data>::Back() const {
  if (tail != nullptr) {
    return tail->element;
  } else {
    throw std::length_error("Access to an empty list");
  }
}

// Specific member function (inherited from MappableContainer)
template<typename Data>
inline void List<Data>::Map(MapFun fun) {
  PreOrderMap(fun, head);
}

// Specific member function (inherited from PreOrderMappableContainer)
template<typename Data>
inline void List<Data>::PreOrderMap(MapFun fun) {
  PreOrderMap(fun, head);
}

// Specific member function (inherited from PostOrderMappableContainer)
template<typename Data>
inline void List<Data>::PostOrderMap(MapFun fun) {
  PostOrderMap(fun, head);
}

// Specific member function (inherited from TraversableContainer)
template<typename Data>
inline void List<Data>::Traverse(TraverseFun fun) const {
  PreOrderTraverse(fun, head);
}

// Specific member function (inherited from PreOrderTraversableContainer)
template<typename Data>
inline void List<Data>::PreOrderTraverse(TraverseFun fun) const {
  PreOrderTraverse(fun, head);
}

// Specific member function (inherited from PostOrderTraversableContainer)
template<typename Data>
inline void List<Data>::PostOrderTraverse(TraverseFun fun) const {
  PostOrderTraverse(fun, head);
}

// Specific member function (inherited from ClearableContainer)
template<typename Data>
void List<Data>::Clear() {
  delete head;
  head = tail = nullptr;
  size = 0;
}

//Altre funzioni
template <typename Data>
void List<Data>::PreOrderTraverse(TraverseFun fun, const Node * cur) const {
  for (; cur != nullptr; cur = cur->next) {
    fun(cur->element);
  }
}

template <typename Data>
void List<Data>::PostOrderTraverse(TraverseFun fun, const Node * node) const {
  if (node == nullptr) return;
  PostOrderTraverse(fun, node->next);
  fun(node->element);
}

template<typename Data>
void List<Data>::PreOrderMap(MapFun fun, Node * cur) {
  for (; cur != nullptr; cur = cur->next) {
    fun(cur->element);
  }
}

template<typename Data>
void List<Data>::PostOrderMap(MapFun fun, Node * cur) {
  if (cur != nullptr) {
    PostOrderMap(fun, cur->next);
    fun(cur->element);
  }
}

/* ************************************************************************** */

}