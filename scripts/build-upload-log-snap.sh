#!/bin/bash

SECONDS_TO_WAIT_AFTER_SHOW_ARGUMENTS=2
SECONDS_TO_WAIT_AFTER_UPLOAD=30

BUILD=y

UPLOAD=y
ADDR=192.168.1.1

SSL_PORT=443
SSL_USR=boschrexroth
SSL_PWD=boschrexroth

SVC=n

ARCH=arm64

LOGS=y
SSH_PORT=22
SSH_USR=rexroot

# Scanning parameter list

i=0
for arg in "$@"; do
	i=$((i+1))
	argNext=$((i+1))

	if grep -q "NA" <<<${arg}
	then
		ARCH=amd64
	fi

	if grep -q "PF" <<<${arg}
	then
		ADDR=10.0.2.2
		SSL_PORT=8443
		ARCH=amd64
		SSH_PORT=8022
	fi

	if grep -q "build" <<<${arg}
	then
		BUILD=${!argNext}
	fi

	if grep -q "upload" <<<${arg}
	then
		UPLOAD=${!argNext}
	fi

	if grep -q "ip" <<<${arg}
	then
		ADDR=${!argNext}
	fi

	if grep -q "addr" <<<${arg}
	then
		ADDR=${!argNext}
	fi

	if grep -q "ssl-port" <<<${arg}
	then
		SSL_PORT=${!argNext}
	fi

	if grep -q "ssl-usr" <<<${arg}
	then
		SSL_USR=${!argNext}
	fi

	if grep -q "ssl-pwd" <<<${arg}
	then
		SSL_PWD=${!argNext}
	fi

	if grep -q "svc" <<<${arg}
	then
		SVC=${!argNext}
	fi

	if grep -q "arch" <<<${arg}
	then
		ARCH=${!argNext}
	fi

	if grep -q "logs" <<<${arg}
	then
		LOGS=${!argNext}
	fi

	if grep -q "ssh-port" <<<${arg}
	then
		SSH_PORT=${!argNext}
	fi

	if grep -q "ssh-usr" <<<${arg}
	then
		SSH_USR=${!argNext}
	fi

	if grep -q "ssh-pwd" <<<${arg}
	then
		SSH_PWD=${!argNext}
	fi

	if grep -q "help" <<<${arg}
	then
		echo " "
		echo "-----------------------------------------------------"
		echo "Build, upload and show snap logs"
		echo " "
		echo "Usage:"
		echo "build-upload-log-snap.sh  - Use default values for ctrlX CORE M3/4"
		echo ""
		echo "All command line parameters with their default value:"
		echo "-----------------------------------------------------"
		echo "-build          Build snap"
		echo "  y             Default: Include snap build"
		echo "  n             Skip snap build"
		echo " "
		echo "-upload         Upload snap"
		echo "  y             Default: Servive State, Upload, Operation State"
		echo "  n             Skip upload"
		echo " "
		echo "-logs           Show snap logs"
		echo "  y             Start remote: sudo snap logs -f ... "
		echo "  n             No logs"
		echo " "
		echo "-addr           IP address or name of the destination ctrlX CORE"
		echo "  192.168.1.1   Default: ctrlX CORE"
		echo "  10.0.2.2      ctrlX CORE virtual with port forwarding"
		echo " "
		echo "-ssl-port       Port number for HTTPS (SSL) connection"
		echo "  443           Default: ctrlX CORE"
		echo "  8443          ctrlX CORE virtual with port forwarding"
		echo " "
		echo "-ssl-usr        User name for HTTPS (SSL) connection"
		echo "  boschrexroth  default value"
		echo " "
		echo "-ssl-pwd        Password for HTTPS (SSL) connection"
		echo "  boschrexroth  Default value"
		echo " "
		echo "-svc            Switch scheduler to state Service"
		echo "  n             Do not swtich"
		echo "  y             state=SERVICE -> install snap -> state=OPERATING"
		echo " "
		echo "-arch           CPU architecture"
		echo "  arm64         Default: CORE M3/4"
		echo "  amd64         ctrlX CORE virtual, OS"
		echo " "
		echo "-ssh-port       Port number for HTTPS (SSL) connection"
		echo "  22            Default: ctrlX CORE"
		echo "  8022          ctrlX CORE virtual with port forwarding"
		echo " "
		echo "-ssh-usr        User name for SSH connection"
		echo "  rexroot       Default value"
		echo " "
		echo " "
		echo "-ctrlx-virt-PF  Use settings for ctrlX CORE virtual with Port Forwarding"
		echo "                10.0.2.2, 8443, amd64, 8022"
		echo " "
		echo "-ctrlx-virt-NA  Use settings for ctrlX CORE virtual with Network Adapter"
		echo "                192.168.1.1, 443, amd64, 22"
		echo " "

		exit 1
	fi
done

echo -build = ${BUILD}
echo " "
echo -upload = ${UPLOAD}
echo -addr = ${ADDR}
echo -arch = ${ARCH}
echo -svc = ${SVC}
echo -ssl-port = ${SSL_PORT}
echo -ssl-usr = ${SSL_USR} 
echo -ssl-pwd = ${SSL_PWD}
echo " "
echo -logs = ${LOGS}
echo -ssh-port = ${SSH_PORT}
echo " "
echo " "
echo "Settings OK? Waiting" ${SECONDS_TO_WAIT_AFTER_SHOW_ARGUMENTS} "s ..."
read -t ${SECONDS_TO_WAIT_AFTER_SHOW_ARGUMENTS} -p "Press ENTER to continue"

