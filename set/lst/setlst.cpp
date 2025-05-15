
namespace lasd {

/* ************************************************************************** */

template <typename Data>
SetLst<Data>::SetLst(const TraversableContainer<Data> & con) {
    con.Traverse([this])(const Data & dat) {
        Insert(dat);
    };
}

template <typename Data>
SetLst<Data>::SetLst(const MappableContainerata> & con) {
    con.Traverse([this])(const Data & dat) {
        Insert(std::move(dat));
    };
}

template <typename Data>
SetLst<Data>::~SetLst() {
    delete head;
}

template <typename Data>
SetLst<Data> & SetLst<Data>::operator=(const SetLst & cpy) {
    if (this != &cpy) {
        Clear();
        Nodde * cur = cpy.head;
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
    if (size == 0) throw std::lenght_error("Set is empty");
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
    if (size == 0) throw std::lenght_error("Set is empty");
    else RemoveFromBack();
}

template <typename Data>
const Data & SetLst<Data>::Predecessor(const Data & dat) const {
    if (size == 0 || dat == head->elements || size == 1) {
        throw std::length_error("Element not found");
    } else {
        Node* current = head;
        Node* predecessor = nullptr;

        // Simula la ricerca binaria
        unsigned long left = 0;
        unsigned long right = size - 1;
        unsigned long mid;
        unsigned long n = 0;

        while (left <= right) {
            mid = left + (right - left) / 2;
            mid = mid - n;
            // Trova il nodo a metà
            Node* midNode = head;
            for (unsigned long i = n; i < mid+n; ++i) {
                midNode = midNode->next;
            }
            n = mid;

            if (midNode->element < value) {
                predecessor = midNode; // Aggiorna il predecessore
                left = mid + 1;        // Cerca a destra
            } else {
                right = mid - 1;       // Cerca a sinistra
            }
        }

        if (predecessor == nullptr) {
            throw std::length_error("No predecessor found");
        }

        return predecessor->element;

    }

}

/* ************************************************************************** */

}
