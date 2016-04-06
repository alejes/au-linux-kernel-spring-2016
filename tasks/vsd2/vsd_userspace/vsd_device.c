#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>





#include "vsd_device.h"

int vsd_init(){
    if( ( vsd_dfd = open( VSD, O_RDWR ) ) < 0 ){
      perror( "Failed open device");
      return VSD_FAILURE;
    }
    return VSD_SUCCESS;
}

int vsd_deinit(){
    CHECK_DEVICE_OPEN();
    close( vsd_dfd );
    return VSD_SUCCESS;
}

int vsd_get_size(size_t *out_size){
    CHECK_DEVICE_OPEN();
    vsd_ioctl_get_size_arg_t arg;

    if( ioctl( vsd_dfd, VSD_IOCTL_GET_SIZE, &arg ) ){
        perror("vsd device get size failed");
        return EXIT_FAILURE;
    }
    *out_size = arg.size;
    return EXIT_SUCCESS;
}

int vsd_set_size(size_t size){
  CHECK_DEVICE_OPEN();
  vsd_ioctl_set_size_arg_t arg;
  arg.size = size;

  if( ioctl( vsd_dfd, VSD_IOCTL_SET_SIZE, &arg ) ){
      perror("vsd device set size failed");
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}



ssize_t vsd_read(char* dst, off_t offset, size_t size){
    int res;
    CHECK_DEVICE_OPEN();
    res = lseek(vsd_dfd, offset, SEEK_SET);
    if (res < 0){
      perror("vsd device lseek failed");
      close( vsd_dfd );
      return EXIT_FAILURE;
    }

    res = read(vsd_dfd, dst, size);
    if(res < 0){
        perror("vsd device read failed");
        close( vsd_dfd );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

ssize_t vsd_write(const char* src, off_t offset, size_t size){
  int res;
  CHECK_DEVICE_OPEN();
  res = lseek(vsd_dfd, offset, SEEK_SET);
  if (res < 0){
    perror("vsd device lseek failed");
    close( vsd_dfd );
    return EXIT_FAILURE;
  }

  res = write(vsd_dfd, src, size);
  if(res < 0){
      perror("vsd device write failed");
      close( vsd_dfd );
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void* vsd_mmap(size_t offset){
  size_t remainder;
  if (vsd_dfd < 0){
      perror( "Device was not opened");
      return MAP_FAILED;
  }

  int pagesize = getpagesize();

  if ((unsigned long )offset % pagesize != 0){
      perror("page not alligned");
      return MAP_FAILED;
  }

  if (vsd_get_size(&remainder) < 0){
      return MAP_FAILED;
  }
  remainder -= offset;

  return mmap(NULL, remainder, PROT_READ | PROT_WRITE, MAP_SHARED, vsd_dfd, offset);
}

int vsd_munmap(void* addr, size_t offset){
    size_t remainder;
    CHECK_DEVICE_OPEN();

    int pagesize = getpagesize();

    if (((unsigned long)addr % pagesize != 0) || ((unsigned long)offset % pagesize != 0)){
        perror("page not alligned");
        return EXIT_FAILURE;
    }

    if (vsd_get_size(&remainder) < 0){
        return EXIT_FAILURE;
    }

    remainder -= offset;
    return munmap(addr, remainder);
}
