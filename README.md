# Beam Position Monitor Software

Software for controlling the AFC BPM boards

## Prerequisites:

Make sure you have the following libraries installed:

* zeromq-4.0.4 (http://zeromq.org/area:download)
* czmq-2.2.0 (http://czmq.zeromq.org/page:get-the-software)

Cloning this repository

	git clone --recursive https://github.com/lerwys/bpm-sw.git

Install the Majordomo application from this repository (autotools-based)

	cd majordomo/libmdp

Execute the traditional sequence of autotools commands:

	./autogen.sh && make && sudo make install

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

	./compile.sh

### Client

Change to the Client API folder

	cd src/libs/libbpmclient

Compile the library, with debug info

	make ERRHAND_DBG=y ERRHAND_MIN_LEVEL=DBG_MIN_TRACE \
        ERRHAND_SUBSYS_ON=’”(DBG_DEV_MNGR | DBG_DEV_IO | DBG_SM_IO | \
        DBG_LIB_CLIENT  | DBG_SM_PR | DBG_SM_CH | DBG_LL_IO | DBG_HAL_UTILS)”’

Install the library

	sudo make install

## Running the examples

Change to the examples folder

    cd examples

Compile the examples

	make

Run an example application, for instance, the leds example

	./leds -v -b <broker_endpoint> -board <board_number> -bpm <bpm_number>

Typically, one should choose the IPC transport method
for its faster than TCP. For instance:

	./leds -v -b ipc:///tmp/bpm -board <board_number> -bpm <bpm_number>

If one would like to use TCP, it should call, for instance:

	./leds -v -b tcp://127.0.0.1:8888 -board <board_number> -bpm <bpm_number>

Leds should be blinking in the FMC ADC board
