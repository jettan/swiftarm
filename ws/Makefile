CXX=arm-v7a8-linux-gnueabi-g++
CPPFLAGS+= -static -Wno-write-strings
ifeq ($(CXX),arm-v7a8-linux-gnueabi-g++)
	LIBS = /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/libutil.a
else
	LIBS = -lutil
endif

ws: ws.o socketlistener.o request.o requesthandler.o
	$(CXX) $(CPPFLAGS) ws.o socketlistener.o request.o requesthandler.o -o ws $(LIBS)

ws.o: ws.cpp ws.h socketlistener.h request.h requesthandler.h
	$(CXX) $(CPPFLAGS) ws.cpp -c

socketlistener.o: socketlistener.cpp ws.h socketlistener.h request.h
	$(CXX) $(CPPFLAGS) socketlistener.cpp -c

request.o: request.cpp ws.h request.h socketlistener.h
	$(CXX) $(CPPFLAGS) request.cpp -c

requesthandler.o: requesthandler.cpp ws.h requesthandler.h request.h
	$(CXX) $(CPPFLAGS) requesthandler.cpp -c

clean:
	rm -f *.o ws
