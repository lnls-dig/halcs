.. _halcs-tools:

HALCS Tools
===========

SMIO modules available
----------------------

HALCS provides a set of example tools that can be used with the provided
*SMIOs*. As of this version the following *SMIOs* are present and known
to HALCS:

Acquisition (acq)
    General-purpose acquisition module, supporting multiple channels,
    generic number of pre-trigger and post-trigger samples, along with
    external and data threshold triggers.

AFC Diagnostics (afc_diag)
    Specific module for collecting sensor data from the AFC [#afc]_
    carrier board.

AFC management (afc_mgmt)
    Specific module for controlling the AFC clock generator.

AFC Timing (afc_timing)
    Specific module for controlling and monitoring all the
    functions provided by the AFC timing receiver gateware.

DSP (dsp)
    Specific module for controlling the DSP module for the LNLS BPM [#bpm]_
    project

FMC ADC 100MSPS 4 Channel (fmc100m_4ch)
    Module for controlling and monitoring the FMC ADC 100MSPS 14 bits,
    4 Channel [#fmcadc100m]_.

FMC ADC 130MSPS 4 Channel (fmc130m_4ch)
    Module for controlling and monitoring the FMC ADC 130MSPS 16 bits,
    4 Channel [#fmcadc130m]_.

FMC ADC 250MSPS 4 Channel (fmc250m_4ch)
    Module for controlling and monitoring the FMC ADC 250MSPS 14 bits,
    4 Channel [#fmcadc250m]_.

FMC Active Clock (fmc_active_clk)
    Generic module containing drivers for the AD9510, Si57x. This is commonly
    used with ``fmc130m_4ch`` and ``fmc250m_4ch`` modules.

FMC ADC Common (fmc_adc_common)
    Generic module containing drivers for trigger components and LEDs.
    This is commonly used with ``fmc130m_4ch`` and ``fmc250m_4ch`` modules.

FMC ADC Pico 1MSPS 4 Channel (fmcpico1m_4ch)
    Specific module for controlling and monitoring the FMC ADC Pico 1MSPS,
    4 Channel [#fmcpico1m_4ch]_

Initialization (init)
    This is a meta-module used so clients can be sure the whole HALCS
    instance has initialized and is ready to receive commands.

LNLS BPM RFFE with BSMP protocol (rffe)
    Module used for controlling and monitoring the LNLS BPM RFFE
    project [#rffe]_. This module implements the first protocol used by
    the RFFE, which is based on a binary RPC protocol called BSMP [#bsmp]_.

LNLS BPM RFFE with SCPI protocol (rffe_scpi)
    Module used for controlling and monitoring the LNLS BPM RFFE
    project. This module implements the second protocol used
    by the RFFE, which is based on an ASCII text protocol (i.e., SCPI).

LNLS BPM Switching (swap)
    Module for controlling the switching scheme used by the
    LNLS BPM electronics. This basically configures the clock
    generation parameters for the LNLS BPM RFFE.

MTCA Trigger Interface (trigger_iface)
    Module for controlling and monitoring the trigger interface used
    by AFC. This controls the buffer directions, debouncing,
    length and trigger counter.

Trigger Multiplexer (trigger_mux)
    Generic module used to multiplex a set of incoming triggers from
    different sources and a set of outgoing triggers to different
    destination.

.. [#afc] |AFC Page|_

.. _`AFC Page`: https://ohwr.org/project/afc/wikis/home
.. |AFC Page| replace:: https://ohwr.org/project/afc/wikis/home

.. [#bpm] |BPM Page|_

.. _`BPM Page`: https://github.com/lnls-dig/bpm-gw
.. |BPM Page| replace:: https://github.com/lnls-dig/bpm-gw

.. [#fmcadc100m] |FMCADC100M Page|_

.. _`FMCADC100M Page`: https://www.ohwr.org/project/fmc-adc-100m14b4cha/wikis/home
.. |FMCADC100M Page| replace:: https://www.ohwr.org/project/fmc-adc-100m14b4cha/wikis/home

.. [#fmcadc130m] |FMCADC130M Page|_

.. _`FMCADC130M Page`: https://www.ohwr.org/project/fmc-adc-130m-16b-4cha/wikis/home
.. |FMCADC130M Page| replace:: https://www.ohwr.org/project/fmc-adc-130m-16b-4cha/wikis/home

.. [#fmcadc250m] |FMCADC250M Page|_

.. _`FMCADC250M Page`: https://www.ohwr.org/project/fmc-adc-250m-16b-4cha/wikis/home
.. |FMCADC250M Page| replace:: https://www.ohwr.org/project/fmc-adc-250m-16b-4cha/wikis/home

.. [#fmcpico1m_4ch] |FMCPICO1M_4CH Page|_

.. _`FMCPICO1M_4CH Page`: https://www.caenels.com/products/fmc-pico-1m4
.. |FMCPICO1M_4CH Page| replace:: https://www.caenels.com/products/fmc-pico-1m4

.. [#rffe] |RFFE Page|_

.. _`RFFE Page`: https://www.ohwr.org/project/bpm-rffe/wikis/home
.. |RFFE Page| replace:: https://www.ohwr.org/project/bpm-rffe/wikis/home

.. [#bsmp] |BSMP Page|_

.. _`BSMP Page`: https://github.com/lnls-sirius/libbsmp
.. |BSMP Page| replace:: https://github.com/lnls-sirius/libbsmp

Example Tools
-------------

Inside the ``examples/src`` directory under the project source tree
you can find tools to control and monitor most of the mentioned modules.
If HALCS was installed via ``make`` then the examples will be available
at the system ``PATH``. Otherwise, you will need to use the examples build
directory.

If in doubt which build system was used you can always build the examples
by using one of the following options:

``Make``
    With ``make`` you can build the examples from the root of HALCS with:

.. code-block:: bash
  :linenos:

  make examples && sudo make examples_install

``Gradle``
    With ``gradle`` you can build the examples from the root of HALCS with:

.. code-block:: bash
  :linenos:

  ./gradlew :examples:assemble

Acquisition
'''''''''''

The acquisition example, also called ``acq``, can be used if the ``wb_acq``
module is instantiated by the FPGA gateware. If this is available, HALCS will
spawn the *SMIO* ``acq`` module to interface with ``wb_acq``. This module
uses the ``SDB`` **vendor id** 0x1000000000001215ULL and **device id**
0x4519a0ad.

The summary of all command-line ``acq`` options are below:

.. code-block:: bash
  :linenos:

    HALCSD Acquisition Utility
    Usage: ./examples/build/exe/acq/production/afcv3_1/acq [options]

      -h  --help                            Display this usage information
      -b  --brokerendp <Broker endpoint>    Broker endpoint
      -v  --verbose                         Verbose output
      -o  --boardslot <Board slot number = [1-12]>
                                            Board slot number
      -s  --halcsnumber <HALCS number = [0|1]>
                                            HALCS instance number
      -c  --channumber <Channel>            Channel number
                                                <Channel> is dependant on the FPGA firmware
      -n  --numsamples <Number of samples>  Number of samples or number of pre-trigger
                                                samples if using triggered mode
      -p  --postsamples <Number of samples> Number of post-trigger samples
                                                if using triggered mode
      -t  --numshots <Number of shots>      Number of shots
                                                if using triggered mode
      -g  --triggertype <Trigger type>      Trigger type. 0 is immediate, 1 is external trigger,
                                                2 is data-driver trigger and 3 is software trigger
      -e  --datatriggerthres <Trigger threshold>
                                            Trigger threshold level for data-driven trigger
      -l  --datatriggerpol <Trigger polarity>
                                            Trigger polarity. 0 is positive edge, 1 is negative edge
      -z  --datatriggersel <Trigger data selection>
                                            Trigger data selection within one channel to compare for threshold level
      -i  --datatriggerfilt <Trigger hysteresis>
                                            Trigger hysteresis value for data trigger detection
      -r  --datatriggerchan <Trigger channel>
                                            Trigger data channel to be compared with the threshold level
      -d  --triggerdelay <Trigger delay>    Trigger delay for all triggers
      -f  --filefmt <Output format = [0 = text | 1 = binary]>
                                            Output format
      -a  --newcq <Trigger new acquisition = [0 = no | 1 = yes]

An example of acquiring 10 samples from acquisition channel 0, from a MTCA
board located at slot 6, FMC 1, broker endpoint ``ipc:///tmp/malamute``,
could be issued like the following:

.. code-block:: bash
  :linenos:

    $ ./examples/build/exe/acq/production/afcv3_1/acq \
        -b ipc:///tmp/malamute \
        --boardslot 6 \
        --halcsnumber 1 \
        --channumber 0 \
        --numsamples 10
      17	      -3	      10	       5
      16	       0	       7	       4
      15	      -2	       7	       5
      15	       1	      10	       1
      13	      -4	       7	       5
       8	      -1	       8	       9
      13	       0	       5	       7
      12	      -4	       7	       7
      19	      -1	      10	       6
      17	       0	      10	       6

Each column in the above example is a different data channel from acquisition
channel 0. The way the data channels are organized is dependent on the FPGA
gateware. In the example shown, each data channel is a 16-bit, signed number,
from a 100MSPS ADC board.

A more complex example of acquiring 2 shots, 10 pre-trigger samples each,
0 post-trigger samples each, with a data-driven trigger, and data threshold
of 1000 is shown below:

.. code-block:: bash
  :linenos:
  :emphasize-lines: 20

    $ ./examples/build/exe/acq/production/afcv3_1/acq \
        -b ipc:///tmp/malamute \
        --boardslot 6 \
        --halcsnumber 1 \
        --channumber 0 \
        --numsamples 10 \
        --postsamples 0 \
        --numshots 2 \
        --triggertype 2 \
        --datatriggerthres 1000
       409	       391	       779	       765
      1490	      1513	       735	       772
      2156	      2150	      1475	      1527
      1337	      1281	      1809	      1817
       344	       295	      1197	      1183
       776	       778	       630	       640
      1926	      1944	      1009	      1057
      1965	      1941	      1727	      1763
       824	       762	      1644	      1639
       319	       303	       883	       872
      1045	      1059	       631	       649
      2089	      2107	      1192	      1239
      1765	      1727	      1806	      1832
       595	       528	      1492	      1482
       438	       427	       752	       742
      1550	      1575	       770	       801
      2150	      2144	      1513	      1562
      1274	      1221	      1798	      1805
       324	       290	      1161	      1145
       823	       838	       628	       635

Note the crossing point in which the sample goes from a value below
1000 to a value above 1000 at line 20. Each 10 samples in the above example
correspond to one shot.

FMC ADC 100MSPS 4 Channel
'''''''''''''''''''''''''

The FMC ADC 100m example, also called ``fmc100m_4ch_ctl``, can be used if the
``wb_fmc100m_4ch`` module is instantiated by the FPGA gateware. If this is available,
HALCS will spawn the *SMIO* ``fmc100m_4ch`` module to interface with ``wb_fmc100m_4ch``.
This module uses the ``SDB`` **vendor id** 0x000000000000CE42ULL and **device id**
0x00000608.

The summary of all command-line ``acq`` options are below:

.. code-block:: bash
  :linenos:

    HALCSD FMC ADC 100M control utility
    Usage: ./examples/build/exe/fmc100m_4ch_ctl/production/afcv3_1/fmc100m_4ch_ctl [options]

      -h  --help                           Display this usage information
      -b  --brokerendp <Broker endpoint>   Broker endpoint
      -v  --verbose                        Verbose output
      -o  --boardslot <Board slot number = [1-12]>
      -s  --halcsnumber <HALCS number = [0|1]> HALCS number
                                           Board slot number
      -t  --test_pattern <Pattern>         Test pattern
      -e  --test_pattern_en <Enable>       Enable test pattern
      -c  --channel <Channel=[0-3]>        Channel to apply operation. Select 4, for all channel
      -r  --ssr <SSR option>               Select SSR option
      -m  --termination <Term=[0|1]>       Select 50-ohm termination

An example of configuring the ADC to output a test pattern :math:`10101010101010`
(:math:`10922`) from a MTCA board located at slot 6, FMC 1, broker endpoint
``ipc:///tmp/malamute``, could be issued like the following:

.. code-block:: bash
  :linenos:

    $ ./examples/build/exe/fmc100m_4ch_ctl/production/afcv3_1/fmc100m_4ch_ctl \
        -b ipc:///tmp/malamute \
        --boardslot 6 \
        --halcsnumber 1 \
        --test_pattern_en 1 \
        --test_pattern 10922
    [client:fmc100m_4ch_ctl]: test_pattern = 0x00002AAA
    [client:fmc100m_4ch_ctl]: test_pattern_en = 0x00000001
    [client:fmc100m_4ch_ctl]: FMC channel #0 status = 0x0000AAA8
    [client:fmc100m_4ch_ctl]: FMC channel #1 status = 0x0000AAA8
    [client:fmc100m_4ch_ctl]: FMC channel #2 status = 0x0000AAA8
    [client:fmc100m_4ch_ctl]: FMC channel #3 status = 0x0000AAA8

The information displayed after executing the command gives you a feedback
of the test pattern set, which was :math:`10101010101010` in binary,
:math:`10922` in decimal or :math:`0x2AAA` in hexadecimal. It also shows
the raw ADC data from all 4 ADC channels.

.. Note:: Keep in mind that we are using a 14-bit ADC (FMC ADC 100M 14-bit
            4 Channel). To make acquisition easier, the ADC in the example,
            LTC2174, has a mode in which it sends a 16-bit data with the 2
            LSB fixed to 0. This make acquisition easier, as we can treat the
            date as 2 bytes. However, it's important to know that we can only
            have control over the 14 MSB, so the test pattern we set refers
            to those 14 MSB, as well. As such, when setting the :math:`0x2AAA`
            test pattern, the expected acquired data would be :math:`0x2AAA << 2`,
            which is equal to :math:`0xAAA8` as shown in the example.

A more complex example of setting the ADC to acquire real data (no test pattern),
100 mVpp ADC range (``--ssr`` option 2), for all ADC channels (``--channel`` option
4), with 50 `ohm` termination enabled is shown below:

.. code-block:: bash
  :linenos:

    $ ./examples/build/exe/fmc100m_4ch_ctl/production/afcv3_1/fmc100m_4ch_ctl \
        -b ipc:///tmp/malamute \
        --boardslot 6 \
        --halcsnumber 1 \
        --test_pattern_en 0 \
        --test_pattern 1 \
        --ssr 2 \
        --channel 4 \
        --termination 1
    [client:fmc100m_4ch_ctl]: test_pattern_en = 0x00000000
    [client:fmc100m_4ch_ctl]: termination = 0x00000001
    [client:fmc100m_4ch_ctl]: channel = 0x00000004
    [client:fmc100m_4ch_ctl]: ssr = 0x00000002
    [client:fmc100m_4ch_ctl]: ssr option 0x00000002, ssr bits 0x0000002B set for channel #0
    [client:fmc100m_4ch_ctl]: ssr option 0x00000002, ssr bits 0x0000002B set for channel #1
    [client:fmc100m_4ch_ctl]: ssr option 0x00000002, ssr bits 0x0000002B set for channel #2
    [client:fmc100m_4ch_ctl]: ssr option 0x00000002, ssr bits 0x0000002B set for channel #3
    [client:fmc100m_4ch_ctl]: FMC channel #0 status = 0x0000013C
    [client:fmc100m_4ch_ctl]: FMC channel #1 status = 0x0000014C
    [client:fmc100m_4ch_ctl]: FMC channel #2 status = 0x0000FD3C
    [client:fmc100m_4ch_ctl]: FMC channel #3 status = 0x0000FE00

And a subsequent acquisition of the ADCs in that configuration could be done with:

.. code-block:: bash
  :linenos:

    $ ./examples/build/exe/acq/production/afcv3_1/acq \
        -b ipc:///tmp/malamute \
        --boardslot 6 \
        --halcsnumber 1 \
        --channumber 0 \
        --numsamples 10
    -193	    -216	    -216	    -200
    -139	    -167	    -177	    -166
     -83	    -112	    -124	    -110
     -19	     -44	     -63	     -53
      46	      29	       7	      13
     112	      91	      74	      77
     161	     137	     124	     136
     192	     166	     169	     180
     205	     180	     192	     203
     205	     169	     192	     198
