############################################################################
# apps/examples/ocf_mylight/Makefile
#
#   Copyright (C) 2008, 2010-2013 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

# built-in application info

APPNAME = ocf_mylight
THREADEXEC = TASH_EXECMD_SYNC

# Example
ASRCS =
CSRCS = ocf_mylight_device.c ocf_mylight_light.c ocf_mylight_verbose.c ocf_mylight_configuration.c ocf_mylight_maintenance.c ocf_mylight_platform.c
MAINSRC = ocf_mylight_main.c

# port_tinyara.c from apps/examples/iotivity_simpleServer
ifeq ($(CONFIG_EXAMPLES_IOTIVITY),y)
CSRCS += port_tinyara.c
else
CSRCS += port_tinyara.c
endif

IOTIVITY_RELEASE=${shell echo $(CONFIG_IOTIVITY_RELEASE_VERSION) | sed 's/"//g'}
IOTIVITY_BASE_DIR = $(APPDIR)/../external/iotivity/iotivity_$(IOTIVITY_RELEASE)

CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/api
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/common/inc
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/logger/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/security/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/stack/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/extlibs/mbedtls/mbedtls/include

CFLAGS += -DWITH_POSIX -DIP_ADAPTER
CFLAGS += -DWITH_BWT
ifeq ($(CONFIG_IOTIVITY_ROUTING),"EP")
CFLAGS += -DROUTING_EP
else
CFLAGS += -DROUTING_GW
endif

CFLAGS += -D__TINYARA__
CFLAGS += -D__TIZENRT__
CFLAGS += -D__WITH_DTLS__

CFLAGS += -std=c99
CFLAGS += -w

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
  OBJS += $(MAINOBJ)
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\..\\libapps$(LIBEXT)
else
  BIN = ../../libapps$(LIBEXT)
endif
endif

ifeq ($(WINTOOL),y)
  INSTALL_DIR = "${shell cygpath -w $(BIN_DIR)}"
else
  INSTALL_DIR = $(BIN_DIR)
endif

CONFIG_EXAMPLES_OCFMYLIGHT_PROGNAME ?= ocf_mylight$(EXEEXT)
PROGNAME = $(CONFIG_EXAMPLES_OCFMYLIGHT_PROGNAME)


ROOTDEPPATH = --dep-path .

# Common build

VPATH =

all: .built
.PHONY: clean depend distclean preconfig

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built

ifeq ($(CONFIG_BUILD_KERNEL),y)
$(BIN_DIR)$(DELIM)$(PROGNAME): $(OBJS) $(MAINOBJ)
	@echo "LD: $(PROGNAME)"
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $(INSTALL_DIR)$(DELIM)$(PROGNAME) $(ARCHCRT0OBJ) $(MAINOBJ) $(LDLIBS)
	$(Q) $(NM) -u  $(INSTALL_DIR)$(DELIM)$(PROGNAME)

install: $(BIN_DIR)$(DELIM)$(PROGNAME)

else
install:

endif

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_OCFMYLIGHT),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,$(APPNAME),$(APPNAME)_main,$(THREADEXEC),$(PRIORITY),$(STACKSIZE))

context: $(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat

else
context:

endif

.depend: Makefile $(SRCS)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
preconfig:
