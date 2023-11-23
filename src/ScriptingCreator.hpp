#pragma once

#include <unordered_map>
#include <string>

class ScriptingCreator
{
private:
    static std::unordered_map<std::string, std::pair<std::string, std::string>> MethodMap;
    static std::unordered_map<std::string, std::string>  ClassMap;
    static std::unordered_map<std::string, std::string>  FunctionMap;
public:
    ScriptingCreator() {}
    static std::unordered_map<std::string, std::pair<std::string, std::string>>& GetMethodMap() { return MethodMap; }
    static std::unordered_map<std::string, std::string>& GetClassMap() { return ClassMap; }
    static std::unordered_map<std::string, std::string>& GetFunctionMap() { return FunctionMap; }

    // Will output the scripting definitions into a cpp file for scripting classes
    // The created class script will have the same member visibility ie. public, private, protected. 
    void CreateScriptClasses();

    // Will output the scripting methods (class) into a cpp file for scripting classes
    void CreateScriptMethods();

    // Will output the scripting functions (global) into a cpp file for scripting classes
    void CreateScriptFunctions();
};