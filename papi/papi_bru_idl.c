/* This code runs some sanity checks on the PAPI_BRU_IDL */
/*   (Branch Unit Idle) event.                           */

/* by Vince Weaver, vweaver1 _at_ eecs.utk.edu           */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "papi.h"

#include "test_utils.h"

int main(int argc, char **argv) {

   int events[1];
   long long counts[1];
   
   int retval,quiet;

   char test_string[]="Testing PAPI_BRU_IDL predefined event...";
   
   quiet=test_quiet();
   
   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT) {
      if (!quiet) printf("Error! PAPI_library_init %d\n",retval);
      test_fail(test_string);
   }

   retval = PAPI_query_event(PAPI_BRU_IDL);
   if (retval != PAPI_OK) {
      if (!quiet) printf("PAPI_BRU_IDL not available\n");
      test_skip(test_string);
   }

   events[0]=PAPI_BRU_IDL;

   PAPI_start_counters(events,1);
     
   PAPI_stop_counters(counts,1);

   if (counts[0]<1) {
      if (!quiet) printf("Error! Count too low\n");
      test_fail(test_string);
   }

   PAPI_shutdown();

   test_unimplemented(test_string);
   
   return 0;
}
