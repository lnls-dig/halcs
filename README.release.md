# Release Instructions

1. Update API version number in HALCS headers

It's is located at:

`include/revision.h`

2. Update API version number in libraries headers

They are located at:

```
libs/acqclient/include/acq_client_classes.h
libs/bpmclient/include/bpm_client_classes.h
libs/convc/include/convc_classes.h
libs/disptable/include/disptable_classes.h
libs/errhand/include/errhand_classes.h
libs/hutils/include/hutils_classes.h
libs/llio/include/ll_io_classes.h
libs/sdbutils/include/sdbutils_classes.h
src/client/include/halcs_client_classes.h
```

3. Update ABI version number (SOVERSION) in CMake files following libtool versioning scheme.

Libtool versioning: https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html

CMakeLists.txt files:

```
libs/acqclient/CMakeLists.txt
libs/bpmclient/CMakeLists.txt
libs/convc/CMakeLists.txt
libs/disptable/CMakeLists.txt
libs/errhand/CMakeLists.txt
libs/hutils/CMakeLists.txt
libs/llio/CMakeLists.txt
libs/sdbutils/CMakeLists.txt
src/client/CMakeLists.txt
```

4. Update examples version number in examples

This is located at:

```
examples/include/halcs_examples_classes.h
```

5. Merge branch with `master` branch

6. Tag the `master` branch with the release version with the HALCS API version number
