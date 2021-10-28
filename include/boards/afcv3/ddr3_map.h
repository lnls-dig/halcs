#ifndef _DDR3_MAP_H_
#define _DDR3_MAP_H_

#include "boards/afcv3/ddr3_defs.h"
#include "boards/afcv3/acq_chan.h"
#include "varg_macros.h"

/* Does the acquisition channel gets a memory region? */
#define DDR3_MEM_BOOL(MEM_SIZE)         IF(MEM_SIZE > 0)(1, 0)

/* Start of the acquisition memory address */
#define DDR3_ACQ_START_ADDR             0x0

/* Dummy region
 * Size: 0 DDR3 regions */
#define DDR3_DUMMY_INIT0_MEM_SIZE       0

#define DDR3_DUMMY_INIT0_MEM_BOOL       DDR3_MEM_BOOL(DDR3_DUMMY_INIT0_MEM_SIZE)
#define DDR3_DUMMY_INIT0_START_ADDR     (DDR3_ACQ_START_ADDR)
#define DDR3_DUMMY_INIT0_END_ADDR       (DDR3_DUMMY_INIT0_START_ADDR + DDR3_DUMMY_INIT0_MEM_SIZE*__MEM_REGION_SIZE)

/************************ Acquistion 0 Channel Parameters **************/

/* ADC 0 (shares the same memory space as the ADCSWAP0)
 * Size: 2 DDR3 regions */
#define DDR3_ADC0_MEM_SIZE              2

#define DDR3_ADC0_MEM_BOOL              DDR3_MEM_BOOL(DDR3_ADC0_MEM_SIZE)
#define DDR3_ADC0_START_ADDR            (DDR3_DUMMY_INIT0_START_ADDR)
#define DDR3_ADC0_END_ADDR              (DDR3_ADC0_START_ADDR + DDR3_ADC0_MEM_SIZE*__MEM_REGION_SIZE)

/* ADCSWAP 0 (shares the same memory space as the ADC0)
 * Size: 2 DDR3 regions */
#define DDR3_ADCSWAP0_MEM_SIZE          2

#define DDR3_ADCSWAP0_MEM_BOOL          DDR3_MEM_BOOL(DDR3_ADCSWAP0_MEM_SIZE)
#define DDR3_ADCSWAP0_START_ADDR        (DDR3_DUMMY_INIT0_START_ADDR)
#define DDR3_ADCSWAP0_END_ADDR          (DDR3_ADCSWAP0_START_ADDR + DDR3_ADCSWAP0_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 1/2 (shares the same memory space as the DUMMY00)
 * Size: 1 DDR3 regions */
#define DDR3_MIXIQ0_MEM_SIZE            1

#define DDR3_MIXIQ0_MEM_BOOL            DDR3_MEM_BOOL(DDR3_MIXIQ0_MEM_SIZE)
#define DDR3_MIXIQ0_START_ADDR          (DDR3_ADCSWAP0_END_ADDR)
#define DDR3_MIXIQ0_END_ADDR            (DDR3_MIXIQ0_START_ADDR + DDR3_MIXIQ0_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 3/4 (shares the same memory space as the MIXIQ0)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY00_MEM_SIZE           0

#define DDR3_DUMMY00_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY00_MEM_SIZE)
#define DDR3_DUMMY00_START_ADDR         (DDR3_ADCSWAP0_END_ADDR)
#define DDR3_DUMMY00_END_ADDR           (DDR3_DUMMY00_START_ADDR + DDR3_DUMMY00_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 1/2 (shares the same memory space as the DUMMY10 and TBTAMP0)
 * Size: 1 DDR3 regions */
#define DDR3_TBTDECIMIQ0_MEM_SIZE       2

#define DDR3_TBTDECIMIQ0_MEM_BOOL       DDR3_MEM_BOOL(DDR3_TBTDECIMIQ0_MEM_SIZE)
#define DDR3_TBTDECIMIQ0_START_ADDR     (DDR3_DUMMY00_END_ADDR)
#define DDR3_TBTDECIMIQ0_END_ADDR       (DDR3_TBTDECIMIQ0_START_ADDR + DDR3_TBTDECIMIQ0_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 3/4 (shares the same memory space as the TBTDECIMIQ0 and TBTAMP0)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY10_MEM_SIZE           0

#define DDR3_DUMMY10_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY10_MEM_SIZE)
#define DDR3_DUMMY10_START_ADDR         (DDR3_DUMMY00_END_ADDR)
#define DDR3_DUMMY10_END_ADDR           (DDR3_DUMMY10_START_ADDR + DDR3_DUMMY10_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 0 AMP (shares the same memory space as the TBTDECIMIQ0 and DUMMY10)
 * Size: 2 DDR3 regions */
#define DDR3_TBTAMP0_MEM_SIZE           2

