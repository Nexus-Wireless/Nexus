FROM --platform=linux/amd64 alpine:3.10
LABEL Description="Build environment for embedded systems"

WORKDIR /tmp

RUN apk --no-cache add --virtual runtime-dependencies \
      libusb \
      libftdi1 \ 
      wget \
      ncurses-dev \
      curl && \
    apk --no-cache add --virtual build-dependencies \
      git \
      build-base \
      libusb-dev \
      libftdi1-dev \
      automake \
      autoconf \
      libtool &&\
    git clone --depth 1 git://git.code.sf.net/p/openocd/code openocd &&\
    cd openocd &&\
    ./bootstrap &&\
    ./configure &&\
    make &&\
    make install &&\
    apk del --purge build-dependencies &&\
    rm -rf /var/cache/apk/* &&\
    rm -rf /tmp/*

WORKDIR /opt

RUN wget "https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2"

RUN tar -jxf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2 -C /usr/share/ && \
    ln -s /usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc && \
    ln -s /usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb
    # ln -s /lib64/ld-linux-x86-64.so.2 /usr/lib/ld-linux-x86-64.so.2
    # ln -s /usr/lib/libtinfo.so.6 /usr/lib/libtinfo.so.2








