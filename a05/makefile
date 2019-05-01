.PHONY: clean debug

mizzo: belt.cpp consumer.cpp producer.cpp belt.hpp
	g++ -o mizzo belt.cpp producer.cpp consumer.cpp -lpthread

debug:
	g++ -g -o mizzo belt.cpp producer.cpp consumer.cpp -lpthread

clean:
	rm mizzo