#define DDR3_TBTAMP0_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTAMP0_MEM_SIZE)
#define DDR3_TBTAMP0_START_ADDR         (DDR3_DUMMY00_END_ADDR)
#define DDR3_TBTAMP0_END_ADDR           (DDR3_TBTAMP0_START_ADDR + DDR3_TBTAMP0_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 0 PHA (shares the same memory space as the TBTDECIMIQ0 and DUMMY10)
 * Size: 2 DDR3 regions */
#define DDR3_TBTPHA0_MEM_SIZE           2

#define DDR3_TBTPHA0_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPHA0_MEM_SIZE)
#define DDR3_TBTPHA0_START_ADDR         (DDR3_DUMMY00_END_ADDR)
#define DDR3_TBTPHA0_END_ADDR           (DDR3_TBTPHA0_START_ADDR + DDR3_TBTPHA0_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 0 POS (shares the same memory space as the TBTDECIMIQ0 and DUMMY10)
 * Size: 0 DDR3 regions */
#define DDR3_TBTPOS0_MEM_SIZE           2

#define DDR3_TBTPOS0_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPOS0_MEM_SIZE)
#define DDR3_TBTPOS0_START_ADDR         (DDR3_DUMMY00_END_ADDR)
#define DDR3_TBTPOS0_END_ADDR           (DDR3_TBTPOS0_START_ADDR + DDR3_TBTPOS0_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 1/2 (shares the same memory space as the DUMMY20 and FOFBAMP0)
 * Size: 1 DDR3 regions */
#define DDR3_FOFBDECIMIQ0_MEM_SIZE      2

#define DDR3_FOFBDECIMIQ0_MEM_BOOL      DDR3_MEM_BOOL(DDR3_FOFBDECIMIQ0_MEM_SIZE)
#define DDR3_FOFBDECIMIQ0_START_ADDR    (DDR3_TBTPOS0_END_ADDR)
#define DDR3_FOFBDECIMIQ0_END_ADDR      (DDR3_FOFBDECIMIQ0_START_ADDR + DDR3_FOFBDECIMIQ0_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 3/4 (shares the same memory space as the FOFBDECIMIQ0 and FOFBAMP0)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY20_MEM_SIZE           0

#define DDR3_DUMMY20_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY20_MEM_SIZE)
#define DDR3_DUMMY20_START_ADDR         (DDR3_TBTPOS0_END_ADDR)
#define DDR3_DUMMY20_END_ADDR           (DDR3_DUMMY20_START_ADDR + DDR3_DUMMY20_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 0 AMP
 * Size: 2 DDR3 regions */
#define DDR3_FOFBAMP0_MEM_SIZE          2

#define DDR3_FOFBAMP0_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBAMP0_MEM_SIZE)
#define DDR3_FOFBAMP0_START_ADDR        (DDR3_TBTPOS0_END_ADDR)
#define DDR3_FOFBAMP0_END_ADDR          (DDR3_FOFBAMP0_START_ADDR + DDR3_FOFBAMP0_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 0 PHA (shares the same memory space as the FOFBDECIMIQ12 and FOFBDECIMIQ34)
 * Size: 2 DDR3 regions */
#define DDR3_FOFBPHA0_MEM_SIZE          2

#define DDR3_FOFBPHA0_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPHA0_MEM_SIZE)
#define DDR3_FOFBPHA0_START_ADDR        (DDR3_TBTPOS0_END_ADDR)
#define DDR3_FOFBPHA0_END_ADDR          (DDR3_FOFBPHA0_START_ADDR + DDR3_FOFBPHA0_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 0 POS
 * Size: 0 DDR3 regions */
#define DDR3_FOFBPOS0_MEM_SIZE          2

#define DDR3_FOFBPOS0_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPOS0_MEM_SIZE)
#define DDR3_FOFBPOS0_START_ADDR        (DDR3_TBTPOS0_END_ADDR)
#define DDR3_FOFBPOS0_END_ADDR          (DDR3_FOFBPOS0_START_ADDR + DDR3_FOFBPOS0_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT1 1 AMP
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1AMP0_MEM_SIZE        1

#define DDR3_MONIT1AMP0_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1AMP0_MEM_SIZE)
#define DDR3_MONIT1AMP0_START_ADDR      (DDR3_FOFBPOS0_END_ADDR)
#define DDR3_MONIT1AMP0_END_ADDR        (DDR3_MONIT1AMP0_START_ADDR + DDR3_MONIT1AMP0_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT1 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1POS0_MEM_SIZE        1

#define DDR3_MONIT1POS0_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1POS0_MEM_SIZE)
#define DDR3_MONIT1POS0_START_ADDR      (DDR3_MONIT1AMP0_END_ADDR)
#define DDR3_MONIT1POS0_END_ADDR        (DDR3_MONIT1POS0_START_ADDR + DDR3_MONIT1POS0_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT 0 AMP
 * Size: 0 DDR3 regions */
