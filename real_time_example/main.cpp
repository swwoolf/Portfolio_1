#include <cstring>
#include <stdexcept>
#include <iostream>
#include <sys/mman.h>
#include <chrono>
#include <pthread.h>

using namespace std;

class Thread {

    int thread_priority;
    int thread_policy;
    time_t thread_start_time;
    int32_t time_ns = 10000000;
    int loop_iterations = 0;
    struct timespec wakeup_time;
    pthread_t _thread;



    static void* RunThread(void* data) {
        Thread* thread = static_cast<Thread*>(data);
        thread->Run();

        return NULL;
    }

public:
    Thread(int priority_lvl, int policy_num){
        thread_priority = priority_lvl;
        thread_policy = policy_num;
    }

    void Start() {

        pthread_attr_t attr;

        int thread_init_return = pthread_attr_init(&attr);

        thread_init_return = pthread_attr_setschedpolicy(&attr, thread_policy);

        struct sched_param sched_parameters_struct;
        sched_parameters_struct.sched_priority = thread_priority;
        thread_init_return = pthread_attr_setschedparam(&attr, &sched_parameters_struct);

        thread_init_return = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

        thread_init_return = pthread_create(&_thread, &attr, &Thread::RunThread, this);

    }

    int Join() {
        return pthread_join(_thread, NULL);
    }

    struct timespec AddTimespec(struct timespec wakeup_time_struct, int32_t num_ns) {
        wakeup_time_struct.tv_nsec += num_ns;

        while (wakeup_time_struct.tv_nsec >= 1000000000) {
            ++wakeup_time_struct.tv_sec;
            wakeup_time_struct.tv_nsec -= 1000000000;
        }
        while (wakeup_time_struct.tv_nsec < 0) {
            --wakeup_time_struct.tv_sec;
            wakeup_time_struct.tv_nsec += 1000000000;
        }
        return wakeup_time_struct;
    }

    void Run() {

        clock_gettime(CLOCK_MONOTONIC, &wakeup_time);

        time_t loop_processing_start_time = time(nullptr);
        cout << "Loop Processing start time: " << loop_processing_start_time << endl;
        while (loop_iterations < 100) {
            loop_iterations++;
            loop_code();
            wakeup_time = AddTimespec(wakeup_time, time_ns);
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeup_time, NULL);
        }
        time_t loop_processing_end_time = time(nullptr);
        cout << "Loop Processing end time: " << loop_processing_end_time << endl;
        cout << "Total Thread duration: " << loop_processing_end_time - loop_processing_start_time << " second" << endl;

    }

    void loop_code() {
        cout << "Loop Processing. Running Iteration "<< loop_iterations << "\n" << endl;

    }


};

void LockMemory() {
    int ret = mlockall(MCL_CURRENT | MCL_FUTURE);
    if (ret) {
        throw std::runtime_error{std::strerror(errno)};
    }
}

int main() {
    LockMemory();
    Thread real_time_trial_thread(80, SCHED_FIFO);
    real_time_trial_thread.Start();
    real_time_trial_thread.Join();

    cout << "The loop above was run for a total of 100 times over a period of 1 second." << endl;

    int32_t starting_int = 57;
    char32_t* some_char_ptr = (char32_t*) &starting_int;
    cout << "\nComparing character value \'9\' with dereference of character pointer to int value 57: " << (*some_char_ptr == '9') << endl;
    cout << "If the above outputs 1, then 57 was evaluated as equal to the character \n\'9\' because 57 is the decimal for the ASCII code for \'9\'" << endl;
    return 0;
}
