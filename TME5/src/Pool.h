#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize) : queue(qsize) {}
	void start(int nbt){
        threads.reserve(nbt);
        for (int i=0;i<nbt;i++){
            threads.emplace_back(&Pool::poolworker, this);
        }
    }
	void submit (Job *j){
        queue.push(j);
    }
	void stop(){
        queue.setBlocking(false);
        for (auto &t : threads){
            t.join();
        }
        threads.clear();
    }
	void poolworker() {
        while ( true ){
            Job* j = queue.pop();
            if (j==nullptr) {break;}
            j->run();
            delete j;
        }
    }
	~Pool(){stop();}
};

}