#define DDR3_MONITAMP0_MEM_SIZE         0

#define DDR3_MONITAMP0_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITAMP0_MEM_SIZE)
#define DDR3_MONITAMP0_START_ADDR       (DDR3_MONIT1POS0_END_ADDR)
#define DDR3_MONITAMP0_END_ADDR         (DDR3_MONITAMP0_START_ADDR + DDR3_MONITAMP0_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT 0 POS
 * Size: 0 DDR3 regions */
#define DDR3_MONITPOS0_MEM_SIZE         0

#define DDR3_MONITPOS0_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITPOS0_MEM_SIZE)
#define DDR3_MONITPOS0_START_ADDR       (DDR3_MONITAMP0_END_ADDR)
#define DDR3_MONITPOS0_END_ADDR         (DDR3_MONITPOS0_START_ADDR + DDR3_MONITPOS0_MEM_SIZE*__MEM_REGION_SIZE)

/* End 0 Dummy region
 * Size: 0 DDR3 regions */
#define DDR3_DUMMY_END0_MEM_SIZE        0

#define DDR3_DUMMY_END0_MEM_BOOL        DDR3_MEM_BOOL(DDR3_DUMMY_END0_MEM_SIZE)
#define DDR3_DUMMY_END0_START_ADDR      (DDR3_MONITAMP0_END_ADDR)
#define DDR3_DUMMY_END0_END_ADDR        (DDR3_DUMMY_END0_START_ADDR + DDR3_DUMMY_END0_MEM_SIZE*__MEM_REGION_SIZE)

/************************ Acquistion 1 Channel Parameters **************/

/* ADC 1
 * Size: 2 DDR3 regions */
#define DDR3_ADC1_MEM_SIZE              2

#define DDR3_ADC1_MEM_BOOL              DDR3_MEM_BOOL(DDR3_ADC1_MEM_SIZE)
#define DDR3_ADC1_START_ADDR            (DDR3_DUMMY_END0_START_ADDR)
#define DDR3_ADC1_END_ADDR              (DDR3_ADC1_START_ADDR + DDR3_ADC1_MEM_SIZE*__MEM_REGION_SIZE)

/* ADCSWAP 1
 * Size: 2 DDR3 regions */
#define DDR3_ADCSWAP1_MEM_SIZE          2

#define DDR3_ADCSWAP1_MEM_BOOL          DDR3_MEM_BOOL(DDR3_ADCSWAP1_MEM_SIZE)
#define DDR3_ADCSWAP1_START_ADDR        (DDR3_DUMMY_END0_START_ADDR)
#define DDR3_ADCSWAP1_END_ADDR          (DDR3_ADCSWAP1_START_ADDR + DDR3_ADCSWAP1_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 1/2 (shares the same memory space as the DUMMY01)
 * Size: 1 DDR3 regions */
#define DDR3_MIXIQ1_MEM_SIZE            1

#define DDR3_MIXIQ1_MEM_BOOL            DDR3_MEM_BOOL(DDR3_MIXIQ1_MEM_SIZE)
#define DDR3_MIXIQ1_START_ADDR          (DDR3_ADCSWAP1_END_ADDR)
#define DDR3_MIXIQ1_END_ADDR            (DDR3_MIXIQ1_START_ADDR + DDR3_MIXIQ1_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 3/4 (shares the same memory space as the MIXIQ1)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY01_MEM_SIZE          0

#define DDR3_DUMMY01_MEM_BOOL          DDR3_MEM_BOOL(DDR3_DUMMY01_MEM_SIZE)
#define DDR3_DUMMY01_START_ADDR        (DDR3_ADCSWAP1_END_ADDR)
#define DDR3_DUMMY01_END_ADDR          (DDR3_DUMMY01_START_ADDR + DDR3_DUMMY01_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 1/2 (shares the same memory space as the DUMMY11 and TBTAMP1)
 * Size: 1 DDR3 regions */
#define DDR3_TBTDECIMIQ1_MEM_SIZE       2

#define DDR3_TBTDECIMIQ1_MEM_BOOL       DDR3_MEM_BOOL(DDR3_TBTDECIMIQ1_MEM_SIZE)
#define DDR3_TBTDECIMIQ1_START_ADDR     (DDR3_DUMMY01_END_ADDR)
#define DDR3_TBTDECIMIQ1_END_ADDR       (DDR3_TBTDECIMIQ1_START_ADDR + DDR3_TBTDECIMIQ1_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 3/4 (shares the same memory space as the TBTDECIMIQ1 and TBTAMP1)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY11_MEM_SIZE           2

#define DDR3_DUMMY11_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY11_MEM_SIZE)
#define DDR3_DUMMY11_START_ADDR         (DDR3_DUMMY01_END_ADDR)
#define DDR3_DUMMY11_END_ADDR           (DDR3_DUMMY11_START_ADDR + DDR3_DUMMY11_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 AMP
 * Size: 2 DDR3 regions */
