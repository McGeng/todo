# 标准 COM 组件说明

## 📂 文件说明

| 文件 | 说明 |
|------|------|
| `SimpleCOM.h/cpp` + `main.cpp` | 简化版（学习用，已排除编译） |
| `StandardCOM.h/cpp` + `TestStandardCOM.cpp` | **标准版（当前编译）** |

## 🔄 简化版 vs 标准版

### 简化版（SimpleCOM）
```cpp
// 直接创建对象
SimpleCalculator* pCalc = new SimpleCalculator();
```
- ✅ 简单易懂
- ❌ 不符合 COM 标准
- ❌ 无法跨进程/跨机器

### 标准版（StandardCOM）
```cpp
// 通过类工厂创建对象
DllGetClassObject(CLSID, IID_IClassFactory, &pFactory);
pFactory->CreateInstance(nullptr, IID, &pCalc);
```
- ✅ 符合 COM 标准
- ✅ 支持远程创建
- ✅ 真实项目使用

## 🏗️ 标准 COM 架构

```
客户端
  ↓
DllGetClassObject (获取类工厂)
  ↓
IClassFactory (类工厂接口)
  ↓
CreateInstance (创建对象)
  ↓
ICalculator (业务接口)
  ↓
Calculator (COM 对象)
```

## 🎯 核心概念

### 1. IClassFactory（类工厂接口）
- **作用**：统一的对象创建接口
- **方法**：
  - `CreateInstance()` - 创建对象
  - `LockServer()` - 锁定服务器（防止 DLL 被卸载）

### 2. DllGetClassObject（全局函数）
- **作用**：COM 系统调用此函数获取类工厂
- **参数**：
  - `CLSID` - 要创建的类 ID
  - `IID` - 请求的接口 ID
  - `ppv` - 返回的接口指针

### 3. 创建流程
```cpp
// 步骤 1: 获取类工厂
IClassFactory* pFactory;
DllGetClassObject(CLSID_Calculator, IID_IClassFactory, &pFactory);

// 步骤 2: 创建对象
ICalculator* pCalc;
pFactory->CreateInstance(nullptr, IID_ICalculator, &pCalc);

// 步骤 3: 使用对象
pCalc->Add(10, 20, &result);

// 步骤 4: 释放
pCalc->Release();
pFactory->Release();
```

## 📊 对象生命周期

```
DllGetClassObject
  → new CalculatorFactory (RefCount=1)
  → QueryInterface (RefCount=2)
  → Release (RefCount=1)

CreateInstance
  → new Calculator (RefCount=1)
  → QueryInterface (RefCount=2)
  → Release (RefCount=1)

客户端 Release
  → Calculator RefCount=0 → delete
  → Factory RefCount=0 → delete
```

## 🔑 关键点

1. **双层工厂模式**
   - 先创建工厂
   - 工厂创建对象

2. **引用计数管理**
   - 工厂有独立的引用计数
   - 对象有独立的引用计数

3. **符合 COM 规范**
   - `DllGetClassObject` 是 COM 标准导出函数
   - `IClassFactory` 是 COM 标准接口

## 🚀 运行效果

```
【步骤 1】获取类工厂
[DllGetClassObject] 请求类工厂...
[Factory] 工厂创建, RefCount = 1
[Factory] QueryInterface -> IClassFactory
[Factory] AddRef, RefCount = 2
[Factory] Release, RefCount = 1

【步骤 2】通过类工厂创建对象
[Factory] CreateInstance 开始...
[Calculator] 对象创建, RefCount = 1
[Calculator] QueryInterface -> ICalculator
[Calculator] AddRef, RefCount = 2
[Calculator] Release, RefCount = 1

【步骤 3】使用 Calculator 对象
[Calculator] Add: 100 + 50 = 150
结果: 150
...

【步骤 5】释放对象
[Calculator] Release, RefCount = 0
[Calculator] 对象销毁
[Factory] Release, RefCount = 0
[Factory] 工厂销毁
```

## 💡 为什么需要类工厂？

### 在真实 COM 系统中：

1. **跨进程创建**
   - 客户端在进程 A
   - COM 服务器在进程 B
   - 通过类工厂统一创建

2. **动态加载 DLL**
   - 不直接 `new` 对象
   - 通过 DLL 导出的工厂函数

3. **注册表查找**
   ```
   CoCreateInstance(CLSID_Calculator, ...)
     → 查注册表找到 DLL 路径
     → LoadLibrary 加载 DLL
     → GetProcAddress("DllGetClassObject")
     → 调用工厂创建对象
   ```

## 🎓 学习建议

1. **先理解简化版**（SimpleCOM）
   - 理解接口、实现、引用计数

2. **再学习标准版**（StandardCOM）
   - 理解类工厂模式
   - 理解 COM 创建流程

3. **对比两者差异**
   - 简化版：直接 new
   - 标准版：通过工厂

## 📚 下一步

学完标准版后，可以继续学习：
- COM DLL 注册（regsvr32）
- 使用 CoCreateInstance
- 进程外 COM 服务器（EXE）
- COM 自动化（IDispatch）
- ATL 简化开发

---

**当前编译配置**：TestStandardCOM.cpp（标准版）
