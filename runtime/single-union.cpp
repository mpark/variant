#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <vector>

class T0 {};
class T1 {};

struct Union {
  explicit Union(T0 t0) : type_(T0Tag), t0_(std::move(t0)) {}
  explicit Union(T1 t1) : type_(T1Tag), t1_(std::move(t1)) {}
  Union(const Union &that) : type_(that.type_) {
    switch (type_) {
      case T0Tag: {
        new (&t0_) T0(that.t0_);
        break;
      }
      case T1Tag: {
        new (&t1_) T1(that.t1_);
        break;
      }
    }
  }
  Union(Union &&that) : type_(that.type_) {
    switch (type_) {
      case T0Tag: {
        new (&t0_) T0(std::move(that.t0_));
        break;
      }
      case T1Tag: {
        new (&t1_) T1(std::move(that.t1_));
        break;
      }
    }
  }
  ~Union() {
    switch (type_) {
      case T0Tag: {
        t0_.~T0();
        break;
      }
      case T1Tag: {
        t1_.~T1();
        break;
      }
    }
  }
  enum Type { T0Tag, T1Tag };
  Type type_;
  union {
    T0 t0_;
    T1 t1_;
  };
};

int main() {
  std::random_device rd;
  std::uniform_int_distribution<int> dist(0, 1);
  std::vector<Union> us;
  us.reserve(10000000);
  for (int i = 0; i < 10000000; ++i) {
    switch (dist(rd)) {
      case 0: {
        us.emplace_back(T0());
        break;
      }
      case 1: {
        us.emplace_back(T1());
        break;
      }
    }
  }
  std::size_t sum = 0;
  auto begin = std::chrono::steady_clock::now();
  for (const auto &u : us) {
    switch (u.type_) {
      case Union::T0Tag: {
        sum += 0;
        break;
      }
      case Union::T1Tag: {
        sum += 1;
        break;
      }
    }
  }
  auto end = std::chrono::steady_clock::now();
  std::fprintf(stderr, "sum: %lu\n", sum);
  std::printf("%lld ns\n",
              std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
                  .count());
}