#define DDR3_TBTAMP1_MEM_SIZE           2

#define DDR3_TBTAMP1_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTAMP1_MEM_SIZE)
#define DDR3_TBTAMP1_START_ADDR         (DDR3_DUMMY01_END_ADDR)
#define DDR3_TBTAMP1_END_ADDR           (DDR3_TBTAMP1_START_ADDR + DDR3_TBTAMP1_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 PHA (shares the same memory space as the TBTDECIMIQ1 and DUMMY11)
 * Size: 2 DDR3 regions */
#define DDR3_TBTPHA1_MEM_SIZE           2

#define DDR3_TBTPHA1_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPHA1_MEM_SIZE)
#define DDR3_TBTPHA1_START_ADDR         (DDR3_DUMMY01_END_ADDR)
#define DDR3_TBTPHA1_END_ADDR           (DDR3_TBTPHA1_START_ADDR + DDR3_TBTPHA1_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_TBTPOS1_MEM_SIZE           2

#define DDR3_TBTPOS1_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPOS1_MEM_SIZE)
#define DDR3_TBTPOS1_START_ADDR         (DDR3_DUMMY01_END_ADDR)
#define DDR3_TBTPOS1_END_ADDR           (DDR3_TBTPOS1_START_ADDR + DDR3_TBTPOS1_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 1/2 (shares the same memory space as the DUMMY21 and FOFBAMP1)
 * Size: 1 DDR3 regions */
#define DDR3_FOFBDECIMIQ1_MEM_SIZE      2

#define DDR3_FOFBDECIMIQ1_MEM_BOOL      DDR3_MEM_BOOL(DDR3_FOFBDECIMIQ1_MEM_SIZE)
#define DDR3_FOFBDECIMIQ1_START_ADDR    (DDR3_TBTPOS1_END_ADDR)
#define DDR3_FOFBDECIMIQ1_END_ADDR      (DDR3_FOFBDECIMIQ1_START_ADDR + DDR3_FOFBDECIMIQ1_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 3/4 (shares the same memory space as the FOFBDECIMIQ1 and FOFBAMP1)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY21_MEM_SIZE           2

#define DDR3_DUMMY21_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY21_MEM_SIZE)
#define DDR3_DUMMY21_START_ADDR         (DDR3_TBTPOS1_END_ADDR)
#define DDR3_DUMMY21_END_ADDR           (DDR3_DUMMY21_START_ADDR + DDR3_DUMMY21_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 AMP
 * Size: 2 DDR3 regions */
#define DDR3_FOFBAMP1_MEM_SIZE          2

#define DDR3_FOFBAMP1_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBAMP1_MEM_SIZE)
#define DDR3_FOFBAMP1_START_ADDR        (DDR3_TBTPOS1_END_ADDR)
#define DDR3_FOFBAMP1_END_ADDR          (DDR3_FOFBAMP1_START_ADDR + DDR3_FOFBAMP1_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 PHA (shares the same memory space as the FOFBDECIMIQ1 and DUMMY21)
 * Size: 2 DDR3 regions */
#define DDR3_FOFBPHA1_MEM_SIZE          2

#define DDR3_FOFBPHA1_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPHA1_MEM_SIZE)
#define DDR3_FOFBPHA1_START_ADDR        (DDR3_TBTPOS1_END_ADDR)
#define DDR3_FOFBPHA1_END_ADDR          (DDR3_FOFBPHA1_START_ADDR + DDR3_FOFBPHA1_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_FOFBPOS1_MEM_SIZE          2

#define DDR3_FOFBPOS1_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPOS1_MEM_SIZE)
#define DDR3_FOFBPOS1_START_ADDR        (DDR3_TBTPOS1_END_ADDR)
#define DDR3_FOFBPOS1_END_ADDR          (DDR3_FOFBPOS1_START_ADDR + DDR3_FOFBPOS1_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT1 1 AMP
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1AMP1_MEM_SIZE        1

#define DDR3_MONIT1AMP1_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1AMP1_MEM_SIZE)
#define DDR3_MONIT1AMP1_START_ADDR      (DDR3_FOFBPOS1_END_ADDR)
#define DDR3_MONIT1AMP1_END_ADDR        (DDR3_MONIT1AMP1_START_ADDR + DDR3_MONIT1AMP1_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT1 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1POS1_MEM_SIZE        1

#define DDR3_MONIT1POS1_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1POS1_MEM_SIZE)
#define DDR3_MONIT1POS1_START_ADDR      (DDR3_MONIT1AMP1_END_ADDR)
#define DDR3_MONIT1POS1_END_ADDR        (DDR3_MONIT1POS1_START_ADDR + DDR3_MONIT1POS1_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT 1 AMP
 * Size: 1 DDR3 regions */
