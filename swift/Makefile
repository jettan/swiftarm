CXX=arm-v7a8-linux-gnueabi-g++
CPPFLAGS+= -O2 -I . -I /home/jettan/Documents/swiftarm/lib/include -I /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/include -I /opt/armv7a8-toolchain-lite/arm-v7a8-linux-gnueabi/libc/include 
#CPPFLAGS+= -O2 -I . -I /home/jettan/Documents/swiftarm/lib2/include
CPPFLAGS +=-Wall -Wno-sign-compare -Wno-unused -g -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE 
LD=arm-v7a8-linux-gnueabi-ld
LDADD= $(HOME)/Documents/swiftarm/lib/lib/libevent.a -lrt -lstdc++ -lgcc -lutil -lpthread


all: ws

eventws: eventws.o
	$(CXX) ${CPPFLAGS} eventws.cpp -o eventws ${LDADD}

ws: ws.o socketlistener.o request.o requesthandler.o sha1.o compat.o sendrecv.o send_control.o hashtree.o bin.o binmap.o binheap.o channel.o transfer.o httpgw.o statsgw.o cmdgw.o avgspeed.o availability.o
#nat_test.o
	$(CXX) ${CPPFLAGS} -o ws *.o  ${LDADD}

ws.o: ws.cpp ws.h socketlistener.h request.h requesthandler.h compat.h swift.h
	$(CXX) $(CPPFLAGS) ws.cpp -c

socketlistener.o: socketlistener.cpp ws.h socketlistener.h request.h
	$(CXX) $(CPPFLAGS) socketlistener.cpp -c

request.o: request.cpp ws.h request.h socketlistener.h
	$(CXX) $(CPPFLAGS) request.cpp -c

requesthandler.o: requesthandler.cpp ws.h requesthandler.h request.h
	$(CXX) $(CPPFLAGS) requesthandler.cpp -c

sha1.o: sha1.cpp sha1.h 
	$(CXX) $(CPPFLAGS) sha1.cpp -c

compat.o: compat.cpp compat.h
	$(CXX) $(CPPFLAGS) compat.cpp -c

sendrecv.o: sendrecv.cpp bin_utils.h swift.h compat.h
	$(CXX) $(CPPFLAGS) sendrecv.cpp -c
	
send_control.o: send_control.cpp swift.h
	$(CXX) $(CPPFLAGS) send_control.cpp -c

hashtree.o: hashtree.cpp hashtree.h bin_utils.h sha1.h compat.h swift.h 
	$(CXX) $(CPPFLAGS) hashtree.cpp -c

bin.o: bin.cpp bin.h
	$(CXX) $(CPPFLAGS) bin.cpp -c

binmap.o: binmap.cpp binmap.h 
	$(CXX) $(CPPFLAGS) binmap.cpp -c

binheap.o: binheap.cpp binheap.h
	$(CXX) $(CPPFLAGS) binheap.cpp -c

channel.o: channel.cpp swift.h compat.h
	$(CXX) $(CPPFLAGS) channel.cpp -c

transfer.o: transfer.cpp swift.h ext/vod_picker.cpp ext/vod_picker.cpp
	$(CXX) $(CPPFLAGS) transfer.cpp -c

httpgw.o: httpgw.cpp swift.h
	$(CXX) $(CPPFLAGS) httpgw.cpp -c

statsgw.o: statsgw.cpp swift.h
	$(CXX) $(CPPFLAGS) statsgw.cpp -c

cmdgw.o: swift.h compat.h cmdgw.cpp
	$(CXX) $(CPPFLAGS) cmdgw.cpp -c

avgspeed.o: avgspeed.cpp avgspeed.h 
	$(CXX) $(CPPFLAGS) avgspeed.cpp -c

availability.o: availability.cpp availability.h 
	$(CXX) $(CPPFLAGS) availability.cpp -c

clean:
	rm -f *.o ws 2>/dev/null

.PHONY: all clean eventws
