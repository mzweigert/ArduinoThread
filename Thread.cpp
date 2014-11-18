
#include "Thread.h"

template <typename Arg>
Thread<Arg>::Thread(void (*callback)(), long _interval){
	enabled = true;
	_callbackArgumentSet = false;
	onRun(callback);
	_cached_next_run = 0;
	last_run = 0;

	ThreadID = (int)this;
	#ifdef USE_THREAD_NAMES
		ThreadName = "Thread ";
		ThreadName = ThreadName + ThreadID;
	#endif

	setInterval(_interval);
}

template <typename Arg>
Thread<Arg>::Thread(void (*callback)(Arg), Arg arg, long _interval){
	enabled = true;
	_callbackArgument = arg;
	_callbackArgumentSet = true;
	onRun(callback);
	_cached_next_run = 0;
	last_run = 0;

	ThreadID = (int)this;
	#ifdef USE_THREAD_NAMES
		ThreadName = "Thread ";
		ThreadName = ThreadName + ThreadID;
	#endif

	setInterval(_interval);
}

template <typename Arg>
void Thread<Arg>::hasRun(long time){
	// If less than 0, than get current ticks
	if(time < 0)
		time = millis();

	// Saves last_run
	last_run = time;

	// Cache next run
	_cached_next_run = last_run + interval;
}

template <typename Arg>
void Thread<Arg>::setInterval(long _interval){
	// Filter intervals less than 0
	interval = (_interval < 0? 0: _interval);

	// Cache the next run based on the last_run
	_cached_next_run = last_run + interval;
}

template <typename Arg>
bool Thread<Arg>::shouldRun(long time){
	// If less than 0, then get current ticks
	if(time < 0)
		time = millis();

	// Exceeded the time limit, AND is enabled? Then should run...
	return ((time >= _cached_next_run) && enabled);
}

template <typename Arg>
void Thread<Arg>::onRun(void (*callback)(void)){
	_onRun = callback;
}

template <typename Arg>
void Thread<Arg>::onRun(void (*callback)(Arg)){
	_onRunArg = callback;
}

template <typename Arg>
void Thread<Arg>::run(){

	if(_callbackArgumentSet){
		if(_onRunArg != NULL)
			_onRunArg(this->_callbackArgument);
	}else{
		if(_onRun != NULL)
			_onRun();
	}
	
	// Update last_run and _cached_next_run
	hasRun();
}
