.. _halcs-usage:

HALCS Usage
===========

After installing all of the dependencies and HALCS itself
to the desired versions, see :ref:`halcs-installation` for
more information, we can check if all of the needed parts
are up and running.

Things to check
---------------

Device detection
''''''''''''''''

The first thing to check is if your device is found by the OS. In the most
typical usage of HALCS this would be a *PCIe* device.

If using the `FPGA PCIe driver`_ you can check if your device was
enumerated by checking the */dev/fpga-<PCIe slot number>* or
*/dev/fpga/<PCIe slot number>*. If the board was detected a file
should be created at */dev*.

.. _`FPGA PCIe driver`: https://github.com/lnls-dig/fpga_pcie_driver

Another way to check if the board was detected is by looking at the
system logs, specifically in */var/log/messages*, */var/log/dmesg* or
via the ``dmesg`` command. A common output of the `FPGA PCIe driver`
is shown below:

.. code-block:: bash

    [Fri Feb 28 11:24:14 2020] pciehp 0000:02:08.0:pcie24: pending interrupts 0x0009 from Slot Status
    [Fri Feb 28 11:24:14 2020] pciehp 0000:02:08.0:pcie24: Button pressed on Slot(8)
    [Fri Feb 28 11:24:14 2020] pciehp 0000:02:08.0:pcie24: Card present on Slot(8)
    [Fri Feb 28 11:24:14 2020] pciehp 0000:02:08.0:pcie24: pciehp_get_power_status: SLOTCTRL 80 value read 1431
    [Fri Feb 28 11:24:14 2020] pciehp 0000:02:08.0:pcie24: PCI slot #8 - powering on due to button press
    [Fri Feb 28 11:24:14 2020] pciehp 0000:02:08.0:pcie24: pciehp_get_power_status: SLOTCTRL 80 value read 1431
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: pending interrupts 0x0010 from Slot Status
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: pciehp_power_on_slot: SLOTCTRL 80 write cmd 0
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: __pciehp_link_set: lnk_ctrl = 0
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: pending interrupts 0x0100 from Slot Status
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: pciehp_check_link_active: lnk_status = 2041
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: slot(8): Link Up event
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: Link Up event ignored on slot(8): already powering on
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: pciehp_check_link_active: lnk_status = 2041
    [Fri Feb 28 11:24:15 2020] pciehp 0000:02:08.0:pcie24: pciehp_check_link_status: lnk_status = 2041
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: [10ee:7014] type 00 class 0x118000
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: reg 0x10: [mem 0x00000000-0x000007ff 64bit]
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: reg 0x18: [mem 0x00000000-0x000fffff 64bit pref]
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: reg 0x20: [mem 0x00000000-0x0007ffff 64bit]
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: Max Payload Size set to 256 (was 128, max 512)
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: BAR 2: assigned [mem 0x91000000-0x910fffff 64bit pref]
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: BAR 4: assigned [mem 0x93900000-0x9397ffff 64bit]
    [Fri Feb 28 11:24:15 2020] pci 0000:0b:00.0: BAR 0: assigned [mem 0x93980000-0x939807ff 64bit]
    [Fri Feb 28 11:24:15 2020] pcieport 0000:02:08.0: PCI bridge to [bus 0b]
    [Fri Feb 28 11:24:15 2020] pcieport 0000:02:08.0:   bridge window [io  0xa000-0xafff]
    [Fri Feb 28 11:24:15 2020] pcieport 0000:02:08.0:   bridge window [mem 0x93900000-0x93cfffff]
    [Fri Feb 28 11:24:15 2020] pcieport 0000:02:08.0:   bridge window [mem 0x91000000-0x911fffff 64bit pref]
    [Fri Feb 28 11:24:15 2020] pciDriver 0000:0b:00.0:  probe for device 000b:0000
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_probe : Found uTCA AMC board at 0000:0b:00.0
    [Fri Feb 28 11:24:15 2020] pciDriver 0000:0b:00.0: enabling device (0000 -> 0002)
    [Fri Feb 28 11:24:15 2020] pciDriver 0000:0b:00.0: Creating device name fpga-8
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_probe_irq : Registered Interrupt Handler at pin 1, line 255, IRQ 16
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_probe : pcidriver_probe: user_refcount = 1
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_open : pcidriver_open: user_refcount = 2
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap : Entering mmap
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap_pci : Entering mmap_pci
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap : Entering mmap
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap_pci : Entering mmap_pci
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap : Entering mmap
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap_pci : Entering mmap_pci
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap : Entering mmap
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap_kmem : Entering mmap_kmem
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap_kmem : Got kmem_entry with id: 0
    [Fri Feb 28 11:24:15 2020] pciDriver - pcidriver_mmap_kmem : Mapping address 44ba00000 / PFN 0044ba00

Device simple readout
'''''''''''''''''''''

