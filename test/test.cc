#include <iostream>
// #include <memory>
#include <tuple>
// #include <unordered_map>
// #include <variant>
// #include <vector>

// namespace myspace {
// int my_func(int a) { return 3; }

// auto my_func_returning_lambda() {
//   using variant = std::variant<int, std::vector<int>>;
//   return []() {
//     variant x = 1;
//     return std::visit([](auto y) { return 1; }, x);
//   };
// }

// struct A {
//   int member;

//   static int func() { return 12; }

//   int func(int x) { return x; }
// };

// } // namespace myspace

#include <atomic>

#define REPEAT1(x) x, x
#define REPEAT2(x) REPEAT1(x), REPEAT1(x)
#define REPEAT3(x) REPEAT2(x), REPEAT2(x)
#define REPEAT4(x) REPEAT3(x), REPEAT3(x)
#define REPEAT5(x) REPEAT4(x), REPEAT4(x)
#define REPEAT6(x) REPEAT5(x), REPEAT5(x)
#define REPEAT7(x) REPEAT6(x), REPEAT6(x)
#define REPEAT8(x) REPEAT7(x), REPEAT7(x)

struct A {
  struct B {
    int a() const;
    int aa() const;
    int aaa() const;
    int aaaa() const;
    int aaaaa() const;
    int aaaaaa() const;
  };
  struct C {
    int a() const;
    int aa() const;
    int aaa() const;
    int aaaa() const;
    int aaaaa() const;
    int aaaaaa() const;
  };
};

// auto lambda_func(int x) {
//   return [](int x) {
//     return [](int x) {
//       return [](int x) {
//         return [](int x) {
//           return [](int x) {
//             return [](int x) {
//               return x * 10;
//             }(x) * 5431;
//           }(x) * 4551;
//         }(x) * 4351;
//       }(x) * 3224;
//     }(x) * 4323;
//   }(x) * 19;
// }

int main() {

  auto tuple = std::make_tuple(REPEAT4((1, 2, 3, 4, 5, 6, 7)));
  std::cout << "pasten: " << std::get<10>(tuple) << std::endl;
  // std::cout << lambda_func(13) << std::endl;
}
