#!/bin/sh

if [ "$(id -u)" != "0" ]; then
	echo "Installer must be run in root."
	exit 1
fi

echo "Detecting system archecture..."

if [ "$(getconf LONG_BIT)" = "64" ]; then
	USR_LIB_DIR=/usr/lib64
else
	USR_LIB_DIR=/usr/lib
fi

echo "Detected user library directory: $USR_LIB_DIR"

echo "Copying files..."
cp libbce.so $USR_LIB_DIR/>/dev/null

echo "Install finished!"

exit 0

