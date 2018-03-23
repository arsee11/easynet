SRC = ./
Inc = ./
CXX = g++
FLAGS = --std=c++11
Target = ../lib/libarseenet.a
OBJS = udpsock.o acceptor.o connector.o epoll.o netevent_listener_helper.o \
	netpeer.o

$(Target):$(OBJS)
	ar -rs $(Target) $(OBJS) 

udpsock.o: fddef.h addr.h udpsock.h 
	$(CXX) -c $(FLAGS) udpsock.cpp

acceptor.o: fddef.h addr.h netpeer.h ../namespdef.h 
	$(CXX) -c $(FLAGS) acceptor.cpp

connector.o: addr.h netpeer.h ../namespdef.h 
	$(CXX) -c $(FLAGS) connector.cpp

epoll.o: fddef.h netevents.h ../namespdef.h acceptor.h netpeer.h 
	$(CXX) -c $(FLAGS) epoll.cpp

netevent_listener_helper.o: netevents.h ../namespdef.h netevent_listener.h 
	$(CXX) -c $(FLAGS) netevent_listener_helper.cpp

netpeer.o: fddef.h addr.h event.h ../namespdef.h
	$(CXX) -c $(FLAGS) netpeer.cpp

clean:
	rm $(Target) $(OBJS)
