
namespace lasd {

/* ************************************************************************** */

template <typename Data>
SetLst<Data>::SetLst(const TraversableContainer<Data> & con) {
    con.Traverse([this](const Data & dat) {
        Insert(dat);
    });
}

template <typename Data>
SetLst<Data>::SetLst(MappableContainer<Data> && con) {
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
        std::swap(head, cpy.head);
        std::swap(tail, cpy.tail);
        std::swap(size, cpy.size);
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
}

template <typename Data>
typename SetLst<Data>::Node** SetLst<Data>::BinarySearch(const Data& dat) const {
    if (size == 0) {
        return const_cast<Node**>(&head);
    }

    long left = 0;
    long right = size - 1;

    Node* leftNode = head;
    Node** leftPtr = const_cast<Node**>(&head);
    Node** resultPtr = leftPtr;

    while (left <= right) {
        long mid = left + (right - left) / 2;
        
        // Calcola la distanza da sinistra
        long steps = mid - left;
        
        Node* midNode = leftNode;
        Node** midPtr = leftPtr;
        for (long i = 0; i < steps && midNode != nullptr; ++i) {
            midPtr = &((*midPtr)->next);
            midNode = *midPtr;
        }

        if (midNode == nullptr) {
            return resultPtr;
        }

        if (midNode->element == dat) {
            return midPtr;
        } else if (midNode->element < dat) {
            // Aggiorna il puntatore risultato
            resultPtr = &(midNode->next);
            
            // Sposta "sinistra" avanti
            left = mid + 1;
            leftNode = midNode->next;
            leftPtr = &((*midPtr)->next);
        } else {
            right = mid - 1;
            resultPtr = midPtr;
        }
    }

    return resultPtr;
}
}

// ...existing code...

template <typename Data>
bool SetLst<Data>::Insert(const Data& dat) {
    Node** pos = BinarySearch(dat);

    // Se già presente, non inserire
    if (pos != nullptr && (*pos) != nullptr && (*pos)->element == dat) {
        return false;
    }

    // Trova dove inserire (se pos è nullptr, inserisci in testa)
    Node** insertPos = &head;
    Node* curr = head;
    while (curr != nullptr && curr->element < dat) {
        insertPos = &(curr->next);
        curr = curr->next;
    }

    Node* newNode = new Node(dat);
    newNode->next = *insertPos;
    *insertPos = newNode;

    if (newNode->next == nullptr) {
        tail = newNode;
    }

    ++size;
    return true;
}


/* ************************************************************************** */

