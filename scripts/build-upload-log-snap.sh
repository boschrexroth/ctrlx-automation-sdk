#!/bin/bash

ARCH=arm64

ADDR=192.168.1.1
SSL_PORT=443
SSH_PORT=22

ASK=y

SECONDS_TO_WAIT_AFTER_SHOW_ARGUMENTS=5
SECONDS_TO_WAIT_AFTER_UPLOAD=30
SECONDS_TO_WAIT_AFTER_UNINSTALL=30

BUILD=y
UPLOAD=y
UNINSTALL=n
BUNDLE=""

SSL_USR=boschrexroth
SSL_PWD=boschrexroth

SERVICE=y
OPERATING=y

SSH_USR=rexroot
SSH_PWD=rexroot

LOGS=y

# Function(s) ------------------------------------------------------------------------------
function WaitForTaskFinished() {

    local task_id=$1

    # Wait some seconds for first try
    sleep 10

    local timeout=60
    local counter=0
    while true; do
        local response=$(curl -X GET https://${ADDR}:${SSL_PORT}/package-manager/api/v1/tasks/$task_id \
            -H "accept: application/json" \
            -H "Authorization: Bearer ${TOKEN}" \
            --insecure --silent)

        # uncomment for debugging
        # echo "----- DEBUG -----"
        # echo $response
        # echo "----- DEBUG -----"

        local snap=$(echo $response | jq -r .parameters.id)
        local action=$(echo $response | jq -r .action)
        local state=$(echo $response | jq -r .state)
        if [[ "$state" == "pending" ]]; then
            echo "INFO action '$action' of '$snap' is pending"
        fi

        if [[ "$state" == "running" ]]; then
            local progress=$(echo $response | jq .progress)
            echo "INFO action '$action' of '$snap' is running, progress=$progress%"
        fi

        if [[ "$state" == "done" ]]; then
            echo "INFO action '$action' of '$snap' is running is done!"
			# Wait some seconds so that switchung to OPERATING Mode works
			sleep 10
            break
        fi

        local counter=$((counter + 1))
        if [[ $counter == $timeout ]]; then
            echo "ERROR action '$action' of '$snap' is timed out!"
            RESULT="ERROR"
            break
        fi

        # wait a second for next try
        sleep 2
    done

}

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


	if grep -q "uninst" <<<${arg}
	then
		UNINSTALL=${!argNext}
	fi

	if [[ "-bundle" =~ ^$arg ]]
	then
		echo *********************************
		BUNDLE=${!argNext}
		echo BUNDLE $BUNDLE
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

	if grep -q "serv" <<<${arg}
	then
		SERVICE=${!argNext}
	fi

	if grep -q "oper" <<<${arg}
	then
		OPERATING=${!argNext}
	fi

	if grep -q "arch" <<<${arg}
	then
		ARCH=${!argNext}
	fi

	if grep -q "log" <<<${arg}
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
		echo "  y             Default: Upload"
		echo "  n             Skip upload"
		echo " "
		echo "-bundle      	  Snap is a bundle"
		echo "  y             Check"
		echo "  n             Default: Skip check"
		echo " "
		echo "-uninstall      Uninstall snap"
		echo "  y             Uninstall"
		echo "  n             Default: Skip uninstall"
		echo " "
		echo "-logs           Show snap logs"
		echo "  y             Default: sudo snap logs -f ... "
		echo "  n             No logs"
		echo " "
		echo "-addr           IP address or name of the destination ctrlX CORE"
		echo "  192.168.1.1   Default: ctrlX CORE"
		echo "  10.0.2.2      ctrlX COREvirtual with port forwarding"
		echo " "
		echo "-ssl-port       Port number for HTTPS (SSL) connection"
		echo "  443           Default: ctrlX CORE"
		echo "  8443          ctrlX COREvirtual with port forwarding"
		echo " "
		echo "-ssl-usr        User name for HTTPS (SSL) connection"
		echo "  boschrexroth  default value"
		echo " "
		echo "-ssl-pwd        Password for HTTPS (SSL) connection"
		echo "  boschrexroth  Default value"
		echo " "
		echo "-service        Switch scheduler to state Service"
		echo "  n             Default: Do not switch"
		echo "  y             Switch to SERVICE"
		echo " "
		echo "-operating      Switch scheduler to state Operating"
		echo "  n             Default: Do not switch"
		echo "  y             Switch to OPERATING"
		echo " "
		echo "-arch           CPU architecture"
		echo "  arm64         Default: CORE X3"
		echo "  amd64         ctrlX COREvirtual, OS"
		echo " "
		echo "-ssh-port       Port number for HTTPS (SSL) connection"
		echo "  22            Default: ctrlX CORE"
		echo "  8022          ctrlX COREvirtual with port forwarding"
		echo " "
		echo "-ssh-usr        User name for SSH connection"
		echo "  rexroot       Default value"
		echo " "
		echo " "
		echo "-ctrlx-virt-PF  Use settings for ctrlX COREvirtual with Port Forwarding"
		echo "                10.0.2.2, 8443, amd64, 8022"
		echo " "
		echo "-ctrlx-virt-NA  Use settings for ctrlX COREvirtual with Network Adapter"
		echo "                192.168.1.1, 443, amd64, 22"
		echo " "

		exit 1
	fi
