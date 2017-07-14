#!/usr/bin/env python

# std::experimental::variant
#     2     compile-time:    0.94 real         0.88 user         0.03 sys
#           runtime:         102314263 ns
#     5     compile-time:    3.85 real         3.79 user         0.05 sys
#           runtime:         126198621 ns
#     8     compile-time:    15.95 real        15.76 user        0.17 sys
#           runtime:         139920716 ns
#     11    compile-time:    52.01 real        51.45 user        0.49 sys
#           runtime:         180197182 ns
#     14    compile-time:    150.70 real       149.30 user       1.22 sys
#           runtime:         220526983 ns
#     17    compile-time:    366.08 real       364.14 user       1.68 sys
#           runtime:         225634973 ns
#     20    compile-time:    936.15 real       926.80 user       6.70 sys
#           runtime:         273726913 ns
#
# boost::variant
#     2     compile-time:    2.26 real         2.14 user         0.09 sys
#           runtime:         142141403 ns
#     5     compile-time:    10.28 real        10.05 user        0.19 sys
#           runtime:         253383596 ns
#     8     compile-time:    36.42 real        35.73 user        0.57 sys
#           runtime:         280005724 ns
#     11    compile-time:    99.66 real        98.03 user        1.37 sys
#           runtime:         309401224 ns
#     14    compile-time:    231.20 real       227.77 user       2.86 sys
#           runtime:         327955207 ns
#     17    compile-time:    499.45 real       493.27 user       4.32 sys
#           runtime:         367981659 ns
#     20    compile-time:    1147.01 real      1132.32 user      11.74 sys
#           runtime:         432689278 ns

import os
import subprocess
import tempfile

num_types = range(2, 21, 3)
size = 10000000