#define DDR3_MONITAMP1_MEM_SIZE         0

#define DDR3_MONITAMP1_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITAMP1_MEM_SIZE)
#define DDR3_MONITAMP1_START_ADDR       (DDR3_MONIT1POS1_END_ADDR)
#define DDR3_MONITAMP1_END_ADDR         (DDR3_MONITAMP1_START_ADDR + DDR3_MONITAMP1_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_MONITPOS1_MEM_SIZE         0

#define DDR3_MONITPOS1_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITPOS1_MEM_SIZE)
#define DDR3_MONITPOS1_START_ADDR       (DDR3_MONITAMP1_END_ADDR)
#define DDR3_MONITPOS1_END_ADDR         (DDR3_MONITPOS1_START_ADDR + DDR3_MONITPOS1_MEM_SIZE*__MEM_REGION_SIZE)

/* End 1 Dummy region
 * Size: 0 DDR3 regions */
#define DDR3_DUMMY_END1_MEM_SIZE        0

#define DDR3_DUMMY_END1_MEM_BOOL        DDR3_MEM_BOOL(DDR3_DUMMY_END1_MEM_SIZE)
#define DDR3_DUMMY_END1_START_ADDR      (DDR3_MONITPOS1_END_ADDR)
#define DDR3_DUMMY_END1_END_ADDR        (DDR3_DUMMY_END1_START_ADDR + DDR3_DUMMY_END1_MEM_SIZE*__MEM_REGION_SIZE)

/************************ Acquistion 2 Channel Parameters **************/

/* ADC 1
 * Size: 2 DDR3 regions */
#define DDR3_ADC2_MEM_SIZE              2

#define DDR3_ADC2_MEM_BOOL              DDR3_MEM_BOOL(DDR3_ADC2_MEM_SIZE)
#define DDR3_ADC2_START_ADDR            (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_ADC2_END_ADDR              (DDR3_ADC2_START_ADDR + DDR3_ADC2_MEM_SIZE*__MEM_REGION_SIZE)

/* ADCSWAP 1
 * Size: 2 DDR3 regions */
#define DDR3_ADCSWAP2_MEM_SIZE          2

#define DDR3_ADCSWAP2_MEM_BOOL          DDR3_MEM_BOOL(DDR3_ADCSWAP2_MEM_SIZE)
#define DDR3_ADCSWAP2_START_ADDR        (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_ADCSWAP2_END_ADDR          (DDR3_ADCSWAP2_START_ADDR + DDR3_ADCSWAP2_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 1/2 (shares the same memory space as the DUMMY02)
 * Size: 1 DDR3 regions */
#define DDR3_MIXIQ2_MEM_SIZE            1

#define DDR3_MIXIQ2_MEM_BOOL            DDR3_MEM_BOOL(DDR3_MIXIQ2_MEM_SIZE)
#define DDR3_MIXIQ2_START_ADDR          (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_MIXIQ2_END_ADDR            (DDR3_MIXIQ2_START_ADDR + DDR3_MIXIQ2_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 3/4 (shares the same memory space as the MIXIQ2)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY02_MEM_SIZE          1

#define DDR3_DUMMY02_MEM_BOOL          DDR3_MEM_BOOL(DDR3_DUMMY02_MEM_SIZE)
#define DDR3_DUMMY02_START_ADDR        (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_DUMMY02_END_ADDR          (DDR3_DUMMY02_START_ADDR + DDR3_DUMMY02_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 1/2 (shares the same memory space as the DUMMY12 and TBTAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_TBTDECIMIQ2_MEM_SIZE       2

#define DDR3_TBTDECIMIQ2_MEM_BOOL       DDR3_MEM_BOOL(DDR3_TBTDECIMIQ2_MEM_SIZE)
#define DDR3_TBTDECIMIQ2_START_ADDR     (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_TBTDECIMIQ2_END_ADDR       (DDR3_TBTDECIMIQ2_START_ADDR + DDR3_TBTDECIMIQ2_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 3/4 (shares the same memory space as the TBTDECIMIQ2 and TBTAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY12_MEM_SIZE           2

#define DDR3_DUMMY12_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY12_MEM_SIZE)
#define DDR3_DUMMY12_START_ADDR         (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_DUMMY12_END_ADDR           (DDR3_DUMMY12_START_ADDR + DDR3_DUMMY12_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 AMP
 * Size: 2 DDR3 regions */
#define DDR3_TBTAMP2_MEM_SIZE           2

#define DDR3_TBTAMP2_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTAMP2_MEM_SIZE)
#define DDR3_TBTAMP2_START_ADDR         (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_TBTAMP2_END_ADDR           (DDR3_TBTAMP2_START_ADDR + DDR3_TBTAMP2_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 PHA (shares the same memory space as the TBTDECIMIQ2 and DUMMY12)
 * Size: 2 DDR3 regions */
