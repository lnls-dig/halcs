.. _halcs-installation:

HALCS Installation
==================

Repositories and Releases
-------------------------

This project is hosted on Github, at the following link:
`HALCS Github Repository`_.

Here a list of resources that you can find on the project page:

Releases [#releases]_
    Contains the `.rpm` packages for installing HALCS on a CentOS/RedHat/Fedora
    and the `.pdf` documentation for every official release.

Repository [#repository]_
    Contains the git repository of the project.

.. [#releases] |HALCS Github Releases|_
.. [#repository] |HALCS Github Repository|_

.. _`HALCS Github Releases`: https://github.com/lnls-dig/halcs/releases
.. _`HALCS Github Repository`: https://github.com/lnls-dig/halcs
.. |HALCS Github Releases| replace:: https://github.com/lnls-dig/halcs/releases
.. |HALCS Github Repository| replace:: https://github.com/lnls-dig/halcs

On the repository the official releases have a tag named ``v#maj.#min.#rev``
where ``#maj`` represent the major release version of the software, ``#min``
the minor one and ``#rev`` the revision (e.g ``v1.9.1``). This follows semantic
versioning as defined in `Semantic Versioning`_. The released binary files
follow the same naming convention.

.. _`Semantic Versioning`: https://semver.org

.. Note:: if you got this from the repository (as opposed to a named ``tar.gz``
          or ``pdf`` file) it may happen that you are looking at a later commit
          than the release this manual claims to document. It is a fact of life
          that developers forget to re-read and fix documentation while updating
          the code. In that case, please run `git describe HEAD` to ensure where
          you are.

About the source code
---------------------

Introduction
''''''''''''

The HALCS software makes use of two build systems: ``make`` [#make]_
and ``cmake`` [#cmake]_. Both of them should produce the same binaries, but
``cmake`` gives you more outputs, such as ``.deb`` and ``.rpm`` packages. Also,
CMake gives you more controlover how binaries are built and is the recommended
way.

.. [#make] |Make Page|_
.. [#cmake] |CMake Page|_

.. _`Make Page`: https://www.gnu.org/software/make
.. _`CMake Page`: https://cmake.org
.. |Make Page| replace:: https://www.gnu.org/software/make
.. |CMake Page| replace:: https://cmake.org

Prior to build HALCS, we need to install its dependencies. As of the moment,
it makes use of 4 external dependencies:

libsodium |libsodium-version|
    | |Libsodium Repository|_
zeromq |libzmq-version|
    | |Libzmq Repository|_
czmq |libczmq-version|
    | |Libczmq Repository|_
mlm |malamute-version|
    | |Malamute Repository|_

.. |libsodium-version| replace:: 1.0.8
.. |libzmq-version| replace:: v4.2.5
.. |libczmq-version| replace:: 4.0.2
.. |malamute-version| replace:: v1.6.1

.. _`Libsodium Repository`: https://github.com/jedisct1/libsodium/tree/1.0.8
.. _`Libzmq Repository`: https://github.com/zeromq/libzmq/tree/v4.2.5
.. _`Libczmq Repository`: https://github.com/zeromq/czmq/tree/v4.0.21.0.8
.. _`Malamute Repository`: https://github.com/lnls-dig/malamute/tree/v1.6.1

.. |Libsodium Repository| replace:: https://github.com/jedisct1/libsodium/tree/|libsodium-version|
.. |Libzmq Repository| replace:: https://github.com/zeromq/libzmq/tree/|libzmq-version|
.. |Libczmq Repository| replace:: https://github.com/zeromq/czmq/tree/|libczmq-version|
.. |Malamute Repository| replace:: https://github.com/lnls-dig/malamute/tree/|malamute-version|

Installing dependencies
'''''''''''''''''''''''

In order to install them with versions that are known to be compatible use the
following snippet:

.. code-block:: bash
  :linenos:

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
          exit 1
      fi
  done

Alternatively you can install them via ``.rpm`` or ``.deb`` packages if available
at your distribution. As all of them use semantic versioning, you can install any
version that is greater or equal than the specified ones for *minor* and *revision*:
numbers.

Installing HALCS server
'''''''''''''''''''''''

Using CMake Build System
~~~~~~~~~~~~~~~~~~~~~~~~~

The recommended way to build the source code is to use ``cmake``:

1. Install ``make`` and ``gcc``:

.. code-block:: bash
  :linenos:

  sudo apt-get install make gcc

for Debian-based systems.

.. code-block:: bash
  :linenos:

  sudo yum install make gcc-c++

for Fedora-based systems.

2. Install ``CMake``. The supported version is ``3.10+``. Check you distribution
   for the correct ``CMake`` version:

.. code-block:: bash
  :linenos:

  sudo apt-get install cmake

for Debian-based systems.

.. code-block:: bash
  :linenos:

  sudo yum install cmake

for Fedora-based systems.

3. Check CMake version with:

.. code-block:: bash

  cmake --version

If the reported version is less than ``3.10`` install it manually with:

.. code-block:: bash
  :linenos:

  version=3.10
  build=3
  wget -c https://cmake.org/files/v$version/cmake-$version.$build.tar.gz -O - | \
      tar -xzv
  cd cmake-$version.$build/

Compile and install ``CMake``:

.. code-block:: bash
  :linenos:

  ./bootstrap
  make -j$(nproc)
  sudo make install

4. Get the source code:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs

5. Go to the source code directory:

.. code-block:: bash
  :linenos:

  cd halcs

6. Build and install the code:

.. code-block:: bash
  :linenos:

  mkdir -p build
  cd build
  cmake ../
  make
  sudo make install

All in all, the full script to install HALCS with ``cmake`` is:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs && \
  cd halcs && \
  mkdir -p build
  cd build
  cmake ../
  make
  sudo make install

Optionally you can build HALCS by creating a ``.deb`` or ``.rpm`` and
install them. You can do that by following the instructions:

For Debian-based distributions:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs && \
  cd halcs && \
  mkdir -p build
  cd build
  cmake ../
  cpack -G "DEB"

For Redhat-based distributions:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs && \
  cd halcs && \
  mkdir -p build
  cd build
  cmake ../
  cpack -G "RPM"

Then, just proceed normally with installing the packages on your system.

Using Make Build System
~~~~~~~~~~~~~~~~~~~~~~~

Here is the procedure to build the binary images from the source using ``make``:

1. Install ``make`` and ``gcc``:

.. code-block:: bash
  :linenos:

  sudo apt-get install make gcc

for Debian-based systems.

.. code-block:: bash
  :linenos:

  sudo yum install make gcc-c++

for Fedora-based systems.

2. Get the source code:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs

3. Go to the source code directory:

.. code-block:: bash
  :linenos:

  cd halcs

4. Build and install the code:

.. code-block:: bash
  :linenos:

  make && sudo make install

All in all, the full script to install HALCS with ``make`` is:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs && \
  cd halcs && \
  make && \
  sudo make install

Alternatively you can use a script called ``./compile.sh`` that gives you
more flexibility in terms of compilation-time configurability. You should only
use this method if the defaults are not sufficient for you use case:

1. Install ``make`` and ``gcc``:

.. code-block:: bash
  :linenos:

  sudo apt-get install make gcc

for Debian-based systems.

.. code-block:: bash
  :linenos:

  sudo yum install make gcc-c++

for Fedora-based systems.

2. Get the source code:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs

3. Go to the source code directory:

.. code-block:: bash
  :linenos:

  cd halcs

4. Build and install the code:

.. code-block:: bash
  :linenos:

  ./compile.sh -b afcv3_1 -a halcsd -e yes -l yes -d yes

The full procedure would be:

.. code-block:: bash
  :linenos:

  git clone --recursive https://github.com/lnls-dig/halcs && \
  cd halcs && \
  ./compile.sh -b afcv3_1 -a halcsd -e yes -l yes -d yes

Installing HALCS client
'''''''''''''''''''''''

Using CMake Build System
~~~~~~~~~~~~~~~~~~~~~~~~~

The recommended way to build the client libraries is to use ``cmake``:

Build and install libraries:

.. code-block:: bash
  :linenos:

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

If you need to uninstall the libraries you can do with:

.. code-block:: bash
  :linenos:

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

Using Make Build System
~~~~~~~~~~~~~~~~~~~~~~~

Another way to build the client libraries is to use ``make``:

Build and install inner dependencies:

.. code-block:: bash
  :linenos:

  make deps
  sudo make deps_install

And, then the client libraries themselves:

.. code-block:: bash
  :linenos:

  make libs
  sudo make libs_install

If you need to uninstall the libraries do:

.. code-block:: bash
  :linenos:

  sudo make libs_uninstall

Installing HALCS examples
'''''''''''''''''''''''''

Change to the examples folder:

.. code-block:: bash
  :linenos:

  cd examples

Using CMake Build System
~~~~~~~~~~~~~~~~~~~~~~~~~

The recommended way to build the examples is to use ``cmake``:

.. code-block:: bash
  :linenos:

  mkdir -p build
  cd build
  cmake ../
  make

Using Make Build System
~~~~~~~~~~~~~~~~~~~~~~~

Another way to build the examples is to use ``make``:

.. code-block:: bash
  :linenos:

  make

UDEV scripts
''''''''''''

HALCS includes three udev scripts, two generic one that sets the device permissions
to the appropriate ones (called ``60-udev-fpga-rules`` and ``80-udev-fpga-rules``)
and another customized udev script, in which you can automatically start a userspace
application if a certain condition is detected (called ``81-udev-fpga-rules``). If this,
an application will automatically start a given program when some ID is detected.

Typically the ID used is the *Gateware Name* represented by the SDB [#sdb]_ property
``synthesis-name`` that is baked inside the FPGA Gateware.

.. [#sdb] |SDB Wiki|_

.. _`SDB Wiki`: https://ohwr.org/project/fpga-config-space/wikis/home
.. |SDB Wiki| replace:: https://ohwr.org/project/fpga-config-space/wikis/home

To add your specific program to start when some ID is found, the ``run-fpga-program.sh``
(typically installed in ``/usr/local/share/halcs`` or ``/usr/share/halcs``) script
can be modified. Below, an excerpt of the script is shown with a possible
modification to allow starting another program:

.. code-block:: bash
  :linenos:
  :emphasize-lines: 38-40

  ...

  for i in $(seq 1 "${#HALCS_IDXS[@]}"); do
      prog_inst=$((i-1));
      case "${GATEWARE_NAME}" in
          bpm-gw*)
              case "${FMC_NAMES[$prog_inst]}" in
                  LNLS_FMC250M*)
                      START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
                      ;;
                  LNLS_FMC130M*)
                      START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
                      ;;
                  *)
                      echo "Unsupported Gateware Module: "${FPGA_FMC_NAME} >&2
                      exit 1
                      ;;
              esac
              ;;

          tim-receiver*)
              START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
              ;;

          afc-tim*)
              # Only start IOCs for even-numbered instances, as there is no device for odd-numbered instances
              if [ $((prog_inst%2)) -eq 0 ]; then
                  START_PROGRAM="/usr/bin/systemctl --no-block start tim-rx-ioc@${HALCS_IDXS[$prog_inst]}.service"
              else
                  START_PROGRAM=""
              fi
              ;;

          pbpm-gw*)
              START_PROGRAM="/usr/bin/systemctl --no-block start halcs-ioc@${HALCS_IDXS[$prog_inst]}.target"
              ;;

          <ADD YOU GATEWARE NAME HERE>*)
              START_PROGRAM="<ADD YOUR START PROGRAM HERE>"
              ;;

          *)
              echo "Invalid Gateware: "${GATEWARE_NAME} >&2
              exit 2
              ;;
      esac

      eval ${START_PROGRAM}
  done

  ...

Updating Dependencies
'''''''''''''''''''''

If you already have dependencies installed and wish to remote update to a
specific release you can run the following snippet. Be advised that you
would need to change the repository locations to match your environment:

.. code-block:: bash
  :linenos:

  NODES=()
  NODES+=("<type the computer IP that you wish to update>")

  for crate in "${NODES[@]}"; do
      SSHPASS=root sshpass -e ssh -o StrictHostKeyChecking=no \
      root@${crate} bash -s <<'EOF'
          set -x && \
          export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig && \
          DEP_DIR=/root/postinstall/apps/bpm-app && \
          libsodium_VER=1.0.8 && \
          libzmq_VER=v4.2.5 && \
          libczmq_VER=v4.0.2 && \
          malamute_VER=v1.6.1 && \
          cd ${DEP_DIR} && \
          for project in libsodium libzmq czmq; do
              PROJECT_VER=${project}_VER
              CONFIG_OPTS=()
              CONFIG_OPTS+=("CFLAGS=-Wno-format-truncation")
              CONFIG_OPTS+=("CPPFLAGS=-Wno-format-truncation")
              if [ $project == "libzmq" ]; then
                  CONFIG_OPTS+=("--with-libsodium")
              fi

              cd $project && \
              git fetch --all && \
              git stash && \
              git checkout -f ${!PROJECT_VER} && \
              git reset --hard ${!PROJECT_VER} && \
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

          cd ${DEP_DIR} && \
          for project in malamute; do
              PROJECT_VER=${project}_VER
              CONFIG_OPTS=()
              CONFIG_OPTS+=("--with-systemd-units")
              CONFIG_OPTS+=("--sysconfdir=/usr/etc")
              CONFIG_OPTS+=("--prefix=/usr")
              CONFIG_OPTS+=("CFLAGS=-Wno-format-truncation")
              CONFIG_OPTS+=("CPPFLAGS=-Wno-format-truncation")

              cd $project && \
              git fetch --all && \
              git stash && \
              git checkout -f ${!PROJECT_VER} && \
              git reset --hard ${!PROJECT_VER} && \
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
              sudo systemctl restart malamute || /bin/true

              # Check last command return status
              if [ $? -ne 0 ]; then
                  echo "Could not compile/install project $project." >&2
                  exit 1
              fi
          done
  EOF
  done

Updating HALCS
''''''''''''''

If you already have the software installed and wish to remote update to the
latest release you can run the following snippet. Be advised that this is just
an example and assumes specific paths and running applications:

.. code-block:: bash
  :linenos:

  NODES=()
  NODES+=("<type the computer IP that you wish to update>")

  for crate in "${NODES[@]}"; do
      SSHPASS=root sshpass -e ssh -o StrictHostKeyChecking=no \
      root@${crate} bash -s <<'EOF'
          set -x && \
          DEP_DIR=/root/postinstall/apps/bpm-app && \
          PROJECT_VER=master && \
          cd ${DEP_DIR}/halcs && \
          git fetch --all && \
          git checkout -b stable-$(date +%Y%m%d-%H%M%S) && \
          git checkout ${!PROJECT_VER} && \
          git reset --hard ${!PROJECT_VER} && \
          cp /usr/local/etc/halcs/halcs.cfg /home/lnls-bpm/halcs.cfg.temp && \
          systemctl stop \
              halcs@{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}.target && \
          cd ${DEP_DIR}/halcs && \
          mkdir -p build && \
          cd build && \
          cmake ../ && \
          make && \
          sudo make install && \
          mv /home/lnls-bpm/halcs.cfg.temp /usr/local/etc/halcs/halcs.cfg && \
          systemctl daemon-reload && \
          systemctl start \
              halcs-ioc@{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}.target
  EOF
  done

Source Code Organization
''''''''''''''''''''''''

To be written

Dependencies
''''''''''''

HALCS depends on the following libraries:

libsodium
    | Repository: |Libsodium Repository|_
    | Version: |libsodium-version|
zeromq
    | Repository: |Libzmq Repository|_
    | Version: |libzmq-version|
czmq
    | Repository: |Libczmq Repository|_
    | Version: |libczmq-version|
mlm
    | Repository: |Malamute Repository|_
    | Version: |malamute-version|