if grep -q "y" <<<${BUILD}
then
	echo " "
	echo -----------------------------------------------------------------
	echo Building snap
	echo " "
	source build-snap-${ARCH}.sh
fi
	
if grep -q "y" <<<${UPLOAD}
then
	
	echo " "
	echo -----------------------------------------------------------------
	echo Requesting new Bearer Token
	echo " "

	res=$(curl --insecure --no-progress-meter --request POST https://${ADDR}:${SSL_PORT}/identity-manager/api/v1/auth/token --header 'Content-Type: application/json' --data-raw '{"name":"'${SSL_USR}'","password":"'${SSL_PWD}'"}')
	# https://stackoverflow.com/questions/9733338/shell-script-remove-first-and-last-quote-from-a-variable
	# Remove first and last quote (") from a variable
	TOKEN=$(echo ${res} | jq .access_token | xargs)

	echo Bearer ${TOKEN}
	echo " "


	echo " "
	echo -----------------------------------------------------------------
	echo Allow installation from unknown source
	echo " "

	curl -X PUT \
	  https://${ADDR}:${SSL_PORT}/package-manager/api/v1/settings \
	  -H 'accept: */*' \
	  -H "Authorization: Bearer ${TOKEN}" \
	  -H 'Content-Type: application/json' \
	  -d '{ "allowUnknownApps": true }' \
	  --insecure --no-progress-meter

	if grep -q "y" <<<${SVC}
	then
		echo " "
		echo -----------------------------------------------------------------
		echo Switching Scheduler to state SERVICE
		echo " "

		curl -X 'PUT' \
		  https://${ADDR}:${SSL_PORT}/automation/api/v1/scheduler/admin/state?format=json \
		  -H 'accept: application/json' \
		  -H "Authorization: Bearer ${TOKEN}" \
		  -H 'Content-Type: application/json' \
		  -d '{"state":"SERVICE"}' \
		  --insecure --no-progress-meter
	fi


	echo " "
	echo -----------------------------------------------------------------
	echo Uploading and installing ${ARCH} snaps

	for f in $(find . -maxdepth 1 -name "*_${ARCH}.snap"); 
	do 
	  echo " "  
	  echo $f  
	  echo " "  
	  curl -X POST https://${ADDR}:${SSL_PORT}/package-manager/api/v1/packages \
	  -H "accept: */*" \
	  -H "Authorization: Bearer ${TOKEN}" \
	  -H "Content-Type: multipart/form-data" \
	  -F "file=@${f}" \
	  -F update=true \
	  --insecure --no-progress-meter
	done;

	echo "Waiting ${SECONDS_TO_WAIT_AFTER_UPLOAD}s after installation ..."
	read -t ${SECONDS_TO_WAIT_AFTER_UPLOAD} -p "Wait or press ENTER to continue"

	if grep -q "y" <<<${SVC}
	then

		echo " "
		echo -----------------------------------------------------------------
		echo Switching Scheduler to state OPERATING
		echo " "

		RESPONSE=Error
		while grep -q "Error" <<<${RESPONSE}
		do

			RESPONSE=$(curl -X 'PUT' \
			  https://${ADDR}:${SSL_PORT}/automation/api/v1/scheduler/admin/state?format=json \
			  -H 'accept: application/json' \
			  -H "Authorization: Bearer ${TOKEN}" \
			  -H 'Content-Type: application/json' \
			  -d '{"state":"OPERATING"}' \
			  --insecure --no-progress-meter)
			 
			if grep -q "Error" <<<${RESPONSE}
			then			 
				echo " "
				echo WARNING Switching Scheduler to state OPERATING failed
				echo "We are repeating in ${SECONDS_TO_WAIT_AFTER_UPLOAD}s ..."
				read -t ${SECONDS_TO_WAIT_AFTER_UPLOAD} -p "Wait or press ENTER to continue"
			fi
		done
				
	fi

	echo " "
	echo -----------------------------------------------------------------
	echo Deleting Bearer Token
	echo " "
	curl \
	  --insecure --no-progress-meter \
	  --request DELETE https://${ADDR}:${SSL_PORT}/identity-manager/api/v1/auth/token \
	  -H 'accept: */*' -H "Authorization: Bearer ${TOKEN}"

fi

SNAP_FILE=$(ls *${ARCH}.snap)
SNAP_VERSION=${SNAP_FILE%_*}
SNAP=${SNAP_VERSION%_*}

echo Searching ${SNAP} in snap list...

SNAP_LIST=$(ssh -o StrictHostKeyChecking=no -p ${SSH_PORT} ${SSH_USR}@${ADDR} snap list)
echo " "

if grep -q "${SNAP}" <<<${SNAP_LIST}
then
	echo "Our snap is installed"
else
	echo "Our snap is NOT installed!"
	read -t ${SECONDS_TO_WAIT_AFTER_UPLOAD} -p "Press Ctrl-C to exit"
fi

echo " "

if grep -q "y" <<<${LOGS}
then
	echo " "
	echo -----------------------------------------------------------------
	echo Viewing logs 
	echo " "

	ssh -o StrictHostKeyChecking=no -p ${SSH_PORT} ${SSH_USR}@${ADDR} sudo snap logs -f ${SNAP}
fi
	