#define DDR3_TBTPHA2_MEM_SIZE           2

#define DDR3_TBTPHA2_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPHA2_MEM_SIZE)
#define DDR3_TBTPHA2_START_ADDR         (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_TBTPHA2_END_ADDR           (DDR3_TBTPHA2_START_ADDR + DDR3_TBTPHA2_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_TBTPOS2_MEM_SIZE           2

#define DDR3_TBTPOS2_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPOS2_MEM_SIZE)
#define DDR3_TBTPOS2_START_ADDR         (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_TBTPOS2_END_ADDR           (DDR3_TBTPOS2_START_ADDR + DDR3_TBTPOS2_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 1/2 (shares the same memory space as the DUMMY22 and FOFBAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_FOFBDECIMIQ2_MEM_SIZE      2

#define DDR3_FOFBDECIMIQ2_MEM_BOOL      DDR3_MEM_BOOL(DDR3_FOFBDECIMIQ2_MEM_SIZE)
#define DDR3_FOFBDECIMIQ2_START_ADDR    (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_FOFBDECIMIQ2_END_ADDR      (DDR3_FOFBDECIMIQ2_START_ADDR + DDR3_FOFBDECIMIQ2_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 3/4 (shares the same memory space as the FOFBDECIMIQ2 and FOFBAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY22_MEM_SIZE           2

#define DDR3_DUMMY22_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY22_MEM_SIZE)
#define DDR3_DUMMY22_START_ADDR         (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_DUMMY22_END_ADDR           (DDR3_DUMMY22_START_ADDR + DDR3_DUMMY22_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 AMP
 * Size: 2 DDR3 regions */
#define DDR3_FOFBAMP2_MEM_SIZE          2

#define DDR3_FOFBAMP2_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBAMP2_MEM_SIZE)
#define DDR3_FOFBAMP2_START_ADDR        (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_FOFBAMP2_END_ADDR          (DDR3_FOFBAMP2_START_ADDR + DDR3_FOFBAMP2_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 PHA (shares the same memory space as the FOFBDECIMIQ2 and DUMMY22)
 * Size: 2 DDR3 regions */
#define DDR3_FOFBPHA2_MEM_SIZE          2

#define DDR3_FOFBPHA2_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPHA2_MEM_SIZE)
#define DDR3_FOFBPHA2_START_ADDR        (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_FOFBPHA2_END_ADDR          (DDR3_FOFBPHA2_START_ADDR + DDR3_FOFBPHA2_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_FOFBPOS2_MEM_SIZE          2

#define DDR3_FOFBPOS2_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPOS2_MEM_SIZE)
#define DDR3_FOFBPOS2_START_ADDR        (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_FOFBPOS2_END_ADDR          (DDR3_FOFBPOS2_START_ADDR + DDR3_FOFBPOS2_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT1 1 AMP
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1AMP2_MEM_SIZE        1

#define DDR3_MONIT1AMP2_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1AMP2_MEM_SIZE)
#define DDR3_MONIT1AMP2_START_ADDR      (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_MONIT1AMP2_END_ADDR        (DDR3_MONIT1AMP2_START_ADDR + DDR3_MONIT1AMP2_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT1 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1POS2_MEM_SIZE        1

#define DDR3_MONIT1POS2_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1POS2_MEM_SIZE)
#define DDR3_MONIT1POS2_START_ADDR      (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_MONIT1POS2_END_ADDR        (DDR3_MONIT1POS2_START_ADDR + DDR3_MONIT1POS2_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT 1 AMP
 * Size: 1 DDR3 regions */
#define DDR3_MONITAMP2_MEM_SIZE         0

#define DDR3_MONITAMP2_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITAMP2_MEM_SIZE)
#define DDR3_MONITAMP2_START_ADDR       (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_MONITAMP2_END_ADDR         (DDR3_MONITAMP2_START_ADDR + DDR3_MONITAMP2_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_MONITPOS2_MEM_SIZE         0

#define DDR3_MONITPOS2_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITPOS2_MEM_SIZE)
#define DDR3_MONITPOS2_START_ADDR       (DDR3_DUMMY_END1_END_ADDR)
#define DDR3_MONITPOS2_END_ADDR         (DDR3_MONITPOS2_START_ADDR + DDR3_MONITPOS2_MEM_SIZE*__MEM_REGION_SIZE)

/* End 2 Dummy region. Default this to the maximum memory area size.
 * Size: 2 DDR3 regions */
#define DDR3_DUMMY_END2_MEM_SIZE        2

