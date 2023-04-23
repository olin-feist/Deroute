#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

class Vector {
 protected:
  std::vector<float> data_;

 public:
  explicit Vector(int64_t);
  Vector(const Vector&) = default;
  Vector(Vector&&) noexcept = default;
  Vector& operator=(const Vector&) = default;
  Vector& operator=(Vector&&) = default;

  inline float* data() {
    return data_.data();
  }
  inline const float* data() const {
    return data_.data();
  }
  inline float& operator[](int64_t i) {
    return data_[i];
  }
  inline const float& operator[](int64_t i) const {
    return data_[i];
  }

  inline int64_t size() const {
    return data_.size();
  }
  void zero();
  void mul(float);
  float norm() const;
  void addVector(const Vector& source);
  void addVector(const Vector&, float);

};
