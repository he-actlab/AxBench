/*
 * Timer.h
 *
 *  Created on: Aug 17, 2012
 *      Author: hadianeh
 */

#ifndef TIMER_H_
#define TIMER_H_

/** rdtsc: The time stamp timer */
#if 0 //defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
  unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
}
#else //defined(__x86_64__)
static __inline__ unsigned long long rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32);
}
#endif

#define RESET_TIMER(t) t.t0 = t.t1 = t.total = 0
#define START_TIMER(t) t.t0 = rdtsc()
#define STOP_TIMER(t)  t.t1 = rdtsc(); t.total += t.t1 - t.t0

class Timer {
public:
	  unsigned long long int t0;
	  unsigned long long int t1;
	  unsigned long long int total;

public:

	Timer() {
		RESET_TIMER((*this));
	}

	double getTotalTime() {
		return (double)total / 1000000.;
	}

	double getLastTime() {
		return (double)(t1 - t0) / 1000000.;
	}
};

#endif /* TIMER_H_ */
