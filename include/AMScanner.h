#pragma once

#include <optional>

namespace amparser {

enum class TokenType {
    kNum, kPlus, kMinus, kMul, kDiv, kLParen, kRParen
};

std::optional<TokenType> char2Token(char c);

struct Token {
    TokenType type = TokenType::kNum;

    // TODO: This is long to make sure that something like "-(MAX_INT+1)" (which is actually MIN_INT)
    // parses, since it will be scanned as two separate tokens "-" and "MAX_INT+1", where the second one
    // will overflow. We should find a better way to do that, but is requires a lot of work with a
    // hand-made parser.
    std::optional<long> val = 0;
    size_t pos = 0;
};

class AMScanner {
public:
    explicit AMScanner(const char* expr);
    std::optional<Token> getNextToken();

private:
    const char* expr;
    size_t idx;
};

} // amparser
