#include "Thread.h"
#include "ThreadController.h"

template <typename Arg>
ThreadController<Arg>::ThreadController(long _interval): Thread<Arg>(){
	cached_size = 0;

	clear();
	this->setInterval(_interval);

	#ifdef USE_THREAD_NAMES
		// Overrides name
		ThreadName = "ThreadController ";
		ThreadName = ThreadName + ThreadID;
	#endif
}

/*
	ThreadController run()
*/
template <typename Arg>
void ThreadController<Arg>::run(){

	if(this->_callbackArgumentSet){
		// Run this thread before
		if(this->_onRunArg != NULL)
			this->_onRunArg(this->_callbackArgument);
	}else{
		// Run this thread before
		if(this->_onRun != NULL)
			this->_onRun();
	}

	long time = millis();
	int checks = 0;
	for(int i = 0; i < MAX_THREADS && checks <= cached_size; i++){
		// Object exists? Is enabled? Timeout exceeded?
		if(thread[i]){
			checks++;
			if(thread[i]->shouldRun(time)){
				thread[i]->run();
			}
		}
	}

	// ThreadController extends Thread, so we should flag as hasRun thread
	this->hasRun();
}

/*
	List controller (boring part)
*/
template <typename Arg>
bool ThreadController<Arg>::add(Thread<Arg>* _thread){
	// Check if the Thread already exists on the array
	for(int i = 0; i < MAX_THREADS; i++){
		if(thread[i] != NULL && thread[i]->ThreadID == _thread->ThreadID)
			return true;
	}

	// Find an empty slot
	for(int i = 0; i < MAX_THREADS; i++){
		if(!thread[i]){
			// Found a empty slot, now add Thread
			thread[i] = _thread;
			cached_size++;
			return true;
		}
	}

	// Array is full
	return false;
}

template <typename Arg>
void ThreadController<Arg>::remove(int id){
	// Find Threads with the id, and removes
	bool found = false;
	for(int i = 0; i < MAX_THREADS; i++){
		if(thread[i]->ThreadID == id){
			thread[i] = NULL;
			cached_size--;
			return;
		}
	}
}

template <typename Arg>
void ThreadController<Arg>::remove(Thread<Arg>* _thread){
	remove(_thread->ThreadID);
}

template <typename Arg>
void ThreadController<Arg>::clear(){
	for(int i = 0; i < MAX_THREADS; i++){
		thread[i] = NULL;
	}
	cached_size = 0;
}

template <typename Arg>
int ThreadController<Arg>::size(bool cached){
	if(cached)
		return cached_size;

	int size = 0;
	for(int i = 0; i < MAX_THREADS; i++){
		if(thread[i])
			size++;
	}
	cached_size = size;

	return cached_size;
}

template <typename Arg>
Thread<Arg>* ThreadController<Arg>::get(int index){
	int pos = -1;
	for(int i = 0; i < MAX_THREADS; i++){
		if(thread[i] != NULL){
			pos++;

			if(pos == index)
				return thread[i];
		}
	}

	return NULL;
}