If the driver is working ok, you can read/write to/from device registers
by using a simple program called ``regAccess``. This is shipped with
`FPGA PCIe driver`_ and is located at ``tests/pcie/bin``. From then
HALCS source tree you can located it at ``foreign/pcie-driver/tests/pcie/bin``.

The usage is as follows:

.. code-block:: bash
  :linenos:

    Simple Register Access to FPGA
    Usage: ./bin/regAccess [options]
      -h  --help                           Display this usage information
      -b  --devicefile <Device File>       Device File
      -v  --verbose                        Verbose output
      -r  --read                           Perform read access
      -w  --write                          Perform write access
      -n  --barno <BAR number = [0|2|4]    Bar Number
      -a  --address <FPGA Address in hex>  Address to read/write
      -d  --data <FPGA Data in hex>        Data to write to FPGA

In that case, if we want to read a register from board located at slot 1,
at address 0x0 from BAR 4, which is the Wishbone register space in most cases,
you could issue the following:

.. code-block:: bash
  :linenos:

    $ ./bin/regAccess \
        --devicefile /dev/fpga-1
        --read \
        --barno 4 \
        --address 0x0
    0x0000AA55

To write a value to register address ``0x0`` you could issue:

.. code-block:: bash
  :linenos:

    $ ./bin/regAccess \
        --devicefile /dev/fpga-1
        --write \
        --barno 4 \
        --address 0x0 \
        --data 0xAA55AA55
    0xAA55AA55

