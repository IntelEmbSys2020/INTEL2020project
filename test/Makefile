preEnv:
	mkdir build
client: clientTest.cpp
	g++ clientTest.cpp ../P2P/p2p.cpp -o ./build/client -I ../P2P -std=c++11
server: serverTest.cpp
	g++ serverTest.cpp ../P2P/p2p.cpp -o ./build/server -I ../P2P -std=c++11
clean:
	rm ./build/* -rf