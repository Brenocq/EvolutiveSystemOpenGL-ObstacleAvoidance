#include "environment.h"
#include <vector>

Environment::Environment(){
  for (int i = 0; i < 6; i++) {
    genes.push_back(0);
  }
}
