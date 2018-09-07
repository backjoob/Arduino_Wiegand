/*
  Wiegand.cpp - Library for receiving data via Wiegand interface.
  Author: backjoob
  6 September 2018
*/

#ifndef Wiegand_h
#define Wiegand_h

#include "Arduino.h"

namespace Wiegand{

  void initialize(int d0, int d1);
  bool avail(void);
  long dequeue_buf(void);
  
}

#endif
