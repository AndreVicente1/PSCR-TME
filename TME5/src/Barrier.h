/*
 * Barrier.h
 *
 *  Created on: 18 oct. 2023
 *      Author: 28706087
 */

#ifndef BARRIER_H_
#define BARRIER_H_

#include <mutex>
#include <condition_variable>


class Barrier {
private:
	int counter;
	const int max;
	std::mutex m;
	std::condition_variable cv;

public:
	Barrier(int m) : counter(0), max(m){}

	void done(){
		std::unique_lock<std::mutex> ul(m);
		counter++;
		if(counter==max){
			cv.notify_all();
		}
	}

	void waitFor(){
		std::unique_lock<std::mutex> ul(m);
		while(counter != max){
			cv.wait(ul);
		}
	}

};

#endif /* BARRIER_H_ */
