FROM gcc:4.9
COPY . /usr/weeny
WORKDIR /usr/weeny
RUN gcc -o weeny ./src/main.c
CMD ["./weeny"]