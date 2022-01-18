#include <iostream>

#include "ConfigReader.hpp"

int main() {
    ConfigReader cr("c.conf");
    const auto tokenlist = cr.read();
    for (const auto token : tokenlist) {
        std::cout << token.text << ' ';
        switch(token.type) {
            case TokenType::ParameterName:
                std::cout << "parameter name\n";
                break;
            case TokenType::ParameterValue:
                std::cout << "parameter value\n";
                break;
            case TokenType::Separator:
                std::cout << "separator\n";
        }
    }
    
    return 0;
}