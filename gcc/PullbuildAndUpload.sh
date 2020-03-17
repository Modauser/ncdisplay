#!/bin/bash
#
# A simple script to automate compilation and uploading.
# Written by Clyne.
#

MAKEARGS = -j4

# Check for code updates
echo
echo "Updating code: 'git pull'..."
echo
git pull
if [ $? -ne 0 ]
then
	echo
	echo "Updating code failed."
	exit 1
fi

# Build program
echo
echo "Building code: 'make'..."
echo
make system user $MAKEARGS
if [ $? -ne 0 ]
then
	echo
	echo "Building code failed."
	exit 1
fi

# Start OpenOCD
echo
echo "Starting OpenOCD: 'openocd -f openocd.cfg &'..."
echo
openocd -f openocd.cfg &
sleep 1

# Run GDB
echo
echo "Running GDB to upload code..."
echo
gdb-multiarch --batch --command=upload.gdb
if [ $? -ne 0 ]
then
	echo
	echo "GDB failed."
fi

# End OpenOCD
echo
echo "Stopping OpenOCD..."
pkill openocd
sleep 1

echo
echo "Finished."

