#pragma once

#include <string>
#include <sstream>
#include "ast_node.hpp"

using namespace std;

class IdentifierNode : public ASTNode {
    public:
        string nodeType;
        string identifier;
        shared_ptr<ASTNode> type;

        IdentifierNode(string ident) : identifier(ident), nodeType("Identifier") {};

        string getNodeType() const override { return nodeType; }

        string toJSON() const override {
            ostringstream oss;
            
            oss << "{";
            oss << "\"type\": \"" << nodeType << "\"";
            oss << ", \"value\": \"" << identifier << "\"";
            oss << ", \"variableType\": " << (value ? value->toJSON() : "null");
            oss << "}";

            return oss.str();
        }
};