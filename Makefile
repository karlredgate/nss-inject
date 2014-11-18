
CLEANS += nss_inject.o
CLEANS += libnss_inject.so
libnss_inject.so: nss_inject.o
	$(CC) -shared $^ -o $@
