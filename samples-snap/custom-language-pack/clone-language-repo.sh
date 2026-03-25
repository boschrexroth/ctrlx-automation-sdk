SEPARATION_LINE_1="===================================================================================================="
SEPARATION_LINE_2="----------------------------------------------------------------------------------------------------"

WORKING_DIR=$(pwd)
LANGUAGE_DIR=${WORKING_DIR}/ctrlx-automation-i18n

if [ -d "${LANGUAGE_DIR}" ]; then
	echo ""
	read -rp "Delete existing directory ${LANGUAGE_DIR} Y/n? " ANS
	if [[ "$ANS" == *"n"* ]]; then
		echo "Change your working directory or delete the directory yourself ctrlx-automation-sdk/"
		exit 1
	fi

	# Older versions have directories without x-permission so sudo ...
	sudo rm -rf "${LANGUAGE_DIR}"/ 2>/dev/null
fi

echo ""
echo $SEPARATION_LINE_1
echo "Cloning https://github.com/boschrexroth/ctrlx-automation-i18n.git ..."
echo $SEPARATION_LINE_2
git clone --quiet https://github.com/boschrexroth/ctrlx-automation-i18n.git
cd "$LANGUAGE_DIR" || exit

echo ""
echo $SEPARATION_LINE_1
echo "Enter the desired tag to be checked out from this list:"
git tag
echo $SEPARATION_LINE_2
TAG_LATEST=$(git describe --tags --abbrev=0)
read -rp "Version ($TAG_LATEST)? " TAG
if [ -z "$TAG" ]; then
	TAG=$TAG_LATEST
fi

echo ""
echo $SEPARATION_LINE_1
echo "Github repo: ${LANGUAGE_DIR}"
echo "Version (release):     ${TAG}"
echo $SEPARATION_LINE_2
read -rt 20 -p "OK? "

echo ""
echo $SEPARATION_LINE_1
echo "Checking out tag ${TAG} ..."
echo $SEPARATION_LINE_2
git checkout tags/"${TAG}"