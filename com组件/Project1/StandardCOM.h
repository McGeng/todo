// StandardCOM.h - 标准 COM 组件定义
#pragma once
#include <Windows.h>
#include <unknwn.h>  // IClassFactory 在这里已经定义

// 接口 ID
static const IID IID_ICalculator =
{ 0xAABBCCDD, 0x1234, 0x5678, { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 } };

// 类 ID
static const CLSID CLSID_Calculator =
{ 0xDDCCBBAA, 0x4321, 0x8765, { 0x21, 0x43, 0x65, 0x87, 0xA9, 0xCB, 0xED, 0x0F } };

// 接口定义
class __declspec(novtable) ICalculator : public IUnknown
{
public:
    virtual HRESULT __stdcall Add(int a, int b, int* result) = 0;          // 加法
    virtual HRESULT __stdcall Subtract(int a, int b, int* result) = 0;     // 减法
    virtual HRESULT __stdcall Multiply(int a, int b, int* result) = 0;     // 乘法
    virtual HRESULT __stdcall Divide(int a, int b, int* result) = 0;       // 除法
};

// 注意：IClassFactory 是 Windows 系统定义的标准接口
// 定义在 unknwn.h 中，包含 CreateInstance 和 LockServer 方法

// 实现类
class Calculator : public ICalculator
{
private:
    ULONG m_refCount;  // 引用计数

public:
    Calculator();
    virtual ~Calculator();

    // IUnknown 接口
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
    virtual ULONG __stdcall AddRef() override;
    virtual ULONG __stdcall Release() override;

    // ICalculator 接口
    virtual HRESULT __stdcall Add(int a, int b, int* result) override;
    virtual HRESULT __stdcall Subtract(int a, int b, int* result) override;
    virtual HRESULT __stdcall Multiply(int a, int b, int* result) override;
    virtual HRESULT __stdcall Divide(int a, int b, int* result) override;
};

// 类工厂实现
class CalculatorFactory : public IClassFactory
{
private:
    ULONG m_refCount;  // 引用计数

public:
    CalculatorFactory();
    virtual ~CalculatorFactory();

    // IUnknown 接口
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
    virtual ULONG __stdcall AddRef() override;
    virtual ULONG __stdcall Release() override;

    // IClassFactory 接口
    virtual HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
    virtual HRESULT __stdcall LockServer(BOOL fLock) override;
};

// 全局函数：模拟 COM 注册
HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);
