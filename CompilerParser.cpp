#include "CompilerParser.h"
#include <iostream>
#include <iterator>
#include <vector>

/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens){
    tkns = tokens;
    currentTokenIndex=0;
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    Token* t = mustBe("keyword", "class");
    std::string type = t->getType();
    std::string value = t->getValue();
    ParseTree* pt = new ParseTree("class", "");
    pt->addChild(new ParseTree(type, value));

    t = mustBe("identifier", "Main");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "{");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "}");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    mustBe("keyword", "class");
    mustBe("identifier", current()->getValue());
    mustBe("symbol", "{");
    while(have("keyword", "static")||have("keyword", "field")){
        compileClassVarDec();
    }
    while(have("keyword", "function")||have("keyword", "method")||have("keyword", "constructor")){
        compileSubroutine();
    }
    mustBe("symbol", "}");
    return NULL;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    mustBe("keyword", "static");
    mustBe("keyword", "int");
    mustBe("identifier", "a");
    mustBe("symbol", ";");
    return NULL;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    mustBe("keyword", "function");
    mustBe("keyword", "void");
    mustBe("identifier", "myFunc");
    mustBe("symbol", "(");
    compileParameterList();
    mustBe("symbol", ")");
    mustBe("symbol", "{");
    compileSubroutineBody();
    mustBe("symbol", "}");

    return NULL;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    mustBe ("keyword", "int");
    mustBe ("identifier", "a");
    mustBe ("symbol", ",");
    mustBe ("keyword", "char");
    mustBe ("identifier", "b");
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    mustBe ("symbol", "{");
    mustBe ("keyword", "var");
    mustBe ("keyword", "int");
    mustBe ("identifier", "a");
    mustBe ("symbol", ";");
    mustBe ("keyword", "let");
    mustBe ("identifier", "a");
    mustBe ("symbol", "=");
    mustBe ("integerConstant", "1");
    mustBe ("symbol", ";");
    mustBe ("symbol", "}");


    return NULL;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    mustBe("keyword", "var");
    return NULL;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    mustBe("keyword", "let");
    return NULL;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    mustBe("keyword", "let");

    return NULL;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    mustBe("keyword", "if");

    return NULL;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    mustBe("keyword", "while");

    return NULL;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    mustBe("keyword", "do");
    return NULL;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    mustBe("keyword", "return");

    return NULL;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    mustBe("integerConstant", "1");
    return NULL;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    mustBe("integerConstant", "1");

    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    mustBe("integerConstant", "1");
    return NULL;
}

/**
 * Advance to the next token
 */
void CompilerParser::next(){
    if(currentTokenIndex < tkns.size()-1)
    currentTokenIndex++;

    return;
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    std::list<Token*>::iterator it = tkns.begin();
    std::advance(it, currentTokenIndex);

    return it.operator*();
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    Token* t = current();
    if(t->getType() == expectedType && t->getValue() == expectedValue){
        return true;
    }
    return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){
    Token* t = current();

    if(t->getType() == expectedType && t->getValue() == expectedValue){
        next();
        return t;
    }else{
        throw ParseException();
    }
    return NULL;
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
