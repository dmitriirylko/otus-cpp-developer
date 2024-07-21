#define BOOST_TEST_MODULE test_version

#include <string>

#include <boost/test/unit_test.hpp>

#include "lib.h"

/**
 * @brief Compares two versions represented as strings in format
 * 			"n1.n2.n3" like (1.12.52).
 *  @param v1 Version (string) to check.
 *  @param v2 Version (string) to check with.
 *  @return true if v1 > v2, false otherwise.
 */
bool isVersionGreater(std::string v1, std::string v2)
{
	/* Stores integer value of each part of version. */
    int vnum1 = 0, vnum2 = 0;

    for(int i = 0, j = 0; (i < v1.length() || j < v2.length());)
	{
		/* Convert one part of version to number. */
        while(i < v1.length() && v1[i] != '.')
		{
            vnum1 = vnum1 * 10 + (v1[i] - '0');
            i++;
        }
		/* Convert other part of version to number. */
        while(j < v2.length() && v2[j] != '.')
		{
            vnum2 = vnum2 * 10 + (v2[j] - '0');
            j++;
        }
        /* Compare part of versions. */
        if(vnum1 > vnum2)
		{
			return true;
		}
        else if(vnum1 < vnum2)
		{
			return false;
		}
		else
		{
            /* Reset number variables and go for next numeric part. */ 
            vnum1 = vnum2 = 0;
            i++;
            j++;
            continue;
        }
    }
    return false;
}

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version)
{
	BOOST_CHECK(isVersionGreater("13.2.4.1", "0.0.0"));
}

BOOST_AUTO_TEST_SUITE_END()