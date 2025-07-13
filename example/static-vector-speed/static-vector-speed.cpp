#include <cstdlib>
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>
#include "../../StaticVector.hpp"

class Duration{
    typedef std::chrono::time_point<std::chrono::system_clock> time_point_type;
  public:

    void start(){
      _start = std::chrono::system_clock::now();
    }

    void end(){
      _end = std::chrono::system_clock::now();
    }

    unsigned long long duration(){
      return std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();
    }

  private:
    time_point_type _start;
    time_point_type _end;
};


std::string rpad(const std::string& a_source, size_t a_size) {
  std::string result(a_source);
  while(result.length() < a_size){
    result += ' ';
  }
  return result;
}


template<typename TContainer>
Duration testPushBack(size_t a_itSize, size_t a_vecSize){
  Duration duration;
  duration.start();
  {
    for(size_t i = 0; i < a_itSize; ++i){
      TContainer v;
      for(int j = 0; j < a_vecSize; ++j){
        v.push_back(j);
      }
    }
  }
  duration.end();
  return duration;
}


template<typename TContainer>
Duration testResize(size_t a_itSize, size_t a_vecSize){
  Duration duration;
  duration.start();
  {
    for(size_t i = 0; i < a_itSize; ++i){
      TContainer v(a_vecSize);

      for(int j = 0; j < a_vecSize; ++j){
        v[j] = j;
      }

    }
  }
  duration.end();
  return duration;
}

#define StaticSize 64

int main(int a_argc, char* a_argv[]){
  size_t itSize = 1024;

  typedef fcf::StaticVector<int, StaticSize, 2, 2, 2, 0> static_vetor_type;
  std::vector<size_t> columns = { 30, 20, 20, 30 };
  std::vector<int> testSizes =  { 1, 4, 8, 16, 64, 256, 1024, 32*1024, 128*1024 };


  std::cout << "Speed compare StaticVector with std::vector..." << std::endl;
  std::cout << "" << std::endl;
  std::cout << "Number of iterations: " << itSize << std::endl;
  std::cout << "StaticSize parameter: " << StaticSize << std::endl;

  std::cout << "" << std::endl;
  std::cout << rpad("", columns[0])
            << rpad("StaticVector usec", columns[1])
            << rpad("std::vector usec", columns[2])
            << rpad("(std::vector / StaticVector)", columns[3])
            << std::endl;
            ;
  std::cout << "" << std::endl;

  {
    for (int vecSize : testSizes) {
      unsigned long long n2 = testPushBack< std::vector<int> >(itSize, vecSize).duration();
      unsigned long long n1 = testPushBack< static_vetor_type >(itSize, vecSize).duration();

      std::cout << rpad((std::stringstream() << "Push back test (" << vecSize << " items):").str(), columns[0])
        << rpad((std::stringstream() << n1).str(), columns[1])
        << rpad((std::stringstream() << n2).str(), columns[2])
        << rpad((std::stringstream() << ((double)n2 / (double)n1)).str(), columns[3])
        << std::endl;
    }
  }

  {
    for (int vecSize : testSizes) {
      unsigned long long n1 = testResize< static_vetor_type >(itSize, vecSize).duration();
      unsigned long long n2 = testResize< std::vector<int> >(itSize, vecSize).duration();
      std::cout << rpad((std::stringstream() << "Initialize test(" << vecSize << " items):").str(), columns[0])
        << rpad((std::stringstream() << n1).str(), columns[1])
        << rpad((std::stringstream() << n2).str(), columns[2])
        << rpad((std::stringstream() << ((double)n2 / (double)n1)).str(), columns[3])
        << std::endl;
    }
  }

  return 0;
}
