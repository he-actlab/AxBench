/*
 * Copyright 2006 by the Massachusetts Institute of Technology.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

#ifndef __TIMER_H
#define __TIMER_H

#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <stdio.h>

// Amir: {
#include <sys/syscall.h>
#include <sched.h>

void reader_set_affinity(unsigned long cpu);
// }

// timer keeps track of the wall time clock

class timer {

  unsigned long long int t_start, t_end;

  // starting and ending times for complete run
  unsigned long long int start_run, end_run;
  double my_time;
  struct timeval tv1, tv2;
  struct timezone tz;
  char str[128];

 public:
  void start();
  void stop();
  char *get_str();
  void output(FILE *f);
};

#define GET_TIME(x) __asm__ volatile(".byte 0x0f,0x31":"=A"(x))
inline unsigned long long get_timebase(void) {
  unsigned long long retval;
  unsigned long junk;
//__asm__ __volatile__ ("\n\
//    1:      mftbu   %1\n\
//    mftb    %L0\n\
//    mftbu   %0\n\
//    cmpw    %0,%1\n\
//    bne     1b"
//    : "=r" (retval), "=r" (junk));
  return retval;
}
extern double total_time;

// proc_timer keeps track of the clock cycles consumed by a process
class proc_timer {

  // keeps track of start and stop for current segment
  unsigned long long int t_start, t_end;

  // starting and ending times for complete run
  unsigned long long int start_run, end_run;

  // total times accumulated by this procedure
  double my_time;

  // name for this timer
  char* name;

 public:
  proc_timer(char* _name);

  // inline to help avoid big overhead
  void start() { 
      GET_TIME(t_start);
      //t_start = get_timebase();
  }

  // inline to help avoid big overhead
  void stop() {
    GET_TIME(t_end);
    //t_end = get_timebase();
    
    // add to cumulative
    my_time += (double)(t_end - t_start) / (1000000);
    //my_time += (t_end - t_start);
    total_time += (double)(t_end - t_start) / (1000000);
  }

  void output(FILE *f);
};

#endif
