//
// Created by 505 on 2022/7/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect == actual),expect,actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")

#define TEST_VALUE(state, test_type, json)\
    do {\
        lept_value v;\
        v.type = LEPT_FALSE;\
        EXPECT_EQ_INT(state, lept_parse(&v, json));\
        EXPECT_EQ_INT(test_type, lept_get_type(&v));\
    } while(0)

#define TEST_NUMBER(expect, json)\
    do {\
        lept_value v;\
        EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, json));\
        EXPECT_EQ_INT(LEPT_NUMBER, lept_get_type(&v));\
        EXPECT_EQ_DOUBLE(expect, lept_get_number(&v));\
    } while(0)

static void test_parse_null()
{
    TEST_VALUE(LEPT_PARSE_OK,LEPT_NULL,"null");
}

static void test_parse_true()
{
    TEST_VALUE(LEPT_PARSE_OK,LEPT_TRUE,"true");
}

static void test_parse_false()
{
    lept_value v;
    v.type = LEPT_TRUE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false"));
    EXPECT_EQ_INT(LEPT_FALSE, lept_get_type(&v));
}

static void test_parse_expect_value()
{
//    lept_value v;
//
//    v.type = LEPT_FALSE;
//    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, ""));
//    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
//
//    v.type = LEPT_FALSE;
//    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, " "));
//    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
    TEST_VALUE(LEPT_PARSE_EXPECT_VALUE,LEPT_NULL,"");
    TEST_VALUE(LEPT_PARSE_EXPECT_VALUE,LEPT_NULL," ");
}

static void test_parse_invalid_value()
{
//    lept_value v;
//    v.type = LEPT_FALSE;
//    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "nul"));
//    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
//
//    v.type = LEPT_FALSE;
//    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "?"));
//    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "nul");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "?");

    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "+0");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "+1");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "。123");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "1.");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "INF");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "inf");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "NAN");
    TEST_VALUE(LEPT_PARSE_INVALID_VALUE, LEPT_NULL, "nan");
}

static void test_parse_root_not_singular()
{
//    lept_value v;
//    v.type = LEPT_FALSE;
//    EXPECT_EQ_INT(LEPT_PARSE_ROOT_NOT_SINGULAR, lept_parse(&v, "null x"));
//    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
    TEST_VALUE(LEPT_PARSE_ROOT_NOT_SINGULAR, LEPT_NULL, "null x");
}

static void test_parse_number()
{
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}
static void test_parse_number_too_big()
{
    TEST_VALUE(LEPT_PARSE_NUMBER_TOO_BIG,LEPT_NULL,"1e309");
    TEST_VALUE(LEPT_PARSE_NUMBER_TOO_BIG,LEPT_NULL,"-1e309");
}
static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}