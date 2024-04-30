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

echo " "
echo "============================================"
echo Installing required debian packages
echo "============================================"
echo " "

sudo apt-get install -y ncurses-dev
sudo apt-get install -y flex
sudo apt-get install -y bison



