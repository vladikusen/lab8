#include <iostream>
#include <array>
#include <random>
#include <chrono>
#include <future>
#include <vector>
#include <conio.h>
#include "gp.h"

constexpr size_t arrayLength = 5008;
constexpr size_t parts = 16;
constexpr size_t partLength = arrayLength / parts;
std::vector<size_t> generatorValues;


size_t getRandomNumber() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, arrayLength);

	return dist(mt);
}

size_t squarefy(std::array<size_t, arrayLength>& arr, size_t whichPart = arrayLength) {
	auto start = std::chrono::high_resolution_clock::now();
	auto end = start;
	if (whichPart == arrayLength) {
		std::array<size_t, arrayLength>::iterator it;
		start = std::chrono::high_resolution_clock::now();
		for (it = arr.begin(); it < arr.end(); it++) {
			*it *= *it;
		}
		end = std::chrono::high_resolution_clock::now();
	}
	else {
		start = std::chrono::high_resolution_clock::now();
		for (auto i = partLength * whichPart; i < partLength * whichPart + partLength; i++) {
			arr[i] *= arr[i];
		}
		end = std::chrono::high_resolution_clock::now();
	}

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	return duration.count();
}

void fillArray(std::array<size_t, arrayLength>& arr) {
	std::array<size_t, arrayLength>::iterator it;

	for (it = arr.begin(); it < arr.end(); it++) {
		*it = getRandomNumber();
	}
}

void printArray(std::array<size_t, arrayLength>& arr) {
	std::array<size_t, arrayLength>::iterator it;

	for (it = arr.begin(); it < arr.end(); it++) {
		std::cout << *it << "   ";
	}
	std::cout << "\n";
}
int main()
{
	using array = std::array<size_t, arrayLength>;

	array arr;
	array arr2;
	fillArray(arr);
	fillArray(arr2);

	using future = std::future<size_t>;
	std::array<future, parts> futureArray;
	for (auto i = 0; i < parts; i++) {
		futureArray[i] = std::async(std::launch::deferred, squarefy, ref(arr), i);
	}

	auto timeSuccessively = squarefy(arr2);

	auto time = [&futureArray]() {
		auto duration = 0;
		for (auto i = 0; i < parts; i++) {
			duration += futureArray[i].get();
		}


		return duration;
	};

	std::cout << "Time async: " << time() << "\n";
	std::cout << "Time successively: " << timeSuccessively;



	Generator g1("G1", 0, 100, 200);
	Generator g2("G2", 0, 50, 500);
	Generator g3("G3", 0, 10, 0);
	Generator g4("G4", 5, 30, 40);
	Generator g5("G5", 100, 1000, 500);
	Processor p1;

	while (!_kbhit()) {
		g1.pushValue();
		g2.pushValue();
		g3.pushValue();
		g4.pushValue();
		g5.pushValue();
		p1.popValue();	
	}


	return 0;
}
