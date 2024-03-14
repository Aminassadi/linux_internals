#include "../pch.h"
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

static constexpr auto kDefaultNiceValue{0};

class process : public ::testing::Test {
protected:
 process() 
 {
    // This method will be called before each test.
    Reset();
 }


 void SetUp() override {
 }

 void TearDown() override {
     Reset();
 }

 void Reset()
 {
    errno = 0;
    if(nice(kDefaultNiceValue) == -1)
    {
        EXPECT_FALSE( errno == -1) << "setting default nice value failed with error: "<<strerror(errno);
    }
 }
};

 /*
       nice() adds inc to the nice value for the calling thread.  (A
       higher nice value means a lower priority.)

       The range of the nice value is +19 (low priority) to -20 (high
       priority).  Attempts to set a nice value outside the range are
       clamped to the range.

       Traditionally, only a privileged process could lower the nice
       value (i.e., set a higher priority).

       A successful call can legitimately return -1 :))).  To detect an
       error, set errno to 0 before the call, and check whether it is
       nonzero after nice() returns -1.
*/

/*
    from the kernel view there is no difference between process and thread,
    both name as task, threads are tasks with the same address space.
*/

TEST_F(process, set_priority_high)
{   
    errno = 0;
    const int highest_value{-20};
    std::cout << "nice value: " << getpriority(PRIO_PROCESS, 0) <<std::endl;
    //increase the priority of the curent task  
    if(nice(highest_value) == -1)
    {
        EXPECT_TRUE( errno == 0) << "setting nice value failed with error: "<< strerror(errno);
    } 
    EXPECT_EQ(getpriority(PRIO_PROCESS, 0), highest_value);    
}

TEST_F(process, set_priority_low)
{
     errno = 0;
    const int lowest_value{19};
    //decrease the priority of the curent task   
    if(nice(lowest_value) == -1)
    {
        EXPECT_TRUE( errno == 0) << "setting nice value failed with error: "<< strerror(errno);
    } 
    auto current_priority{getpriority(PRIO_PROCESS, 0)};
    if(current_priority == -1)
    {
        perror("getting current nice value failed.");
        FAIL();
    }
    else
    {
        EXPECT_EQ(current_priority, lowest_value); 
    }
}