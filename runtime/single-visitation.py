#!/usr/bin/env python

# union
#     2     compile-time:    0.70 real         0.66 user         0.02 sys
#           runtime:         6757802 ns
#     5     compile-time:    0.79 real         0.75 user         0.03 sys
#           runtime:         67577267 ns
#     8     compile-time:    0.85 real         0.81 user         0.03 sys
#           runtime:         80226419 ns
#     11    compile-time:    0.89 real         0.85 user         0.03 sys
#           runtime:         82135356 ns
#     14    compile-time:    0.94 real         0.90 user         0.03 sys
#           runtime:         87735826 ns
#     17    compile-time:    1.01 real         0.97 user         0.03 sys
#           runtime:         92854294 ns
#     20    compile-time:    1.05 real         1.01 user         0.03 sys
#           runtime:         86025919 ns
#
# std::experimental::variant
#     2     compile-time:    0.81 real         0.76 user         0.03 sys
#           runtime:         56702411 ns
#     5     compile-time:    0.87 real         0.83 user         0.03 sys
#           runtime:         76458919 ns
#     8     compile-time:    0.99 real         0.94 user         0.03 sys
#           runtime:         81208022 ns
#     11    compile-time:    1.15 real         1.10 user         0.04 sys
#           runtime:         82880236 ns
#     14    compile-time:    1.26 real         1.21 user         0.03 sys
#           runtime:         80662013 ns
#     17    compile-time:    1.44 real         1.39 user         0.04 sys
#           runtime:         87254816 ns
#     20    compile-time:    1.57 real         1.52 user         0.04 sys
#           runtime:         85653798 ns
#
# boost::variant
#     2     compile-time:    1.28 real         1.12 user         0.07 sys
#           runtime:         50688755 ns
#     5     compile-time:    1.24 real         1.18 user         0.05 sys
#           runtime:         73628200 ns
#     8     compile-time:    1.36 real         1.29 user         0.05 sys
#           runtime:         83373976 ns
#     11    compile-time:    1.44 real         1.38 user         0.05 sys
#           runtime:         86678410 ns
#     14    compile-time:    1.56 real         1.49 user         0.06 sys
#           runtime:         86076850 ns
#     17    compile-time:    1.65 real         1.58 user         0.06 sys
#           runtime:         89691905 ns
#     20    compile-time:    1.73 real         1.67 user         0.05 sys
#           runtime:         88320070 ns

import os
import subprocess
import tempfile

ns = range(2, 21, 3)
size = 10000000

