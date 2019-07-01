AM_CPPFLAGS = \
	$(MPIT_CPPFLAGS)

AM_LDFLAGS = -lm
compdir = $(pkglibdir)
comp_LTLIBRARIES = libGpgpusim.la

libGpgpusim_la_SOURCES = \
	mempool.h \
	mempool.cpp \
	Gpgpusim.cc \
	Gpgpusim.h

libGpgpusim_la_CPPFLAGS = \
	-I$(top_srcdir)/src \
	$(SST_CXX0X_FLAGS) -fPIC -Wall \
	$(MPI_CPPFLAGS) \
	$(CUDA_CPPFLAGS)

libGpgpusim_la_LDFLAGS = \
	-avoid-version

libGpgpusim_la_LIBADD = \
	$(SST_SYSTEMC_LIB) \
	-lcudart_mod

