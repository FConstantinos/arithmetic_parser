# Introduction 
This is a simple application that parses a string that represents an arithmetic
expression and prints either the result of the operation, or a parsing error if
it fails to parse it. It is intended as an exercise in writting a parser from
scratch for a very simple expression and have a feel about how that compares
with using tools such as flex and bison with which I have experience. It is
unusual in that the precedence of the * (multiply) and / (divide) operators is
the same as the +/- (addition/subtraction) operators. This seemingly arbitrary
requirement is for two purposes. First, it makes it easier to parse the
expression; flex/bison would serve better for complex rules, but the question is
what would be better for very simple rules. Second, it makes the exercise more
difficult to complete; one could easily find a string expression evaluator (e.g
python's eval) to easily solve this, but this rule necessitates that the
exrpession has to be parsed. My takeaway from this exercise is that, even for
parsing expressions with very simple rules, I would rather use an already
existing lexer/parser tool than write my own, due to the many opportunities for
bugs and errors and the amount of code that needs to be written.

# Build and Installation

## Requirements
 * A linux environment
 * cmake >= 3.16
 * googletest >= 1.10
   * for building unit tests
   
## Instructions
 * Clone this repository to your local machine
 * run the following commands:
    * cd /your/clone/directory
    * mkdir build && cd build
    * cmake -DCMAKE_INSTALL_PREFIX=/your/install/directory ..
	* to be able to build documentation, add "-DBUILD_DOCUMENTATION=ON"
	  (without quotes)
        * to be able to build the tests, add "-DENABLE_TEST=ON" (without quotes)
    * make all 
        * to build everything
    * make am_parser
        * to build the **am_parser** application
    * make am_parser_test
        * to build the unit tests
        * to run the unit tests, simply run the am_parser_test executable
    * make install
        * to install the **am_parser** application
        * will be under /your/install/directory/bin

# Usage
````
$ am_parser "1+1"
2
````

````
$ am_parser "1/0"
failed to parse: division by zero detected, position: 2
````

 - valid symbols:
    - operators: '+', '-', '*', '/'
    - parenthesis: '(', ')'
    - numbers: [0-9]+
    - signs: '+', '-'
 - Supported arithmetic operators:
    - addition(+)
    - subtraction(-)
    - multiplication(*)
    - division(/)
 - Supported operands:
    - 32 bit integers
        - all numbers in the range [-2147483648, 2147483647]
 - Operator precedence:
    - equal between all operators
    - left to right order of evaluation
    - expressions in parenthesis 
 - Error conditions:
    - unrecognized tokens
    - arithmetic overflow
    - division by 0
    - invalid syntax (e.g 10++)
    - unmatched parenthesis
    
# Examples
````
$ ./am_parser "0/0"
failed to parse: division by zero detected, position: 2
$ ./am_parser "1+3"
4
$ ./am_parser "(1+3)*2"
8
$ ./am_parser "(4/2)+6"
8
$ ./am_parser "4+(12/(1*2))"
10
$ ./am_parser "(1+(12*2)"
failed to parse: unbalanced parenthesis
````