#define DDR3_DUMMY_END2_MEM_BOOL        DDR3_MEM_BOOL(DDR3_DUMMY_END2_MEM_SIZE)
#define DDR3_DUMMY_END2_START_ADDR      (DDR3_MONITPOS2_END_ADDR)
#define DDR3_DUMMY_END2_END_ADDR        (DDR3_DUMMY_END2_START_ADDR + DDR3_DUMMY_END2_MEM_SIZE*__MEM_REGION_SIZE)

/************************ Acquistion 3 Channel Parameters **************/

/* ADC 1
 * Size: 2 DDR3 regions */
#define DDR3_ADC3_MEM_SIZE              2

#define DDR3_ADC3_MEM_BOOL              DDR3_MEM_BOOL(DDR3_ADC3_MEM_SIZE)
#define DDR3_ADC3_START_ADDR            (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_ADC3_END_ADDR              (DDR3_ADC3_START_ADDR + DDR3_ADC3_MEM_SIZE*__MEM_REGION_SIZE)

/* ADCSWAP 1
 * Size: 2 DDR3 regions */
#define DDR3_ADCSWAP3_MEM_SIZE          2

#define DDR3_ADCSWAP3_MEM_BOOL          DDR3_MEM_BOOL(DDR3_ADCSWAP3_MEM_SIZE)
#define DDR3_ADCSWAP3_START_ADDR        (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_ADCSWAP3_END_ADDR          (DDR3_ADCSWAP3_START_ADDR + DDR3_ADCSWAP3_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 1/2 (shares the same memory space as the DUMMY02)
 * Size: 1 DDR3 regions */
#define DDR3_MIXIQ3_MEM_SIZE            1

#define DDR3_MIXIQ3_MEM_BOOL            DDR3_MEM_BOOL(DDR3_MIXIQ3_MEM_SIZE)
#define DDR3_MIXIQ3_START_ADDR          (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_MIXIQ3_END_ADDR            (DDR3_MIXIQ3_START_ADDR + DDR3_MIXIQ3_MEM_SIZE*__MEM_REGION_SIZE)

/* MIXER I/Q 3/4 (shares the same memory space as the MIXIQ2)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY03_MEM_SIZE          1

#define DDR3_DUMMY03_MEM_BOOL          DDR3_MEM_BOOL(DDR3_DUMMY03_MEM_SIZE)
#define DDR3_DUMMY03_START_ADDR        (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_DUMMY03_END_ADDR          (DDR3_DUMMY03_START_ADDR + DDR3_DUMMY03_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 1/2 (shares the same memory space as the DUMMY12 and TBTAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_TBTDECIMIQ3_MEM_SIZE       2

#define DDR3_TBTDECIMIQ3_MEM_BOOL       DDR3_MEM_BOOL(DDR3_TBTDECIMIQ3_MEM_SIZE)
#define DDR3_TBTDECIMIQ3_START_ADDR     (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_TBTDECIMIQ3_END_ADDR       (DDR3_TBTDECIMIQ3_START_ADDR + DDR3_TBTDECIMIQ3_MEM_SIZE*__MEM_REGION_SIZE)

/* TBTDECIM I/Q 3/4 (shares the same memory space as the TBTDECIMIQ2 and TBTAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY13_MEM_SIZE           2

#define DDR3_DUMMY13_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY13_MEM_SIZE)
#define DDR3_DUMMY13_START_ADDR         (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_DUMMY13_END_ADDR           (DDR3_DUMMY13_START_ADDR + DDR3_DUMMY13_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 AMP
 * Size: 2 DDR3 regions */
#define DDR3_TBTAMP3_MEM_SIZE           2

#define DDR3_TBTAMP3_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTAMP3_MEM_SIZE)
#define DDR3_TBTAMP3_START_ADDR         (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_TBTAMP3_END_ADDR           (DDR3_TBTAMP3_START_ADDR + DDR3_TBTAMP3_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 PHA (shares the same memory space as the TBTDECIMIQ2 and DUMMY12)
 * Size: 2 DDR3 regions */
#define DDR3_TBTPHA3_MEM_SIZE           2

#define DDR3_TBTPHA3_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPHA3_MEM_SIZE)
#define DDR3_TBTPHA3_START_ADDR         (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_TBTPHA3_END_ADDR           (DDR3_TBTPHA3_START_ADDR + DDR3_TBTPHA3_MEM_SIZE*__MEM_REGION_SIZE)

/* TBT 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_TBTPOS3_MEM_SIZE           2

#define DDR3_TBTPOS3_MEM_BOOL           DDR3_MEM_BOOL(DDR3_TBTPOS3_MEM_SIZE)
#define DDR3_TBTPOS3_START_ADDR         (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_TBTPOS3_END_ADDR           (DDR3_TBTPOS3_START_ADDR + DDR3_TBTPOS3_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 1/2 (shares the same memory space as the DUMMY22 and FOFBAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_FOFBDECIMIQ3_MEM_SIZE      2

#define DDR3_FOFBDECIMIQ3_MEM_BOOL      DDR3_MEM_BOOL(DDR3_FOFBDECIMIQ3_MEM_SIZE)
#define DDR3_FOFBDECIMIQ3_START_ADDR    (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_FOFBDECIMIQ3_END_ADDR      (DDR3_FOFBDECIMIQ3_START_ADDR + DDR3_FOFBDECIMIQ3_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFBDECIM I/Q 3/4 (shares the same memory space as the FOFBDECIMIQ2 and FOFBAMP2)
 * Size: 1 DDR3 regions */
