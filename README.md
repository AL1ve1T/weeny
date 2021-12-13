# weeny
Self-developing proxy server

## Set-Up for development
First update all submodules with:

1. `git submodule update --init --recursive`

Then pull proper docker image

2. `docker pull rikorose/gcc-cmake`

and run container with your repo mapped in it

3. DEPRECATED`docker run -it -v "$(pwd):/usr/weeny" -p 7777:7777 --security-opt seccomp=unconfined rikorose/gcc-cmake bash`

4. https://austinmorlan.com/posts/docker_clion_development/

### Make builds with following shell scripts:
1. build-debug.sh
2. build-release.sh

## Docker build and publish
TBD
