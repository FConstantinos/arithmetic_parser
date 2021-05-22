#include <iostream>
#include "AMScanner.h"
#include "AMParser.h"

using namespace std;
using namespace amparser;

bool evaluate(const char *expression, int &result)
{
    try {
        AMScanner amscanner(expression);
        AMParser amparser;
        result = amparser.parse(amscanner);
    } catch (const exception& ex) {
        cout << "failed to parse: " + string(ex.what()) << endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    const char *input = argv[1];
    int result;
    if (evaluate(input, result)) {
        cout << result << endl;
    }
    return 0;
}
