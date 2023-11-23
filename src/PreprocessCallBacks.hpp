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
        std::string methodName, internalMethodName, className;

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

        llvm::StringRef stringInternalMethodName = tokInternalMethodName->getLiteralData();
        stringInternalMethodName = stringInternalMethodName.substr(1, tokInternalMethodName->getLength() - 1);

        llvm::StringRef stringClassName = tokClassName->getLiteralData();
        stringClassName = stringClassName.substr(1, tokClassName->getLength() - 1);

        std::pair<std::string, std::string> classInfo{stringInternalMethodName, stringClassName};
        auto classMap = scriptingCreator.GetClassMap();
        classMap.insert_or_assign(stringClassName.str() + "::" + stringMethodName.str(), classInfo);

    }

    //#define CREATE_FUNCTION(FunctionName, InternalFunctionName)
    void AddCreateFunction(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args)
    {
        unsigned int argNum = Args->getNumMacroArguments();
        for(int i = 0; i < argNum; i++)
        {
            const clang::Token* tok = Args->getUnexpArgument(i);

            if(tok->getKind() != clang::tok::TokenKind::string_literal)
            {
                // we only support string literals in the macros
                llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tok->getLocation().printToString(SourceManager) << "\n";
                exit(1);
            }

            llvm::StringRef string = tok->getLiteralData();
            string = string.substr(0, tok->getLength());
            llvm::outs() << string << " ";

        }
        llvm::outs() << "\n";
    }

    //#define CREATE_CLASS(ClassName, InternalClassName)
    void AddCreateClass(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD, clang::SourceRange Range, const clang::MacroArgs* Args)
    {
        unsigned int argNum = Args->getNumMacroArguments();
        for(int i = 0; i < argNum; i++)
        {
            const clang::Token* tok = Args->getUnexpArgument(i);

            if(tok->getKind() != clang::tok::TokenKind::string_literal)
            {
                // we only support string literals in the macros
                llvm::outs() << "FAILURE: NOT A STRING LITERAL. LOCATION: " << tok->getLocation().printToString(SourceManager) << "\n";
                exit(1);
            }

            llvm::StringRef string = tok->getLiteralData();
            string = string.substr(0, tok->getLength());
            llvm::outs() << string << " ";

        }
        llvm::outs() << "\n";
    }
};
