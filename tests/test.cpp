#include <string>

#include "gtest/gtest.h"
#include "../src/core/ipc/unnamedPipe.hpp"

TEST(UnnamedPipeTest, HandlesReadAndWrite)
{
    UnnamedPipe *pipe = new UnnamedPipe();

    pipe->write("Hello World!\nfuck this shit");

    std::string read = pipe->read();
    EXPECT_EQ(read, "Hello World!") << "Output from pipe is different from input: " << read;
}