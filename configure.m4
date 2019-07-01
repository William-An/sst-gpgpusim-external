dnl -*- Autoconf -*-

AC_DEFUN([SST_Gpgpusim_CONFIG], [
  sst_Gpgpusim_comp_happy="yes"
AS_IF([test "x$sst_Gpgpusim_comp_happy" = "xyes"], [$1], [$2])
])
