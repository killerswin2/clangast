
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/raw_ostream.h>

#include "DeclFinderAction.hpp"
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
    return 0;
}