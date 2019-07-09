dnl -*- Autoconf -*-

AC_DEFUN([SST_Gpgpusim_CONFIG], [
  sst_check_gpgpusim="yes"

  SST_CHECK_CUDA([have_cuda=1],[have_cuda=0],[AC_MSG_ERROR([Cuda required, but not found])])

  AS_IF([test "$have_cuda" = 1], [sst_check_gpgpusim="yes"], [sst_check_gpgpusim="no"])
  AS_IF([test "$sst_check_gpgpusim" = "yes"], [$1], [$2])
])
