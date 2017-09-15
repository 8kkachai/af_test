#!/bin/bash

echo "Remove old ext directory..."
rm -rf ext

echo "Create new ext directory..."
mkdir ext
mkdir ext/include
mkdir ext/include/equinox
mkdir ext/lib

echo "Copy dependency files..."
cp ../include/common.h ext/include/
cp ../include/equinox/* ext/include/equinox/
cp ../EC/EC00/*.h ext/include/
cp ../libs/sf/*.h ext/include/

cp ../EC/EC00/*.a ext/lib/
cp ../libs/sf/*.a ext/lib/

echo "Done"

exit 0
