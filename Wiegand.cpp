/*
  Wiegand.cpp - Library for receiving data via Wiegand interface.
  Author: backjoob
  6 September 2018
*/
#include "Wiegand.h"

namespace Wiegand{
  static inline unsigned long handle_frame_break(void);
  static void d0_handler(void);
  static void d1_handler(void);
  static void both_handler(bool bit);
  
  const int BUF_SIZE = 32;
  static volatile unsigned int buf[BUF_SIZE];
  static volatile int head = 0;
  static volatile int tail = 0;
  
  static volatile unsigned int bit_val = 0;
  static volatile int bit_count = 0;
  static volatile unsigned long last_interrupt = 0;
  const int FRAME_BREAK_MILLIS = 20;
  
/*
 * Buffer Functions
 */
  static inline void increment(volatile int &var, int sz){
    var = ((var + 1) % sz);
  }
  
  static inline bool buf_empty(void){
    return head == tail;
  }
  
  static inline bool buf_avail(void){
    return !buf_empty();
  }
  
  static inline bool buf_full(void){
    return ((tail + 1) % BUF_SIZE) == head;
  }
  
  static inline void enqueue(unsigned int val){
    buf[tail] = val;
    increment(tail, BUF_SIZE);
  }
  
  static inline void enqueue_and_overwrite(unsigned int val){
    if(buf_full()){
      increment(head, BUF_SIZE);
    }
    enqueue(val);
  }
  
  static inline void enque_no_overwrite(unsigned int val){
    if(buf_full()){
      return;
    }
    enqueue(val);
  }

  // Also a Wiegand function
  long dequeue_buf(void){
    if(buf_empty()){
      return -1L;
    }
    noInterrupts();
    handle_frame_break();
    int ret = buf[head];
    increment(head, BUF_SIZE);
    interrupts();
    return ret;
  }
  
/*
 *  Wiegand Functions
 */
  
  void initialize(int d0, int d1){
    attachInterrupt(digitalPinToInterrupt(d0), d0_handler, FALLING);
    attachInterrupt(digitalPinToInterrupt(d1), d1_handler, FALLING);
  }
  
  static void d0_handler(void){
    both_handler(0);
  }
  
  static void d1_handler(void){
    both_handler(1);
  }

  static inline void both_handler(bool bit){
    last_interrupt = handle_frame_break();
    bit_val = (bit_val << 1) | bit;
    bit_count += 1;
  }

  static inline unsigned long handle_frame_break(void){
    unsigned long curr_time = millis();
    unsigned long diff = curr_time - last_interrupt;
    if( diff >= FRAME_BREAK_MILLIS && bit_count > 0 ){
      enqueue_and_overwrite(bit_val);
      bit_val = 0U;
      bit_count = 0;
    }
    return curr_time;
  }
  
  bool avail(void){
    //TODO: do interrupts need to be disabled?
    noInterrupts();
    (void) handle_frame_break();
    interrupts();
    return buf_avail();
  }

}
