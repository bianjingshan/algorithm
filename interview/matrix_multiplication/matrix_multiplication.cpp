#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ************************* Complex mat mul ****************** */

typedef struct {
  float r;
  float i;
}ComplexNumber;

class ComplexMat {
 public:
  ComplexMat() {}
  ~ComplexMat() {}
  bool Create(uint32_t rows, uint32_t cols) {
    rows_ = rows;
    cols_ = cols;
    uint32_t mem_size = sizeof(ComplexNumber) * rows_ * cols_;
    p_data_ = (ComplexNumber *)malloc(mem_size);
    if (nullptr == p_data_) {
      return false;
    }
    memset(p_data_, 0, mem_size);
    return true;
  }
  ComplexNumber * Item(uint32_t row, uint32_t col) {
    if (nullptr == p_data_) {
      return nullptr;
    }
    if ((row >= rows_) || (col >= cols_)) {
      return nullptr;
    }
    return &p_data_[row * cols_ + col];
  }
  bool Set(uint32_t row, uint32_t col, float r, float i) {
    ComplexNumber *item = Item(row, col);
    if (nullptr == item) {
      return false;
    }
    item->r = r;
    item->i = i;
    return true;
  }
  uint32_t Rows() {
    return rows_;
  }
  uint32_t Cols() {
    return cols_;
  }

  void Print(const char *caption) {
    printf("\n%s:\n", caption);
    char item_buf[16];
    for (uint32_t i = 0; i < rows_; i++) {
      for (uint32_t j = 0; j < cols_; j++) {
        ComplexNumber *item = Item(i, j);
        snprintf(item_buf, sizeof(item_buf), "(%.1f+%.1fi)", item->r, item->i);
        printf("%-16s", item_buf);
      }
      printf("\n");
    }
  }

 private:
  ComplexNumber *p_data_;
  uint32_t rows_;
  uint32_t cols_;
};

void ComplexMul(ComplexNumber *a, ComplexNumber *b, ComplexNumber *output) {
  output->r = a->r * b->r - a->i * b->i;
  output->i = a->r * b->i + a->i * b->r;
}

void ComplexAdd(ComplexNumber *a, ComplexNumber *b, ComplexNumber *output) {
  output->r = a->r + b->r;
  output->i = a->i + b->i;
}

bool ComplexMatMul(ComplexMat &a, ComplexMat &b, ComplexMat &output) {
  if (a.Cols() != b.Rows()) {
    return false;
  }
  if (!output.Create(a.Rows(), b.Cols())) {
    return false;
  }
  ComplexNumber *dst_item;
  ComplexNumber temp_mul;
  for (uint32_t i = 0; i < output.Rows(); i++) {
    for (uint32_t j = 0; j < output.Cols(); j++) {
      dst_item = output.Item(i, j);
      for (uint32_t p = 0; p < a.Cols(); p++) {
        ComplexMul(a.Item(i, p), b.Item(p, j), &temp_mul);
        ComplexAdd(&temp_mul, dst_item, dst_item);
      }
    }
  }
  return true;
}

void ComplexMatMulTest() {
  printf("\n\n----------------- ComplexMatMulTest ----------------\n");
  ComplexMat a, b, c;
  a.Create(2, 3);
  a.Set(0, 0, 0.0, 0.0);
  a.Set(0, 1, 0.0, 1.0);
  a.Set(0, 2, 0.0, 2.0);
  a.Set(1, 0, 1.0, 0.0);
  a.Set(1, 1, 1.0, 1.0);
  a.Set(1, 2, 1.0, 2.0);
  b.Create(3, 1);
  b.Set(0, 0, 0.0, 5.0);
  b.Set(1, 0, 1.0, 5.0);
  b.Set(2, 0, 2.0, 5.0);
  a.Print("A");
  b.Print("B");

  ComplexMatMul(a, b, c);
  c.Print("AB");
  printf("\n\n");
}


/* ************************* triple mat mul ****************** */


