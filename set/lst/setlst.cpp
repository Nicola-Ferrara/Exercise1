
namespace lasd {

/* ************************************************************************** */

template <typename Data>
SetLst<Data>::SetLst(const TraversableContainer<Data> & con) {
    con.Traverse([this])(const Data & dat) {
        Insert(dat);
    };
}

template <typename Data>
SetLst<Data>::SetLst(MappableContainer<Data> & con) {
    con.Traverse([this](const Data & dat) {
        Insert(std::move(dat));
    });
}

template <typename Data>
SetLst<Data>::~SetLst() {
    delete head;
}

template <typename Data>
SetLst<Data> & SetLst<Data>::operator=(const SetLst & cpy) {
    if (this != &cpy) {
        Clear();
        Node * cur = cpy.head;
        while (cur != nullptr) {
            Insert(cur->element);
            cur = cur->next;
        }
    }
    return *this;
}

template <typename Data>
SetLst<Data> & SetLst<Data>::operator=(SetLst && cpy) noexcept {
    if (this != &cpy) {
        Clear();
        std::swap(head, mv.head);
        std::swap(tail, mv.tail);
        std::swap(size, mv.size);
    }
    return *this;
}

template <typename Data>
bool SetLst<Data>::operator==(const SetLst<Data>& other) const noexcept {
    if (size != other.size) {
        return false; // Se le dimensioni sono diverse, i set non possono essere uguali
    }

    Node* current1 = head;
    Node* current2 = other.head;

    while (current1 != nullptr && current2 != nullptr) {
        if (current1->element != current2->element) {
            return false; // Se un elemento è diverso, i set non sono uguali
        }
        current1 = current1->next;
        current2 = current2->next;
    }

    return true; // Tutti gli elementi sono uguali
}

template <typename Data>
inline bool SetLst<Data>::operator!=(const SetLst<Data>& other) const noexcept {
    return !(*this == other); // Se sono diversi, restituisci true
}

template <typename Data>
const Data & SetLst<Data>::Min() const {
    if (size == 0) throw std::length_error("Set is empty");
    else return Front();
}

template <typename Data>
Data & SetLst<Data>::MinNRemove() {
    if (size == 0) throw std::length_error("Set is empty");
    else return FrontNRemove();
}

template <typename Data>
void SetLst<Data>::RemoveMin() {
    if (size == 0) throw std::length_error("Set is empty");
    else RemoveFromFront();
}

template <typename Data>
const Data & SetLst<Data>::Max() const {
    if (size == 0) throw std::length_error("Set is empty");
    else return Back();
}

template <typename Data>
Data & SetLst<Data>::MaxNRemove() {
    if (size == 0) throw std::length_error("Set is empty");
    else return BackNRemove();
}

template <typename Data>
void SetLst<Data>::RemoveMax() {
    if (size == 0) throw std::length_error("Set is empty");
    else RemoveFromBack();
}

template <typename Data>
const Data & SetLst<Data>::Predecessor(const Data & dat) const {
    if (size == 0 || dat == head->element || size == 1) {
        throw std::length_error("Element not found");
    } else {
        Node ** result = BinarySearch(dat);
        if (result == nullptr) {
            throw std::length_error("Element not found");
        }
        Node * current = head;
        while(current->next != *result) current = current->next;
        return current->element;
    }

template <typename Data>
typename SetLst<Data>::Node** SetLst<Data>::BinarySearch(const Data& dat) const {
    long left = 0;
    long right = size - 1;
    Node* leftNode = head;
    Node** current = &head;

    while (left <= right) {
        long mid = left + (right - left) / 2;
        Node* midNode = leftNode;
        Node** midPtr = current;
        for (long i = left; i < mid; ++i) {
            midPtr = &(midNode->next);
            midNode = midNode->next;
        }

        if (midNode->element == dat) {
            return midPtr;
        } else if (midNode->element < dat) {
            left = mid + 1;
            leftNode = midNode->next;
            current = &(midNode->next);
        } else {
            right = mid - 1;
        }
    }

    return nullptr;
}

template <typename Data>
bool SetLst<Data>::Insert(const Data& dat) {
    Node** pos = BinarySearch(dat);

    // Se l'elemento esiste già, non inserire
    if (pos != nullptr && (*pos) != nullptr && (*pos)->element == dat) {
        return false;
    }

    // Trova dove inserire (in testa o tra due nodi)
    Node** curr = &head;
    Node* prev = nullptr;
    while (*curr != nullptr && (*curr)->element < dat) {
        prev = *curr;
        curr = &((*curr)->next);
    }

    // Crea nuovo nodo
    Node* newNode = new Node(dat);

    // Inserisci in lista
    newNode->next = *curr;
    *curr = newNode;

    // Aggiorna tail se necessario
    if (newNode->next == nullptr) {
        tail = newNode;
    }

    ++size;
    return true;
}


/* ************************************************************************** */

}
