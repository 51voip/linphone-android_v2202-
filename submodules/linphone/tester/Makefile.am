EXTRA_DIST=pauline_rc laure_rc marie_rc marie_no_sdp_rc


if BUILD_CUNIT_TESTS

noinst_PROGRAMS=liblinphone_tester  
TESTS=$(noinst_PROGRAMS)

liblinphone_tester_SOURCES=	liblinphone_tester.c liblinphone_tester.h\
				setup_tester.c \
				register_tester.c \
				message_tester.c \
				call_tester.c \
				presence_tester.c \
				upnp_tester.c \
				eventapi_tester.c


#liblinphone_tester_CFLAGS=$(CUNIT_CFLAGS)

#liblinphone_tester_LDFLAGS=$(CUNIT_LIBS)


AM_CPPFLAGS=-I$(top_srcdir)/include -I$(top_srcdir)/coreapi

LDADD=$(top_builddir)/coreapi/liblinphone.la 

AM_LDFLAGS=$(CUNIT_LIBS)

AM_CFLAGS=$(STRICT_OPTIONS)  -DIN_LINPHONE $(ORTP_CFLAGS) $(MEDIASTREAMER_CFLAGS) $(CUNIT_CFLAGS)  

endif
