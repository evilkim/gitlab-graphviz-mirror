FROM centos:8

COPY requirements.txt ./requirements.txt

# Development tools
RUN yum -y groupinstall 'Development Tools' \
&& dnf config-manager --set-enabled powertools \
# Centos build utilities
&& yum -y install \
libtool-ltdl-devel \
libedit-devel \
ksh \
cmake \
# Option glut
freeglut-devel \
# Option gts
# Option swig
swig \
# Command lefty
# Command smyra
# Command gvedit
qt5-devel \
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
poppler-qt5 \
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
python3-pip \
python3-setuptools \
# install Python dependencies
&& python3 -m pip install --user --upgrade pip \
&& python3 -m pip install --user --requirement requirements.txt \
# Clean up
&& yum clean all && rm -rf /var/cache/yum