typedef struct {
  float a[3];
}Triple;

class TripleMat {
 public:
  TripleMat() {}
  ~TripleMat() {}
  bool Create(uint32_t rows, uint32_t cols) {
    rows_ = rows;
    cols_ = cols;
    uint32_t mem_size = sizeof(Triple) * rows_ * cols_;
    p_data_ = (Triple *)malloc(mem_size);
    if (nullptr == p_data_) {
      return false;
    }
    memset(p_data_, 0, mem_size);
    return true;
  }
  Triple * Item(uint32_t row, uint32_t col) {
    if (nullptr == p_data_) {
      return nullptr;
    }
    if ((row >= rows_) || (col >= cols_)) {
      return nullptr;
    }
    return &p_data_[row * cols_ + col];
  }
  bool Set(uint32_t row, uint32_t col, float a0, float a1, float a2) {
    Triple *item = Item(row, col);
    if (nullptr == item) {
      return false;
    }
    item->a[0] = a0;
    item->a[1] = a1;
    item->a[2] = a2;
    return true;
  }
  uint32_t Rows() {
    return rows_;
  }
  uint32_t Cols() {
    return cols_;
  }

  void Print(const char *caption) {
    printf("\n%s:\n", caption);
    char item_buf[16];
    for (uint32_t i = 0; i < rows_; i++) {
      for (uint32_t j = 0; j < cols_; j++) {
        Triple *item = Item(i, j);
        snprintf(item_buf, sizeof(item_buf), "(%.1f,%.1f,%.1f)", item->a[0], item->a[1], item->a[2]);
        printf("%-16s", item_buf);
      }
      printf("\n");
    }
  }

 private:
  Triple *p_data_;
  uint32_t rows_;
  uint32_t cols_;
};

void TripleMul(Triple *a, Triple *b, Triple *output) {
  for (uint32_t i = 0; i < 3; i++) {
    output->a[i] = a->a[i] * b->a[i];
  }
}

void TripleAdd(Triple *a, Triple *b, Triple *output) {
  for (uint32_t i = 0; i < 3; i++) {
    output->a[i] = a->a[i] + b->a[i];
  }
}

bool TripleMatMul(TripleMat &a, TripleMat &b, TripleMat &output) {
  if (a.Cols() != b.Rows()) {
    return false;
  }
  if (!output.Create(a.Rows(), b.Cols())) {
    return false;
  }
  Triple *dst_item;
  Triple temp_mul;
  for (uint32_t i = 0; i < output.Rows(); i++) {
    for (uint32_t j = 0; j < output.Cols(); j++) {
      dst_item = output.Item(i, j);
      for (uint32_t p = 0; p < a.Cols(); p++) {
        TripleMul(a.Item(i, p), b.Item(p, j), &temp_mul);
        TripleAdd(&temp_mul, dst_item, dst_item);
      }
    }
  }
  return true;
}

void TripleMatMulTest() {
  printf("\n\n----------------- TripleMatMulTest ----------------\n");
  TripleMat a, b, c;
  a.Create(2, 3);
  a.Set(0, 0, 0.0, 0.0, 2.0);
  a.Set(0, 1, 0.0, 1.0, 3.0);
  a.Set(0, 2, 0.0, 2.0, 4.0);
  a.Set(1, 0, 1.0, 0.0, 5.0);
  a.Set(1, 1, 1.0, 1.0, 6.0);
  a.Set(1, 2, 1.0, 2.0, 7.0);
  b.Create(3, 1);
  b.Set(0, 0, 0.0, 5.0, 8.0);
  b.Set(1, 0, 1.0, 5.0, 9.0);
  b.Set(2, 0, 2.0, 5.0, 10.0);
  a.Print("A");
  b.Print("B");

  TripleMatMul(a, b, c);
  c.Print("AB");
  printf("\n\n");
}


int main() {
  ComplexMatMulTest();
  TripleMatMulTest();
  return 0;
}
