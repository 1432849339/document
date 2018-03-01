depends:
sudo apt-get install build-essential cmake libevent-dev libevent-2.0-5

compile:
mkdir build
cd build
cmake ..
make