with open(os.devnull, 'w') as subprocess.DEVNULL:

  print 'union'

  for n in ns:
    with tempfile.NamedTemporaryFile(suffix='.cpp') as f:
      f.write('#include <chrono>\n')
      f.write('#include <cstdio>\n')
      f.write('#include <string>\n')
      f.write('#include <random>\n')
      f.write('#include <vector>\n')
      f.write('\n')
      for x in xrange(n):
        f.write('class T{0} {{}};\n'.format(x))
      f.write('struct Union {\n')
      for x in xrange(n):
        cnstr = \
              '  explicit Union(T{0} t{0}) \n' \
              '    : type_(T{0}Tag), t{0}_(std::move(t{0})) {{}}\n'
        f.write(cnstr.format(x))
      f.write('  Union(const Union &that) : type_(that.type_) {\n')
      f.write('    switch (type_) {\n')
      for x in xrange(n):
        case = \
              '      case T{0}Tag: {{\n' \
              '        new (&t{0}_) T{0}(that.t{0}_);\n' \
              '        break;\n' \
              '      }}\n'
        f.write(case.format(x))
      f.write('    }\n')
      f.write('  }\n')
      f.write('  Union(Union &&that) : type_(that.type_) {\n')
      f.write('    switch (type_) {\n')
      for x in xrange(n):
        case = \
              '      case T{0}Tag: {{\n' \
              '        new (&t{0}_) T{0}(std::move(that.t{0}_));\n' \
              '        break;\n' \
              '      }}\n'
        f.write(case.format(x))
      f.write('    }\n')
      f.write('  }\n')
      f.write('  ~Union() {\n')
      f.write('    switch (type_) {\n')
      for x in xrange(n):
        case = \
              '      case T{0}Tag: {{\n' \
              '        t{0}_.~T{0}();\n' \
              '        break;\n' \
              '      }}\n'
        f.write(case.format(x))
      f.write('    }\n')
      f.write('  }\n')
      f.write('  enum Type {\n')
      for x in xrange(n - 1):
        f.write(
              '    T{0}Tag,\n'.format(x))
      f.write('    T{0}Tag\n'.format(n - 1))
      f.write('  };\n')
      f.write('  Type type_;\n')
      f.write('  union {\n')
      for x in xrange(n):
        f.write(
              '    T{0} t{0}_;\n'.format(x))
      f.write('  };\n')
      f.write('};\n')
      f.write('\n')
      f.write('int main() {\n')
      f.write('  std::random_device rd;\n')
      f.write('  std::uniform_int_distribution<int> dist(0, {});\n'.format(n - 1))
      f.write('  std::vector<Union> us;\n')
      f.write('  us.reserve({});'.format(size))
      f.write('  for (int i = 0; i < {}; ++i) {{\n'.format(size))
      f.write('    switch (dist(rd)) {\n')
      for x in xrange(n):
        case = \
              '      case {0}: {{\n' \
              '        us.emplace_back(T{0}());\n' \
              '        break;\n' \
              '      }}\n'
        f.write(case.format(x))
      f.write('    }\n')
      f.write('  }\n')
      f.write('  std::size_t sum = 0;\n')
      f.write('  auto begin = std::chrono::steady_clock::now();\n')
      f.write('  for (const auto &u : us) {\n')
      f.write('    switch (u.type_) {\n')
      for x in xrange(n):
        case = \
              '      case Union::T{0}Tag: {{\n' \
              '        sum += {0};\n' \
              '        break;\n' \
              '      }}\n'
        f.write(case.format(x))
      f.write('    }\n')
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

  print 'std::experimental::variant'

  for n in ns:
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
      f.write('  std::vector<Variant> vs;\n')
      f.write('  vs.reserve({});\n'.format(size))
      f.write('  for (int i = 0; i < {}; ++i) {{\n'.format(size))
      f.write('    switch (dist(rd)) {\n')
      for x in xrange(n):
        case = \
              '      case {0}: {{\n' \
              '        vs.emplace_back(T{0}());\n' \
              '        break;\n' \
              '      }}\n'
        f.write(case.format(x))
      f.write('    }\n')
      f.write('  }\n')
      f.write('  std::size_t sum = 0;\n')
      f.write('  struct visitor_t {\n')
      f.write('    explicit visitor_t(std::size_t &s) : s_(s) {}\n')
      for x in xrange(n):
        f.write(
              '    void operator()(T{0}) const {{ s_ += {0}; }}\n'.format(x))
      f.write('    std::size_t &s_;\n')
      f.write('  };\n')
      f.write('  auto visitor = visitor_t{sum};\n')
      f.write('  auto begin = std::chrono::steady_clock::now();\n')
      f.write('  for (const auto &v : vs) {\n')
      f.write('    std::experimental::visit(visitor, v);\n')
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

  for n in ns:
    with tempfile.NamedTemporaryFile(suffix='.cpp') as f:
      f.write('#include <chrono>\n')
      f.write('#include <cstdio>\n')
      f.write('#include <string>\n')
      f.write('#include <random>\n')
      f.write('#include <vector>\n')
      f.write('\n')
      f.write('#include <boost/variant.hpp>\n')
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
      f.write('  std::vector<Variant> vs;\n')
      f.write('  vs.reserve({});\n'.format(size))
      f.write('  for (int i = 0; i < {}; ++i) {{\n'.format(size))
      f.write('    switch (dist(rd)) {\n')
      for x in xrange(n):
        case = \
              '      case {0}: {{\n' \
              '        vs.emplace_back(T{0}());\n' \
              '        break;\n' \
              '      }}\n'
        f.write(case.format(x))
      f.write('    }\n')
      f.write('  }\n')
      f.write('  std::size_t sum = 0;\n')
      f.write('  struct visitor_t : public boost::static_visitor<> {\n')
      f.write('    explicit visitor_t(std::size_t &s) : s_(s) {}\n')
      for x in xrange(n):
        f.write(
              '    void operator()(T{0}) const {{ s_ += {0}; }}\n'.format(x))
      f.write('    std::size_t &s_;\n')
      f.write('  };\n')
      f.write('  auto visitor = visitor_t{sum};\n')
      f.write('  auto begin = std::chrono::steady_clock::now();\n')
      f.write('  for (const auto &v : vs) {\n')
      f.write('    boost::apply_visitor(visitor, v);\n')
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
