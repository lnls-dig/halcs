# Hardware Abstraction Layer for Control Systems

[![Build Status](https://travis-ci.org/lnls-dig/halcs.svg)](https://travis-ci.org/lnls-dig/halcs)
![Latest tag](https://img.shields.io/github/tag/lnls-dig/halcs.svg?style=flat)
[![Latest release](https://img.shields.io/github/release/lnls-dig/halcs.svg?style=flat)](https://github.com/lnls-dig/halcs/releases)
[![GPL License 3.0](https://img.shields.io/github/license/lnls-dig/halcs.svg?style=flat)](COPYING)
[![Quality Gate](https://sonarqube.com/api/badges/gate?key=br.com.lnls.dig.halcs%3Adevel)](https://sonarqube.com/dashboard/index/br.com.lnls.dig.halcs%3Adevel)
[![Technical debt ratio](https://sonarqube.com/api/badges/measure?key=br.com.lnls.dig.halcs%3Adevel&metric=sqale_debt_ratio)](https://sonarqube.com/dashboard/index/br.com.lnls.dig.halcs%3Adevel)

Software for creating controlling daemons for devices

## Prerequisites:

Make sure you have the following libraries installed, either by download
the binaries or executing the instructions below:

* libsodium-1.0.8 (https://github.com/jedisct1/libsodium/tree/1.0.8)
* zeromq-4.2.2 (https://github.com/zeromq/libzmq/tree/v4.2.2)
* czmq-4.0.2 (https://github.com/zeromq/czmq/tree/v4.0.2)
* mlm-1.5.0 (https://github.com/lnls-dig/malamute/tree/v1.5.0)

## Optional libraries:

* uuid (distribution available):

	sudo apt-get install uuid

### Prerequisites Installation Instructions

	git clone --branch=1.0.8 https://github.com/jedisct1/libsodium.git && \
	git clone --branch=v4.2.2-pre https://github.com/lnls-dig/libzmq.git && \
	git clone --branch=v4.0.2 https://github.com/zeromq/czmq.git && \
	git clone --branch=v1.5.0 https://github.com/lnls-dig/malamute.git &&
	for project in libsodium libzmq czmq malamute; do
	    cd $project
	    ./autogen.sh
	    ./configure && make check
	    sudo make install
	    sudo ldconfig
	    cd ..
	done

## Cloning this repository

	git clone --recursive https://github.com/lnls-dig/halcs.git

## PCIe Installation Instructions

Install linux header files

	sudo apt-get install linux-headers-generic

Install the GIT package

	sudo apt-get install binutils gcc

Change folder to the pcie driver location

	cd kernel

Compile the PCIe driver and its tests

	make

Install the PCIe drivers and libraries

	sudo make install

Load the Driver module

	sudo insmod /lib/modules/$(uname -r)/extra/PciDriver.ko

After this the kernel should have found the FPGA board
and initialized it. Run the following command and check its output

	dmesg | tail

You should see something like the excerpt below:

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

## Running the PCIe self-test

After the installation of the PCIe driver (see above)
it is possible to run a self test to check if
everything is setup properly. For this run the following:

Change to the "compiled tests folder"

	cd tests/pcie/bin

Run the test entitled "testPciDriverMod"

	sudo ./testPciDriverMod

You should get an output like the following, if everythig is ok:

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

Notice that some tests that try to evaluate the limits of a current
Linux Kernel may fail in some cases. In the example above,
two tests , due to an attempt to allocate a large buffer in kernel
space.

This is not actually an error or a failure, it is just trying to
allocate more memory than the kernel has available.

## Installation Instructions

### Server

Compile everything with debug info. The superuser access
is necessary because it checks (and installs if needed)
the PCIe kernel driver.

If the PCIe driver is already installed, you could
run it without superuser.

	Usage: ./compile.sh [-b <board>] [-a <applications>] [-e <with examples = yes/no>]
		[-l <with library linking = yes/no>] [-d <with driver = yes/no>] [-x <extra flags>]

### Client

Change to the Client API folder

	cd src/libs/libhalcsclient

Compile the library, with debug info

	make BOARD=<board> ERRHAND_DBG=y ERRHAND_MIN_LEVEL=DBG_MIN_TRACE \
        ERRHAND_SUBSYS_ON=’”(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | \
        DBG_LIB_CLIENT  | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)”’

or

	sudo ./compile.sh [<board>]

Install the library

	sudo make install

## Running the examples

Change to the examples folder

    cd examples

Compile the examples

	make

Run an example application, for instance, the leds example

	./leds -v -b <broker_endpoint> -board <board_number> -halcs <halcs_number>

Typically, one should choose the IPC transport method
for its faster than TCP. For instance:

	./leds -v -b ipc:///tmp/halcs -board <board_number> -halcs <halcs_number>

If one would like to use TCP, it should call, for instance:

	./leds -v -b tcp://127.0.0.1:8888 -board <board_number> -halcs <halcs_number>

Leds should be blinking in the FMC ADC board
