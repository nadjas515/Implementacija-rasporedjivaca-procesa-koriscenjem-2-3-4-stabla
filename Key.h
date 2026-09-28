#include <string>
#include <iostream>

enum colour { red, black };

class Key
{
public:
	Key();
	Key(Key* other);
	Key(std::string name, int complete, int waiting);
	void incrementWaitingTime(int increment);
	int incrementExecutionTime(int increment);
	bool isDone();
	bool isWaitingTimeExceeded();
	void resetWaitingTime();
	colour getColour();
	bool operator>(Key other);
	bool operator<(Key other);
	void turnBlack();
	void turnRed();
	int getWaitingTime();
	int getExecutionTime();
	void print(std::ostream& output);
	bool operator==(Key other);

private:
	std::string name;
	int time_to_complete;
	int maximum_waiting_time;
	int waiting_time;
	int execution_time;
	colour colour;
};

inline void Key::incrementWaitingTime(int increment) {
	waiting_time += increment;

}

inline int Key::incrementExecutionTime(int increment) {
	if (execution_time + increment <= time_to_complete) {
		execution_time += increment;
		return increment;
	}
	int p = time_to_complete - execution_time;
	execution_time = time_to_complete;
	return p;
}

inline bool Key::isDone()
{
	return execution_time == time_to_complete;
}

inline bool Key::isWaitingTimeExceeded() {
	return waiting_time >= maximum_waiting_time;
}

inline void Key::resetWaitingTime() {
	waiting_time -= maximum_waiting_time;
}

inline colour Key::getColour()
{
	return colour;
}

inline void Key::turnBlack() {
	colour = black;
}

inline void Key::turnRed() {
	colour = red;
}
