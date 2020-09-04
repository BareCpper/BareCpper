/** Enable use of SteadyClock (i.e.e Realtime-Counter [RTC]) as the clock source of the std::chrono::steady_clock and std::chrono::system_clock
* @note Implementing std::chrono:steady clock required implementing gettimeofday(struct timeval*, void*)
* @todo Decide how to integrate this support if at all
*/
#if 0

// returning a set time of one second just for testing
int gettimeofday(struct timeval* tv, void* tz) {
	auto timeNow = BareCpper::SteadyClock::now().time_since_epoch();
	auto timeSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeNow);
	tv->tv_sec = timeSeconds.count();
	tv->tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(timeNow - timeSeconds).count();
	return 0;
}

#endif