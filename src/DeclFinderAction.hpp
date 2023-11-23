#pragma once

#include "DeclFinder.hpp"

#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/CompilerInstance.h>

#include <memory>

#include "PreprocessCallBacks.hpp"

namespace clang
{
    class ASTConsumer;
}

class DeclFindingAction : public clang::ASTFrontendAction
{
public:
    std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef) final
    {
        return std::unique_ptr<clang::ASTConsumer>(new DeclFinder(CI.getSourceManager(), CI.getASTContext()));
    }

    bool BeginSourceFileAction(clang::CompilerInstance &CI) override
    {
        std::unique_ptr<clang::PPCallbacks> scriptingCallbacks {new ScriptingPPCallbacks(CI.getSourceManager())};
        
        clang::Preprocessor& pp = CI.getPreprocessor();
        pp.addPPCallbacks(std::move(scriptingCallbacks));

        return true;
    }
};