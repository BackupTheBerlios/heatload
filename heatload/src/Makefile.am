# generated 2002/3/18 6:58:49 CET by thoma@Tiger.(none)
# using glademm V0.6.4b_cvs
#
# newer (non customized) versions of this file go to Makefile.am_new

bin_PROGRAMS = heatload
sbin_SCRIPTS = set_throttling

heatload_SOURCES = \
	gtkacpi.cc\
   gtk_acpi_menu.cc \
	gtk_acpi_glade.cc \
	gtk_acpi.cc \
   show_values.cc \
   get_values.cc \
   MeterStruct.cc \
   GraphDrawingArea.cc \
   RC.cc \
   TagStream.cc \
   Tag.cc \
   WindowInfo.cc \
   WindowInfo_glade.cc \
   itos.cc

man_MANS = heatload.1 set_throttling.1
EXTRA_DIST = $(man_MANS)

noinst_HEADERS = \
	gtk_acpi_glade.hh \
	gtk_acpi.hh \
   MeterStruct.hh \
   GraphDrawingArea.hh \
   RC.hh \
   TagStream.hh \
   Tag.hh \
   WindowInfo.hh \
   WindowInfo_glade.hh \
   itos.h 

CXXFLAGS = @CXXFLAGS@ @GTKMM_CFLAGS@ 

heatload_LDADD = @LIBS@ @GTKMM_LIBS@ 
