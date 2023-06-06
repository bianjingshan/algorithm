#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


void TestLevel2() {
  printf(".");
  sleep(1);
}

void TestLevel1() {
  TestLevel2();
}

