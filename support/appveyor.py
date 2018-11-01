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

for exceptions in ['OFF', 'ON']:
  config = exceptions
  build_dir = 'build-{}'.format(config)
  os.mkdir(build_dir)
  os.chdir(build_dir)
  result[config] = [['configure', None], ['build', None], ['test', None]]
  configure = [
    'cmake', '-G', os.environ['GENERATOR'],
             '-DMPARK_VARIANT_EXCEPTIONS={}'.format(exceptions),
             '-DMPARK_VARIANT_INCLUDE_TESTS=mpark',
             '..',
  ]
  result[config][0][1] = subprocess.call(configure)
  if result[config][0][1] == 0:
    for build_type in ['Debug', 'Release']:
      result[config][1][1] = subprocess.call([
        'cmake', '--build', '.', '--config', build_type])
      if result[config][1][1] == 0:
        result[config][2][1] = subprocess.call([
          'ctest', '--output-on-failure', '--build-config', build_type])
  os.chdir('..')

pprint.pprint(result)
exit(any(status != 0 for l in result.itervalues() for _, status in l))
