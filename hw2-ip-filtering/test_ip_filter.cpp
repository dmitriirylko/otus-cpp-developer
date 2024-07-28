#include <iostream>

#include <gtest/gtest.h>

#include "main.h"
#include "reader.h"
#include "ipaddrpool.h"

struct ip_filter_fixture : public testing::Test
{
    ip_filter_fixture() : reader(filePathDefault)
    {
        ipPoolSorted = reader.getPool();
        ipPoolSorted.sortDescending();
    }
    Reader reader;
    IpAddrPool ipPoolSorted;
};

TEST_F(ip_filter_fixture, FirstByte)
{
    /* Arranged in ip_filter_fixture. */
    /* Act. */
    IpAddrPool ipPoolFiltered = ipPoolSorted.filter({1});
    testing::internal::CaptureStdout();
    ipPoolFiltered.print();
    std::string output = testing::internal::GetCapturedStdout();
    /* Assert. */
    EXPECT_STREQ(output.c_str(), "1.231.69.33\n1.87.203.225\n1.70.44.170\n1.29.168.152\n1.1.234.8\n");
}

TEST_F(ip_filter_fixture, FirstBytes)
{
    /* Arranged in ip_filter_fixture. */
    /* Act. */
    IpAddrPool ipPoolFiltered = ipPoolSorted.filter({46, 70});
    testing::internal::CaptureStdout();
    ipPoolFiltered.print();
    std::string output = testing::internal::GetCapturedStdout();
    /* Assert. */
    EXPECT_STREQ(output.c_str(), "46.70.225.39\n46.70.147.26\n46.70.113.73\n46.70.29.76\n");
}

TEST_F(ip_filter_fixture, AnyByte)
{
    /* Arranged in ip_filter_fixture. */
    /* Act. */
    IpAddrPool ipPoolFiltered = ipPoolSorted.filterAny(46);
    testing::internal::CaptureStdout();
    ipPoolFiltered.print();
    std::string output = testing::internal::GetCapturedStdout();
    /* Assert. */
    EXPECT_STREQ(output.c_str(), "186.204.34.46\n"
                                 "186.46.222.194\n"
                                 "185.46.87.231\n"
                                 "185.46.86.132\n"
                                 "185.46.86.131\n"
                                 "185.46.86.131\n"
                                 "185.46.86.22\n"
                                 "185.46.85.204\n"
                                 "185.46.85.78\n"
                                 "68.46.218.208\n"
                                 "46.251.197.23\n"
                                 "46.223.254.56\n"
                                 "46.223.254.56\n"
                                 "46.182.19.219\n"
                                 "46.161.63.66\n"
                                 "46.161.61.51\n"
                                 "46.161.60.92\n"
                                 "46.161.60.35\n"
                                 "46.161.58.202\n"
                                 "46.161.56.241\n"
                                 "46.161.56.203\n"
                                 "46.161.56.174\n"
                                 "46.161.56.106\n"
                                 "46.161.56.106\n"
                                 "46.101.163.119\n"
                                 "46.101.127.145\n"
                                 "46.70.225.39\n"
                                 "46.70.147.26\n"
                                 "46.70.113.73\n"
                                 "46.70.29.76\n"
                                 "46.55.46.98\n"
                                 "46.49.43.85\n"
                                 "39.46.86.85\n"
                                 "5.189.203.46\n");
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}