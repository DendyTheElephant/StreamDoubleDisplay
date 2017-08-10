#include "CTimer.h"

namespace CommonPhetamine {

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- Internal methods -----                                                                                                     ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Updates the elapsed ticks: adds to the elapsed ticksthe duration between now and previous ticks using the consistent clock.
	/// And updates the start tick value.
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CTimer::_updateElapsedTicks() {
		//if (m_isHightPerformanceClockIsSteady)
		//	m_elapsedTicks += std::chrono::high_resolution_clock::now() - m_startTicks;
		//else
			m_elapsedTicks += std::chrono::steady_clock::now() - m_startTicks;
		_updateStartTicks();
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Sets start ticks to current tick value using the consistent clock.
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CTimer::_updateStartTicks() {
		//if (m_isHightPerformanceClockIsSteady)
		//	m_startTicks = std::chrono::high_resolution_clock::now();
		//else
			m_startTicks = std::chrono::steady_clock::now();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- Getter -----                                                                                                               ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Returns the elapsed time in microseconds since last Timer() OR Timer.reset().
	/// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
	/// If the timer is in pause: returns the elapsed time since it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	pxUInt64 CTimer::getElapsedMicroseconds() {
		if (m_isPaused)
			return std::chrono::duration_cast<std::chrono::microseconds>(m_elapsedTicks).count();
		else {
			_updateElapsedTicks();
			return std::chrono::duration_cast<std::chrono::microseconds>(m_elapsedTicks).count();
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Returns the elapsed time in nanoseconds since last Timer() OR Timer.reset().
	/// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
	/// If the timer is in pause: returns the elapsed time since it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	pxUInt64 CTimer::getElapsedNanoseconds() {
		if (m_isPaused)
			return std::chrono::duration_cast<std::chrono::nanoseconds>(m_elapsedTicks).count();
		else {
			_updateElapsedTicks();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(m_elapsedTicks).count();
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Returns the elapsed time in milliseconds since last Timer() OR Timer.reset().
	/// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
	/// If the timer is in pause: returns the elapsed time since it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	pxUInt64 CTimer::getElapsedMiliseconds() {
		if (m_isPaused)
			return std::chrono::duration_cast<std::chrono::milliseconds>(m_elapsedTicks).count();
		else {
			_updateElapsedTicks();
			return std::chrono::duration_cast<std::chrono::milliseconds>(m_elapsedTicks).count();
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Returns the elapsed time in seconds since last Timer() OR Timer.reset().
	/// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
	/// If the timer is in pause: returns the elapsed time since it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	pxUInt64 CTimer::getElapsedSeconds() {
		if (m_isPaused)
			return std::chrono::duration_cast<std::chrono::seconds>(m_elapsedTicks).count();
		else {
			_updateElapsedTicks();
			return std::chrono::duration_cast<std::chrono::seconds>(m_elapsedTicks).count();
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Returns the elapsed time in minutes since last Timer() OR Timer.reset().
	/// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
	/// If the timer is in pause: returns the elapsed time since it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	pxUInt64 CTimer::getElapsedMinutes() {
		if (m_isPaused)
			return std::chrono::duration_cast<std::chrono::minutes>(m_elapsedTicks).count();
		else {
			_updateElapsedTicks();
			return std::chrono::duration_cast<std::chrono::minutes>(m_elapsedTicks).count();
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Returns the elapsed time in hours since last Timer() OR Timer.reset().
	/// Takes into account Timer.pause() and Timer.resume() -> elapsed time ;)
	/// If the timer is in pause: returns the elapsed time since it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	pxUInt64 CTimer::getElapsedHours() {
		if (m_isPaused)
			return std::chrono::duration_cast<std::chrono::hours>(m_elapsedTicks).count();
		else {
			_updateElapsedTicks();
			return std::chrono::duration_cast<std::chrono::hours>(m_elapsedTicks).count();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- Core -----                                                                                                                 ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Resets the timer (elapsed time -> near 0) and starts it if it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CTimer::reset() {
		m_isPaused = false;
		_updateStartTicks();
		// Elapsed ticks are now reset:
		m_elapsedTicks = std::chrono::duration<double>(0);
		/*if (m_isHightPerformanceClockIsSteady)
			m_elapsedTicks = std::chrono::high_resolution_clock::now() - m_startTicks;
		else
			m_elapsedTicks = std::chrono::steady_clock::now() - m_startTicks;*/
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Resume the timer if it was paused!
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CTimer::resume() {
		if (m_isPaused) {
			_updateStartTicks();
			m_isPaused = false;
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	/// Pauses the timer. This means that the elapsed time will not be updated: to unpause call Timer.resume() or Timer.reset()
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CTimer::pause() {
		_updateElapsedTicks();
		m_isPaused = true;
	}
}
