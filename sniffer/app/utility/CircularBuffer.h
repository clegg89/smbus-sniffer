#pragma once
//! @file

#include <cstddef>
#include <array>

//! Statically allocated Circular Buffer implementation
template <typename T, size_t SIZE>
class CircularBuffer {
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  static constexpr size_t size() {
    return SIZE;
  }

private:
  //! Underlying buffer
  std::array<value_type, size()> buffer_{};

  using buffer_iter = typename decltype(buffer_)::iterator;
  using const_buffer_iter = typename decltype(buffer_)::const_iterator;

  //! Where to read from
  buffer_iter head_ = std::begin(buffer_);
  //! Where to write to
  buffer_iter tail_ = std::begin(buffer_);

  //! Incrememt a buffer iterator, wrapping if it goes past the end
  const_buffer_iter increment(buffer_iter iter) const {
    ++iter;
    if (iter == std::end(buffer_)) {
      return std::begin(buffer_);
    }
    return iter;
  }

  //! Incrememt a buffer iterator, wrapping if it goes past the end
  buffer_iter increment(buffer_iter iter) {
    ++iter;
    if (iter == std::end(buffer_)) {
      return std::begin(buffer_);
    }
    return iter;
  }

public:
  //! Constructor
  constexpr CircularBuffer() = default;

  //! @returns true if empty, false otherwise
  bool empty() const {
    return (head_ == tail_);
  }

  //! @returns true if full, false otherwise
  bool full() const {
    return (increment(tail_) == head_);
  }

  //! @returns Number of items in buffer
  size_t count() const {
    if (std::distance(head_, tail_) >= 0) {
      // not wrapped
      return std::distance(head_, tail_);
    } else {
      // wrapped
      return size() - std::distance(tail_, head_);
    }
  }

  //! Push a new item into the buffer
  //! @return false if full
  bool push(value_type newval) {
    if (full()) {
      return false;
    }
    *tail_ = newval;
    tail_ = increment(tail_);
    return true;
  }

  //! Attempt to pop an item from the buffer
  //! @returns true if an item was popped, false otherwise (buffer empty)
  bool pop(reference retval) {
    if (empty()) {
      return false;
    }
    retval = *head_;
    head_ = increment(head_);
    return true;
  }
};

