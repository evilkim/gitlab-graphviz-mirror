#!/bin/sh -e

set -x

if test -f /etc/os-release; then
    cat /etc/os-release
    . /etc/os-release
    if [ -z ${ID_LIKE:+x} ]; then
        if [ ! -z ${ID:+x} ]; then
            ID_LIKE=${ID}
        fi
    fi
elif [ "$( uname -s )" = "Darwin" ]; then
    ID=$( uname -s )
    VERSION_ID=$( uname -r )
else
    cat /etc/redhat-release
    ID=$( cat /etc/redhat-release | cut -d' ' -f1 | tr 'A-Z' 'a-z' )
    VERSION_ID=$( cat /etc/redhat-release | cut -d' ' -f3  | cut -d'.' -f1 )
fi
printf '%s\n' "${ID}" >OS_ID
GV_VERSION=$( cat GRAPHVIZ_VERSION )
COLLECTION=$( cat COLLECTION )
DIR=Packages/${COLLECTION}/${ID}/${VERSION_ID}
ARCH=$( uname -m )

# Remove possible old version of Graphviz so that we can detect if
# individual Graphviz tools are missing in the current release
if [ "${ID}" = "centos" -o "${ID}" = "fedora" ]; then
    # Both doxgen and asciidoc depend on graphviz and needs to be
    # removed to allow Graphviz to be removed
    if rpm -q doxygen; then
        rpm --erase doxygen
    fi
    if rpm -q asciidoc; then
        rpm --erase asciidoc
    fi
    if rpm -q graphviz; then
        rpm --erase graphviz
    fi
fi
if [ "${build_system}" = "cmake" ]; then
    if [ "${ID_LIKE}" = "debian" ]; then
        apt install ./${DIR}/os/${ARCH}/Graphviz-${GV_VERSION}-Linux.deb
    elif [ "${ID}" = "Darwin" ]; then
        unzip ${DIR}/os/${ARCH}/Graphviz-${GV_VERSION}-Darwin.zip
        cp -rp Graphviz-${GV_VERSION}-Darwin/* /usr/local
    else
        rpm --install --force ${DIR}/os/${ARCH}/Graphviz-${GV_VERSION}-Linux.rpm
    fi
else
    if [ "${ID_LIKE}" = "debian" ]; then
        apt install ./${DIR}/os/${ARCH}/libgraphviz4_${GV_VERSION}-1_amd64.deb
        apt install ./${DIR}/os/${ARCH}/libgraphviz-dev_${GV_VERSION}-1_amd64.deb
        apt install ./${DIR}/os/${ARCH}/graphviz_${GV_VERSION}-1_amd64.deb
    elif [ "${ID}" = "Darwin" ]; then
        tar xf ${DIR}/os/${ARCH}/graphviz-${GV_VERSION}-${ARCH}.tar.gz
        cp -rp build/* /usr/local
    else
        rpm --install --force \
            ${DIR}/os/${ARCH}/graphviz-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-libs-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-devel-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-plugins-core-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-plugins-x-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-x-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-gd-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-qt-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-plugins-gd-${GV_VERSION}*.rpm \
            ${DIR}/os/${ARCH}/graphviz-nox-${GV_VERSION}*.rpm
    fi
fi
