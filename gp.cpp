#include "gp.h"

std::mutex Base::mutex;
std::vector<std::pair<std::string, size_t>> Base::values;

Generator::Generator(std::string gId, size_t left, size_t right, size_t sD) : g_id(gId), sleepDuration(sD)
{
	range.first = left;
	range.second = right;
	future = std::async(&Generator::pushValue, this);
}

void Generator::pushValue()
{
	{
		std::lock_guard<std::mutex> lock(Base::mutex);
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> dist(1, 1000);
		Base::values.push_back(std::make_pair(g_id, dist(mt)));
	}
	sleep();
}

void Generator::sleep() const
{
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
}

void Generator::setRange(size_t left, size_t right) {
	range.first = left;
	range.second = right;
}


Processor::Processor()
{
	future = std::async(&Processor::popValue, this);
}

void Processor::popValue()
{
	const std::lock_guard<std::mutex> lock(Base::mutex);
	if (!Base::values.empty()) {
		std::vector<std::pair<std::string, size_t>>::iterator it;
		for (it = Base::values.begin(); it < Base::values.end(); it++) {
			std::cout << (*it).first << ": " << (*it).second << "\n";
		}
		Base::values.clear();
		
	}
	
	
}
