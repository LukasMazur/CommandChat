CXX=g++
LD=g++
CFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11

compile: obj

obj: Socket.o Socket.o Command_Base.o Command_Factory.o Server_Channel.o Command_Join.o Command_Leave.o Command_ChangeName.o Command_Message.o Server_Channel.o Server_User.o Server_Server.o Client_Client.o Client_Window.o Client_Channel.o main.o
	$(LD) -o commandchat Socket.o Command_Base.o Command_Factory.o Server_Channel.o Command_Join.o Command_Leave.o Command_ChangeName.o Command_Message.o Server_User.o Server_Server.o Client_Client.o Client_Window.o Client_Channel.o main.o -lncurses

Socket.o: src/Socket.cpp src/Socket.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Command_Base.o: src/Command/Base.cpp src/Command/Base.hpp src/Server/Server.hpp src/Client/Client.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Command_Factory.o: src/Command/Factory.cpp src/Command/Factory.hpp src/Command/Base.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Command_Join.o: src/Command/Join.cpp src/Command/Join.hpp src/Command/Factory.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Command_Leave.o: src/Command/Leave.cpp src/Command/Leave.hpp src/Command/Factory.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Command_ChangeName.o: src/Command/ChangeName.cpp src/Command/ChangeName.hpp src/Command/Factory.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Command_Message.o: src/Command/Message.cpp src/Command/Message.hpp src/Command/Factory.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Server_Channel.o: src/Server/Channel.cpp src/Server/Channel.hpp src/Server/User.hpp src/Command/Join.hpp src/Command/Leave.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

Server_User.o: src/Server/User.cpp src/Server/User.hpp src/Socket.hpp src/Server/Channel.hpp src/Command/Factory.hpp src/Command/Base.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Server_Server.o: src/Server/Server.cpp src/Server/Server.hpp src/Socket.hpp src/Server/User.hpp src/Server/Channel.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Client_Client.o: src/Client/Client.cpp src/Client/Client.hpp src/Command/Base.hpp src/Socket.hpp src/Command/Factory.hpp src/Client/Window.hpp src/Client/Channel.hpp src/Command/ChangeName.hpp src/Command/Join.hpp src/Command/Message.hpp src/Command/Join.hpp src/Command/Leave.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

Client_Window.o: src/Client/Window.cpp src/Client/Window.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
Client_Channel.o: src/Client/Channel.cpp src/Client/Channel.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
main.o: src/main.cpp src/Socket.hpp src/Server/Server.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o obj doc commandchat
	
doc:
	doxygen
	
all: compile doc

run:
	./commandchat --server 1234
	
run-client:
	./commandchat localhost 1234
