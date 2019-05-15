#!/bin/bash
#
# A simple script to automate user uploading.
# Written by Clyne.
#

# Start OpenOCD
echo
echo "Starting OpenOCD: 'openocd -f openocd.cfg &'..."
echo
openocd -f openocd.cfg &
sleep 2

# Run GDB
echo
echo "Running GDB to upload code..."
echo
gdb-multiarch --batch --command=user.gdb
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

