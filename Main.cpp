/*
 * Main.cpp
 *
 *  Created on: Jun 8, 2014
 *      Author: sam
 */


#include "Framework.h"
#if !defined(EMBEDDED_MODE)
//
#include <iostream>
#include <csignal>

void signalHandler(int signum)
{
  std::cout << "Interrupt signal (" << signum << ") received" << std::endl;
  // cleanup and close up stuff here
  // terminate program
  //Communication::getInstance().closeInstance();
  Controller::deleteInstance();
  exit(signum);
}

int main(int argc, char** argv)
{
  std::cout << "*** start ***" << std::endl;
  // register signal SIGINT and signal handler
  signal(SIGINT, signalHandler);
  Controller& controller = Controller::getInstance();
  controller.activateThreads();
  controller.computeGraph();
  controller.sort();
  controller.stream();
  controller.mainLoop();
  Controller::deleteInstance();
  std::cout << "*** end  ***" << std::endl;
  return 0;
}
#endif
