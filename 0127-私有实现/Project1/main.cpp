/*
 * Pimpl 模式使用注意点：
 *
 * 1. 内存管理：
 *    - faceClass 析构函数必须 delete d_ptr，否则 innerClass 会泄漏
 *    - innerClass 不能 delete p_ptr（外部传入的指针），否则双重释放
 *
 * 2. 拷贝语义：
 *    - 默认拷贝构造函数只拷贝指针，会导致两个对象共享同一个 innerClass
 *    - 需要自定义拷贝构造函数和赋值运算符（深拷贝），或者禁用它们
 *
 * 3. 推荐使用智能指针：
 *    - 用 std::unique_ptr<innerClass> 代替裸指针
 *    - 自动管理内存，避免忘记 delete
 *
 * 4. 性能开销：
 *    - 每次访问成员需要通过指针间接访问
 *    - 构造时额外的堆分配开销
 */

#include "faceClass.h"
#include <iostream>
using namespace std;

int main()
{
	faceClass* pFace = new faceClass;
	cout << "ID is: " << pFace->getID() << endl;
	pFace->setID(111);
	cout << "Now, id is: " << pFace->getID() << endl;

	delete pFace;  // 必须 delete，否则 faceClass 和 innerClass 都会泄漏
	return 0;
}