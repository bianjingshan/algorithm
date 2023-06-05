#include "file_lock.h"
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>



#define DATA_SIZE (1 << 20)
static char wbuffer[DATA_SIZE];
static char rbuffer[DATA_SIZE];
static uint32_t size = DATA_SIZE;

void WriteFileFlock(int n) {
  const char *pathname = "./tmp";
  int fd = open(pathname, O_RDWR | O_CREAT, 0777);
  if (-1 == fd) {
    printf("Error: open(%s) failed\n", pathname);
    return;
  }

  int ret;

  ret = flock(fd, LOCK_EX);

  ftruncate(fd, 0);

  unsigned seed = time(0);
  srand(seed);
  for (int i = 0; i < size; i++) {
    wbuffer[i] = (char)rand();
    write(fd, &wbuffer[i], 1);
  }

  memset(rbuffer, 255, size);
  lseek(fd, 0, SEEK_SET);
  int read_size = read(fd, rbuffer, size);
  for (int i = 0; i < size; i++) {
    if (rbuffer[i] != wbuffer[i]) {
      printf("thread-%d verify data error, r[%d]: %d, w: %d\n", n, i, rbuffer[i], wbuffer[i]);
      return;
    }
  }
  printf("thread-%d verify data ok, data_size: %u\n", n, read_size);
  close(fd);
}

void FileLockTest() {
  #define THREAD_NUM 16
  std::thread *th[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++) {
      th[i] = new std::thread(WriteFileFlock, i);
  }

  for (int i = 0; i < THREAD_NUM; i++) {
      th[i]->join();
  }
}
