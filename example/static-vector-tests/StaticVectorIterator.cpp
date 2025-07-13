#include <iostream>
#include <fcfTest/test.hpp>
#include "../../StaticVector.hpp"
void staticVectorIteratorTest(){
  std::cout << "Start staticVectorIteratorTest()..." << std::endl;

  {
    typedef fcf::StaticVector<int, 2, 0, 2, 2, 0, 0> static_vector_type;
    static_vector_type v = {1,2,3};
    size_t key = 0;
    for(static_vector_type::iterator it = v.begin(); it != v.end(); ++it){
      FCF_TEST(*it == (key+1), *it, (key+1));
      ++key;
    }
  }
  {
    typedef fcf::StaticVector<int, 2, 0, 2, 2, 0, 0> static_vector_type;
    static_vector_type v = {1,2,3};
    size_t key = 0;
    for(int itm : v){
      FCF_TEST(itm == (key+1), itm, (key+1));
      ++key;
    }
  }
  {
    typedef fcf::StaticVector<int, 2, 0, 2, 2, 0, 0> static_vector_type;
    static_vector_type v = {1,2,3};
    size_t key = 0;
    for(static_vector_type::const_iterator it = v.cbegin(); it != v.cend(); ++it){
      FCF_TEST(*it == (key+1), *it, (key+1));
      FCF_TEST(*(it.operator->()) == (key+1), *it, (key+1));
      ++key;
    }
  }

}
