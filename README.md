# weeny
Self-developing proxy server

## Set-Up for development
First update all submodules with:

1. `git submodule update --init --recursive`

Then pull proper docker image

2. `docker pull rikorose/gcc-cmake`

and run container with your repo mapped in it

3. `docker run -it -v $(pwd):/usr/weeny rikorose/gcc-cmake bash`

### Make builds with following shell scripts:
1. build-debug.sh
2. build-release.sh

## Docker build and publish
TBD
