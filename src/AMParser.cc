#include <deque>
#include <cmath>
#include "AMParser.h"

namespace amparser {

AMParser::AMParser()= default;

int
AMParser::parse(AMScanner& scanner) const{

    deque<Token> tokens;
    size_t parenCount = 0;
    optional<Token> tok;
    while((tok = scanner.getNextToken()).has_value()) {
        Token curr = tok.value();
        switch(curr.type) {
            case TokenType::kPlus: {
                if (!tokens.empty()) {
                    Token prev = *tokens.rbegin();
                    if (prev.type != TokenType::kNum && prev.type != TokenType::kLParen && !tokens.empty()) {
                        throw invalid_argument("operator following invalid token: position " + to_string(curr.pos));
                    }
                }
                tokens.push_back(curr);
                break;
            }
            case TokenType::kMinus: {
                if (!tokens.empty()) {
                    Token prev = *tokens.rbegin();
                    if (prev.type != TokenType::kNum && prev.type != TokenType::kLParen && !tokens.empty()) {
                        throw invalid_argument("operator following invalid token: position " + to_string(curr.pos));
                    }
                }
                tokens.push_back(curr);
                break;
            }
            case TokenType::kMul: {
                if (tokens.empty()) {
                    throw invalid_argument("expected token before '*', position: " + to_string(curr.pos));
                }
                Token prev = *tokens.rbegin();
                if (prev.type != TokenType::kNum) {
                    throw invalid_argument("operator following invalid token: position " + to_string(curr.pos));
                }
                tokens.push_back(curr);
                break;
            }
            case TokenType::kDiv: {
                if (tokens.empty()) {
                    throw invalid_argument("expected token before '/', position: " + to_string(curr.pos));
                }
                Token prev = *tokens.rbegin();
                if (prev.type != TokenType::kNum) {
                    throw invalid_argument("operator following invalid token: position " + to_string(curr.pos));
                }
                tokens.push_back(curr);
                break;
            }
            case TokenType::kLParen: {
                parenCount++;
                tokens.push_back(curr);
                break;
            }
            case TokenType::kRParen: {

                // Check for unbalanced parenthesis:
                if (!parenCount) {
                    throw invalid_argument("unbalanced parenthesis, position: " + to_string(curr.pos));
                }
                parenCount--;

                if (tokens.empty()) {
                    throw invalid_argument("expected token before ')', position: " + to_string(curr.pos));
                }
                Token prev = *tokens.rbegin();
                // Check for empty parenthesis:
                if (prev.type == TokenType::kLParen) {
                    throw invalid_argument("empty parenthesis, position: " + to_string(prev.pos));
                }

                // remove the parenthesis
                tokens.pop_back();
                tokens.pop_back();

                // continue handling as if it was a kNum token case:
                curr = prev;
            }
            case TokenType::kNum: {
                if (!tokens.empty()) {
                    Token prev = *tokens.rbegin();

                    if (prev.type != TokenType::kPlus && prev.type != TokenType::kMinus
                        && prev.type != TokenType::kMul && prev.type != TokenType::kDiv
                        && prev.type != TokenType::kLParen && !tokens.empty()) {
                        throw invalid_argument("number following invalid token: position " + to_string(curr.pos));
                    }

                    if (prev.type == TokenType::kPlus || prev.type == TokenType::kMinus) {
                        const bool neg = (prev.type == TokenType::kMinus);
                        const long val = neg ? -curr.val.value() : curr.val.value();
                        tokens.pop_back();
                        if (!tokens.empty() && (tokens.back().type == TokenType::kNum)) {
                            const long left = tokens.back().val.value();
                            const long right = val;
                            tokens.pop_back();

                            // check for overflow:
                            const long result = left + right;
                            if (result > numeric_limits<int>::max() || result < numeric_limits<int>::min()) {
                                throw overflow_error("overflow detected, position: " + to_string(curr.pos));
                            }
                            tokens.push_back({TokenType::kNum, left + right, curr.pos});
                        } else {
                            tokens.push_back({TokenType::kNum, val, curr.pos});
                        }
                    } else if (prev.type == TokenType::kMul) {
                        tokens.pop_back();
                        const long left = tokens.back().val.value();
                        const long right = curr.val.value();
                        tokens.pop_back();
                        long sign = copysign(1, left);

                        // Check for overflow:
                        if (((sign > 0) && right && (left > numeric_limits<int>::max() / right)) ||
                            ((sign < 0) && right && (left < numeric_limits<int>::min() / right))) {
                            throw overflow_error("overflow detected, position: " + to_string(curr.pos));
                        } else {
                            tokens.push_back({TokenType::kNum, left * right, curr.pos});
                        }
                    } else if (prev.type == TokenType::kDiv) {
                        tokens.pop_back();
                        const long left = tokens.back().val.value();
                        const long right = curr.val.value();
                        tokens.pop_back();

                        // Check for division by zero:
                        if (!right) {
                            throw overflow_error("division by zero detected, position: " + to_string(curr.pos));
                        } else {
                            tokens.push_back({TokenType::kNum, left / right, curr.pos});
                        }
                    } else {
                        tokens.push_back(curr);                    }
                } else {
                    tokens.push_back(curr);
                }
                break;
            }
        }
    }
    if(tokens.empty()) {
        throw invalid_argument("Empty expression");
    }

    const Token resTok = tokens.back();
    tokens.pop_back();
    if(!tokens.empty() || !resTok.val.has_value()) {
        if (parenCount) {
            throw invalid_argument("unbalanced parenthesis");
        } else {
            throw invalid_argument("failed to parse due to unidentified error");
        }
    }

    return resTok.val.value();
}

} // amparser