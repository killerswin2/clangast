// #pragma once

// #include <clang/Frontend/FrontendAction.h>
// #include <clang/Frontend/CompilerInstance.h>
// #include <clang/AST/ASTConsumer.h>
// #include <clang/AST/ASTContext.h>
// #include <clang/AST/Mangle.h>
// #include <clang/Basic/SourceManager.h>

// #include <memory>

// namespace clang
// {
//     class ASTConsumer;
// }

// class PreprocessClassFinder : public clang::ASTConsumer
// {
//     clang::SourceManager &SourceManager;
//     //DeclVisitor Visitor;

// public:
//     PreprocessClassFinder(clang::SourceManager &SM, clang::ASTContext &CON) : SourceManager(SM), Visitor(SM, CON) {}

// };


// class PreprocessorClassFinderAction : public clang::PreprocessorFrontendAction
// {
// public:
//     std::unique_ptr<clang::ASTConsumer>
//     CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef) final
//     {
//         return std::unique_ptr<clang::ASTConsumer>(new DeclFinder(CI.getSourceManager(), CI.getASTContext()));
//     }
// };