ifeq ($(SRCDIR)x,x)
SRCDIR = $(CURDIR)
endif
SUBDIR = .
BUILDDIR = $(SRCDIR)
VPATH = .:$(SRCDIR)

include $(SRCDIR)/Makefile.config

SUBDIRS = include lib src tools examples

PROGRAMS_TO_INSTALL = xmlrpc-c-config

# We're in a transition between the bloated, complex GNU
# Autoconf/Automake style of build, in which 'configure' creates all
# the make files, to simpler static make files.  Some directories have
# been converted; some haven't.  So we have the hack of putting
# 'xmlrpc_config.h' as the first dependency of 'all' to make sure
# 'configure runs before anything in the case that the user neglects
# to run 'configure' before doing 'make'.

default: xmlrpc_config.h all

.PHONY: all
all: xmlrpc-c-config xmlrpc-c-config.test $(SUBDIRS:%=%/all)

version.h: $(BUILDDIR)/Makefile.config
	rm -f $@
	echo '/* This file was generated by a make rule */' >>$@
	echo 'XMLRPC_C_VERSION=$(XMLRPC_C_VERSION)' >>$@

transport_config.h: $(BUILDDIR)/Makefile.config
	rm -f $@
	echo '/* This file was generated by a make rule */' >>$@
ifeq ($(MUST_BUILD_WININET_CLIENT),yes)
	echo '#define MUST_BUILD_WININET_CLIENT 1' >>$@
else
	echo '#define MUST_BUILD_WININET_CLIENT 0' >>$@
endif
ifeq ($(MUST_BUILD_CURL_CLIENT),yes)
	echo '#define MUST_BUILD_CURL_CLIENT 1' >>$@
else
	echo '#define MUST_BUILD_CURL_CLIENT 0' >>$@
endif
ifeq ($(MUST_BUILD_LIBWWW_CLIENT),yes)
	echo '#define MUST_BUILD_LIBWWW_CLIENT 1' >>$@
else
	echo '#define MUST_BUILD_LIBWWW_CLIENT 0' >>$@
endif
	echo "static const char * const XMLRPC_DEFAULT_TRANSPORT =" >>$@
ifeq ($(MUST_BUILD_LIBWWW_CLIENT),yes)
	echo '"libwww";' >>$@
else
  ifeq ($(MUST_BUILD_CURL_CLIENT),yes)
	echo '"curl";' >>$@
  else
    ifeq ($(MUST_BUILD_WININET_CLIENT),yes)
	echo '"wininet";' >>$@
    else
	@echo 'ERROR: no client XML transport configured'; rm $@; false
    endif
  endif
endif

.PHONY: clean clean-local
clean: $(SUBDIRS:%=%/clean) clean-common clean-local

clean-local:

.PHONY: distclean distclean-local
distclean: $(SUBDIRS:%=%/distclean) distclean-common distclean-local

distclean-local:
	rm -f version.h
	rm -f transport_config.h

.PHONY: tags
tags: $(SUBDIRS:%=%/tags) TAGS

DISTFILES = 

.PHONY: distdir
distdir: distdir-common

.PHONY: install
install: $(SUBDIRS:%=%/install) install-common install-compat-hdr

.PHONY: install-compat-hdr
install-compat-hdr:
# Install old names of header files for backward compatibility
	cd $(DESTDIR)$(HEADERINST_DIR); \
	  $(LN_S) xmlrpc-c/oldxmlrpc.h         xmlrpc.h; \
	  $(LN_S) xmlrpc-c/client.h            xmlrpc_client.h; \
	  $(LN_S) xmlrpc-c/server.h            xmlrpc_server.h; \
	  $(LN_S) xmlrpc-c/server_cgi.h        xmlrpc_cgi.h; \
	  $(LN_S) xmlrpc-c/server_abyss.h      xmlrpc_server_abyss.h; \
	  $(LN_S) xmlrpc-c/server_w32httpsys.h xmlrpc_server_w32httpsys.h; \
	  $(LN_S) xmlrpc-c/oldcppwrapper.hpp   XmlRpcCpp.h ;\

include/xmlrpc-c/config.h:
	$(MAKE) -C include xmlrpc-c/config.h

.PHONY: dep
dep: version.h include/xmlrpc-c/config.h $(SUBDIRS:%=%/dep)

xmlrpc-c-config xmlrpc-c-config.test xmlrpc_config.h xmlrpc_amconfig.h \
	:%:%.in $(SRCDIR)/configure
	$(SRCDIR)/configure

include $(SRCDIR)/Makefile.common
