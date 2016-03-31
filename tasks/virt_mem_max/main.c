#include <stdio.h>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

///char * mb = ["B", "KB", "MB", "TB"];


int main(){
  size_t mx = std::numeric_limits<size_t>::max();
  size_t total_mem = 0;
  size_t cnt = 0;
  while(mx > 0){
      ++cnt;
      size_t all = mx >> 1;
      if ((char*)mmap(NULL, all, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0) != (char*)-1){
          total_mem += all;
      }
      else{
        mx -= all;
      }
      if (all <= 0)break;
  }

  printf("%lu\n", total_mem);
  printf("%lu\n", cnt);

  return 0;
}
