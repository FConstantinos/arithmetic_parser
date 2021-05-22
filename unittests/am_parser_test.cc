#include <gtest/gtest.h>
#include "AMScanner.h"
#include "AMParser.h"

using namespace std;
using namespace amparser;

// TODO: Refine tests so that they capture the exact throw expected.
struct AMParserTestCase {
    const char *expr;
    optional<int> resexp; // if nullopt, a throw is expected
};

class AMParserTestFixture : public ::testing::TestWithParam<AMParserTestCase> {
protected:
    const AMParser parser;
};

TEST_P(AMParserTestFixture, AMParserTest) {
    const auto testCase = GetParam();
    const auto& expr = testCase.expr;
    const auto& exp = testCase.resexp;
    AMScanner scanner(expr);

    if(exp.has_value()) {
        ASSERT_EQ(parser.parse(scanner), exp.value());
    } else {
        ASSERT_THROW(parser.parse(scanner), exception);
    }

}

vector<AMParserTestCase> amptc = {
        {"1+1/(1+1/(1+1)) + (((+0))) + ((((+1))) + (((-1))) )", 2},
        {""            , nullopt },
        {"()"          , nullopt },
        {"++1"            , nullopt },
        {"(())"          , nullopt },
        {"("           , nullopt },
        {")"           , nullopt },
        {"+"           , nullopt },
        {"-"           , nullopt },
        {"*"           , nullopt },
        {"/"           , nullopt },
        {"2147483647", 2147483647 },
        {"2147483647 + 1", nullopt },
        {"2147483647*2", nullopt },
        {"-2147483648", -2147483648 },
        {"-2147483648 - 1", nullopt },
        {"-2147483648*2"  , nullopt },
        {"1+3"         , 4 },
        {"(1+3)*2"     , 8 },
        {"(4/2)+6"     , 8 },
        {"4+(12/(1*2))", 10 },
        {"(1+(12*2)"   , nullopt },
        {"1+3*4"       , 16 },
        {"1/0"         , nullopt },
        {"0/0"         , nullopt },
        {"-0+0-0"      , 0},
        {"-1+(-1)+(+1)", -1 }
};

INSTANTIATE_TEST_CASE_P(
        AMParserTest,
        AMParserTestFixture,
        ::testing::ValuesIn(amptc)
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}