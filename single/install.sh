#!/bin/sh

if [ "$(id -u)" != "0" ]; then
	echo "Installer must be run in root."
	exit 1
fi

echo "Copying files..."
cp bcsh-single /usr/bin/bce

echo "Install finished!"

exit 0

