#include "AMScanner.h"

using namespace std;

namespace amparser {

optional<TokenType> char2Token(const char c)  {
    switch(c) {
        case '(':
            return TokenType::kLParen;
        case ')':
            return TokenType::kRParen;
        case '+':
            return TokenType::kPlus;
        case '-':
            return TokenType::kMinus;
        case '*':
            return TokenType::kMul;
        case '/':
            return TokenType::kDiv;
        default:
            return nullopt;
    }
}

AMScanner::AMScanner(const char* expr) :
        expr(expr),
        idx(0) {}

optional <Token>
AMScanner::getNextToken() {
    char c;
    while ((c = expr[idx]) != '\0') {
        const auto tok = char2Token(c);
        if (tok.has_value()) {
            idx++;
            return Token{tok.value(), nullopt, idx - 1};
        } else if (isdigit(c)) {
            string num;
            const size_t pos = idx;
            do {
                num.append(1, c);
                idx++;
            } while (isdigit(c = expr[idx]));
            long val;
            try {
                val = stol(num);
            } catch (const out_of_range& ex) {
                throw out_of_range("number: " + num + " at position: " + to_string(pos) + " is out of range");
            } catch (const exception& ex) {
                throw invalid_argument("unable to parse number: " + num + " at position: " + to_string(pos));
            }
            return Token{TokenType::kNum, val, pos };
        } else if (isspace(c)) {
            idx++;
            continue;
        } else {
            throw invalid_argument("symbol: " + string(1, c) + "not recognized at position: " + to_string(idx));
        }
    }

    return nullopt;
}

} // amparser
