
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
Data SetLst<Data>::MinNRemove() {
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
Data SetLst<Data>::MaxNRemove() {
    if (size == 0) throw std::length_error("Set is empty");
    else return BackNRemove();
}

template <typename Data>
void SetLst<Data>::RemoveMax() {
    if (size == 0) throw std::length_error("Set is empty");
    else RemoveFromBack();
}

template <typename Data>
const Data & SetLst<Data>::Predecessor(const Data& data) const {
    // caso base: lista vuota o primo elemento >= data → niente predecessore
    if (size == 0 || head->element >= data)
        throw std::length_error("Predecessore non trovato");

    // BinarySearch restituisce il puntatore al puntatore del primo nodo >= data
    Node** pos = BinarySearch(data);

    // se pos punta ad head, allora *pos == head, ma abbiamo già escluso head->element >= data
    // *** predPtr deve sempre esistere ***
    Node* pred = nullptr;
    // scorriamo fino a trovare il nodo che punta a *pos
    for (Node* curr = head; curr != nullptr; curr = curr->next) {
        if (curr->next == *pos) {
            pred = curr;
            break;
        }
    }

    if (pred == nullptr)
        throw std::length_error("Predecessore non trovato");

    return pred->element;
}

template <typename Data>
Data SetLst<Data>::PredecessorNRemove(const Data& value) {
    Data val = Predecessor(value);
    Remove(val);
    return val;
}

template <typename Data>
void SetLst<Data>::RemovePredecessor(const Data& value) {
    Data val = Predecessor(value);
    Remove(val);
}

template <typename Data>
const Data & SetLst<Data>::Successor(const Data& data) const {
    // caso base: lista vuota o ultimo elemento <= data → niente successore
    if (size == 0 || tail->element <= data)
        throw std::length_error("Successore non trovato");

    // BinarySearch restituisce il puntatore al puntatore del primo nodo >= data
    Node** pos = BinarySearch(data);

    // se *pos esiste ed è esattamente == data, il successore è il nodo successivo
    Node* succ = (*pos != nullptr && (*pos)->element == data) ? (*pos)->next: *pos;

    if (succ == nullptr)
        throw std::length_error("Successore non trovato");

    return succ->element;
}

template <typename Data>
Data SetLst<Data>::SuccessorNRemove(const Data& value) {
    Data val = Successor(value);
    Remove(val);
    return val;
}

template <typename Data>
void SetLst<Data>::RemoveSuccessor(const Data& value) {
    Data val = Successor(value);
    Remove(val);
}


template <typename Data>
bool SetLst<Data>::Insert(const Data& value) {
    // Controlla se il valore esiste già
    if (Exists(value)) {
        return false; // Non inserire duplicati
    }

    // Trova la posizione corretta per l'inserimento
    Node** pos = BinarySearch(value);

    // Crea un nuovo nodo e inseriscilo nella lista
    Node* newNode = new Node(value);
    newNode->next = *pos;
    *pos = newNode;

    // Aggiorna la dimensione della lista
    ++size;

    return true;
}

template <typename Data>
bool SetLst<Data>::Insert(Data&& value) {
    // Controlla se il valore esiste già
    if (Exists(value)) {
        return false; // Non inserire duplicati
    }

    // Trova la posizione corretta per l'inserimento
    Node** pos = BinarySearch(value);

    // Crea un nuovo nodo e inseriscilo nella lista
    Node* newNode = new Node(std::move(value));
    newNode->next = *pos;
    *pos = newNode;

    // Aggiorna la dimensione della lista
    ++size;

    return true;
}

template <typename Data>
bool SetLst<Data>::Remove(const Data& value) {
    // Trova il nodo da rimuovere
    Node** pos = BinarySearch(value);

    // Controlla se il nodo esiste
    if (*pos == nullptr || (*pos)->element != value) {
        return false; // Il valore non esiste nella lista
    }

    // Rimuovi il nodo
    Node* temp = *pos;
    *pos = (*pos)->next;
    delete temp;

    // Aggiorna la dimensione della lista
    --size;

    return true;
}

template <typename Data>
const Data & SetLst<Data>::operator[](const unsigned long index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }

    Node* current = head;
    for (unsigned long i = 0; i < index; ++i) {
        current = current->next;
    }

    return current->element;
}

template <typename Data>
bool SetLst<Data>::Exists(const Data& value) const noexcept {
    // Usa BinarySearch per trovare la posizione del valore
    Node** pos = BinarySearch(value);

    // Controlla se il valore esiste
    return (*pos != nullptr && (*pos)->element == value);
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

template <typename Data>
void SetLst<Data>::Clear() {
    List<Data>::Clear();
}


/* ************************************************************************** */

}