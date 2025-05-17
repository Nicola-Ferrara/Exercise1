#ifndef SETLST_HPP
#define SETLST_HPP

/* ************************************************************************** */

#include "../set.hpp"
#include "../../list/list.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class SetLst : virtual public Set<Data>,
              virtual protected List<Data> {// dovrebbe essere protected
  // Must extend Set<Data>,
  //             List<Data>

private:

  // ...

protected:

  using List<Data>::size;
  using List<Data>::head;
  using List<Data>::tail;
  using typename List<Data>::Node;

  // ...

public:

  // Default constructor
  inline SetLst() = default;

  /* ************************************************************************ */

  // Specific constructors
  SetLst(const TraversableContainer<Data> &); // A set obtained from a TraversableContainer
  SetLst(MappableContainer<Data> &&); // A set obtained from a MappableContainer

  /* ************************************************************************ */

  // Copy constructor
  SetLst(const SetLst & cpy) : List<Data>(cpy) {};

  // Move constructor
  SetLst(SetLst && mv) noexcept : List<Data>(std::move(mv)) {};

  /* ************************************************************************ */

  // Destructor
  virtual ~SetLst() {};

  /* ************************************************************************ */

  // Copy assignment
  SetLst & operator=(const SetLst &);

  // Move assignment
  SetLst & operator=(SetLst &&) noexcept;

  /* ************************************************************************ */

  // Comparison operators
  bool operator==(const SetLst &) const noexcept;
  inline bool operator!=(const SetLst &) const noexcept;

  /* ************************************************************************ */

  // Specific member functions (inherited from OrderedDictionaryContainer)

  const Data & Min() const override; //(sarebbe Front) Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  Data MinNRemove() override; //(sarebbe FrontNRemove) Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  void RemoveMin() override; //(sarebbe RemoveFromFront) Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)

  const Data & Max() const override; //(sarebbe Back) Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  Data MaxNRemove() override; //(sarebbe BackNRemove) Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
  void RemoveMax() override; //(sarebbe RemoveFromBack) Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)

  const Data & Predecessor(const Data &) const override; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  Data PredecessorNRemove(const Data &) override; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  void RemovePredecessor(const Data &) override; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)

  const Data & Successor(const Data &) const override; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  Data SuccessorNRemove(const Data &) override; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
  void RemoveSuccessor(const Data &) override; // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)

  /* ************************************************************************ */

  // Specific member functions (inherited from DictionaryContainer)

  bool Insert(const Data &) override; // Override DictionaryContainer member (copy of the value)
  bool Insert(Data &&) override; // Override DictionaryContainer member (move of the value)
  bool Remove(const Data &) override; // Override DictionaryContainer member

  /* ************************************************************************ */

  // Specific member functions (inherited from LinearContainer)

  const Data & operator[](ulong) const override; // Override LinearContainer member (must throw std::out_of_range when out of range)
  //using List<Data>::operator[];

  /* ************************************************************************** */

  // Specific member function (inherited from TestableContainer)

  bool Exists(const Data &) const noexcept override; // Override TestableContainer member

  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)

  void Clear() override; // Override ClearableContainer member

protected:

  // Auxiliary functions, if necessary!
  //Funzione che arriva a metà lista, deve restituire il puntatore al nodo che ha raggiunto
  Node** BinarySearch(const Data&) const;
  using List<Data>::Front;
  using List<Data>::Back;
  using List<Data>::BackNRemove;
  using List<Data>::FrontNRemove;
  using List<Data>::RemoveFromFront;
  using List<Data>::RemoveFromBack;

};

/* ************************************************************************** */

}

#include "setlst.cpp"

#endif
