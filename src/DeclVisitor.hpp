#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/Support/raw_ostream.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Mangle.h>

#include "ScriptingCreator.hpp"

#include <string>
#include <sstream>

class DeclVisitor : public clang::RecursiveASTVisitor<DeclVisitor>
{
    clang::SourceManager &SourceManager;
    clang::ASTContext &Context;

public:
    DeclVisitor(clang::SourceManager &SourceManager, clang::ASTContext &Context)
        : SourceManager(SourceManager), Context(Context) {}

    bool VisitFunctionDecl(clang::FunctionDecl *funDecl)
    {
        auto *mangledContext = Context.createMangleContext();
        if (mangledContext->shouldMangleDeclName(funDecl))
        {
            llvm::outs() << "Mangled Name: " << funDecl->getQualifiedNameAsString() << " at "
                         << getDeclLocation(funDecl->getBeginLoc()) << " ";

            std::string mangledName;
            llvm::raw_string_ostream ostream(mangledName);

            // constructors require a different way for getting the mangled name Else they break.
            if (funDecl->getKind() == clang::Decl::Kind::CXXConstructor)
            {
                clang::GlobalDecl GDCon{static_cast<clang::CXXConstructorDecl *>(funDecl), clang::CXXCtorType::Ctor_Comdat};
                mangledContext->mangleName(GDCon, ostream);
            }
            else
            {
                mangledContext->mangleName(funDecl, ostream);
            }
            ostream.flush();

            llvm::outs() << mangledName << "\n";

            llvm::outs() << "Parameter count: " << funDecl->getNumParams() << "\n";

            auto funcParams = funDecl->parameters();

            if (funcParams.size() > 0)
            {
                llvm::outs() << "Parameters: ";
                for (int i = 0; i < funcParams.size(); i++)
                {
                    llvm::outs() << funcParams[i]->getQualifiedNameAsString() << " ";
                }
                llvm::outs() << "\n";

                // types
                llvm::outs() << "Types: ";
                for (int i = 0; i < funcParams.size(); i++)
                {
                    llvm::outs() << funcParams[i]->getOriginalType().getAsString() << " ";
                }
                llvm::outs() << "\n";
            }

            if (funDecl->getKind() == clang::Decl::Kind::CXXMethod)
            {
                auto* methDecl = static_cast<clang::CXXMethodDecl*>(funDecl);

                // qualifed types
                llvm::outs() << "Qualifier Types: ";
                auto type = methDecl->getMethodQualifiers();
                llvm::outs() << type.getAsString();

                llvm::outs() << "\n";
            }

            // return type
            llvm::outs() << "return type: " << funDecl->getReturnType().getAsString() << "\n";

            delete mangledContext;
        }
        llvm::outs() << "\n";
        return true;
    }

    bool VisitNamedDecl(clang::NamedDecl *NamedDecl)
    {

        // llvm::outs() << "Found " << NamedDecl->getQualifiedNameAsString() << " at "
        //              << getDeclLocation(NamedDecl->getBeginLoc()) << "\n";
        return true;
    }

    bool VisitCXXRecordDecl(clang::CXXRecordDecl *recordDecl)
    {
        //ScriptingCreator scriptCreator;
        //auto map = scriptCreator.GetClassMap();
        clang::RecordDecl::field_range fields = recordDecl->fields();
        clang::RecordDecl::field_iterator end = fields.end();

        llvm::outs() << "Class: " << recordDecl->getName() << "\n";
        

        for(auto iter = fields.begin(); iter != end; iter++)
        {
            if(iter->getAccess() == clang::AccessSpecifier::AS_public)
            {
                llvm::outs() << iter->getType().getAsString() << " " << iter->getQualifiedNameAsString() << ", ";
            }

            //llvm::outs() << iter->getType().getAsString() << " " << iter->getQualifiedNameAsString() << ", ";
        }
        llvm::outs() << "\n";

        return true;
    }

private:
    std::string getDeclLocation(clang::SourceLocation Loc) const
    {
        std::ostringstream OSS;
        OSS << SourceManager.getFilename(Loc).str() << ":"
            << SourceManager.getSpellingLineNumber(Loc) << ":"
            << SourceManager.getSpellingColumnNumber(Loc);
        return OSS.str();
    }
};