VERSION=0.3
//CXXFLAGS=-Wall -O2 -g -DVERSION=\"$(VERSION)\"
CXXFLAGS=-Wall -O0 -g -DVERSION=\"$(VERSION)\"
TAR=heatload-$(VERSION).tar.gz
SOURCES=heatload.cc
BINARY=heatload
LIBS=`sigc-config --cflags --libs` `gtkmm-config --cflags --libs` `gnome-config --cflags --libs gnome gnomeui` -lgnomemm 
PACKAGE=heatload

all: $(BINARY)

$(BINARY): $(SOURCES)
	g++ $(CXXFLAGS) $(SOURCES) -o $(BINARY) $(LIBS)

strip: $(BINARY)
	@du -h $(BINARY)
	strip -s $(BINARY)
	@du -h $(BINARY)

clean:
	rm -f $(BINARY)
	rm -f *~ *.bak *.tar.gz

install: strip
	install -g root -o root -m 555 $(BINARY) /usr/local/bin

deinstall:
	rm -f /usr/local/bin/$(BINARY)

README: README.in
	sed s/VERSION/$(VERSION)/ < README.in > README

index.html: README
	txt2html -t "$(PACKAGE) $(VERSION)" < README > index.html

html: index.html

$(TAR): clean README
	tar -C.. --exclude=$(PACKAGE)/$(TAR) -czvf $(TAR) $(PACKAGE)

tar: $(TAR)

web: $(TAR) README
	cp README $(TAR) ../../homepage/lennart/projects/$(PACKAGE)/ && $(MAKE) -C ../../homepage/lennart/projects/$(PACKAGE)

#upload: web
#	$(MAKE) -C ../../homepage/lennart upload