#define DDR3_DUMMY23_MEM_SIZE           2

#define DDR3_DUMMY23_MEM_BOOL           DDR3_MEM_BOOL(DDR3_DUMMY23_MEM_SIZE)
#define DDR3_DUMMY23_START_ADDR         (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_DUMMY23_END_ADDR           (DDR3_DUMMY23_START_ADDR + DDR3_DUMMY23_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 AMP
 * Size: 2 DDR3 regions */
#define DDR3_FOFBAMP3_MEM_SIZE          2

#define DDR3_FOFBAMP3_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBAMP3_MEM_SIZE)
#define DDR3_FOFBAMP3_START_ADDR        (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_FOFBAMP3_END_ADDR          (DDR3_FOFBAMP3_START_ADDR + DDR3_FOFBAMP3_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 PHA (shares the same memory space as the FOFBDECIMIQ2 and DUMMY22)
 * Size: 2 DDR3 regions */
#define DDR3_FOFBPHA3_MEM_SIZE          2

#define DDR3_FOFBPHA3_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPHA3_MEM_SIZE)
#define DDR3_FOFBPHA3_START_ADDR        (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_FOFBPHA3_END_ADDR          (DDR3_FOFBPHA3_START_ADDR + DDR3_FOFBPHA3_MEM_SIZE*__MEM_REGION_SIZE)

/* FOFB 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_FOFBPOS3_MEM_SIZE          2

#define DDR3_FOFBPOS3_MEM_BOOL          DDR3_MEM_BOOL(DDR3_FOFBPOS3_MEM_SIZE)
#define DDR3_FOFBPOS3_START_ADDR        (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_FOFBPOS3_END_ADDR          (DDR3_FOFBPOS3_START_ADDR + DDR3_FOFBPOS3_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT1 1 AMP
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1AMP3_MEM_SIZE        1

#define DDR3_MONIT1AMP3_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1AMP3_MEM_SIZE)
#define DDR3_MONIT1AMP3_START_ADDR      (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_MONIT1AMP3_END_ADDR        (DDR3_MONIT1AMP3_START_ADDR + DDR3_MONIT1AMP3_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT1 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_MONIT1POS3_MEM_SIZE        1

#define DDR3_MONIT1POS3_MEM_BOOL        DDR3_MEM_BOOL(DDR3_MONIT1POS3_MEM_SIZE)
#define DDR3_MONIT1POS3_START_ADDR      (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_MONIT1POS3_END_ADDR        (DDR3_MONIT1POS3_START_ADDR + DDR3_MONIT1POS3_MEM_SIZE*__MEM_REGION_SIZE)

 /* MONIT 1 AMP
 * Size: 1 DDR3 regions */
#define DDR3_MONITAMP3_MEM_SIZE         0

#define DDR3_MONITAMP3_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITAMP3_MEM_SIZE)
#define DDR3_MONITAMP3_START_ADDR       (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_MONITAMP3_END_ADDR         (DDR3_MONITAMP3_START_ADDR + DDR3_MONITAMP3_MEM_SIZE*__MEM_REGION_SIZE)

/* MONIT 1 POS
 * Size: 1 DDR3 regions */
#define DDR3_MONITPOS3_MEM_SIZE         0

#define DDR3_MONITPOS3_MEM_BOOL         DDR3_MEM_BOOL(DDR3_MONITPOS3_MEM_SIZE)
#define DDR3_MONITPOS3_START_ADDR       (DDR3_DUMMY_END2_END_ADDR)
#define DDR3_MONITPOS3_END_ADDR         (DDR3_MONITPOS3_START_ADDR + DDR3_MONITPOS3_MEM_SIZE*__MEM_REGION_SIZE)

/* End 3 Dummy region. Default this to the maximum memory area size.
 * Size: 2 DDR3 regions */
#define DDR3_DUMMY_END3_MEM_SIZE        2

#define DDR3_DUMMY_END3_MEM_BOOL        DDR3_MEM_BOOL(DDR3_DUMMY_END3_MEM_SIZE)
#define DDR3_DUMMY_END3_START_ADDR      (DDR3_MONIT1POS3_END_ADDR)
#define DDR3_DUMMY_END3_END_ADDR        (DDR3_DUMMY_END2_END_ADDR + DDR3_DUMMY_END3_MEM_SIZE*__MEM_REGION_SIZE)

#endif

