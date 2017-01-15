#include <iostream>
#include "induco.h"
#include "water.h"
#include <appareo.h>
int main() {
  appareo::InitializeAppareo();
  water::MainLoop();
  appareo::TerminateAppareo();
}
