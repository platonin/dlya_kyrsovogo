#ifndef VECTOR_H

#include <stdexcept>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <algorithm>
#include <memory>
#include <type_traits>

template <typename T>
class Vector {
 public:
  Vector() : array_(nullptr), size_(0), capacity_(0) {
  }

  explicit Vector(size_t num) : size_(num), capacity_(num) {
    if (size_ > 0) {
      array_ = new T[size_];
      try {
        for (size_t i = 0; i < size_; i++) {
          array_[i] = T();
        }
      } catch (...) {
        delete[] array_;
        throw;
      }
    } else {
      array_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }
  }

  Vector(size_t num, T value) : size_(num), capacity_(num) {
    if (size_ > 0) {
      array_ = new T[size_];
      try {
        std::fill(array_, array_ + num, value);
      } catch (...) {
        delete[] array_;
        throw;
      }
    } else {
      array_ = nullptr;
      size_ = 0;
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator second)
      : size_(std::distance(first, second)), capacity_(std::distance(first, second)) {
    if (size_ > 0) {
      array_ = new T[size_];
      try {
        std::copy(first, second, array_);
      } catch (...) {
        delete[] array_;
        throw;
      }
    } else {
      array_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }
  }

  Vector(std::initializer_list<T> list) : size_(list.size()), capacity_(list.size()) {
    if (size_ != 0) {
      array_ = new T[size_];
      try {
        std::copy(list.begin(), list.end(), array_);
      } catch (...) {
        delete[] array_;
        throw;
      }
    } else {
      array_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }
  }

  Vector(const Vector& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (size_ != 0) {
      array_ = new T[size_];
      try {
        std::copy(other.array_, other.array_ + size_, array_);
      } catch (...) {
        delete[] array_;
        throw;
      }
    } else {
      array_ = nullptr;
    }
  }

  Vector& operator=(const Vector& other) {
    if (this != &other) {
      delete[] array_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      array_ = nullptr;
      if (size_ != 0) {
        array_ = new T[size_];
      }
      std::copy(other.array_, other.array_ + size_, array_);
    }
    return *this;
  }

  Vector(Vector&& other) noexcept : array_(other.array_), size_(other.size_), capacity_(other.capacity_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.array_ = nullptr;
  }

  Vector& operator=(Vector&& other) noexcept {
    if (this != &other) {
      delete[] array_;
      array_ = other.array_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.array_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  ~Vector() {
    delete[] array_;
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  bool Empty() const {
    return (size_ == 0);
  }

  T& operator[](size_t i) {
    return array_[i];
  }

  const T& operator[](size_t i) const {
    return array_[i];
  }

  T& At(size_t i) {
    if (i >= size_) {
      throw std::out_of_range("");
    }
    return array_[i];
  }

  const T& At(size_t i) const {
    if (i >= size_) {
      throw std::out_of_range("");
    }
    return array_[i];
  }

  T& Front() {
    return array_[0];
  }

  const T& Front() const {
    return array_[0];
  }

  T& Back() {
    return array_[size_ - 1];
  }

  const T& Back() const {
    return array_[size_ - 1];
  }

  T* Data() {
    return array_;
  }

  const T* Data() const {
    return array_;
  }

  void Swap(Vector& other) {
    std::swap(array_, other.array_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  void Resize(size_t new_size) {
    if (new_size <= 0) {
      delete[] array_;
      array_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else if (new_size <= size_) {
      size_ = new_size;
    } else {
      if (new_size > capacity_) {
        Reserve(new_size);
      }
      for (size_t i = size_; i < new_size; i++) {
        array_[i] = T();
      }
      size_ = new_size;
    }
  }

  void Resize(size_t new_size, T value) {
    if (new_size <= 0) {
      delete[] array_;
      array_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      return;
    }
    if (new_size <= size_) {
      size_ = new_size;
      return;
    }
    if (new_size > capacity_) {
      Reserve(new_size);
    }
    for (size_t i = size_; i < new_size; i++) {
      array_[i] = std::move(value);
    }
    size_ = new_size;
  }

  void Reserve(size_t new_cap) {
    if (new_cap == capacity_) {
      return;
    }

    if (new_cap == 0) {
      delete[] array_;
      array_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      return;
    }

    if (capacity_ == 0) {
      array_ = new T[new_cap];
      capacity_ = new_cap;
      return;
    }

    if (size_ == 0) {
      delete[] array_;
      array_ = new T[new_cap];
      capacity_ = new_cap;
    }
    if (new_cap < capacity_) {
      return;
    }
    auto tmp = new T[new_cap];
    for (size_t i = 0; i < size_; i++) {
      tmp[i] = std::move(array_[i]);
    }
    delete[] array_;
    array_ = tmp;
    capacity_ = new_cap;
  }

  void ShrinkToFit() {
    if (size_ == 0) {
      capacity_ = 0;
      delete[] array_;
      array_ = nullptr;
    }
    if (size_ == capacity_) {
      return;
    }
    auto tmp = new T[size_];
    for (size_t i = 0; i < size_; i++) {
      tmp[i] = std::move(array_[i]);
    }
    delete[] array_;
    array_ = tmp;
    capacity_ = size_;
  }

  void Clear() {
    size_ = 0;
  }

  void PushBack(const T& el1) {
    auto tmp_old = array_[size_ - 1];
    try {
      array_[size_ - 1] = el1;
    } catch (...) {
      throw;
    }
    array_[size_ - 1] = tmp_old;

    if (capacity_ == 0) {
      Reserve(1);
    }
    if (size_ == capacity_) {
      Reserve(capacity_ * 2);
    }
    try {
      array_[size_] = el1;
      size_++;
    } catch (...) {
      throw;
    }
  }

  void PushBack(T&& el2) {
    if (capacity_ == 0) {
      Reserve(1);
    }

    if (size_ == capacity_) {
      Reserve(capacity_ * 2);
    }

    array_[size_] = std::move(el2);
    size_++;
  }

  bool PopBack() {
    if (Empty()) {
      return false;
    }
    size_--;
    return true;
  }

  bool operator==(const Vector& other) const {
    if (size_ != other.size_) {
      return false;
    }
    return std::equal(array_, array_ + size_, other.array_);
  }

  bool operator!=(const Vector& other) const {
    return !(*this == other);
  }

  bool operator<(const Vector& other) const {
    return std::lexicographical_compare(array_, array_ + size_, other.Data(), other.Data() + other.Size());
  }

  bool operator>(const Vector& other) const {
    return other < *this;
  }

  bool operator<=(const Vector& other) const {
    return !(*this > other);
  }

  bool operator>=(const Vector& other) const {
    return !(*this < other);
  }

  T* begin() noexcept {
    return array_;
  }

  const T* begin() const noexcept {
    return array_;
  }

  const T* cbegin() const noexcept {
    return array_;
  }

  T* end() noexcept {
    return array_ + size_;
  }

  const T* end() const noexcept {
    return array_ + size_;
  }

  const T* cend() const noexcept {
    return array_ + size_;
  }

  std::reverse_iterator<T*> rbegin() noexcept {
    return std::reverse_iterator<T*>(end());
  }

  std::reverse_iterator<const T*> rbegin() const noexcept {
    return std::reverse_iterator<const T*>(end());
  }

  std::reverse_iterator<const T*> crbegin() const noexcept {
    return std::reverse_iterator<const T*>(end());
  }

  std::reverse_iterator<T*> rend() noexcept {
    return std::reverse_iterator<T*>(begin());
  }

  std::reverse_iterator<const T*> rend() const noexcept {
    return std::reverse_iterator<const T*>(begin());
  }

  std::reverse_iterator<const T*> crend() const noexcept {
    return std::reverse_iterator<const T*>(begin());
  }

 private:
  T* array_;
  size_t size_;
  size_t capacity_;
};
#endif