done

echo -arch ${ARCH}
echo -addr ${ADDR}
echo -ssl-port ${SSL_PORT}
echo -ssl-usr ${SSL_USR} 
echo -ssl-pwd ${SSL_PWD}
echo -ssh-port ${SSH_PORT}
echo " "
echo -build ${BUILD}
echo -upload ${UPLOAD}
echo -bundle ${BUNDLE}
echo -uninstall ${UNINSTALL}
echo " "
echo -service ${SERVICE}
echo -operating ${OPERATING}
echo " "
echo -logs ${LOGS}
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

if grep -q "y" <<<${SERVICE}
then
	echo " "
	echo -----------------------------------------------------------------
	echo Switching Scheduler to state SERVICE
	echo " "

	RESPONSE=$(curl -X 'PUT' \
	  https://${ADDR}:${SSL_PORT}/automation/api/v1/scheduler/admin/state?format=json \
	  -H 'accept: application/json' \
	  -H "Authorization: Bearer ${TOKEN}" \
	  -H 'Content-Type: application/json' \
	  -d '{"state":"SERVICE"}' \
      --insecure --no-progress-meter --silent --show-error)

	if grep -q "SERVICE" <<<${RESPONSE}
	then			 
		echo OK
	else 
		echo WARNING Switching Scheduler to state SERVICE failed
		read -t ${SECONDS_TO_WAIT_AFTER_UPLOAD} -p "Wait or press ENTER to continue"
	fi

	echo " "
fi

if grep -q "y" <<<${UNINSTALL}
then

	echo " "
	echo -----------------------------------------------------------------
	echo Uninstalling ${ARCH} snaps

	# Read list of installed snaps
	ALL_INSTALLED_SNAPS=$(curl -X 'GET' \
		  https://${ADDR}:${SSL_PORT}/package-manager/api/v1/packages \
		  -H 'accept: application/json' \
		  -H "Authorization: Bearer ${TOKEN}" \
		  -H 'Content-Type: application/json' \
		  --insecure --no-progress-meter)

	# Loop over all found snaps
	for f in $(find . -name "*_${ARCH}.snap"); 
	do 

		# Get snap name without arch and version info
		SNAP=$(basename $f)
		SNAP=${SNAP%_*}
		SNAP=${SNAP%_*}

		if grep -q $SNAP <<<${ALL_INSTALLED_SNAPS}
		then

			echo " "  
			echo Uninstalling $SNAP  --------------------------
			echo " " 

			curl -X POST \
			https://${ADDR}:${SSL_PORT}/package-manager/api/v1/tasks \
			-H 'accept: */*' \
			-H "Authorization: Bearer ${TOKEN}" \
			-H 'Content-Type: application/json' \
			-d '{ "action": "uninstall", "parameters": { "id": "'${SNAP}'" } }' \
			--insecure --no-progress-meter --silent --show-error

			echo "Waiting ${SECONDS_TO_WAIT_AFTER_UNINSTALL}s after uninstallation ..."
			read -t ${SECONDS_TO_WAIT_AFTER_UNINSTALL} -p "Wait or press ENTER to continue"
			echo " "  

		else
			echo NOT installed: $SNAP
		fi
	done;
fi

if grep -q "y" <<<${UPLOAD}
then

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
	  --insecure --no-progress-meter --silent --show-error

	echo " "
	echo -----------------------------------------------------------------
	echo Uploading and installing ${ARCH} snaps

	for f in $(find . -name "*_${ARCH}.snap"); 
	do 
		SNAP=$(basename $f)
		SNAP=${SNAP%_*}
		SNAP=${SNAP%_*}

		echo " "  
		echo Uploading ...
		echo "     file" $f
		echo "     snap" ${SNAP}
		echo " "  
		LOCATION=$(curl -X POST https://${ADDR}:${SSL_PORT}/package-manager/api/v1/packages \
		-H "accept: */*" \
		-H "Authorization: Bearer ${TOKEN}" \
		-H "Content-Type: multipart/form-data" \
		-F "file=@${f}" \
		-F update=true \
        --insecure --silent --include | tr -d '\r' | sed -En 's/^location: (.*)/\1/p')

    	TASK_ID=$(echo $LOCATION | cut -d'/' -f 3)

 		WaitForTaskFinished $TASK_ID
	done;

	echo " "
	echo -----------------------------------------------------------------
	echo Checking if ${ARCH} snaps are installed

	# Read list of installed snaps
	ALL_INSTALLED_SNAPS=$(curl -X 'GET' \
		https://${ADDR}:${SSL_PORT}/package-manager/api/v1/packages \
		-H 'accept: application/json' \
		-H "Authorization: Bearer ${TOKEN}" \
		-H 'Content-Type: application/json' \
		--insecure --no-progress-meter)

	for f in $(find . -name "*_${ARCH}.snap"); 
	do 
		SNAP=$(basename $f)
		SNAP=${SNAP%_*}
		SNAP=${SNAP%_*}

		if grep -q ${SNAP} <<<${ALL_INSTALLED_SNAPS}
		then
			echo INFO Is installed ${SNAP}
		else
			echo ERROR Is NOT installed ${SNAP}
		fi
	done;
