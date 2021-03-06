/* inherit.c */
/* simple test of the inherit flag              */

/* by Vince Weaver   vincent.weaver@maine.edu   */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <unistd.h>
#include <asm/unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "perf_event.h"
#include "perf_helpers.h"
#include "test_utils.h"

int fd;


void *thread_work(void *blah) {

	return NULL;
}


char test_string[]="Testing inherit...";

int main(int argc, char** argv) {

	int j;
	int quiet;
	pthread_t our_thread[8];
	int read_result;
	long long inherit_count,count;

	struct perf_event_attr pe;

	quiet=test_quiet();

	if (!quiet) {
		printf("This test checks inherit functionality.\n");
		printf("Starting and stopping 8 threads\n");
	}

	/*************************/
	/* With inherit set      */
	/*************************/

	memset(&pe,0,sizeof(struct perf_event_attr));

	pe.type=PERF_TYPE_HARDWARE;
	pe.config=PERF_COUNT_HW_INSTRUCTIONS;
	pe.disabled=1;
	pe.inherit=1;
	pe.exclude_kernel=1;
	pe.exclude_hv=1;

	arch_adjust_domain(&pe,quiet);

	fd=perf_event_open(&pe,0,-1,-1,0);
	if (fd<0) {
		fprintf(stderr,"Error opening first: %s\n",strerror(errno));
		test_fail(test_string);
	}

	ioctl(fd, PERF_EVENT_IOC_RESET, 0);
	ioctl(fd, PERF_EVENT_IOC_ENABLE,0);


	for(j=0;j<8;j++) {
		pthread_create(&our_thread[j],NULL,thread_work,0);
	}

	for(j=0;j<8;j++) {
		pthread_join(our_thread[j],NULL);
	}

	ioctl(fd, PERF_EVENT_IOC_DISABLE,0);

	read_result=read(fd,&inherit_count,sizeof(long long));

	if (read_result!=sizeof(long long)) {
		fprintf(stderr,"\tImproper return from read: %d\n",read_result);
		test_fail(test_string);
	}

	close(fd);

	/****************************/
	/* Without inherit set      */
	/****************************/

	memset(&pe,0,sizeof(struct perf_event_attr));

	pe.type=PERF_TYPE_HARDWARE;
	pe.config=PERF_COUNT_HW_INSTRUCTIONS;
	pe.disabled=1;
	pe.inherit=0;
	pe.exclude_kernel=1;
	pe.exclude_hv=1;

	arch_adjust_domain(&pe,quiet);

	fd=perf_event_open(&pe,0,-1,-1,0);
	if (fd<0) {
		fprintf(stderr,"Error opening second: %s\n",
			strerror(errno));
		test_fail(test_string);
	}

	ioctl(fd, PERF_EVENT_IOC_RESET, 0);
	ioctl(fd, PERF_EVENT_IOC_ENABLE,0);

	for(j=0;j<8;j++) {
		pthread_create(&our_thread[j],NULL,thread_work,0);
	}

	for(j=0;j<8;j++) {
		pthread_join(our_thread[j],NULL);
	}

	ioctl(fd, PERF_EVENT_IOC_DISABLE,0);

	read_result=read(fd,&count,sizeof(long long));

	if (read_result!=sizeof(long long)) {
		fprintf(stderr,"\tImproper return from read: %d\n",read_result);
		test_fail(test_string);
	}

	close(fd);

	if (!quiet) {

		printf("\tFound %lld instructions with inherit enabled\n",
			inherit_count);
		printf("\tFound %lld instructions with inherit disabled\n",
			count);
	}

	if ((inherit_count < 6*count) || (inherit_count > 9*count)) {
		fprintf(stderr,"\tInherit count unexpected.\n");
		test_fail(test_string);
	}

	test_pass(test_string);

	return 0;
}
