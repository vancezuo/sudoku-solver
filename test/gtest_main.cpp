/*
 * gtest_main.cpp
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#include "gmock/gmock.h"
#include <stdio.h>

int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cpp\n");
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

