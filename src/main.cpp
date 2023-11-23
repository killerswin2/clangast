
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>

#include "DeclFinderAction.hpp"
#include "ScriptingCreator.hpp"
#include <vector>

static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
llvm::cl::OptionCategory FindDeclCategory("ASCLANG options");

int main(int argc, const char **argv)
{
    llvm::outs() << "Scripting Stuff\n";

    llvm::Expected<clang::tooling::CommonOptionsParser> option = clang::tooling::CommonOptionsParser::create(argc, argv, FindDeclCategory);

    auto files = option.get().getSourcePathList();
    clang::tooling::ClangTool tool{option.get().getCompilations(), files};

    tool.run(clang::tooling::newFrontendActionFactory<DeclFindingAction>().get());

    ScriptingCreator scriptC;
    auto methodMap = scriptC.GetMethodMap();

    for(auto& [key, value]: methodMap)
    {
       llvm::outs() << "Key: " << key << " Value: " << value.first << ", " << value.second << "\n";
    }

    auto functionMap = scriptC.GetFunctionMap();

    for(auto& [key, value]: functionMap)
    {
        llvm::outs() << "Key: " << key << " Value: " << value << "\n";
    }

    auto classMap = scriptC.GetClassMap();

    for(auto& [key, value]: classMap)
    {
        llvm::outs() << "Key: " << key << " Value: " << value << "\n";
    }

    return 0;
}