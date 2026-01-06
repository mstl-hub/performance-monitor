#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <cstdint>
#include "gpio.h"

class Error {
  private:


  public:
    enum SrcErr : uint8_t {
      i2c,
      spi,
      adc,
      logic,
    };

  public:
  Error();
  void SetError(SrcErr err);
};

#endif //_ERRORS_H_