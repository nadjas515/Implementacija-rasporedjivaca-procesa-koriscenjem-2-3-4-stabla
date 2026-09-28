#include "Key.h"

Key::Key() :
	name(""),
	time_to_complete(0),
	maximum_waiting_time(0),
	waiting_time(0),
	execution_time(0),
	colour(red) {
}

Key::Key(Key* other) :
	name(other->name),
	time_to_complete(other->time_to_complete),
	maximum_waiting_time(other->maximum_waiting_time),
	waiting_time(other->waiting_time),
	execution_time(other->execution_time),
	colour(other->colour) {
}

Key::Key(std::string name, int complete, int waiting) :
	name(name),
	time_to_complete(complete),
	maximum_waiting_time(waiting),
	waiting_time(0),
	execution_time(0),
	colour(red) {
}

bool Key::operator>(Key other) {
	return waiting_time > other.waiting_time;
}

bool Key::operator<(Key other) {
	return waiting_time < other.waiting_time;
}

int Key::getWaitingTime() {
	return waiting_time;
}

int Key::getExecutionTime() {
	return execution_time;
}

void Key::print(std::ostream& output) {
	if (colour == red) output << "red ";
	else output << "black ";
	output << name << " w:" << waiting_time << " e:" << execution_time;
}

bool Key::operator==(Key other) {
	return name == other.name\
		&& waiting_time == other.waiting_time \
		&& execution_time == other.execution_time;
}