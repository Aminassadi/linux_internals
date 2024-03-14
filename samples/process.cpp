#include "../pch.h"
#include <sched.h>
#include <unistd.h>
#include <string.h>

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
    EXPECT_FALSE(nice(kDefaultNiceValue) == -1) << "setting default nice value failed with error: "<<strerror(errno);   
 }
};


TEST_F(process, set_priority)
{
    EXPECT_EQ(true,true);
}