fi

if grep -q "y" <<<${OPERATING}
then

	for i in $(seq 1 5);
	do

		echo " "
		echo -----------------------------------------------------------------
		echo Switching Scheduler to state OPERATING
		echo " "

		# Two steps are needed
		# 1. Start rexroth-automationcore.control (scheduler)
		NOT_USED_RESPONSE=$(curl -X 'PUT' \
		  https://${ADDR}:${SSL_PORT}/automation/api/v1/scheduler/admin/state?format=json \
		  -H 'accept: application/json' \
		  -H "Authorization: Bearer ${TOKEN}" \
		  -H 'Content-Type: application/json' \
		  -d '{"action":"start","parameters":{"service":"rexroth-automationcore.control","enable":"true"}}' \
		  --insecure --no-progress-meter --silent)

		# 2. Switch scheduler to OPERATING
		RESPONSE=$(curl -X 'PUT' \
		  https://${ADDR}:${SSL_PORT}/automation/api/v1/scheduler/admin/state?format=json \
		  -H 'accept: application/json' \
		  -H "Authorization: Bearer ${TOKEN}" \
		  -H 'Content-Type: application/json' \
		  -d '{"state":"OPERATING"}' \
		  --insecure --no-progress-meter --silent --show-error)
		
		if grep -q "Error" <<<${RESPONSE}
		then			 
			echo " "
			echo WARNING Switching Scheduler to state OPERATING failed
			echo "We are repeating in ${SECONDS_TO_WAIT_AFTER_UPLOAD}s ..."
			read -t ${SECONDS_TO_WAIT_AFTER_UPLOAD} -p "Wait or press ENTER to continue"
			echo " "
		else 
			echo OK
			break
		fi
	done
			
fi

# Not empty
if [ -n "$BUNDLE" ]
then

	# No logs
	LOGS=n
	
		echo " "
		echo -----------------------------------------------------------------
		echo Checking bundle $BUNDLE ...
		echo " "
		BUNDLES=$(curl -X 'GET' \
			https://${ADDR}:${SSL_PORT}/automation/api/v2/nodes/framework/bundles?type=browse \
			-H 'accept: application/json' \
			-H "Authorization: Bearer ${TOKEN}" \
			--insecure --no-progress-meter)

		echo " "
		echo $BUNDLES
		echo " "

		if grep -q $BUNDLE <<<"$BUNDLES"; then
			echo "INFO Is registered: $BUNDLE"
			
			STATE=$(curl -X 'GET' \
				https://${ADDR}:${SSL_PORT}/automation/api/v2/nodes/framework/bundles/$BUNDLE \
				-H 'accept: application/json' \
				-H "Authorization: Bearer ${TOKEN}" \
				--insecure --no-progress-meter)

			echo $STATE
			echo " "
			echo "	name" $(echo $STATE | jq .value.name)
			echo "	version" $(echo $STATE | jq .value.version)
			echo "	components" $(echo $STATE | jq .value.components)
			echo "	state" $(echo $STATE | jq .value.state)
			echo "	active" $(echo $STATE | jq .value.active)
			echo "	installed" $(echo $STATE | jq .value.installed)
			
		else
			echo "ERROR Is NOT registered: $BUNDLE"
		fi
fi

echo " "
echo -----------------------------------------------------------------
echo Deleting Bearer Token
echo " "
curl \
  --insecure --no-progress-meter \
  --request DELETE https://${ADDR}:${SSL_PORT}/identity-manager/api/v1/auth/token \
  -H 'accept: */*' -H "Authorization: Bearer ${TOKEN}"

if grep -q "y" <<<${LOGS}
then
	echo " "
	echo -----------------------------------------------------------------
	echo Activate SSH access 
	echo " "

	curl -X 'PUT' \
			https://${ADDR}:${SSL_PORT}/ssh/api/v1/status \
			-H 'accept: application/json' \
			-H "Authorization: Bearer ${TOKEN}" \
			-H 'Content-Type: application/json' \
			-d '{"state": "activated"}' \
		    --insecure --no-progress-meter --silent --show-error

	echo " "

	ssh-keygen -f "/home/boschrexroth/.ssh/known_hosts" -R "[${ADDR}]:${SSH_PORT}" 2>&1 1>/dev/null

	echo " "
	echo -----------------------------------------------------------------
	echo Viewing logs 
	echo " "

	sshpass -p ${SSH_PWD} ssh -o StrictHostKeyChecking=no -p ${SSH_PORT} ${SSH_USR}@${ADDR} sudo snap logs -f ${SNAP}
fi