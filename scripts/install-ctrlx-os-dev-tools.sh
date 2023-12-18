echo " "
echo "============================================"
echo Adding required apt sources
echo "============================================"
echo " "

sudo add-apt-repository -y ppa:snappy-dev/image

source ~/scripts/install-snapcraft.sh

source ~/scripts/install-go.sh

echo " "
echo "============================================"
echo Installing ubuntu-image snap
echo "============================================"
echo " "

sudo snap install ubuntu-image --classic
