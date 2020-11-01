#!/bin/bash

set -x
set -e
set -o pipefail

if test -f /etc/os-release; then
    cat /etc/os-release
    . /etc/os-release
elif [ "$( uname -s )" = "Darwin" ]; then
    ID=$( uname -s )
    VERSION_ID=$( uname -r )
else
    cat /etc/redhat-release
    ID=$( cat /etc/redhat-release | cut -d' ' -f1 | tr 'A-Z' 'a-z' )
    VERSION_ID=$( cat /etc/redhat-release | cut -d' ' -f3  | cut -d'.' -f1 )
fi
GV_VERSION=$( cat VERSION )
COLLECTION=$( cat COLLECTION )
META_DATA_DIR=Metadata/${COLLECTION}/${ID}/${VERSION_ID}
mkdir -p ${META_DATA_DIR}
DIR=Packages/${COLLECTION}/${ID}/${VERSION_ID}
ARCH=$( uname -m )
mkdir -p ${DIR}/os/${ARCH}
mkdir -p ${DIR}/debug/${ARCH}
mkdir -p ${DIR}/source
if [ "${build_system}" = "cmake" ]; then
    mkdir build
    cd build
    cmake ..
    cmake --build .
    cpack
    cd ..
    if [ "${ID_LIKE}" = "debian" ]; then
        mv build/*.deb ${DIR}/os/${ARCH}/
    elif [ "${ID}" = "Darwin" ]; then
        mv build/*.zip ${DIR}/os/${ARCH}/
    else
        mv build/*.rpm ${DIR}/os/${ARCH}/
    fi
else
    if [ "${ID_LIKE}" = "debian" ]; then
        tar xfz graphviz-${GV_VERSION}.tar.gz
        (cd graphviz-${GV_VERSION}; fakeroot make -f debian/rules binary) | tee >(ci/extract-configure-log.sh >${META_DATA_DIR}/configure.log)
        mv *.deb ${DIR}/os/${ARCH}/
        mv *.ddeb ${DIR}/debug/${ARCH}/
    elif [ "${ID}" = "Darwin" ]; then
        ./autogen.sh
        ./configure --prefix=$( pwd )/build --with-quartz=yes
        make
        make install
        tar cfz graphviz-${GV_VERSION}-${ARCH}.tar.gz --options gzip:compression-level=9 build
        mv graphviz-${GV_VERSION}-${ARCH}.tar.gz ${DIR}/os/${ARCH}/
    else
        rm -rf ${HOME}/rpmbuild
        rpmbuild -ta graphviz-${GV_VERSION}.tar.gz | tee >(ci/extract-configure-log.sh >${META_DATA_DIR}/configure.log)
        mv ${HOME}/rpmbuild/SRPMS/*.src.rpm ${DIR}/source/
        mv ${HOME}/rpmbuild/RPMS/*/*debuginfo*rpm ${DIR}/debug/${ARCH}/
        mv ${HOME}/rpmbuild/RPMS/*/*.rpm ${DIR}/os/${ARCH}/
    fi
fi
