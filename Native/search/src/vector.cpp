#include "vector.h"

#include <assert.h>

#include <cmath>
#include <iomanip>

Vector::Vector(int64_t m) : data_(m) {}

void Vector::zero() {
  std::fill(data_.begin(), data_.end(), 0.0);
}

float Vector::norm() const {
  float sum = 0;
  for (int64_t i = 0; i < size(); i++) {
    sum += data_[i] * data_[i];
  }
  return std::sqrt(sum);
}

void Vector::mul(float a) {
  for (int64_t i = 0; i < size(); i++) {
    data_[i] *= a;
  }
}

void Vector::addVector(const Vector& source) {
  assert(size() == source.size());
  for (int64_t i = 0; i < size(); i++) {
    data_[i] += source.data_[i];
  }
}

void Vector::addVector(const Vector& source, float s) {
  assert(size() == source.size());
  for (int64_t i = 0; i < size(); i++) {
    data_[i] += s * source.data_[i];
  }
}



