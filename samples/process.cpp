#include "../pch.h"
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

#define PRINT_MACRO_NAME(X) printf("%s", #X)
/*
    nice() adds inc to the nice value for the calling thread.  (A
    higher nice value means a lower priority.)

    The range of the nice value is +19 (low priority) to -20 (high
    priority).  Attempts to set a nice value outside the range are
    clamped to the range.

    Traditionally, only a privileged Process could lower the nice
    value (i.e., set a higher priority).

    A successful call can legitimately return -1 :))).  To detect an
    error, set errno to 0 before the call, and check whether it is
    nonzero after nice() returns -1.
*/

/*
    from the kernel view there is no difference between Process and thread,
    both name as task, threads are tasks with the same address space.
*/



class Process : public ::testing::Test 
{ 
private:
    void ResetNiceValue()
    {
        errno = 0;
        auto current_nice_value{getpriority(PRIO_PROCESS, 0)};        
        ASSERT_FALSE(current_nice_value == -1);
        std::cout<<"current nice value: " << current_nice_value << '\n';
        if(nice(kDefaultNiceValue - current_nice_value) == -1)
        {
            EXPECT_FALSE( errno == -1) << "setting default nice value failed with error: "<<strerror(errno);
        }
        std::cout<<"nice value reset to it's default: " << getpriority(PRIO_PROCESS, 0) << '\n';
    }

    void ResetSchedPolicy()
    {
        auto min_sched_priority{sched_get_priority_min(SCHED_OTHER)};
        const sched_param param{min_sched_priority};
        //0 can be used instead of getpid(), measns calling process
        ASSERT_EQ(sched_setscheduler(0, SCHED_OTHER, &param), 0) << "cannot set the schedular policy with error: " << strerror(errno) << '\n';
    }
protected:
    static constexpr auto kDefaultNiceValue{0}; 
    static constexpr auto kMaxNiceValue{-20};
    static constexpr auto kMinNiceValue{19};
    Process() 
    {
        // This method will be called before each test.
        Reset();
    }

    void SetUp() override 
    {
    }

    void TearDown() override 
    {
    }
   

    void Reset()
    {
        ResetNiceValue();
        ResetSchedPolicy();
    } 


    void PrintShcedularPolicyForCurrentProcess()
    {
        switch (sched_getscheduler(getpid()))
        {
        case SCHED_OTHER:
            std::cout<<"normal process, round-robin schedular\n";
            break;
        case SCHED_FIFO:
            std::cout<<"real-time process, FIFO schedular\n";
            break;
        case SCHED_RR:
            std::cout<<"real-time process, round-robin schedular\n";
            break;
        default:
            break;
        }  
    }

};

//setting the high priority nice value needs root privilege
TEST_F(Process, set_priority_high)
{   
    errno = 0;
    //increase the priority of the curent task  
    if(nice(kMaxNiceValue) == -1)
    {
        EXPECT_TRUE( errno == 0) << "setting nice value failed with error: "<< strerror(errno) << '\n';
    } 
    EXPECT_EQ(getpriority(PRIO_PROCESS, 0), kMaxNiceValue);    
}

TEST_F(Process, set_priority_low)
{
    errno = 0;
    //decrease the priority of the curent task   
    if(nice(kMinNiceValue) == -1)
    {
        EXPECT_TRUE( errno == 0) << "setting nice value failed with error: "<< strerror(errno) << '\n';
    } 
    EXPECT_EQ(getpriority(PRIO_PROCESS, 0), kMinNiceValue);    
}


/*
    For processes scheduled under one of the normal scheduling policies (SCHED_OTHER, SCHED_IDLE, SCHED_BATCH), 
    sched_priority is not used in scheduling decisions (it must be specified as 0). 
    setting real-time scheduling policy needs root privileg.

    When you change the scheduler policy to FIFO or RR in Linux, which are real-time scheduling policies, and set a priority for it, 
    the nice value does not directly affect the scheduling of the process. The nice value is primarily associated with the SCHED_OTHER policy, 
    which is the default scheduling policy for processes in Linux. 
*/

TEST_F(Process, sched_setscheduler_fifo_min)
{
    auto min_sched_priority{sched_get_priority_min(SCHED_FIFO)};
    std::cout<<"mimimum priority for fifo schedular policy is " << min_sched_priority << '\n';
    const sched_param param{min_sched_priority};
    EXPECT_EQ(sched_setscheduler(0, SCHED_FIFO, &param), 0) << "cannot set the schedular policy with error: " << strerror(errno) << '\n';
    PrintShcedularPolicyForCurrentProcess();
}

TEST_F(Process, sched_setscheduler_fifo_max)
{
    auto max_sched_priority{sched_get_priority_max(SCHED_FIFO)};
    std::cout<<"maximum priority for fifo schedular policy is " << max_sched_priority << '\n';
    const sched_param param{max_sched_priority};
    EXPECT_EQ(sched_setscheduler(0, SCHED_FIFO, &param), 0) << "cannot set the schedular policy with error: " << strerror(errno) << '\n';
    PrintShcedularPolicyForCurrentProcess();
}


TEST_F(Process, sched_setscheduler_rr_min)
{
    auto min_sched_priority{sched_get_priority_min(SCHED_RR)};
    std::cout<<"mimimum priority for round-robin schedular policy is " << min_sched_priority << '\n';
    const sched_param param{min_sched_priority};
    EXPECT_EQ(sched_setscheduler(0, SCHED_FIFO, &param), 0) << "cannot set the schedular policy with error: " << strerror(errno) << '\n';
    PrintShcedularPolicyForCurrentProcess();
}

TEST_F(Process, sched_setscheduler_rr_max)
{
    auto max_sched_priority{sched_get_priority_max(SCHED_RR)};
    std::cout<<"maximum priority for round-robin schedular policy is " << max_sched_priority << '\n';
    const sched_param param{max_sched_priority};
    EXPECT_EQ(sched_setscheduler(0, SCHED_RR, &param), 0) << "cannot set the schedular policy with error: " << strerror(errno) << '\n';
    PrintShcedularPolicyForCurrentProcess();
}




