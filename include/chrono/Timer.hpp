#ifndef CHRONO_TIMER_HPP
#define CHRONO_TIMER_HPP

#include "ClockTime.hpp"
#include "MicroTime.hpp"

namespace chrono {

/*! \brief Timer Class
 * \details This class implements a logical timer based on the Stratify OS
 * system timer.
 *
 * Physical timers are controlled using the hal::Tmr class.
 *
 * The Timer has the following states:
 *
 * - Reset: not running, not stopped
 * - Running: running, not stopped
 * - Stopped: not running, stopped
 *
 * These methods are used to jump between states:
 *
 * - start(): start running if not already running
 * - restart(): restart running from zero
 * - stop(): stop running (if running)
 * - resume(): resume counting after a stop
 * - reset(): set to not running, not stopped, zero value
 *
 *
 * Here is an example of using the timer to time events.
 * \code
 * #include <sapi/sys.hpp>
 *
 * int main(int argc, char * argv[]){
 * 	Timer t;
 * 	t.start(); //start
 * 	Timer::wait(500);
 * 	t.stop();
 * 	printf("Timer value after 500usec is %d\n", t.usec());
 *
 * }
 * \endcode
 *
 * The output of the above code varies depending on the MCU clock cycles and the
 * scheduler.  The following is a sample output.
 *
 *     Timer value after 500usec is 502
 *
 *
 */
class Timer : public api::ChronoWorkObject {
public:


    //deprecated methods
    static void wait_sec(u32 timeout){ wait_seconds(timeout); }
    static void wait_msec(u32 timeout){ wait_milliseconds(timeout); }
    static void wait_usec(u32 timeout){ wait_microseconds(timeout); }
#if !defined __link

    /*! \details Constructs an empty Timer. */
    Timer();


    /*! \details Starts the timer.
     *
     * If the timer is currently running, this method has no effect.  If the timer has been
     * stopped, it will restart.  Use resume() to resume running a stopped timer.
     *
     */
    void start(); //start counting

    /*! \details Restarts the timer.
     *
     * If the timer is currently running,
     * it starts over.  If it is not running, it is started.
     *
     */
    void restart();


    /*! \details Resumes counting after a stop().
     *
     * If the timer is currently running
     * this method has no effect.  If the timer has not been started, this method will
     * start the timer.
     */
    void resume();

    /*! \details Returns true if the timer is currently counting meaning
     * it has been started but has not been stopped.
     *
     * If the timer has been stopped and resumed, this method will return true;
     *
     */
    bool is_running() const { return !is_stopped(); }

    /*! \details Returns true if the timer has been started.
     *
     * It the timer has been started and stopped, this method will return true.
     * If the timer has been reset() or never started, this method will return false.
     *
     */
    bool is_started() const { return (m_start.nanoseconds() + m_start.seconds()) != 0; }

    /*! \details Returns true if the timer is stopped.
     *
     * If the timer has not yet been started or has been reset(), this method will return true.
     * If the timer is currently running, this method will return false.
     *
     *
     */
    bool is_stopped() const { return (m_stop.seconds() != -1) || (m_stop.seconds() == 0); }


    /*! \details Returns true if the timer is in a reset state.
     *
     */
    bool is_reset() const { return m_stop.seconds() == 0; }


    /*! \details Resets the value of the timer.
     *
     *  After calling this method, is_running(), and is_started() will both
     *  all return false;
     *
     */
    void reset();

    /*! \details Calculates the timer value in milliseconds.
     *
     * @return The number of milliseconds that have elapsed since start.  This value
     * can be read when the timer is running to get a live value or after it has
     * been stopped to get the time elapsed between start() and stop()
     */
    u32 milliseconds() const { return calc_value().milliseconds(); }
    //deprecated
    u32 msec() const { return calc_msec(); }
    u32 calc_msec() const { return calc_value().msec(); }

    /*! \details Calculates the timer value in microseconds.
     *
     * This is similar to calc_msec() but returns
     * the value in microseconds rather than milliseconds.
     *
     * @return The number of microseconds that have elapsed since start.
     */
    u32 microseconds() const { return calc_value(); }

    //deprecated
    u32 usec() const { return calc_value(); }
    u32 calc_usec() const { return calc_value(); }


    /*! \details Calculates the timer value in seconds.
     *
     * This is similar to calc_msec() but returns
     * the value in seconds rather than milliseconds.
     *
     * @return The number of seconds that have elapsed since start.
     */
    u32 seconds() const { return calc_value().seconds(); }

    /*! \details Returns the value of the timer as a ClockTime object. */
    ClockTime clock_time() const;

    //deprecated
    u32 calc_sec() const { return seconds(); }
    u32 sec() const { return seconds(); }

    /*! \details Stops the timer from counting.
     *
     * Subsequent calls to value() will return the same number.  Once the timer has been stopped(),
     * a call to resume() will resume counting and a call to start() or restart() will restart
     * counting from zero.
     */
    void stop();

#endif

private:
    MicroTime calc_value() const;


#if !defined __link
    ClockTime m_start;
    ClockTime m_stop;
#endif
};

}

#endif // CHRONO_MICRO_TIMER_HPP
