#!/usr/bin/env bash

FILE=/var/log/cloud-init-output.log

# Do not use $0 because '...bash...' is returned at login
SCRIPT_NAME=check-cloud-init-log.sh

# Add/remove search patterns here
PATTERNS=(
	"Could not connect"
	"Unable to connect"
	"Failed to fetch"
	"failed to download"
	"Unable to locate"
	"is not available, but is referred to by another package"
	"util.py[WARNING]:"
)

echo " "
echo "======================================================================="
echo "Searching for errors and warnings reported during initialization phase."
echo "======================================================================="
echo " "

read -t 5 -p "Hit ENTER to continue - cancel with Ctrl-C "

# Search patterns
for PATTERN in "${PATTERNS[@]}"; do
	ERRORS_WARNINGS=${ERRORS_WARNINGS}$(sudo grep -i "${PATTERN}" ${FILE})
done

# Calculate the length
ERRORS_WARNINGS_LEN=${#ERRORS_WARNINGS}

if (( ERRORS_WARNINGS_LEN == 0)); then
	echo " "
	echo "--------------------------------------------------------------------------- "
	echo "                  No errors/warnings were found."
	echo "--------------------------------------------------------------------------- "
	echo " "

	read -t 5 -p "Remove this check script from ~/.bashrc? Y/n" INPUT

	if grep -q "n" <<<${INPUT}
	then
		return 0
	fi	

	# Remove line with containing the call of this script
	sed -i /${SCRIPT_NAME}/d ~/.bashrc

	# Add enabled call
  	echo "# source /home/boschrexroth/${SCRIPT_NAME}" >>/home/boschrexroth/.bashrc
	
	return 0
fi

echo " "
echo "Errors/warnings ----------------------------------------------------------------"
echo " "

for PATTERN in "${PATTERNS[@]}"; do
	sudo grep -i "${PATTERN}" ${FILE}
done

echo " "
echo "--------------------------------------------------------------------------------"
echo " "
echo "       Errors/warnings that occurred during the initialization phase were found,"
echo "        we strongly recommend to discard this App Build Environment."
echo " "
echo " Before creating a new App Build Environment, please check these preconditions:"
echo " - Is there enough disk memory available on your host system? At least 50GB."
echo " - Is a working internet connection available?"
echo " - Is the Ubuntu APT repository mirror available?"
echo " - If a proxy is needed:"
echo "   Is the proxy server on the host operating system running and available under port 3128?"
echo "--------------------------------------------------------------------------------"
echo " "

return 1

