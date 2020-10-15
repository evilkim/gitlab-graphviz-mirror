FROM centos:7

# Development tools
RUN yum -y groupinstall 'Development Tools'

RUN yum install -y \
# Install epel-release rpm (Needed for cmake3. Must be a separate step)
https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/e/epel-release-7-12.noarch.rpm \
# Install ius-release rpm (Needed for git version 2. Must be a separate step)
https://repo.ius.io/ius-release-el7.rpm \
&& yum clean all && rm -rf /var/cache/yum

# Remove old version of git
RUN yum -y remove git

# Centos build utilities
RUN yum -y install \
libtool-ltdl-devel \
libedit-devel \
ksh \
cmake3 \
# Git version 2 needed to get the option --date=format-local:... used in gen_version.py
https://repo.ius.io/7/x86_64/packages/g/git224-2.24.3-1.el7.ius.x86_64.rpm \
# Option glut
freeglut-devel \
# Option gts
# Option swig
swig \
# Command lefty
# Command smyra
# Command gvedit
qt5-qtbase-devel \
ncurses-devel \
# Plugin library devil
# Plugin library gd
gd-devel \
# Documentation
ghostscript \
# Plugin library
ghostscript-devel \
# Plugin library gtk
gtk2-devel \
# Plugin library lasi
# Plugin library pangocairo
# Plugin library poppler
poppler-glib-devel \
poppler-qt \
# Plugin library rsvg
librsvg2-devel \
# Plugin library webp
# Language extension gv_sharp & gv_ruby
# Language extension gv_go
# Language extension gv_guile
guile-devel \
# Language extension gv_java
java-devel \
# Language extension gv_lua
lua-devel \
# Language extension gv_ocaml
ocaml \
# Language extension gv_perl
perl-ExtUtils-Embed \
# Language extension gv_php
php-devel \
bzip2-devel \
openssl-devel \
libxml2-devel \
# Language extension gv_python & gv_python2
python2-devel \
# Language extension gv_python3
python3-devel \
# Language extension gv_ruby
ruby-devel \
# Language extension gv_tcl
tcl-devel \
# Requred in redhat/graphviz.spec.fedora.in
libICE-devel \
libSM-devel \
libXaw-devel \
tk-devel \
# Test utilities
python-pytest \
&& rpm --install https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/p/python36-py-1.4.32-2.el7.noarch.rpm \
&& rpm --install https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/p/python36-pytest-2.9.2-4.el7.noarch.rpm \
&& alternatives --install /usr/local/bin/cmake cmake /usr/bin/cmake3 20 \
--slave /usr/local/bin/ctest ctest /usr/bin/ctest3 \
--slave /usr/local/bin/cpack cpack /usr/bin/cpack3 \
--slave /usr/local/bin/ccmake ccmake /usr/bin/ccmake3 \
--family cmake \
# Clean up
&& yum clean all && rm -rf /var/cache/yum