SDB readout
'''''''''''

On gateware projects that support SDB you can read its contents by
using the ``sdb-read-lnls`` program located at ``foreign/libsdbfs/tools``.

The usage is as follows:

.. code-block:: bash
  :linenos:

    sdb-read-lnls: Use: "sdb-read-lnls [options] <image-file> [<file>]
       -l          long listing (like ls -l)
       -v          verbose
       -e <num>    entry point offset

To read the SDB information from a board located at slot 11, from starting address
``0x0``, you could issue:

.. code-block:: bash
  :linenos:

    $ sdb-read-lnls -l -e 0x0 /dev/fpga-11
    INFO : [20-03-31 11:46:59] [ll_io] Ops set
    INFO : [20-03-31 11:46:59] [ll_io] Created instance of llio
    INFO : [20-03-31 11:46:59] [ll_io_pcie] Opened PCIe device located at /dev/fpga-11
    sdb-read-lnls: listing format is to be defined
    0000000000000651:e6a542c9 @ 00000000-007fffff WB4-Crossbar-GSI
    0000000000000651:eef0b198 @ 00310000-00310fff WB4-Bridge-GSI
    0000000000000651:e6a542c9 @ 00310000-003107ff   WB4-Crossbar-GSI
    1000000000001215:1bafbf1e @ 00310000-003100ff   LNLS_POS_CALC_REGS
    1000000000001215:12897592 @ 00310100-003101ff   LNLS_BPM_SWAP
    0000000000000651:eef0b198 @ 00320000-0032ffff WB4-Bridge-GSI
    0000000000000651:e6a542c9 @ 00320000-00327fff   WB4-Crossbar-GSI
    1000000000001215:68e3b1af @ 00320000-003200ff   LNLS_FMC250M_REGS
    1000000000001215:2403f569 @ 00321000-003210ff   LNLS_ACOMMON_REGS
    0000000000000651:eef0b198 @ 00322000-00322fff   WB4-Bridge-GSI
    0000000000000651:e6a542c9 @ 00322000-003223ff     WB4-Crossbar-GSI
    1000000000001215:88c67d9c @ 00322000-003220ff     LNLS_ACLK_REGS
    000000000000ce42:123c5443 @ 00322100-003221ff     WB-I2C-Master
    000000000000ce42:e503947e @ 00322200-003222ff     WB-SPI.Control
    000000000000ce42:123c5443 @ 00323000-003230ff   WB-I2C-Master
    000000000000ce42:e503947e @ 00324000-003240ff   WB-SPI.Control
    000000000000ce42:e503947e @ 00325000-003250ff   WB-SPI.Control
    1000000000001215:4519a0ad @ 00330000-00330fff LNLS_BPM_ACQ_CORE
    0000000000000651:eef0b198 @ 00340000-00340fff WB4-Bridge-GSI
    0000000000000651:e6a542c9 @ 00340000-003407ff   WB4-Crossbar-GSI
    1000000000001215:1bafbf1e @ 00340000-003400ff   LNLS_POS_CALC_REGS
    1000000000001215:12897592 @ 00340100-003401ff   LNLS_BPM_SWAP
    0000000000000651:eef0b198 @ 00350000-0035ffff WB4-Bridge-GSI
    0000000000000651:e6a542c9 @ 00350000-00357fff   WB4-Crossbar-GSI
    1000000000001215:68e3b1af @ 00350000-003500ff   LNLS_FMC250M_REGS
    1000000000001215:2403f569 @ 00351000-003510ff   LNLS_ACOMMON_REGS
    0000000000000651:eef0b198 @ 00352000-00352fff   WB4-Bridge-GSI
    0000000000000651:e6a542c9 @ 00352000-003523ff     WB4-Crossbar-GSI
    1000000000001215:88c67d9c @ 00352000-003520ff     LNLS_ACLK_REGS
    000000000000ce42:123c5443 @ 00352100-003521ff     WB-I2C-Master
    000000000000ce42:e503947e @ 00352200-003522ff     WB-SPI.Control
    000000000000ce42:123c5443 @ 00353000-003530ff   WB-I2C-Master
    000000000000ce42:e503947e @ 00354000-003540ff   WB-SPI.Control
    000000000000ce42:e503947e @ 00355000-003550ff   WB-SPI.Control
    1000000000001215:4519a0ad @ 00360000-00360fff LNLS_BPM_ACQ_CORE
    0000000000000651:eef0b198 @ 00370000-00370fff WB4-Bridge-GSI
    0000000000000651:e6a542c9 @ 00370000-003707ff   WB4-Crossbar-GSI
    000000000000ce42:8a5719ae @ 00370000-003700ff   CERN_SIMPLE_UART
    0000000000000651:35aa6b95 @ 00370100-003701ff   GSI_GPIO_32
    0000000000000651:35aa6b95 @ 00370200-003702ff   GSI_GPIO_32
    000000000000ce42:fdafb9dd @ 00370300-0037030f   CERN_TICS_COUNTER
    1000000000001215:51954750 @ 00380000-003800ff LNLS_AFCDIAG
    1000000000001215:bcbb78d2 @ 00390000-003903ff LNLS_TRIGGER_IFACE
    1000000000001215:84b6a5ac @ 00400000-004003ff LNLS_TRIGGER_MUX
    1000000000001215:84b6a5ac @ 00410000-004103ff LNLS_TRIGGER_MUX
    1000000000001215:7f9e3377 @ 00420000-00420fff LNLS_ACQ_CORE_PM
    1000000000001215:7f9e3377 @ 00430000-00430fff LNLS_ACQ_CORE_PM
    1000000000001215:84b6a5ac @ 00440000-004403ff LNLS_TRIGGER_MUX
    1000000000001215:84b6a5ac @ 00450000-004503ff LNLS_TRIGGER_MUX
    repo-url: https://github.com/lnls-dig/bpm-gw.git
    synthesis-name: bpm-gw-sr-siriu+
      commit-id: 4bf1f18fedb7a9694e8d5a80a859bc4e
      tool-name: VIVADO
      tool-version: 0x00020183
      build-date: 20191206
      build-user: LRusso
    synthesis-name: dsp-cores+
      commit-id: 31d42c16f00e75e3fa3d52ef2951ba71
    synthesis-name: general-cores+
      commit-id: b056d1983e92c723a20c78eecebea2cb
    synthesis-name: infra-cores+
      commit-id: dd80bd238f200c16e77f4125e31bc160
    INFO : [20-03-31 11:46:59] [ll_io_pcie] Closed PCIe device located at /dev/fpga-11
    INFO : [20-03-31 11:46:59] [ll_io] Ops unset

Malamute application
''''''''''''''''''''

If the board was successfully detected, the next thing to check is
the state of the *Malamute* daemon configuration file and state.

The daemon state can be check with the following command:

.. code-block:: bash

    $ sudo systemctl status malamute

And the output should be something as the following:

.. _malamute-systemd-status:

.. code-block:: bash
  :emphasize-lines: 3,6

    * malamute.service - malamute service
       Loaded: loaded (/usr/lib/systemd/system/malamute.service; enabled; vendor preset: disabled)
       Active: active (running) since Tue 2020-03-10 12:24:52 -03; 3s ago
     Main PID: 18844 (malamute)
       CGroup: /system.slice/malamute.service
               └─18844 /usr/bin/malamute -f /usr/etc/malamute/malamute.cfg

    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 705 address='8fd66a4a-7acd-79b8-fb12-1c6d80f61121' - closed connection
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 705 address='8fd66a4a-7acd-79b8-fb12-1c6d80f61121' - de-registering
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 716 address='469b0b4b-e012-2366-0693-ccd4e4391b81' - registering
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 713 address='a2fb9c93-6d13-eb32-1ce2-1f7c3076ee54' - closed connection
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 713 address='a2fb9c93-6d13-eb32-1ce2-1f7c3076ee54' - de-registering
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 715 address='367d4945-8da9-9c22-658d-497016a61ca2' - closed connection
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 715 address='367d4945-8da9-9c22-658d-497016a61ca2' - de-registering
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 717 address='HALCS6:DEVIO:INIT0' - registering
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 716 address='469b0b4b-e012-2366-0693-ccd4e4391b81' - closed connection
    Mar 10 12:24:53 ia-testrabpmco-iocsrv malamute[18844]: client 716 address='469b0b4b-e012-2366-0693-ccd4e4391b81' - de-registering

The daemon should be in a *running* state as shown above and highlighted at the
**Active:** line. Also, the configuration file in use by the daemon is shown
after the `-f` option shown above and highlighted at the **/usr/bin/malamute**
line.

If the application is not in a *running* state you can start it with:

.. code-block:: bash

    $ sudo systemctl status malamute

Be sure that Malamute is *enabled* to start automatically. You can
check if it is with:

.. code-block:: bash

    $ sudo systemctl is-enabled malamute

And it should output:

.. code-block:: bash

    enabled

If the output is not *enabled* you can enable it with:

.. code-block:: bash

    $ sudo systemctl enable malamute

Lastly, the configuration file contents used by Malamute
(the pathname given after the `-f` option) can be shown with:

.. code-block:: bash

    $ cat /usr/etc/malamute/malamute.cfg

And it should show something like:

.. code-block:: bash

    #   Malamute configuration

    #   Apply to the whole broker
    server
        timeout = 10000     #   Client connection timeout, msec
        background = 0      #   Run as background process
        workdir = .         #   Working directory for daemon
        verbose = 0
        auth
            verbose = 0
            plain =

    #   Apply to the Malamute service
    mlm_server
        security
            mechanism = null
        echo = binding Malamute service to 'ipc:///tmp/malamute'
        bind
            endpoint = ipc:///tmp/malamute
        service
            queue
                size-limit = max     # Maximum size of service queue (in bytes)
                size-warn = max      # Warn if a service queue reaches this size
        mailbox
            size-limit = max        # Maximum mailbox size (integer or "max")
            size-warn = max         # Warn if a mailbox reaches this size

HALCS application
'''''''''''''''''

