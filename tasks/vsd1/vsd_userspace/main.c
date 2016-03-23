#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include "../vsd_driver/vsd_ioctl.h"

#define VSD "/dev/vsd"
//typedef short bool;
//#define true 1
//#define false 0

int printf_size(){
  int dfd;
  if( ( dfd = open( VSD, O_RDONLY ) ) < 0 ){
      fprintf(stderr, "Failed open device %s\n", VSD);
      return EXIT_FAILURE;
    }
   vsd_ioctl_get_size_arg_t arg;
   if( ioctl( dfd, VSD_IOCTL_GET_SIZE, &arg ) ){
     fprintf(stderr, "IOCTL get size failed\n");
     close( dfd );
     return EXIT_FAILURE;
   }
   fprintf( stdout, "IOCTL size = %zu\n", arg.size);
   close( dfd );
   return EXIT_SUCCESS;
}

int set_size(const int size){
  int dfd;
  if( ( dfd = open( VSD, O_WRONLY ) ) < 0 ){
      fprintf(stderr, "Failed open device %s\n", VSD);
      return EXIT_FAILURE;
    }
   vsd_ioctl_get_size_arg_t arg;
   arg.size = size;
   if( ioctl( dfd, VSD_IOCTL_SET_SIZE, &arg ) ){
     fprintf(stderr, "IOCTL set size failed\n");
     close( dfd );
     return EXIT_FAILURE;
   }
   fprintf( stdout, "IOCTL set size = %zu\n", arg.size);
   close( dfd );
   return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  int exit = EXIT_SUCCESS;
  switch(argc){
    case 1:
    break;
    case 2:
      if (strcmp(argv[1], "size_get")){
        exit = EXIT_FAILURE;
        printf("Error: Unknown argument: %s\n", argv[1]);
      }
      return printf_size();
    break;
    case 3:
      if (strcmp(argv[1], "size_set")){
        exit = EXIT_FAILURE;
        printf("Error: Unknown argument: %s\n", argv[1]);
        break;
      }
      int set = atoi(argv[2]);
      return set_size(set);
    break;
    default:
    printf("Error: Not supported count of arguments\n");
    exit = EXIT_FAILURE;
  }

  printf("Help\n");
  printf("Run $ vsd_userspace size_get\n");
  printf("for execute VSD_IOCTL_GET_SIZE and print result to stdout.\n\n");
  printf("Run $ vsd_userspace size_set SIZE_IN_BYTES\n");
  printf("for execute VSD_IOCTL_SET_SIZE with argument SIZE_IN_BYTES.\n");

    return exit;
}
