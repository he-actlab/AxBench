/*
 * CpuAffinity.cpp
 *
 *  Created on: Aug 20, 2012
 *      Author: hadianeh
 */

#include "CpuAffinity.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <sched.h>

void setCpuAffinity(int cpu) {
	int err;
	cpu_set_t cpumask;

	CPU_ZERO(&cpumask);
	CPU_SET(cpu, &cpumask);
	err = sched_setaffinity(syscall(SYS_gettid), sizeof(cpu_set_t), &cpumask);

	if (err == -1) {
		printf("Error: Oops! Cannot set the affinity for CPU %d!\n", cpu);
		exit(1);
	}
}

bool testCpuAffinity(int cpu) {
	int err;
	cpu_set_t cpumask;

	CPU_ZERO(&cpumask);
	err = sched_getaffinity(syscall(SYS_gettid), sizeof(cpu_set_t), &cpumask);

	if (err == -1) {
		printf("Error: Oops! Cannot get the affinity!\n");
		exit(1);

		return false;
	}

	return CPU_ISSET(cpu, &cpumask) && (CPU_COUNT(&cpumask) == 1);
}
