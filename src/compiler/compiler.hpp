#pragma once

#include <vector>
#include <deque>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>
#include "../parser/ast/ast_node.hpp"
#include "../parser/ast/link_node.hpp"
#include "../util/exceptions.hpp"

using namespace std;

/**
 * @brief Singleton class responsible for compiling Theta source code into an Abstract Syntax Tree (AST).
 */
class ThetaCompiler {
    public:
        /**
         * @brief Compiles the Theta source code starting from the specified entry point.
         * @param entrypoint The entry point file name or identifier.
         * @param outputFile The output file which will be the result of the compilation
         * @param isEmitTokens Toggles whether or not the lexer tokens should be output to the console
         * @param isEmitAST Toggles whether or not the AST should be output to the console
         */
        void compile(string entrypoint, string outputFile, bool isEmitTokens = false, bool isEmitAST = false);

        /**
         * @brief Compiles the Theta source code starting from the specified entry point.
         * @param source The source code to compile.
         * @return A shared pointer to the root node of the constructed AST
         */
        shared_ptr<ASTNode> compileDirect(string source);

        /**
         * @brief Builds the Abstract Syntax Tree (AST) for the Theta source code starting from the specified file.
         * @param fileName The file name of the Theta source code.
         * @return A shared pointer to the root node of the constructed AST.
         */
        shared_ptr<ASTNode> buildAST(string fileName);

        /**
         * @brief Builds the Abstract Syntax Tree (AST) for the Theta source code provided.
         * @param source The source code to compile.
         * @param fileName The file name of the Theta source code.
         * @return A shared pointer to the root node of the constructed AST.
         */
        shared_ptr<ASTNode> buildAST(string source, string fileName);

        /**
         * @brief Gets the singleton instance of the ThetaCompiler.
         * @return Reference to the singleton instance of ThetaCompiler.
         */
        static ThetaCompiler& getInstance();

        /**
         * @brief Adds an encountered exception to the list of exceptions to display later
         * @param e The exception to add
         */
        void addException(ThetaCompilationError e);

        /**
         * @brief Returns all the exceptions we encountered during the compilation process
         * @return A vector of compilation errors
         */
        vector<ThetaCompilationError> getEncounteredExceptions();

        /**
         * @brief Clears the list of compilation errors
         */
        void clearExceptions();


        shared_ptr<LinkNode> getIfExistsParsedLinkAST(string capsuleName);

        void addParsedLinkAST(string capsuleName, shared_ptr<LinkNode>);

        shared_ptr<map<string, string>> filesByCapsuleName;
    private:
        /**
         * @brief Private constructor for ThetaCompiler. Initializes the compiler and discovers all capsules in the source files.
         */
        ThetaCompiler() {
            filesByCapsuleName = make_shared<map<string, string>>();
            discoverCapsules();
        }

        // Delete copy constructor and assignment operator to enforce singleton pattern
        ThetaCompiler(const ThetaCompiler&) = delete;
        ThetaCompiler& operator=(const ThetaCompiler&) = delete;


        bool isEmitTokens = false;
        bool isEmitAST = false;
        vector<ThetaCompilationError> encounteredExceptions;
        map<string, shared_ptr<LinkNode>> parsedLinkASTs;

        /**
         * @brief Discovers all capsules in the Theta source code.
         *
         * Scans the current directory and subdirectories to find all `.th` files and extracts capsule names.
         */
        void discoverCapsules();

        /**
         * @brief Finds the capsule name associated with the given file.
         *
         * Reads the content of the file and searches for the `capsule` keyword to identify the capsule name.
         *
         * @param file The file for which to find the capsule name.
         * @return The capsule name corresponding to the file.
         */
        string findCapsuleName(string file);
};
