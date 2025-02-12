#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200
/*
void filep(int fd) {

  char* buf = (char*)malloc(1000*sizeof(char));
  for(int i = get_callcount()-1; i >= 0; i--){
      memset(&buf[0], 0, sizeof(buf));
      dump(buf,i);
      if(*buf == 'T'){
        write(fd, buf, 1000);
        write(fd,"\n",2);
      }
    }

    for(int i = getN(); i > get_callcount()-1; i--){
        memset(&buf[0], 0, sizeof(buf));
        dump(buf,i);
        if(*buf == 'T'){
            write(fd, buf, 1000);
            write(fd,"\n",2);
        }
      }
    free(buf);
}

void write_on_open(int argc, char* argv[]) {
  int fd, i;
  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], 0x200 | 0X002)) < 0) {
      printf(1, "Command cannot open %s\n", argv[i]);
      //exit();
    }
    filep(fd);
  }
  exit();
}

int main(int argc, char *argv[]) {
  //int fd, i;

  if (argc <= 1) {
    filep(0);
    exit();
  }

  write_on_open(argc, argv);
*/

  /*
  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], 0x200 | 0X002)) < 0) {
      printf(1, "Command cannot open %s\n", argv[i]);
      //exit();
    }
    filep(fd);
  }
  */
  /*
  exit();
}
*/