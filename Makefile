targets:trend_server send_msg bcast_msg trend_daqconf trend_getconf trend_netconf dump_bin_file

CXX=clang++
CXX_FLAGS=-O3 -pthread -std=c++14 -g

LD_FLAGS=-lboost_system -pthread -std=c++14

trend_server:trend_server.o trend_message.o data_file.o
	$(CXX) $^ -o $@ $(LD_FLAGS)

trend_server.o:trend_server.cpp
	$(CXX) -c $(CXX_FLAGS) $<

trend_getconf:trend_getconf.o 
	$(CXX) $^ -o $@ $(LD_FLAGS)

trend_getconf.o: trend_getconf.cpp 
	$(CXX) -c $(CXX_FLAGS) $<

trend_daqconf:trend_daqconf.o 
	$(CXX) $^ -o $@ $(LD_FLAGS)

trend_daqconf.o: trend_daqconf.cpp 
	$(CXX) -c $(CXX_FLAGS) $<

trend_netconf:trend_netconf.o 
	$(CXX) $^ -o $@ $(LD_FLAGS)

trend_netconf.o: trend_netconf.cpp 
	$(CXX) -c $(CXX_FLAGS) $<

trend_message.o:trend_message.cpp trend_message.hpp
	$(CXX) -c $(CXX_FLAGS) $<

data_file.o:data_file.cpp data_file.hpp
	$(CXX) -c $(CXX_FLAGS) $<

send_msg:send_msg.o trend_message.o data_file.o
	$(CXX) $^ -o $@ $(LD_FLAGS)

send_msg.o:send_msg.cpp
	$(CXX) -c $(CXX_FLAGS) $<

bcast_msg:bcast_msg.o
	$(CXX) $^ -o $@ $(LD_FLAGS)

bcast_msg.o:bcast_msg.cpp
	$(CXX) -c $(CXX_FLAGS) $<

dump_bin_file: dump_bin_file.o data_file.o
	$(CXX) $^ -o $@ $(LD_FLAGS)

dump_bin_file.o: dump_bin_file.cpp
	$(CXX) -c $(CXX_FLAGS) $<

clean:
	rm -f *.o
	rm -f *~
