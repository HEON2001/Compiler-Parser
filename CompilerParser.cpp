#include "CompilerParser.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <cstring>

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
    Token* t = mustBe("keyword", "class");
    std::string type = t->getType();
    std::string value = t->getValue();
    ParseTree* pt = new ParseTree("class", "");
    pt->addChild(new ParseTree(type, value));

    t = mustBe("identifier", identifier(current()->getValue()));
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "{");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    while(have("keyword", "static") || have("keyword", "field")){
        pt->addChild(compileClassVarDec());
    }
    while(have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")){
        pt->addChild(compileSubroutine());
    }

    t = mustBe("symbol", "}");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    Token* t=NULL;
    if(have("keyword", "static")){
        t = mustBe("keyword", "static");
    }
    else if(have("keyword", "field")){
        t = mustBe("keyword", "field");
    }else{
        throw ParseException();
    }

    std::string type = t->getType();
    std::string value = t->getValue();
    ParseTree* pt = new ParseTree("classVarDec", "");
    pt->addChild(new ParseTree(type, value));

    if(have("keyword", "int")){
        t = mustBe("keyword", "int");
    }
    else if(have("keyword", "char")){
        t = mustBe("keyword", "char");
    }
    else if(have("keyword", "boolean")){
        t = mustBe("keyword", "boolean");
    }
    else if(have("identifier", identifier(current()->getValue()))){
        t = mustBe("identifier", current()->getValue());
    }else{
        throw ParseException();
    }

    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("identifier", identifier(current()->getValue()));
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    while(have("symbol", ",")){
        t = mustBe("symbol", ",");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));

        t = mustBe("identifier", identifier(current()->getValue()));
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));
    }

    t = mustBe("symbol", ";");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;

}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    Token* t=NULL;
    if(have("keyword", "constructor")){
        t = mustBe("keyword", "constructor");
    }
    else if(have("keyword", "function")){
        t = mustBe("keyword", "function");
    }
    else if(have("keyword", "method")){
        t = mustBe("keyword", "method");
    }else{
        throw ParseException();
    }
    std::string type = t->getType();
    std::string value = t->getValue();
    ParseTree* pt = new ParseTree("Subroutine", "");
    pt->addChild(new ParseTree(type, value));

    if(have("keyword", "void")){
        t = mustBe("keyword", "void");
    }
    else if(have("keyword", "int")){
        t = mustBe("keyword", "int");
    }
    else if(have("keyword", "char")){
        t = mustBe("keyword", "char");
    }
    else if(have("keyword", "boolean")){
        t = mustBe("keyword", "boolean");
    }
    else if(have("identifier", identifier(current()->getValue()))){
        t = mustBe("identifier", current()->getValue());
    }else{
        throw ParseException();
    }
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("identifier", identifier(current()->getValue()));
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "(");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    if(!have("symbol", ")")){
        pt->addChild(compileParameterList());
    }
    

    t = mustBe("symbol", ")");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));


    pt->addChild(compileSubroutineBody());
   
    
    return pt;

}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("parameterList", "");
    
    if(have("keyword", "int")){
        t = mustBe("keyword", "int");
    }
    else if(have("keyword", "char")){
        t = mustBe("keyword", "char");
    }
    else if(have("keyword", "boolean")){
        t = mustBe("keyword", "boolean");
    }
    else if(have("identifier", identifier(current()->getValue()))){
        t = mustBe("identifier", current()->getValue());
    }else{
        throw ParseException();
    }
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("identifier", identifier(current()->getValue()));
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    while(have("symbol", ",")){
        t = mustBe("symbol", ",");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));

        if(have("keyword", "int")){
        t = mustBe("keyword", "int");
        }
        else if(have("keyword", "char")){
            t = mustBe("keyword", "char");
        }
        else if(have("keyword", "boolean")){
            t = mustBe("keyword", "boolean");
        }
        else if(have("identifier", identifier(current()->getValue()))){
            t = mustBe("identifier", current()->getValue());
        }else{
            throw ParseException();
        }
        
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));

        t = mustBe("identifier", identifier(current()->getValue()));
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));
    }

    return pt;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("subroutineBody", "");

    t = mustBe("symbol", "{");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    while(have("keyword", "var")){
        pt->addChild(compileVarDec());
    }

    while(have("keyword", "let") || have("keyword", "if") || have("keyword", "while") || have("keyword", "do") || have("keyword", "return")){
        pt->addChild(compileStatements());
    }

    t = mustBe("symbol", "}");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
    return NULL;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("varDec", "");

    t = mustBe("keyword", "var");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    if(have("keyword", "int")){
        t = mustBe("keyword", "int");
    }
    else if(have("keyword", "char")){
        t = mustBe("keyword", "char");
    }
    else if(have("keyword", "boolean")){
        t = mustBe("keyword", "boolean");
    }
    else if(have("identifier", identifier(current()->getValue()))){
        t = mustBe("identifier", current()->getValue());
    }else{
        throw ParseException();
    }

    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("identifier", identifier(current()->getValue()));
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    while(have("symbol", ",")){
        t = mustBe("symbol", ",");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));

        t = mustBe("identifier", identifier(current()->getValue()));
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));
    }

    t = mustBe("symbol", ";");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("statements", "");

    if(have("keyword", "let")){
        pt->addChild(compileLet());
    }
    else if(have("keyword", "if")){
        pt->addChild(compileIf());
    }
    else if(have("keyword", "while")){
        pt->addChild(compileWhile());
    }
    else if(have("keyword", "do")){
        pt->addChild(compileDo());
    }
    else if(have("keyword", "return")){
        pt->addChild(compileReturn());
    }else{
        throw ParseException();
    }

    return pt;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("letStatement", "");

    t = mustBe("keyword", "let");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("identifier", identifier(current()->getValue()));
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    if(have("symbol", "[")){
        t = mustBe("symbol", "[");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));

        pt->addChild(compileExpression());

        t = mustBe("symbol", "]");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));
    }

    t = mustBe("symbol", "=");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    pt->addChild(compileExpression());

    t = mustBe("symbol", ";");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("ifStatement", "");

    t = mustBe("keyword", "if");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "(");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    pt->addChild(compileExpression());

    t = mustBe("symbol", ")");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "{");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    pt->addChild(compileStatements());

    t = mustBe("symbol", "}");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    if(have("keyword", "else")){
        t = mustBe("keyword", "else");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));

        t = mustBe("symbol", "{");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));

        pt->addChild(compileStatements());

        t = mustBe("symbol", "}");
        type = t->getType();
        value = t->getValue();
        pt->addChild(new ParseTree(type, value));
    }

    return pt;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("whileStatement", "");

    t = mustBe("keyword", "while");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "(");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    pt->addChild(compileExpression());

    t = mustBe("symbol", ")");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    t = mustBe("symbol", "{");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    pt->addChild(compileStatements());

    t = mustBe("symbol", "}");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("doStatement", "");

    t = mustBe("keyword", "do");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    pt->addChild(compileExpression());

    t = mustBe("symbol", ";");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("returnStatement", "");

    t = mustBe("keyword", "return");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    if(!have("symbol", ";")){
        pt->addChild(compileExpression());
    }

    t = mustBe("symbol", ";");
    type = t->getType();
    value = t->getValue();
    pt->addChild(new ParseTree(type, value));

    return pt;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {

    return NULL;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {

    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    Token* t=NULL;
    std::string type;
    std::string value;

    ParseTree* pt = new ParseTree("expressionList", "");

    if(!have("symbol", ")")){
        pt->addChild(compileExpression());
        while(have("symbol", ",")){
            t = mustBe("symbol", ",");
            type = t->getType();
            value = t->getValue();
            pt->addChild(new ParseTree(type, value));

            pt->addChild(compileExpression());
        }
    }
    return pt;
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

std::string CompilerParser::identifier(std::string value){
    if(value[0]=='_'||isalpha(value[0])==0){
        throw ParseException();
    }
    return value;
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
