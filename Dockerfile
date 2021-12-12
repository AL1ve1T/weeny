FROM rikorose/gcc-cmake
COPY . /usr/weeny
WORKDIR /usr/weeny
RUN gcc -o weeny ./src/main.c
CMD ["./weeny"]