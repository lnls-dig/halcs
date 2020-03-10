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

Build from sources
''''''''''''''''''

The HALCS software makes use of two build systems: ``make`` [#make]_
and ``gradle`` [#gradle]_. Both of them should produce the same binaries, but
``gradle`` gives you more outputs, such as ``.rpm`` packages and automatically
increments the version number in header files during development.

.. [#make] |Make Page|_
.. [#gradle] |Gradle Page|_

.. _`Make Page`: https://www.gnu.org/software/make
.. _`Gradle Page`: https://gradle.org
.. |Make Page| replace:: https://www.gnu.org/software/make
.. |Gradle Page| replace:: https://gradle.org

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
            CONFIG_OPTS+=("--with-libsodium")
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

Additionaly, it's often interesting to install customized udev scripts, so
your application will automatically start a given program when some ID is
detected:

.. code-block:: bash
  :linenos:

    git clone --recursive https://github.com/lnls-dig/halcs-generic-udev.git && \
    for project in halcs-generic-udev; do
        cd $project && \
        git submodule update --init --recursive &&
        sudo make install && \
        cd ..

        # Check last command return status
        if [ $? -ne 0 ]; then
            echo "Could not compile/install project $project." >&2
            exit 1
        fi
    done

Typically the ID used is the *Gateware Name* represented by the SDB [#sdb]_ property
``synthesis-name`` that is baked inside the FPGA Gateware.

.. [#sdb] |SDB Wiki|_

.. _`SDB Wiki`: https://ohwr.org/project/fpga-config-space/wikis/home
.. |SDB Wiki| replace:: https://ohwr.org/project/fpga-config-space/wikis/home

To add your specific program to start when some ID is found, the ``run-fpga-program.sh``
(typically installed in ``/usr/local/share/halcs/scripts`` or under
``halcs-generic-udev`` repository path ``scripts/share/halcs/scripts``) script
can be modified. Below, an excerpt of the script is showm with a possible
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

Using Gradle Build System
~~~~~~~~~~~~~~~~~~~~~~~~~

Another way to build the source code is to use ``gradle``:

1. Install ``make``, ``gcc`` and ``java``:

.. code-block:: bash
  :linenos:

    sudo apt-get install openjdk-8-jdk openjdk-8-jre gcc

for Debian-based systems.

.. code-block:: bash
  :linenos:

   sudo yum install java-1.8.0-openjdk java-1.8.0-openjre gcc-c++

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

   ./gradle_compile.sh -b afcv3_1 -a halcsd -e yes -f yes

All in all, the full script to install HALCS with ``gradle`` is:

.. code-block:: bash
  :linenos:

    git clone --recursive https://github.com/lnls-dig/halcs && \
    cd halcs && \
   ./gradle_compile.sh -b afcv3_1 -a halcsd -e yes -f yes

Updating Dependencies
~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~

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
            ./gradle_uninstall.sh && \
            ./gradle_compile.sh -a halcsd -b afcv3_1 -e yes && \
            mv /home/lnls-bpm/halcs.cfg.temp /usr/local/etc/halcs/halcs.cfg && \
            systemctl daemon-reload && \
            cd ${DEP_DIR}/halcs-generic-udev && \
            make install &&  \
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
