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

#include <timer.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// to keep track of the total elapsed time
double total_time = 0.0;

void timer::start() {
      GET_TIME(t_start);
 // gettimeofday(&tv1, &tz);
}

void timer::stop() {
    GET_TIME(t_end);
    //t_end = get_timebase();
    
    // add to cumulative
    //  printf ("2%d\n", t_end);
    //my_time += (double)(t_end - t_start) / (1000000);
    //total_time += (double)(t_end - t_start) / (1000000);
//  gettimeofday(&tv2, &tz);
}

char *timer::get_str() {
 // double elapsed = tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec - tv1.tv_usec) / 1000000.0;
    my_time += (double)(t_end - t_start) / (1000000);
  sprintf(str, "%s : %lf %lf", "Time", my_time, my_time);
  return str;
}

void timer::output(FILE *f) {
  fprintf(f, "%s\n", get_str());
}

proc_timer::proc_timer(char* _name) {
  name = _name;
  my_time = 0.0;
}

void proc_timer::output(FILE *f) {
  my_time = (double)my_time;
  fprintf(f, "*%s: %lf %lf\n",
	 name, my_time, total_time);
  my_time = my_time * 100.0 / total_time;

  fprintf(f, "%s: %.f%\n",
	 name, my_time);
}

// Amir: {
void reader_set_affinity(unsigned long cpu)
{
  int err;
  cpu_set_t cpumask;

  CPU_ZERO(&cpumask);
  CPU_SET(cpu, &cpumask);
  err = sched_setaffinity(syscall(SYS_gettid),
      sizeof(cpu_set_t), &cpumask);

  if (err == -1)
  {
    printf("Error in setting affinity\n");
    exit(1);
  }
}
// }
