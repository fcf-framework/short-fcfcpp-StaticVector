namespace FcfTest {
  namespace BasisTest {

    void staticVectorTest();
    void staticVectorCopyTest();
    void staticVectorExceptionTest();
    void staticVectorIteratorTest();
    void staticVectorNotReduce();
    void staticVectorPushTest();

  } // BasisTest namespace
} // FcfTest namespace

int main(int a_argc, char* a_argv[]){
  FcfTest::BasisTest::staticVectorTest();
  FcfTest::BasisTest::staticVectorCopyTest();
  FcfTest::BasisTest::staticVectorExceptionTest();
  FcfTest::BasisTest::staticVectorIteratorTest();
  FcfTest::BasisTest::staticVectorNotReduce();
  FcfTest::BasisTest::staticVectorPushTest();
  return 0;
}
