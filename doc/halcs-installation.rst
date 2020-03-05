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

The HALCS software makes use of two build systems: ``make`` [#make]
and ``gradle`` [#gradle]. Both of them should produce the same binaries, but
``gradle`` gives you more outputs, such as ``.rpm`` packages and automatically
increments the version number in header files during development.

.. [#make] `Make Page`_
.. [#gradle] `Gradle Page`_

.. _`Make Page`: https://www.gnu.org/software/make
.. _`Gradle Page`: https://gradle.org

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

Here is the procedure to build the binary images from the source using ``make``:

1. Install ``make`` and ``gcc``:

.. code-block:: bash

    sudo apt-get install make gcc

for Debian-based systems.

.. code-block:: bash

    sudo yum install make gcc-c++

for Fedora-based systems.

2. Get the source code:

.. code-block:: bash

    git clone --recursive https://github.com/lnls-dig/halcs

3. Go to the source code directory:

.. code-block:: bash

    cd halcs

4. Build and install the code:

.. code-block:: bash

    make && sudo make install

Alternatively you can use a script called ``./compile.sh`` that gives you
more flexibility in terms of compilation-time configurability. You should only
use this method if the defaults are not sufficient for you use case:

1. Install ``make`` and ``gcc``:

.. code-block:: bash

    sudo apt-get install make gcc

for Debian-based systems.

.. code-block:: bash

    sudo yum install make gcc-c++

for Fedora-based systems.

2. Get the source code:

.. code-block:: bash

   git clone --recursive https://github.com/lnls-dig/halcs

3. Go to the source code directory:

.. code-block:: bash

    cd halcs

4. Build and install the code:

.. code-block:: bash

    ./compile.sh -b afcv3_1 -a halcsd -e yes -l yes -d yes

Yet another way to build the source code is to use ``gradle``:

1. Install ``make``, ``gcc`` and ``java``:

.. code-block:: bash

    sudo apt-get install openjdk-8-jdk openjdk-8-jre gcc

for Debian-based systems.

.. code-block:: bash

   sudo yum install java-1.8.0-openjdk java-1.8.0-openjre gcc-c++

for Fedora-based systems.

2. Get the source code:

.. code-block:: bash

    git clone --recursive https://github.com/lnls-dig/halcs

3. Go to the source code directory:

.. code-block:: bash

    cd halcs

4. Build and install the code:

.. code-block:: bash

   ./gradle_compile.sh -b afcv3_1 -a halcsd -e yes -f yes

If you already have the software installed and wish to remote update to the
latest release you can run the following snippet. Be advised that this is just
an example and assumes specific paths and running applications:

.. code-block:: bash

  NODES=()
  NODES+=("<type the computer IP that you wish to update>")

  for crate in "${NODES[@]}"; do
    SSHPASS=root sshpass -e ssh -o StrictHostKeyChecking=no \
    root@${crate} bash -c "\
      set -x && \
      cd /root/postinstall/apps/bpm-app/halcs && \
      git fetch --all && \
      git checkout -b stable-\$(date +%Y%m%d-%H%M%S) && \
      git checkout master && \
      git reset --hard origin/master && \
      cp /usr/local/etc/halcs/halcs.cfg /home/lnls-bpm/halcs.cfg.temp && \
      systemctl stop \
        halcs@{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}.target && \
      cd /root/postinstall/apps/bpm-app/halcs && \
      ./gradle_uninstall.sh && \
      ./gradle_compile.sh -a halcsd -b afcv3_1 -e yes && \
      mv /home/lnls-bpm/halcs.cfg.temp /usr/local/etc/halcs/halcs.cfg && \
      systemctl daemon-reload && \
      cd /root/postinstall/apps/bpm-app/halcs-generic-udev && \
      make install &&  \
      systemctl start \
        halcs-ioc@{7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}.target" &
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