with open(os.devnull, 'w') as subprocess.DEVNULL:

  print 'std::experimental::variant'

  for n in num_types:
    with tempfile.NamedTemporaryFile(suffix='.cpp') as f:
      f.write('#include <chrono>\n')
      f.write('#include <cstdio>\n')
      f.write('#include <string>\n')
      f.write('#include <random>\n')
      f.write('#include <vector>\n')
      f.write('\n')
      f.write('#include <experimental/variant.hpp>\n')
      f.write('\n')
      for x in xrange(n):
        f.write('class T{0} {{}};\n'.format(x))
      f.write('int main() {\n')
      f.write('  using Variant = std::experimental::variant<\n')
      for x in xrange(n - 1):
        f.write(
              '    T{0},\n'.format(x))
      f.write('    T{0}\n'.format(n - 1))
      f.write('  >;\n')
      f.write('  std::random_device rd;\n')
      f.write('  std::uniform_int_distribution<int> dist(0, {});\n'.format(n - 1))
      f.write('  std::vector<std::tuple<Variant, Variant, Variant>> vs;\n')
      f.write('  vs.reserve({});\n'.format(size))
      f.write('  for (int i = 0; i < {}; ++i) {{\n'.format(size))
      f.write('    switch (dist(rd)) {\n')
      for x in xrange(n):
        f.write(
              '      case {0}: {{\n'.format(x))
        f.write('      switch (dist(rd)) {\n')
        for y in xrange(n):
          f.write(
                '        case {0}: {{\n'.format(y))
          f.write('        switch (dist(rd)) {\n')
          for z in xrange(n):
            f.write(
                  '          case {0}: {{\n'.format(z))
            f.write(
                  '            vs.emplace_back(T{0}(), T{1}(), T{2}());'.format(x, y, z))
            f.write(
                  '            break;\n' \
                  '          }\n')
          f.write('        }\n')
          f.write('        break;\n' \
                  '      }\n')
        f.write(
              '        }\n' \
              '        break;\n' \
              '      }\n')
      f.write('    }\n')
      f.write('  }\n')
      f.write('  std::size_t sum = 0;\n')
      f.write('  struct visitor_t {\n')
      f.write('    explicit visitor_t(std::size_t &s) : s_(s) {}\n')
      for x in xrange(n):
        for y in xrange(n):
          for z in xrange(n):
            f.write(
                  '  void operator()(T{0}, T{1}, T{2}) const {{' \
                  '    s_ += {0};' \
                  '    s_ += {1};' \
                  '    s_ += {2};' \
                  '  }}\n'.format(x, y, z))
      f.write('    std::size_t &s_;\n')
      f.write('  };\n')
      f.write('  auto visitor = visitor_t{sum};\n')
      f.write('  auto begin = std::chrono::steady_clock::now();\n')
      f.write('  for (const auto &t : vs) {\n')
      f.write('    std::experimental::visit(\n')
      f.write('        visitor, std::get<0>(t), std::get<1>(t), std::get<2>(t));\n')
      f.write('  }\n')
      f.write('  auto end = std::chrono::steady_clock::now();\n')
      f.write('  std::fprintf(stderr, "sum: %lu\\n", sum);\n')
      f.write('  std::printf(\n')
      f.write('      "%lld ns\\n",\n')
      f.write('      std::chrono::duration_cast<std::chrono::nanoseconds>(\n')
      f.write('          end - begin).count());\n')
      f.write('}\n')
      f.flush()

      output = os.path.splitext(f.name)[0]

      try:
        result = subprocess.check_output(
            ['time',
             'clang++-3.7',
             '-std=c++14',
             '-O2',
             '-ftemplate-depth=2048',
             '-I{}'.format(os.path.join(os.getcwd(), '../include')),
             '-o',
             output,
             f.name],
            stderr=subprocess.STDOUT)
      except subprocess.CalledProcessError as e:
        print "Compilation Error:", e.output

      print n, '\t\t', 'compile-time:', '\t\t', result.strip()

      result = subprocess.check_output([output], stderr=subprocess.DEVNULL)

      print '\t\t', 'runtime:', '\t\t', result.strip()

      os.unlink(output)

  print

  print 'boost::variant'

  for n in num_types:
    with tempfile.NamedTemporaryFile(suffix='.cpp') as f:
      f.write('#include <chrono>\n')
      f.write('#include <cstdio>\n')
      f.write('#include <string>\n')
      f.write('#include <random>\n')
      f.write('#include <vector>\n')
      f.write('\n')
      f.write('#include <boost/variant.hpp>\n')
      f.write('#include <boost/variant/multivisitors.hpp>\n')
      f.write('\n')
      for x in xrange(n):
        f.write('class T{0} {{}};\n'.format(x))
      f.write('int main() {\n')
      f.write('  using Variant = boost::variant<\n')
      for x in xrange(n - 1):
        f.write(
              '    T{0},\n'.format(x))
      f.write('    T{0}\n'.format(n - 1))
      f.write('  >;\n')
      f.write('  std::random_device rd;\n')
      f.write('  std::uniform_int_distribution<int> dist(0, {});\n'.format(n - 1))
      f.write('  std::vector<std::tuple<Variant, Variant, Variant>> vs;\n')
      f.write('  vs.reserve({});\n'.format(size))
      f.write('  for (int i = 0; i < {}; ++i) {{\n'.format(size))
      f.write('    switch (dist(rd)) {\n')
      for x in xrange(n):
        f.write(
              '      case {0}: {{\n'.format(x))
        f.write('      switch (dist(rd)) {\n')
        for y in xrange(n):
          f.write(
                '        case {0}: {{\n'.format(y))
          f.write('        switch (dist(rd)) {\n')
          for z in xrange(n):
            f.write(
                  '          case {0}: {{\n'.format(z))
            f.write(
                  '            vs.emplace_back(T{0}(), T{1}(), T{2}());'.format(x, y, z))
            f.write(
                  '            break;\n' \
                  '          }\n')
          f.write('        }\n')
          f.write('        break;\n' \
                  '      }\n')
        f.write(
              '        }\n' \
              '        break;\n' \
              '      }\n')
      f.write('    }\n')
      f.write('  }\n')
      f.write('  std::size_t sum = 0;\n')
      f.write('  struct visitor_t : public boost::static_visitor<> {\n')
      f.write('    explicit visitor_t(std::size_t &s) : s_(s) {}\n')
      for x in xrange(n):
        for y in xrange(n):
          for z in xrange(n):
            f.write(
                  '  void operator()(T{0}, T{1}, T{2}) const {{' \
                  '    s_ += {0};' \
                  '    s_ += {1};' \
                  '    s_ += {2};' \
                  '  }}\n'.format(x, y, z))
      f.write('    std::size_t &s_;\n')
      f.write('  };\n')
      f.write('  auto visitor = visitor_t{sum};\n')
      f.write('  auto begin = std::chrono::steady_clock::now();\n')
      f.write('  for (const auto &t : vs) {\n')
      f.write('    boost::apply_visitor(\n')
      f.write('        visitor, std::get<0>(t), std::get<1>(t), std::get<2>(t));\n')
      f.write('  }\n')
      f.write('  auto end = std::chrono::steady_clock::now();\n')
      f.write('  std::fprintf(stderr, "sum: %lu\\n", sum);\n')
      f.write('  std::printf(\n')
      f.write('      "%lld ns\\n",\n')
      f.write('      std::chrono::duration_cast<std::chrono::nanoseconds>(\n')
      f.write('          end - begin).count());\n')
      f.write('}\n')
      f.flush()

      output = os.path.splitext(f.name)[0]

      try:
        result = subprocess.check_output(
            ['time',
             'clang++-3.7',
             '-std=c++14',
             '-O2',
             '-ftemplate-depth=8192',
             '-o',
             output,
             f.name],
            stderr=subprocess.STDOUT)
      except subprocess.CalledProcessError as e:
        print "Compilation Error:", e.output

      print n, '\t\t', 'compile-time:', '\t\t', result.strip()

      result = subprocess.check_output([output], stderr=subprocess.DEVNULL)

      print '\t\t', 'runtime:', '\t\t', result.strip()

      os.unlink(output)
