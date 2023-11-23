#define CREATE_FUNCTION(FunctionName, InternalFunctionName)
#define CREATE_METHOD(MethodName, InternalMethodName, ClassName)
#define CREATE_CLASS(ClassName, InternalClassName)

class MyClass
{
    int foo;

public:
    void bar() CREATE_METHOD("bar", "bar", "MyClass") {}
    CREATE_CLASS("MyClass", "MyClass")
};

class Test
{
    static int foo_s;
    const int foo_c = 0;
    const MyClass *TestClassStuff(const MyClass &objectClass, const int &valueForFoo)
    {
        return new MyClass();
    }

    const MyClass *TestClassStuff2(const MyClass &objectClass, const int &valueForFoo) const
    {
        return new MyClass();
    }
    Test() {}
};

int CreateNewClass() CREATE_FUNCTION("CreateNewClass", "CreateNewClass")
{
    return 1;
}