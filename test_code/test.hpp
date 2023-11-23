#define CREATE_FUNCTION(FunctionName, InternalFunctionName)
#define CREATE_METHOD(MethodName, InternalMethodName, ClassName)
#define CREATE_CLASS(ClassName, InternalClassName)

class MyClass
{
    int foo;

public:
    void bar(){}
};

class Test
{
    public:
    int foo_public;

    const MyClass *TestClassStuff(const MyClass &objectClass, const int &valueForFoo)
    CREATE_METHOD("TestClassStuff", "TestClassStuff", "Test")
    {
        return new MyClass();
    }

    const MyClass *TestClassStuff2(const MyClass &objectClass, const int &valueForFoo) const
    CREATE_METHOD("TestClassStuff2", "TestClassStuff2", "Test")
    {
        return new MyClass();
    }

    private:
    static int foo_s;
    const int foo_c = 0;
    double foo_d;

    Test() {}

    CREATE_CLASS("Test", "Test")
};

int CreateNewClass() CREATE_FUNCTION("CreateNewClass", "CreateNewClass")
{
    return 1;
}