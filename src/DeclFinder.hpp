#pragma once

#include "DeclVisitor.hpp"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Mangle.h>
#include <clang/Basic/SourceManager.h>

class DeclFinder : public clang::ASTConsumer
{
    clang::SourceManager &SourceManager;
    DeclVisitor Visitor;

public:
    DeclFinder(clang::SourceManager &SM, clang::ASTContext &CON) : SourceManager(SM), Visitor(SM, CON) {}

    void HandleTranslationUnit(clang::ASTContext &Context) final
    {
        auto Decls = Context.getTranslationUnitDecl()->decls();
        for (auto &Decl : Decls)
        {
            const auto &FileID = SourceManager.getFileID(Decl->getLocation());
            if (FileID != SourceManager.getMainFileID())
                continue;
            Visitor.TraverseDecl(Decl);
        }
    }
};