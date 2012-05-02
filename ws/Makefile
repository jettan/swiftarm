CXX= arm-v7a8-linux-gnueabi-g++
CPPFLAGS+= -static
LD=arm-v7a8-linux-gnueabi-ld 

ws: ws.o socketlistener.o request.o requesthandler.o
	$(CXX) $(CFLAGS) $(CPPFLAGS) ws.o socketlistener.o request.o requesthandler.o -o ws /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/usr/lib/libutil.a

ws.o: ws.cpp ws.h socketlistener.h request.h requesthandler.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) ws.cpp -c

socketlistener.o: socketlistener.cpp ws.h socketlistener.h request.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) socketlistener.cpp -c

request.o: request.cpp ws.h request.h socketlistener.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) request.cpp -c

requesthandler.o: requesthandler.cpp ws.h requesthandler.h request.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) requesthandler.cpp -c

clean:
	rm -f *.o ws
