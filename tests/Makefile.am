
TESTS = test

AM_CPPFLAGS = -I$(top_srcdir)/include
LIBS = $(top_builddir)/src/libxmlwrapp.la \
	   -lboost_unit_test_framework \
	   -lboost_iostreams

noinst_PROGRAMS = test

test_SOURCES = \
		test.h \
		test_main.cxx \
		attributes/test_attributes.cxx \
		document/test_document.cxx \
		event/test_event.cxx \
		node/test_node.cxx \
		tree/test_tree.cxx \
		namespaces/test_namespaces.cxx

if WITH_XSLT
LIBS += $(top_builddir)/src/libxsltwrapp.la
test_SOURCES += xslt/test_xslt.cxx
endif

EXTRA_DIST = \
		$(srcdir)/*/data/*.out \
		$(srcdir)/*/data/*.xml \
		$(srcdir)/*/data/*.xsl \
		$(srcdir)/*/data/*.dtd \
		$(srcdir)/*/data/output