The HALCS application state itself can also be checked with ``systemctl``
commands. In order to do that, we must figure it out the correct HALCS
instance number.

For PCIe devices, it is given by the formula:

.. math::

    HALCS\_instance\_number = 2 * <board\_slot\_number> - 1

In which :math:`board slot number` is the slot in which the board
is plugged in. It usually is in the range :math:`[1...number\_of\_slots]`.

For other device types it could anything and it is application-specific.

Once the instance number is known, you can check its state with:

.. code-block:: bash

    $ sudo systemctl status halcs-be@<HALCS_instance_number>

For example, in a system with a PCIe board using HALCS in slot 6, you
would run:

.. code-block:: bash

    $ sudo systemctl status halcs-be@11

And the output should be something like:

.. code-block:: bash
  :emphasize-lines: 3,7

    * halcs-be@11.service - HALCS server instance 11
       Loaded: loaded (/etc/systemd/system/halcs-be@.service; enabled; vendor preset: disabled)
       Active: active (running) since Tue 2020-03-10 12:24:52 -03; 1h 58min ago
      Process: 18846 ExecStartPre=/bin/mkdir -p ${HALCS_LOG_DIR} (code=exited, status=0/SUCCESS)
     Main PID: 18852 (halcsd)
       CGroup: /system.slice/system-halcs\x2dbe.slice/halcs-be@11.service
               └─18852 /usr/local/bin/halcsd -f /usr/local/etc/halcs/halcs.cfg -n be -t pcie -i 11 -b ipc:///tmp/malamute -l /var/log/halcs

    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [hutils:utils] CFG hints hash key: "board11/halcs0", ...
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [hutils:utils] CFG hints hash key: "board11/halcs1", ...
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [hutils:utils] CFG hints hash key: "board12/halcs0", ...
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [hutils:utils] CFG hints hash key: "board12/halcs1", ...
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [halcsd] Dev_id parameter was set, but Dev_entry was not.
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: Defaulting Dev_entry to /dev/fpga-<dev_id>
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [halcsd] Dev_entry parameter was set to /dev/fpga-6.
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [halcsd] Dev_id parameter was set to 6.
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: INFO : [20-03-10 12:24:52] [halcsd] Smio_id parameter was set to 0.
    Mar 10 12:24:52 ia-testrabpmco-iocsrv halcsd[18852]: I: 20-03-10 12:24:52 My address is 'HALCS6:DEVIO:AFC_DIAG0'

