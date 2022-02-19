# zlog
cd ./external
wget -O zlog.tar.gz https://github.com/HardySimpson/zlog/archive/refs/tags/1.2.15.tar.gz
tar -zxvf zlog.tar.gz
cd ./zlog-1.2.15
make PREFIX=/usr/local
make PREFIX=/usr/local install

ldconfig
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> ~/.bashrc

# configure zlog
echo "[formats]
simple = "%m%n"
[rules]
default.DEBUG    >stdout; simple" >> /etc/zlog

# return to root folder
cd ../..

# Unity
# (pwd: ./external)
git submodule update --init --recursive

# DONE
