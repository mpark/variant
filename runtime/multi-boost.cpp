#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <vector>

#include <boost/variant.hpp>
#include <boost/variant/multivisitors.hpp>

class T0 {};
class T1 {};

int main() {
  using Variant = boost::variant<T0, T1>;
  std::random_device rd;
  std::uniform_int_distribution<int> dist(0, 1);
  std::vector<std::tuple<Variant, Variant, Variant>> vs;
  vs.reserve(10000000);
  for (int i = 0; i < 10000000; ++i) {
    switch (dist(rd)) {
      case 0: {
        switch (dist(rd)) {
          case 0: {
            switch (dist(rd)) {
              case 0: {
                vs.emplace_back(T0(), T0(), T0());
                break;
              }
              case 1: {
                vs.emplace_back(T0(), T0(), T1());
                break;
              }
            }
            break;
          }
          case 1: {
            switch (dist(rd)) {
              case 0: {
                vs.emplace_back(T0(), T1(), T0());
                break;
              }
              case 1: {
                vs.emplace_back(T0(), T1(), T1());
                break;
              }
            }
            break;
          }
        }
        break;
      }
      case 1: {
        switch (dist(rd)) {
          case 0: {
            switch (dist(rd)) {
              case 0: {
                vs.emplace_back(T1(), T0(), T0());
                break;
              }
              case 1: {
                vs.emplace_back(T1(), T0(), T1());
                break;
              }
            }
            break;
          }
          case 1: {
            switch (dist(rd)) {
              case 0: {
                vs.emplace_back(T1(), T1(), T0());
                break;
              }
              case 1: {
                vs.emplace_back(T1(), T1(), T1());
                break;
              }
            }
            break;
          }
        }
        break;
      }
    }
  }
  std::size_t sum = 0;
  struct visitor_t : public boost::static_visitor<> {
    explicit visitor_t(std::size_t &s) : s_(s) {}
    void operator()(T0, T0, T0) const {
      s_ += 0;
      s_ += 0;
      s_ += 0;
    }
    void operator()(T0, T0, T1) const {
      s_ += 0;
      s_ += 0;
      s_ += 1;
    }
    void operator()(T0, T1, T0) const {
      s_ += 0;
      s_ += 1;
      s_ += 0;
    }
    void operator()(T0, T1, T1) const {
      s_ += 0;
      s_ += 1;
      s_ += 1;
    }
    void operator()(T1, T0, T0) const {
      s_ += 1;
      s_ += 0;
      s_ += 0;
    }
    void operator()(T1, T0, T1) const {
      s_ += 1;
      s_ += 0;
      s_ += 1;
    }
    void operator()(T1, T1, T0) const {
      s_ += 1;
      s_ += 1;
      s_ += 0;
    }
    void operator()(T1, T1, T1) const {
      s_ += 1;
      s_ += 1;
      s_ += 1;
    }
    std::size_t &s_;
  };
  auto visitor = visitor_t{sum};
  auto begin = std::chrono::steady_clock::now();
  for (const auto &t : vs) {
    boost::apply_visitor(
        visitor, std::get<0>(t), std::get<1>(t), std::get<2>(t));
  }
  auto end = std::chrono::steady_clock::now();
  std::fprintf(stderr, "sum: %lu\n", sum);
  std::printf("%lld ns\n",
              std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
                  .count());
}
