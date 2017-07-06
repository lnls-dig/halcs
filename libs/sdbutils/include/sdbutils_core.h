/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SDBUTILS_CORE_H_
#define _SDBUTILS_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Print device as ASCII */
sdbutils_err_e sdbutils_list_device (struct sdb_device *d, int depth, int base,
        int opt_long);
sdbutils_err_e sdbutils_list_device_file(struct sdb_device *d, int depth, int base,
        int opt_long, FILE *stream);

/* Print whole SDBFS as ASCII */
sdbutils_err_e sdbutils_do_list (struct sdbfs *fs, int opt_long);
sdbutils_err_e sdbutils_do_list_file (struct sdbfs *fs, int opt_long, FILE *stream);

/* Search for device name and print device */
sdbutils_err_e sdbutils_do_cat_name (struct sdbfs *fs, char *name);
sdbutils_err_e sdbutils_do_cat_name_file (struct sdbfs *fs, char *name, FILE *stream);

/* Search for device Vendor/ID and print device */
sdbutils_err_e sdbutils_do_cat_id (struct sdbfs *fs, uint64_t vendor, uint32_t dev);
sdbutils_err_e sdbutils_do_cat_id_file (struct sdbfs *fs, uint64_t vendor, uint32_t dev,
        FILE *stream);

/* Return next device on current SDB position */
struct sdb_device *sdbutils_next_device (struct sdbfs *fs);

#ifdef __cplusplus
}
#endif

#endif

