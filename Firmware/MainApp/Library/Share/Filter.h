#ifndef _FILTER_H_
#define _FILTER_H_

#include <cstdint>

// typedef struct FILTER_Light16 {
//   uint16_t    ALGN(A2) data[FILTER_L16_MAX_LEN];  /// ����� ������ �������� �������
//   uint16_t    ALGN(A2) summ;                      /// ����� ���� �������� (������� ����� ������), ��� ��������� ��������
//   uint16_t    ALGN(A2) avg;             /// ������� �������� �� �������
//   uint8_t     ALGN(A1) index;           /// ������ ��������, ��� ������ ���������� ��������
//   uint8_t     ALGN(A1) length;          /// ����� �������
//   uint8_t     ALGN(A1) count;           /// ����������� ����� �������� � �������
// } FilterMovAvgL16_t;

#define MAX_LEN_FILT (16U)

template<typename T>
class FilterMVG {
  // public:
    T data[MAX_LEN_FILT];
    T summ;
    T avg;
    uint8_t index;
    uint8_t length;
    uint8_t count;
  public:
    FilterMVG(uint8_t _max_len);
    void AddValue(T value);
    T GetAVG(void) ;
};

template<typename T>
FilterMVG<T>::FilterMVG(uint8_t _max_len) {
  this->length = _max_len;
}

template<typename T>
void FilterMVG<T>::AddValue(T value) { 
  this->summ += value;
  this->summ -= this->data[this->index];
  this->data[this->index] = value;
  this->index++;
  if (this->index >= this->length)
    this->index = 0;
  if(this->count < this->length)
    this->count++;
  this->avg = this->summ / this->count;
}

template<typename T>
T FilterMVG<T>::GetAVG(void) { 
  return this->avg;
}

#endif