#pragma once

#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Mangle.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/PPCallbacks.h>
#include <llvm/Support/raw_ostream.h>
#include <clang/Lex/MacroArgs.h>

#include <string>
#include <sstream>
#include <memory>

#include "ScriptingCreator.hpp"

class ScriptingPPCallbacks : public clang::PPCallbacks
{
    clang::SourceManager &SourceManager;
public:
    ScriptingPPCallbacks(clang::SourceManager& SM) : SourceManager{ SM } {} 

    void MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args) override
    {
        if(MacroNameTok.getIdentifierInfo()->getName() == "CREATE_METHOD")
        {
            AddCreateMethod(MacroNameTok, MD, Range, Args);
        }
        else if (MacroNameTok.getIdentifierInfo()->getName() == "CREATE_FUNCTION")
        {
            AddCreateFunction(MacroNameTok, MD, Range, Args);
        }
        else if (MacroNameTok.getIdentifierInfo()->getName() == "CREATE_CLASS")
        {
            AddCreateClass(MacroNameTok, MD, Range, Args);
        }

        //llvm::outs() << MacroNameTok.getIdentifierInfo()->getName() << "\n";
        //llvm::outs() << "Args: ";

        //llvm::outs() << "\n";
    }

    //#define CREATE_METHOD(MethodName, InternalMethodName, ClassName)
    void AddCreateMethod(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args)
    {
        ScriptingCreator scriptingCreator;

        const clang::Token* tokMethodName = Args->getUnexpArgument(0);
        const clang::Token* tokInternalMethodName = Args->getUnexpArgument(1);
        const clang::Token* tokClassName = Args->getUnexpArgument(2);

        // we only support string literals in the macros
        if(tokMethodName->getKind() != clang::tok::TokenKind::string_literal)
        {
            llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tokMethodName->getLocation().printToString(SourceManager) << "\n";
            exit(1);
        }
        else if (tokInternalMethodName->getKind() != clang::tok::TokenKind::string_literal)
        {
            llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tokInternalMethodName->getLocation().printToString(SourceManager) << "\n";
            exit(1);
        }
        else if (tokClassName->getKind() != clang::tok::TokenKind::string_literal)
        {
            llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tokClassName->getLocation().printToString(SourceManager) << "\n";
            exit(1);
        }
        
        llvm::StringRef stringMethodName = tokMethodName->getLiteralData();
        stringMethodName = stringMethodName.substr(1, tokMethodName->getLength() - 1);
        stringMethodName = stringMethodName.trim(" \t\n\v\f\r\"");

        llvm::StringRef stringInternalMethodName = tokInternalMethodName->getLiteralData();
        stringInternalMethodName = stringInternalMethodName.substr(1, tokInternalMethodName->getLength() - 1);
        stringInternalMethodName = stringInternalMethodName.trim(" \t\n\v\f\r\"");

        llvm::StringRef stringClassName = tokClassName->getLiteralData();
        stringClassName = stringClassName.substr(1, tokClassName->getLength() - 1);
        stringClassName = stringClassName.trim(" \t\n\v\f\r\"");

        std::pair<std::string, std::string> methodInfo{stringInternalMethodName.str(), stringClassName.str()};
        auto& MethodMap = scriptingCreator.GetMethodMap();

        if(MethodMap.find(stringClassName.str() + "::" + stringMethodName.str()) != MethodMap.end())
        {
            // TODO: Add warning code for duplicates
        }
        else
        {
            MethodMap[stringClassName.str() + "::" + stringMethodName.str()] = methodInfo;
        }

    }

    //#define CREATE_FUNCTION(FunctionName, InternalFunctionName)
    void AddCreateFunction(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args)
    {
        ScriptingCreator scriptingCreator;

        const clang::Token* tokFunctionName = Args->getUnexpArgument(0);
        const clang::Token* tokInternalFunctionName = Args->getUnexpArgument(1);

        // we only support string literals in the macros
        if(tokFunctionName->getKind() != clang::tok::TokenKind::string_literal)
        {
            llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tokFunctionName->getLocation().printToString(SourceManager) << "\n";
            exit(1);
        }
        else if (tokInternalFunctionName->getKind() != clang::tok::TokenKind::string_literal)
        {
            llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tokInternalFunctionName->getLocation().printToString(SourceManager) << "\n";
            exit(1);
        }
        
        llvm::StringRef stringFunctionName = tokFunctionName->getLiteralData();
        stringFunctionName = stringFunctionName.substr(1, tokFunctionName->getLength() - 1);
        stringFunctionName = stringFunctionName.trim(" \t\n\v\f\r\"");

        llvm::StringRef stringInternalFunctionName = tokInternalFunctionName->getLiteralData();
        stringInternalFunctionName = stringInternalFunctionName.substr(1, tokInternalFunctionName->getLength() - 1);
        stringInternalFunctionName = stringInternalFunctionName.trim(" \t\n\v\f\r\"");

        auto& functionMap = scriptingCreator.GetFunctionMap();
        functionMap.insert_or_assign(stringFunctionName.str(), stringInternalFunctionName.str());
    }

    //#define CREATE_CLASS(ClassName, InternalClassName)
    void AddCreateClass(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args)
    {
        ScriptingCreator scriptingCreator;

        const clang::Token* tokClassName = Args->getUnexpArgument(0);
        const clang::Token* tokInternalClassName = Args->getUnexpArgument(1);

        // we only support string literals in the macros
        if(tokClassName->getKind() != clang::tok::TokenKind::string_literal)
        {
            llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tokClassName->getLocation().printToString(SourceManager) << "\n";
            exit(1);
        }
        else if (tokInternalClassName->getKind() != clang::tok::TokenKind::string_literal)
        {
            llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tokInternalClassName->getLocation().printToString(SourceManager) << "\n";
            exit(1);
        }
        
        llvm::StringRef stringClassName = tokClassName->getLiteralData();
        stringClassName = stringClassName.substr(1, tokClassName->getLength() - 1);
        stringClassName = stringClassName.trim(" \t\n\v\f\r\"");

        llvm::StringRef stringInternalClassName = tokInternalClassName->getLiteralData();
        stringInternalClassName = stringInternalClassName.substr(1, tokInternalClassName->getLength() - 1);
        stringInternalClassName = stringInternalClassName.trim(" \t\n\v\f\r\"");

        auto& classMap = scriptingCreator.GetClassMap();
        classMap.insert_or_assign(stringClassName.str(), stringInternalClassName.str());
    }
};
