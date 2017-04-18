#ifndef DABTREE_MATRIX_H_
#define DABTREE_MATRIX_H_

#include <memory>
#include <vector>
#include <glog/logging.h>
#include "data.h"

class Matrix {
public:

  Matrix(std::vector<std::vector<Value>>& d,
         std::vector<FeaType>& t) :
    data_(d), fea_types_(t) {}

  Matrix(size_t width, size_t height) {
    data_ = std::vector<std::vector<Value>>(height, std::vector<Value>(width));
  }

  Matrix(size_t width, size_t height, FeaType type) {
    fea_types_ = {type};
    if (type == FeaType::CONT) {
        data_ = std::vector<std::vector<Value>>(height, std::vector<Value>(width, {.v=0.0}));
      } else if (type == FeaType::DISC) {
        data_ = std::vector<std::vector<Value>>(height, std::vector<Value>(width, {.cls=0}));
      } else if (type == FeaType::RANK) {
        data_ = std::vector<std::vector<Value>>(height, std::vector<Value>(width, {.level=0}));
      }
  }

  size_t GetWidth() {
    return data_[0].size();
  }

  size_t GetHeight() {
    return data_.size();
  }

  Value operator() (size_t row_id, size_t col_id) {
    return data_[row_id][col_id];
  }

  FeaType fea_type(size_t col_id) {
    return fea_types_[col_id];
  }

  void SetType(const std::vector<FeaType>& types) {
    fea_types_ = types;
  }

  void SetValue(size_t row_id, size_t col_id, const Value& value) {
    data_[row_id][col_id] = value;
  }

  void Copy(size_t row_id, const std::vector<Value>& values) {
    data_[row_id] = values;
  }

  void Copy(const std::vector<std::vector<Value>>& values) {
    data_ = values;
  }

  void Add(const std::vector<Value>& values) {
    data_.push_back(values);
  }

  void Sort(size_t col_id, size_t low, size_t high) {
    FeaType type = fea_type(col_id);
    if (type == FeaType::CONT) {
        std::sort(data_.begin() + low,
                  data_.begin() + high,
                  [](const Value& a, const Value& b) {
                      return a.v < b.v;
                  });
      } else if (type == FeaType::DISC) {
        std::sort(data_.begin() + low,
                  data_.begin() + high,
                  [](const Value& a, const Value& b) {
                      return a.cls < b.cls;
                  });
      } else if (type == FeaType::RANK) {
        std::sort(data_.begin() + low,
                  data_.begin() + high,
                  [](const Value& a, const Value& b) {
                      return a.level < b.level;
                  });
      }
  }

  float ColMean(size_t col_id, size_t low, size_t high) {
    FeaType type = fea_type(col_id);
    CHECK_EQ(type, FeaType::CONT);
    if (high <= low) return 0.0;
    float result = 0.0;
    for (size_t row_id = low; row_id < high; ++row_id)
      result += data_[row_id][col_id].v;
    return result / (high - low);
  }

  float SSE(size_t low, size_t high) {
    FeaType type = fea_type(0);
    CHECK_EQ(type, FeaType::CONT);
    if (high <= low) return 0.0;
    float mean = ColMean(0, low, high);
    float result = 0.0;
    for (size_t row_id = low; row_id < high; ++row_id) {
      float tmp = data_[row_id][0].v - mean;
      result += tmp*tmp;
    }
    return result;
  }


private:
  std::vector<std::vector<Value>> data_;
  std::vector<FeaType> fea_types_;
};

typedef std::shared_ptr<Matrix> MatrixPtr;
typedef std::shared_ptr<std::vector<float>> VectorPtr;


#endif  // DABTREE_MATRIX_H_

