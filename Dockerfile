FROM ubuntu:latest
LABEL Description="Build environment for embedded systems"

WORKDIR /tmp

RUN apt-get update && apt-get install -y libtool git make pkg-config texinfo udev libusb-dev libusb-1.0.0-dev libftdi-dev autoconf && \
    git clone --depth 1 git://git.code.sf.net/p/openocd/code openocd && \
    cd openocd &&\
    ./bootstrap &&\
    ./configure --enable-stlink --enable-ftdi --disable-dependency-tracking &&\
    make &&\
    make install

# RUN git clone git://github.com/Dashlane/hidapi.git && \
#     cd hidapi && \
#     ./bootstrap && \
#     ./configure --enable-static --disable-shared && \
#     make clean && \
#     make

RUN cp /tmp/openocd/contrib/60-openocd.rules /etc/udev/rules.d/60-openocd.rules

RUN mkdir -p /usr/src/app
VOLUME ["/usr/src/app"]
WORKDIR /usr/src/app
RUN cd /usr/src/app

EXPOSE 4444

# WORKDIR /opt

# RUN wget "https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2"

# RUN tar -jxf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2 -C /usr/share/ && \
#     ln -s /usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc && \
#     ln -s /usr/share/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb
#     # ln -s /lib64/ld-linux-x86-64.so.2 /usr/lib/ld-linux-x86-64.so.2
#     # ln -s /usr/lib/libtinfo.so.6 /usr/lib/libtinfo.so.2








