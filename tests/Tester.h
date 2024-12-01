#ifndef TESTER_H
#define TESTER_H

#include <functional>
#include <stdio.h>
#include <string>
#include <vector>

class Test {
private:
  bool passed;
  std::string name;

public:
  Test(std::string name, bool passed) {
    this->name = name;
    this->passed = passed;
  }

  std::string getName() { return this->name; }
  bool getPassed() { return this->passed; }
};

class Tester {
private:
  std::vector<Test> tests;
  std::string name;

public:
  Tester(std::string name) {
    this->name = name;
    this->tests = std::vector<Test>(); 
  }

  void run() {
    int testsPassed = 0;
    printf("========= %s =========\n", this->name.c_str());
    for(int i = 0; i < (int) this->tests.size(); i++) 
      if (this->tests[i].getPassed()) {
        testsPassed++;
        printf("\033[32m [%d] - %s passed.\033[0m\n", i + 1, this->tests[i].getName().c_str());
      } else
      printf("\033[31m [%d] - %s not passed.\033[0m\n", i + 1, this->tests[i].getName().c_str());
    printf("====== Finished: %d/%d passed ======\n\n", testsPassed, (int) this->tests.size());
  }

  void add(bool test, std::string name) {
    this->tests.push_back(Test(name, test));
  }

};

#endif
