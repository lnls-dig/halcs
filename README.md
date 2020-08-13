# Hardware Abstraction Layer for Control Systems

[![Build Status](https://travis-ci.org/lnls-dig/halcs.svg)](https://travis-ci.org/lnls-dig/halcs)
![Latest tag](https://img.shields.io/github/tag/lnls-dig/halcs.svg?style=flat)
[![Latest release](https://img.shields.io/github/release/lnls-dig/halcs.svg?style=flat)](https://github.com/lnls-dig/halcs/releases)
[![GPL License 3.0](https://img.shields.io/github/license/lnls-dig/halcs.svg?style=flat)](LICENSE)
[![Quality Gate](https://sonarqube.com/api/badges/gate?key=br.com.lnls.dig.halcs%3Adevel)](https://sonarqube.com/dashboard/index/br.com.lnls.dig.halcs%3Adevel)
[![Technical debt ratio](https://sonarqube.com/api/badges/measure?key=br.com.lnls.dig.halcs%3Adevel&metric=sqale_debt_ratio)](https://sonarqube.com/dashboard/index/br.com.lnls.dig.halcs%3Adevel)

Software for creating controlling daemons for devices

Documentation: https://lnls-dig.github.io/halcs

## Prerequisites:

Make sure you have the following libraries installed, either by download
the binaries or executing the instructions below:

* libsodium-1.0.8 (https://github.com/jedisct1/libsodium/tree/1.0.8)
* zeromq-4.2.5 (https://github.com/zeromq/libzmq/tree/v4.2.5)
* czmq-4.0.2 (https://github.com/zeromq/czmq/tree/v4.0.2)
* mlm-1.6.1 (https://github.com/lnls-dig/malamute/tree/v1.6.1)

## Optional libraries:

* uuid (distribution available):

```bash
sudo apt-get install uuid
```

### Prerequisites Installation Instructions

```bash
git clone --branch=1.0.8 https://github.com/jedisct1/libsodium.git && \
git clone --branch=v4.2.5 https://github.com/zeromq/libzmq.git && \
git clone --branch=v4.0.2 https://github.com/zeromq/czmq.git && \
for project in libsodium libzmq czmq; do

    CONFIG_OPTS=()
    CONFIG_OPTS+=("CFLAGS=-Wno-format-truncation")
    CONFIG_OPTS+=("CPPFLAGS=-Wno-format-truncation")
    if [ $project == "libzmq" ]; then
        CONFIG_OPTS+=("PKG_CONFIG_PATH=/usr/local/lib/pkgconfig --with-libsodium")
    fi

    cd $project && \
    ./autogen.sh && \
    ./configure "${CONFIG_OPTS[@]}" && \
    make check && \
    make && \
    sudo make install && \
    sudo ldconfig && \
    cd ..

    # Check last command return status
    if [ $? -ne 0 ]; then
        echo "Could not compile/install project $project." >&2
        exit 1
    fi
done

git clone --branch=v1.6.1 https://github.com/lnls-dig/malamute.git && \
for project in malamute; do

    CONFIG_OPTS=()
    CONFIG_OPTS+=("--with-systemd-units")
    CONFIG_OPTS+=("--sysconfdir=/usr/etc")
    CONFIG_OPTS+=("--prefix=/usr")
    CONFIG_OPTS+=("CFLAGS=-Wno-format-truncation")
    CONFIG_OPTS+=("CPPFLAGS=-Wno-format-truncation")

    cd $project && \
    ./autogen.sh && \
    ./configure "${CONFIG_OPTS[@]}" && \
    make check && \
    make && \
    sudo make install && \
    sudo ldconfig && \
    cd ..

    MALAMUTE_VERBOSE=0
    MALAMUTE_PLAIN_AUTH=
    MALAMUTE_AUTH_MECHANISM=null
    MALAMUTE_ENDPOINT='ipc:///tmp/malamute'
    MALAMUTE_CFG_FILE=/usr/etc/malamute/malamute.cfg
    # Install our custom Malamute config file
    sudo sed -i \
        -e "s|verbose\( *\)=.*|verbose\1= ${MALAMUTE_VERBOSE}|g" \
        -e "s|plain\( *\)=.*|plain\1= ${MALAMUTE_PLAIN_AUTH}|g" \
        -e "s|mechanism\( *\)=.*|mechanism\1= ${MALAMUTE_AUTH_MECHANISM}|g" \
        -e "s|tcp://\*:9999|${MALAMUTE_ENDPOINT}|g" \
        ${MALAMUTE_CFG_FILE}


    # Enable service
    sudo systemctl enable malamute || /bin/true

    # Check last command return status
    if [ $? -ne 0 ]; then
        echo "Could not compile/install project $project." >&2
        echo "Try executing the script with root access." >&2
        exit 1
    fi
done
```

## Cloning this repository

```bash
git clone --recursive https://github.com/lnls-dig/halcs.git
```

## Installation Instructions

### Server

To install the server-side you'll need `libsodium`, `zeromq`, `czmq` and `mlm`.
Check the documentation or the previous section for details.

Currently we support 2 build systems: Make and CMake. The recommended approach
is to use CMake.

#### CMake

To use CMake do the following steps:

```bash
mkdir -p build
cd build
cmake ../
make
sudo make install
```

HALCS CMake scripts also supports CPack. So you can easilly generated
`.deb` or `.rpm` packages for your distribution with:

For Debian-based distros:

```bash
mkdir -p build
cd build
cmake ../
cpack -G "DEB"
```

For Redhat-based distros:

```bash
mkdir -p build
cd build
cmake ../
cpack -G "RPM"
```

#### Make

To use Make do the following steps:

```bash
make
sudo make install
```

Alternatively, you can use the `compile.sh` script that wraps
the make options into command-line options:

```bash
./compile.sh [-b <board>] [-a <applications>] [-e <with examples = yes/no>] \
    [-l <with library linking = yes/no>] [-d <with driver = yes/no>] [-x <extra flags>]
```

### Client

To install the server-side you'll need `libsodium`, `zeromq` and `czmq`.
Check the documentation or the previous section for details.

#### CMake

To use CMake do the following steps:

```bash
TOP=$(pwd)
for lib in \
    errhand \
    convc \
    hutils \
    disptable \
    llio \
    halcsclient \
    acqclient \
    bpmclient \
    sdbfs \
    sdbutils; do

    (
        cd ${TOP}/libs/${lib} && \
        mkdir -p build && \
        cd build && \
        cmake ../ && \
        make && \
        sudo make install
    )
done

```

If you need to uninstall the libraries do:

```bash
TOP=$(pwd)
for lib in \
    sdbutils \
    sdbfs \
    bpmclient \
    acqclient \
    halcsclient \
    llio \
    disptable \
    hutils \
    convc \
    errhand; do

    (
        cd ${TOP}/libs/${lib} && \
        mkdir -p build && \
        cd build && \
        cmake ../ && \
        sudo make uninstall
    )
done

```

#### Make

To use Make do the following steps:

Install the inner dependencies:

```bash
make deps
sudo make deps_install
```

And, then the client libraries themselves:

```bash
make libs
sudo make libs_install
```

If you need to uninstall the libraries do:

```bash
sudo make libs_uninstall
```

## Examples

### Compilation Instructions

Change to the examples folder

```bash
cd examples
```

#### CMake

Compile the examples using CMake (recommended):

```bash
mkdir -p build
cd build
cmake ../
make
```

#### Make

if needed, compile the examples with Make:

```bash
make
```

### Running the examples

Run an example application, for instance, the leds example

```bash
./leds -v -b <broker_endpoint> -board <board_number> -halcs <halcs_number>
```

Typically, one should choose the IPC transport method
for its faster than TCP. For instance:

```bash
./leds -v -b ipc:///tmp/halcs -board <board_number> -halcs <halcs_number>
```

If one would like to use TCP, it should call, for instance:

```bash
./leds -v -b tcp://127.0.0.1:8888 -board <board_number> -halcs <halcs_number>
```

Leds should be blinking in the FMC ADC board

## Additional PCIe driver instructions

### Installation instructions (if not installed with the main application)

Install linux header files

```bash
sudo apt-get install linux-headers-generic
```

Install the GIT package

```bash
sudo apt-get install binutils gcc
```

Change folder to the pcie driver location

```bash
cd kernel
```

Compile the PCIe driver and its tests

```bash
make
```

Install the PCIe drivers and libraries

```bash
sudo make install
```

Load the Driver module

```bash
sudo insmod /lib/modules/$(uname -r)/extra/PciDriver.ko
```

After this the kernel should have found the FPGA board
and initialized it. Run the following command and check its output

```bash
dmesg | tail
```

You should see something like the excerpt below:

```bash
[267002.495109] pciDriver - pcidriver_init :
	Major 250 allocated to nodename 'fpga'
[267002.495130] pciDriver - pcidriver_probe :
	Found ML605 board at 0000:01:00.0
[267002.495224] pciDriver - pcidriver_probe :
	Device /dev/fpga0 added
[267002.495434] pciDriver - pcidriver_probe_irq :
	Registered Interrupt Handler at pin 1, line 11, IRQ 16
[267002.495450] pciDriver - pcidriver_init :
	Module loaded
```

### Running the PCIe self-test

After the installation of the PCIe driver (see above)
it is possible to run a self test to check if
everything is setup properly. For this run the following:

Change to the "compiled tests folder"

```bash
cd tests/pcie/bin
```

Run the test entitled "testPciDriverMod"

```bash
sudo ./testPciDriverMod
```

You should get an output like the following, if everythig is ok:

```bash
Testing OPEN DEVICE... PASSED!
 Testing PCIDRIVER_IOC_MMAP_MODE... PASSED!
 Testing PCIDRIVER_IOC_MMAP_AREA... PASSED!
 Testing PCIDRIVER_IOC_PCI_INFO...  PASSED!
 Testing PCI CONFIG...
   Reading PCI config area in byte mode ... PASSED!
   Reading PCI config area in word mode ... PASSED!
   Reading PCI config area in double-word mode ... PASSED!
 Testing PCI mmap...
   Reading PCI info... PASSED!
   Setting mmap mode... PASSED!
   Setting mmap area... PASSED!
   MMAP'ing BAR0... PASSED!
   Setting mmap area... PASSED!
   MMAP'ing BAR2... PASSED!
   Setting mmap area... PASSED!
   MMAP'ing BAR4... PASSED!
 Testing PCIDRIVER_IOC_KMEM_ALLOC...
   alloc size    1024 :  PASSED!
   alloc size    2048 :  PASSED!
   alloc size    4096 :  PASSED!
   alloc size    8192 :  PASSED!
   alloc size   16384 :  PASSED!
   alloc size   32768 :  PASSED!
   alloc size   65536 :  PASSED!
   alloc size  131072 :  PASSED!
   alloc size  262144 :  PASSED!
   alloc size  524288 :  PASSED!
   alloc size 1048576 :  PASSED!
   alloc size 2097152 :  PASSED!
   alloc size 4194304 :  PASSED!
   alloc size 8388608 :  FAILED (maybe size is just too big)!
 Testing PCIDRIVER_IOC_KMEM_SYNC...
   Setting KMEM SYNC to write mode... PASSED!
   Setting KMEM SYNC to read mode... PASSED!
   Setting KMEM SYNC to read/write mode... PASSED!
 Testing PCIDRIVER_IOC_KMEM_FREE... PASSED!
 Testing Kernel Buffer mmap...
   Setting MMAP mode to KMEM... PASSED!
     Allocing size    1024 : PASSED!
     MMAPing size     1024 : PASSED!
     Allocing size    2048 : PASSED!
     MMAPing size     2048 : PASSED!
     Allocing size    4096 : PASSED!
     MMAPing size     4096 : PASSED!
     Allocing size    8192 : PASSED!
     MMAPing size     8192 : PASSED!
     Allocing size   16384 : PASSED!
     MMAPing size    16384 : PASSED!
     Allocing size   32768 : PASSED!
     MMAPing size    32768 : PASSED!
     Allocing size   65536 : PASSED!
     MMAPing size    65536 : PASSED!
     Allocing size  131072 : PASSED!
     MMAPing size   131072 : PASSED!
     Allocing size  262144 : PASSED!
     MMAPing size   262144 : PASSED!
     Allocing size  524288 : PASSED!
     MMAPing size   524288 : PASSED!
     Allocing size 1048576 : PASSED!
     MMAPing size  1048576 : PASSED!
     Allocing size 2097152 : PASSED!
     MMAPing size  2097152 : PASSED!
     Allocing size 4194304 : PASSED!
     MMAPing size  4194304 : PASSED!
     Allocing size 8388608 : FAILED (maybe size is just too big)!
   Freeing Kernel buffers...
      Buffer index 0... PASSED!
      Buffer index 1... PASSED!
      Buffer index 2... PASSED!
      Buffer index 3... PASSED!
      Buffer index 4... PASSED!
      Buffer index 5... PASSED!
      Buffer index 6... PASSED!
      Buffer index 7... PASSED!
      Buffer index 8... PASSED!
      Buffer index 9... PASSED!
      Buffer index 10... PASSED!
      Buffer index 11... PASSED!
      Buffer index 12... PASSED!
 Testing PCIDRIVER_IOC_UMEM_SGMAP ... PASSED!
 Testing PCIDRIVER_IOC_UMEM_SGGET ... PASSED!
 Testing PCIDRIVER_IOC_UMEM_SYNC ...
   Setting UMEM SYNC to write mode... PASSED!
   Setting UMEM SYNC to read mode... PASSED!
   Setting UMEM SYNC to read/write mode... PASSED!
 Testing PCIDRIVER_IOC_UMEM_SGUNMAP ... PASSED!

-------------------------------------
|        All tests PASSED!          |
-------------------------------------
```

Notice that some tests that try to evaluate the limits of a current
Linux Kernel may fail in some cases. In the example above,
two tests , due to an attempt to allocate a large buffer in kernel
space.

This is not actually an error or a failure, it is just trying to
allocate more memory than the kernel has available.
