
PACKAGE = nss-inject
CFLAGS += -fPIC

default: rpm

rpm: build
	rm -rf rpm
	mkdir -p rpm/BUILD rpm/RPMS rpm/BUILDROOT
	rpmbuild --quiet -bb --buildroot=$(PWD)/rpm/BUILDROOT $(PACKAGE).spec

build: libnss_inject.so

CLEANS += nss_inject.o
CLEANS += libnss_inject.so
libnss_inject.so: nss_inject.o
	$(CC) -shared $^ -o $@

clean:
	rm -f $(CLEANS)
	rm -rf rpm
