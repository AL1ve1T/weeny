FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update \
    && apt upgrade -y \
    && apt install -y \
    apt-utils build-essential clang gdb gdbserver openssh-server rsync cmake git

RUN wget https://cmake.org/files/v3.12/cmake-3.12.3-Linux-x86_64.tar.gz  \
    && tar xzf cmake-3.12.3-Linux-x86_64.tar.gz  \
    && rm -rf cmake-3.12.3-Linux-x86_64.tar.gz  \
    && cd cmake-3.12.3-Linux-x86_64  \
    && apt-get install \
    && ./bin/cmake --version

RUN wget -O cmake-linux.sh https://cmake.org/files/v3.12/cmake-3.12.3-Linux-x86_64.sh \
    && sh cmake-linux.sh -- --skip-license --prefix=/usr/local

# Taken from - https://docs.docker.com/engine/examples/running_ssh_service/#environment-variables
RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# 22 for ssh server. 7777 for gdb server.
EXPOSE 22 7777

# Create dev user with password 'dev'
RUN useradd -ms /bin/bash dev
RUN echo 'dev:dev' | chpasswd

RUN mkdir /usr/weeny

ADD . /home/dev

# Upon start, run ssh daemon
CMD ["/usr/sbin/sshd", "-D"]