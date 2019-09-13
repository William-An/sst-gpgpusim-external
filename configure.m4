dnl -*- Autoconf -*-

AC_DEFUN([SST_Gpgpusim_CONFIG], [
  sst_check_gpgpusimx="yes"

  SST_CHECK_GPGPUSIM([have_gpgpusim=1],[have_gpgpusim=0],[AC_MSG_ERROR([GPGPU-Sim required, but not found])])

  AS_IF([test "$have_gpgpusim" = 1], [sst_check_gpgpusimx="yes"], [sst_check_gpgpusimx="no"])
  AS_IF([test "$sst_check_gpgpusimx" = "yes"], [$1], [$2])
])
