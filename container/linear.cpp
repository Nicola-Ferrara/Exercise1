
namespace lasd {

/* ************************************************************************** */

template<typename Data>
inline bool LinearContainer<Data>::operator==(const LinearContainer<Data> & con) const noexcept {
  if (this->size == con.Size()) {
    for (ulong index = 0; index < this->size; ++index) {
      if (operator[](index) != con.operator[](index)) {
        return false;
      }
    }
    return true;
  } else {
    return false;
  }
}

template<typename Data>
inline bool LinearContainer<Data>::operator!=(const LinearContainer<Data> & con) const noexcept {
  return !(*this == con);
}

/* ************************************************************************** */

template <typename Data>
inline const Data & LinearContainer<Data>::Front() const {
  if (this->size == 0) {
    throw std::length_error("Container is empty");
  }
  return operator[](0);
}

template <typename Data>
inline const Data & LinearContainer<Data>::Back() const {
  if (this->size == 0) {
    throw std::length_error("Container is empty");
  }
  return operator[](this->size - 1);
}

template<typename Data>
inline void LinearContainer<Data>::Traverse(TraverseFun fun) const {
  PreOrderTraverse(fun);
}

template<typename Data>
inline void LinearContainer<Data>::PreOrderTraverse(TraverseFun fun) const {
    for (ulong index = 0; index < this->size; ++index) {
      fun(operator[](index));
    }
}

template<typename Data>
inline void LinearContainer<Data>::PostOrderTraverse(TraverseFun fun) const {
  ulong index = this->size;
  while (index > 0) {
    fun(operator[](--index));
  }
}

template<typename Data>
Data & MutableLinearContainer<Data>::Front() {
    if (this->size == 0) {
        throw std::length_error("Container is empty");
    }
    return operator[](0);
}

template<typename Data>
Data & MutableLinearContainer<Data>::Back() {
    if (this->size == 0) {
        throw std::length_error("Container is empty");
    }
    return operator[](this->size - 1);
}

template<typename Data>
void MutableLinearContainer<Data>::Map(MapFun fun) {
    PreOrderMap(fun);
}

template<typename Data>
void MutableLinearContainer<Data>::PreOrderMap(MapFun fun) {
    for (ulong index = 0; index < this->size; ++index) {
        fun(operator[](index));
    }
}

template<typename Data>
void MutableLinearContainer<Data>::PostOrderMap(MapFun fun) {
    ulong index = this->size;
    while (index > 0) {
        fun(operator[](--index));
    }
}

template<typename Data>
void SortableLinearContainer<Data>::Sort() noexcept {
    InsertionSort(0, this->size);
}

template<typename Data>
void SortableLinearContainer<Data>::InsertionSort(ulong p, ulong r) noexcept {
    for (ulong i = p + 1; i < r; ++i) {
        Data key = std::move(this->operator[](i));
        ulong j = i;
        while (j > p && this->operator[](j - 1) > key) {
            this->operator[](j) = std::move(this->operator[](j - 1));
            --j;
        }
        this->operator[](j) = std::move(key);
    }
}

}
