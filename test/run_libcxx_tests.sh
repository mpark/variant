#!/usr/bin/env bash

set -e

trap "cd ${MPARK_VARIANT_LIBCXX_SOURCE_DIR} && git checkout ." EXIT

cat <<EOF > ${MPARK_VARIANT_LIBCXX_SOURCE_DIR}/include/variant
#define mpark std
$(grep -v "#include <mpark/in_place.hpp>" ${MPARK_VARIANT_SOURCE_DIR}/include/mpark/variant.hpp)
#undef mpark
EOF

${MPARK_VARIANT_LIT} \
    -v \
    --param color_diagnostics \
    --param compile_flags=-I${MPARK_VARIANT_SOURCE_DIR}/include \
    --param cxx_under_test=${MPARK_VARIANT_CXX_COMPILER} \
    --param std=c++1z \
    --param libcxx_site_config=${MPARK_VARIANT_LIBCXX_SITE_CONFIG} \
    ${MPARK_VARIANT_LIBCXX_SOURCE_DIR}/test/std/utilities/variant \
    ${MPARK_VARIANT_LIBCXX_SOURCE_DIR}/test/libcxx/utilities/variant
