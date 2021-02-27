# replacement steps corresponding to the sed commands in Makefile.am
FILE(READ y.tab.c data0)
STRING(REPLACE "fprintf" "sfprintf" data1 "${data0}")
STRING(REPLACE "FILE"    "Sfio_t"   data2 "${data1}")
STRING(REPLACE "stderr"  "sfstderr" data3 "${data2}")
FILE(WRITE exparse.c "${data3}")
