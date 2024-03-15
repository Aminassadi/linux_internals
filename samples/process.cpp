#include "../pch.h"
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

class ProcessPri : public ::testing::Test 
{   
protected:
    static constexpr auto kDefaultNiceValue{0}; 
    static constexpr auto kMaxNiceValue{-20};
    static constexpr auto kMinNiceValue{19};
    ProcessPri() 
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
        errno = 0;
        auto current_nice_value{getpriority(PRIO_PROCESS, 0)};
        ASSERT_FALSE(current_nice_value == -1);
        if(nice(kDefaultNiceValue - current_nice_value) == -1)
        {
            EXPECT_FALSE( errno == -1) << "setting default nice value failed with error: "<<strerror(errno);
        }
        std::cout<<"nice value reset to its default: " << getpriority(PRIO_PROCESS, 0) << '\n';
    } 
};

 /*
    nice() adds inc to the nice value for the calling thread.  (A
    higher nice value means a lower priority.)

    The range of the nice value is +19 (low priority) to -20 (high
    priority).  Attempts to set a nice value outside the range are
    clamped to the range.

    Traditionally, only a privileged ProcessPri could lower the nice
    value (i.e., set a higher priority).

    A successful call can legitimately return -1 :))).  To detect an
    error, set errno to 0 before the call, and check whether it is
    nonzero after nice() returns -1.
*/

/*
    from the kernel view there is no difference between ProcessPri and thread,
    both name as task, threads are tasks with the same address space.
*/


//setting the high priority nice value needs root privilege
TEST_F(ProcessPri, set_priority_high)
{   
    errno = 0;
    //increase the priority of the curent task  
    if(nice(kMaxNiceValue) == -1)
    {
        EXPECT_TRUE( errno == 0) << "setting nice value failed with error: "<< strerror(errno) << '\n';
    } 
    EXPECT_EQ(getpriority(PRIO_PROCESS, 0), kMaxNiceValue);    
}

TEST_F(ProcessPri, set_priority_low)
{
    errno = 0;
    //decrease the priority of the curent task   
    if(nice(kMinNiceValue) == -1)
    {
        EXPECT_TRUE( errno == 0) << "setting nice value failed with error: "<< strerror(errno) << '\n';
    } 
    EXPECT_EQ(getpriority(PRIO_PROCESS, 0), kMinNiceValue);    
}



