# generated 2002/3/18 6:58:49 CET by thoma@Tiger.(none)
# using glademm V0.6.4b_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new

etcdir = /etc

bin_PROGRAMS = heatload
sbin_SCRIPTS = set_throttling swsusp_sleep swsusp_awake
etc_DATA = heatload.conf

heatload_SOURCES = \
   Gizmo.cc \
	gtkacpi.cc\
   gtk_acpi_menu.cc \
	gtk_acpi_glade.cc \
	gtk_acpi.cc \
   show_values.cc \
   MeterStruct.cc \
   GraphDrawingArea.cc \
   RC.cc \
   TagStream.cc \
   Tag.cc \
   WindowInfo.cc \
   WindowInfo_glade.cc \
   FileFinder.cc \
   itos.cc

man_MANS = heatload.1 set_throttling.1 swsusp_awake.1 swsusp_sleep.1
EXTRA_DIST = $(man_MANS)

noinst_HEADERS = \
   Gizmo.hh \
	gtk_acpi_glade.hh \
	gtk_acpi.hh \
   MeterStruct.hh \
   GraphDrawingArea.hh \
   TagStream.h \
   Tag.h \
   WindowInfo.hh \
   WindowInfo_glade.hh \
   FileFinder.hh \
   itos.h 

CXXFLAGS = @CXXFLAGS@ @GTKMM_CFLAGS@ 

heatload_LDADD = @LIBS@ @GTKMM_LIBS@ 
