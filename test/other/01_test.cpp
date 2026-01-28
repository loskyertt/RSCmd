#include <iostream>

/* 测试 1 */
void test_01() {
  int a = 10;
  std::cout << "a = " << a << ", type: " << typeid(a).name() << "\n";

  double b = static_cast<double>(a);  // int → double
  std::cout << "b = " << b << ", type: " << typeid(b).name() << "\n";
}

int main() {
  test_01();
}