The important lines here are the same ones as the :ref:`Systemd Malamute Status <malamute-systemd-status>`,
the daemon status and the command-line executed.

With the HALCS application checked and in a *running* state everything should be
in place for running client applications and communicating with this HALCS
instance. For more information refer to :ref:`halcs-tools`.

Lastly, the configuration file contents used by HALCS
(the pathname given after the `-f` option) can be shown with:

.. code-block:: bash

    $ cat /usr/local/etc/halcs/halcs.cfg

And it should show something like:

.. code-block:: bash

    #   HALCS configuration file

    # Device manager configurations
    dev_mngr
        broker
            bind = tcp://127.0.0.1:9999
        log
            dir = /media/remote_logs
            filename = dev_mngr.log
        verbose = 1             # Ask for a trace
        daemonize = no          # Ask for daemonize process (options are: yes or no)
        workdir = .             #   Working directory for daemon
        spawn_broker = no       # Ask to spawn broker (options are: yes or no)

    # Device I/O configurations
    dev_io
        board1
            halcs0
                dbe
                    board_type = afcv3.1
                afe
                    bind =
                    proto =
            halcs1
                dbe
                    board_type = afcv3.1
                afe
                    bind =
                    proto =

        ...

The configuration file would have an entry for each board slot *board<board_slot>*,
and two entries for each instance of that slot *halcs<instance_number>*.
