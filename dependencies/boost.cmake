# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

find_package(Boost 1.64 REQUIRED)

function(boost_add_dataset dataset range)
  metabench_add_dataset(${dataset} boost.cpp.erb ${range} NAME boost)
  target_link_libraries(${dataset} PUBLIC ${Boost_LIBRARIES})
endfunction()
