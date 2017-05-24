# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import os.path
import re
import subprocess
import sys

# Prints a single header version of `include/mpark/variant.hpp` to stdout.

processed = []

def process(f):
  result = ''
  with open(f, 'r') as variant:
    for line in variant:
      p = re.compile('^#include "(.+)"')
      m = p.match(line)
      if m is None:
        result += line
      else:
        g = m.group(1)
        if g not in processed:
          result += process(os.path.join(include, g))
          result += '\n'
          processed.append(g)
  return result

root = subprocess.check_output(['git', 'rev-parse', '--show-toplevel']).strip()
include = os.path.join(root, 'include', 'mpark')
result = process(os.path.join(include, 'variant.hpp'))

sys.stdout.write(result)
