#include <stdio.h>
#include <omp.h>

void main(void) {
  
  int x = 0;
  omp_lock_t writelock;

  omp_init_lock(&writelock);
  
  #pragma omp parallel num_threads(10000)
  {
    omp_set_lock(&writelock);
    x++;
    omp_unset_lock(&writelock);
  }

  omp_destroy_lock(&writelock);

  printf("final val: %d\n", x);

}
