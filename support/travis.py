# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

import os
import pprint
import subprocess

result = {}

for cxx_version in os.environ['CXX_VERSIONS'].split(';'):
  os.environ['CXXFLAGS'] = '-std=c++{}'.format(cxx_version)
  for build_type in ['Debug', 'Release']:
    for exceptions in ['OFF', 'ON']:
      config = '{}-{}-{}'.format(cxx_version, build_type, exceptions)
      build_dir = 'build-{}'.format(config)
      os.mkdir(build_dir)
      os.chdir(build_dir)
      result[config] = [['configure', None], ['build', None], ['test', None]]
      result[config][0][1] = subprocess.call([
        'cmake', '-DCMAKE_BUILD_TYPE={}'.format(build_type),
                 '-DMPARK_VARIANT_EXCEPTIONS={}'.format(exceptions),
                 '-DMPARK_VARIANT_INCLUDE_TESTS={}'.format(os.environ['TESTS']),
                 '..',
      ])
      if result[config][0][1] == 0:
        result[config][1][1] = subprocess.call([
          'cmake', '--build', '.', '--', '-k', '-j', '2'])
        if result[config][1][1] == 0:
          result[config][2][1] = subprocess.call([
            'ctest', '--output-on-failure'])
      os.chdir('..')

pprint.pprint(result)
exit(any(status != 0 for l in result.itervalues() for _, status in l))
