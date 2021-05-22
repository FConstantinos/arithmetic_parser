#pragma once

#include <optional>
#include "AMScanner.h"

using namespace std;

namespace amparser {

class AMParser {
public:
    explicit AMParser();
    int parse(AMScanner& scanner) const;
};

}
