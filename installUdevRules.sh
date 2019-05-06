#!/bin/bash
#
# Installs the udev script to allow group 'plugdev' access to CMSIS-DAP adapters.
#

echo "Adding current user to group 'plugdev'..."
sudo adduser $(whoami) plugdev

echo "Copying udev script..."
sudo cp -v 98-openocd.rules /etc/udev/rules.d/

echo "Restarting udev..."
sudo udevadm trigger

echo
echo "Finished. If there are issues, try re-plugging the debugger or re-logging in."

