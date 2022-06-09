#ifndef __GP_H__
#define __GP_H__

#include <vector>
#include <mutex>
#include <future>
#include <random>
#include <iostream>
#include <thread>
#include <chrono>


class Base
{
protected:
	static std::vector<std::pair<std::string, size_t>> values;
	static std::mutex mutex;
	std::future<void> future;
public:
	Base() = default;
};


class Generator : public Base {
	std::string g_id;
	std::pair<size_t, size_t> range;
	size_t sleepDuration;
public:
	Generator(std::string gId, size_t left = 0, size_t right = 100, size_t sD = 200);
	void pushValue();
	void setRange(size_t, size_t);
	void setSleepDuration(size_t);
	void sleep() const;
	void deleteSelf() { future.get(); }
	~Generator() = default;
};

class Processor : public Base {
public:
	Processor();
	void popValue();
};

#endif

