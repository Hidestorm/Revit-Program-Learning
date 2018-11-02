## 从C到C++
#### 1. 尽量使用const和inline，不要用#define
- 使用#define定义的变量名不会被加入到符号列表中

		#define ASPECT_RATIO 1.653
		\\ASPECT_RATIO不会加入到符号列表中

常量的定义可以使用const 来实现

- 使用#define指令，来实现那些看起来象函数而又不会导致函数调用的宏

		#define max(a,b) ((a) > (b) ? (a) : (b))
		a = 5, b = 0;
		max(++a, b);
		max(++a, b + 10);

使用内联函数来代替
		
	template<class T>
	inline const T& max(const T& a, const T& b)
	{ return a > b ? a : b; }

#### 2. 尽量用<iostream>而不用<stdio.h>

iostream比scanf和printf具有更好的安全性和可扩展性

但是
1. 有些iostream的操作实现起来比相应的C stream效率要低，所以不同的选择会给你的程序有可能(虽然不一定，参见条款M16)带来很大的不同。但请牢记，这不是对所有的iostream而言，只是一些特殊的实现；参见条款M23。
2. 在标准化的过程中，iostream库在底层做了很多修改(参见条款49)，所以对那些要求最大可移植性的应用程序来说，会发现不同的厂商遵循标准的程度也不同。
3. iostream库的类有构造函数而<stdio.h>里的函数没有，在某些涉及到静态对象初始化顺序的时候，如果可以确认不会带来隐患，用标准C库会更简单实用。

#### 3.尽量用new和delete而不用malloc和free
不要把这两者混用

#### 4：尽量使用c++风格的注释
	if ( a > b ) {
		/*	int temp = a;  /* swap a and b */
		a = b;
		b = temp;
	*/
	}

注释行会提前结束

## 内存管理
#### 5：对应的new和delete要采用相同的形式
对于new 创建的数组要使用delete[]删除

#### 6：析构函数里对指针成员调用delete

#### 7：预先准备好内存不够的情况
c的一种常用的做法是，定义一个类型无关的宏来分配内存并检查分配是否成功。对于c++来说，这个宏看起来可能象这样：

    #define new(ptr, type)		\
    try { (ptr) = new type; }	\
    catch (std::bad_alloc&) { assert(0); }



- assert是个宏。这个宏检查传给它的表达式是否非零，如果不是非零值，就会发出一条出错信息并调用abort。assert只是在没定义标准宏ndebug的时候，即在调试状态下才这么做。在产品发布状态下，即定义了ndebug的时候，assert什么也不做，相当于一条空语句。所以你只能在调试时才能检查断言(assertion)）。
- new会有不同的类型，必须要对每种类型进行处理

新的处理方法：当内存分配请求不能满足时，调用你预先指定的一个出错处理函数。

指定出错处理函数时要用到set_new_handler函数，它在头文件<new>里大致是象下面这样定义的：

	typedef void (*new_handler)();
	new_handler set_new_handler(new_handler p) throw();

- new_handler是一个自定义的函数指针类型，它指向一个没有输入参数也没有返回值的函数。set_new_handler则是一个输入并返回new_handler类型的函数。
- set_new_handler的输入参数是operator new分配内存失败时要调用的出错处理函数的指针，返回值是set_new_handler没调用之前就已经在起作用的旧的出错处理函数的指针。

		// function to call if operator new can't allocate enough memory
		void nomorememory()
		{
			cerr << "unable to satisfy request for memory\n";
		abort();
		}

		int main()
		{
			set_new_handler(nomorememory);
			int *pbigdataarray = new int[100000000];

		}


一个设计得好的new-handler函数必须实现下面功能中的一种。

- 产生更多的可用内存。这将使operator new下一次分配内存的尝试有可能获得成功。实施这一策略的一个方法是：在程序启动时分配一个大的内存块，然后在第一次调用new-handler时释放。释放时伴随着一些对用户的警告信息，如内存数量太少，下次请求可能会失败，除非又有更多的可用空间。
- 安装另一个不同的new-handler函数。如果当前的new-handler函数不能产生更多的可用内存，可能它会知道另一个new-handler函数可以提供更多的资源。这样的话，当前的new-handler可以安装另一个new-handler来取代它(通过调用set_new_handler)。下一次operator new调用new-handler时，会使用最近安装的那个。(这一策略的另一个变通办法是让new-handler可以改变它自己的运行行为，那么下次调用时，它将做不同的事。方法是使new-handler可以修改那些影响它自身行为的静态或全局数据。)
- 卸除new-handler。也就是传递空指针给set_new_handler。没有安装new-handler，operator new分配内存不成功时就会抛出一个标准的std::bad_alloc类型的异常。
-  抛出std::bad_alloc或从std::bad_alloc继承的其他类型的异常。这样的异常不会被operator new捕捉，所以它们会被送到最初进行内存请求的地方。(抛出别的不同类型的异常会违反operator new异常规范。规范中的缺省行为是调用abort，所以new-handler要抛出一个异常时，一定要确信它是从std::bad_alloc继承来的。想更多地了解异常规范，参见条款m14。)
- 没有返回。典型做法是调用abort或exit。abort/exit可以在标准c库中找到(还有标准c++库，参见条款49)。


#### 8.重写Operator new 和Operator delete时要遵循常规

 为什么要自己重写 operator new 和operator delete函数？

- 为了效率，通用的new 操作会在为对象分配内存时，记录该对象所占用的内存空间。某些情况下，这是需要优化的

## 设计与声明

#### 18. 争取使类的接口完整并且最小

不要为类设计过多接口，但要保证类的完整性。
#### 19. 分清成员函数，非成员函数和友元函数

假设f是想正确声明的函数，c是和它相关的类：

- 虚函数必须是成员函数。如果f必须是虚函数，就让它成为c的成员函数。

- operator>>和operator<<决不能是成员函数。如果f是operator>>或operator<<，让f成为非成员函数。如果f还需要访问c的非公有成员，让f成为c的友元函数。

- 只有非成员函数对最左边的参数进行类型转换。如果f需要对最左边的参数进行类型转换，让f成为非成员函数。如果f还需要访问c的非公有成员，让f成为c的友元函数。

- 其它情况下都声明为成员函数。如果以上情况都不是，让f成为c的成员函数。
#### 20: 避免public接口出现数据成员

#### 21: 尽可能使用const

    char *p              = "hello";          // 非const指针,
                                         // 非const数据

    const char *p        = "hello";          // 非const指针,
                                         // const数据

    char * const p       = "hello";          // const指针,
                                         // 非const数据

    const char * const p = "hello";          // const指针,
                                         // const数据

语法并非看起来那么变化多端。一般来说，你可以在头脑里画一条垂直线穿过指针声明中的星号（*）位置，如果const出现在线的左边，指针指向的数据为常量；如果const出现在线的右边，指针本身为常量；如果const在线的两边都出现，二者都是常量。

- 在一个函数声明中，const可以指的是函数的返回值，或某个参数；对于成员函数，还可以指的是整个函数。

##### 数据意义上的const（bitwise constness）和概念意义上的const（conceptual constness）。

- bitwise constness的坚持者认为，当且仅当成员函数不修改对象的任何数据成员（静态数据成员除外）时，即不修改对象中任何一个比特(bit)时，这个成员函数才是const的。bitwise constness最大的好处是可以很容易地检测到违反bitwise constness规定的事件：编译器只用去寻找有无对数据成员的赋值就可以了。实际上，bitwise constness正是c++对const问题的定义，const成员函数不被允许修改它所在对象的任何一个数据成员。

- 不幸的是，很多不遵守bitwise constness定义的成员函数也可以通过bitwise测试。特别是，一个“修改了指针所指向的数据”的成员函数，其行为显然违反了bitwise constness定义，但如果对象中仅包含这个指针，这个函数也是bitwise const的，编译时会通过

mutable bool lengthisvalid：
  - 这些数据成员现在为mutable；他们可以在何地方被修改，即使 在const成员函数里

可以使用const_cast来消除指针的const，只有在被调用的函数不会修改它的参数所指的数据时，才能这么做

#### 22: 尽量用“传引用”而不用“传值”
避免无用的调用构造和析构函数

它避免了所谓的“切割问题（slicing problem）”。当一个派生类的对象作为基类对象被传递时，它（派生类对象）的作为派生类所具有的行为特性会被“切割”掉，从而变成了一个简单的基类对象。

引用几乎都是通过指针来实现的，所以通过引用传递对象实际上是传递指针。因此，如果是一个很小的对象——例如int——传值实际上会比传引用更高效。

#### 23: 必须返回一个对象时不要试图返回一个引用
当需要在返回引用和返回对象间做决定时，你的职责是选择可以完成正确功能的那个。至于怎么让这个选择所产生的代价尽可能的小，那是编译器的生产商去想的事。

#### 24: 在函数重载和设定参数缺省值间慎重选择
如果可以选择一个合适的缺省值并且只是用到一种算法，就使用缺省参数（参见条款38）。否则，就使用函数重载。

#### 25: 避免对指针和数字类型重载
	void f(int x);

	void f(string *ps);

	f(0);			// 调用f(int)还是f(string*)?

#### 26: 当心潜在的二义性
1. 函数对参数的隐式转换
2. 多继承是对基类成员函数的选择

#### 27: 如果不想使用隐式生成的函数就要显式地禁止它

将函数显式定义为私有

#### 28: 划分全局名字空间

使用命名空间防止不同头文件中变量的重名

使用结构体可以达到近似的效果，缺点是：
如果运算符被定义为结构的静态成员，它就只能通过函数调用来使用，而不能象常规的运算符所设计的那样，可以通过自然的中缀语法来使用：
## 类和函数：实现
#### 29: 避免返回内部数据的句柄
对于const成员函数来说，返回句柄是不明智的，因为它会破坏数据的const性

对于非const成员函数来说，返回句柄会带来麻烦，特别是涉及到临时对象时；

#### 30: 避免这样的成员函数：其返回值是指向成员的非const指针或引用，但成员的访问级比这个函数要低

如果需要返回private或protect的成员的引用，最好使用const引用

#### 31: 千万不要返回局部对象的引用，也不要返回函数内部用new初始化的指针的引用
返回函数内部用new初始化的对象的引用，会导致这个对象不会被正确的delete

    inline const rational& operator*(const rational& lhs,
     const rational& rhs)
    {
    // create a new object on the heap
      rational *result =
    new rational(lhs.n * rhs.n, lhs.d * rhs.d);
    
      // return it
      return *result;
    }
    const rational& four = two * two;  // 得到废弃的指针；
       // 将它存在一个引用
    
    delete &four; // 得到指针并删除


这种删除方法违背正常操作习惯

#### 32: 尽可能地推迟变量的定义

如果定义了一个有构造函数和析构函数的类型的变量，当程序运行到变量定义之处时，必然面临构造的开销；当变量离开它的生命空间时，又要承担析构的开销。这意味着定义无用的变量必然伴随着不必要的开销，所以只要可能，就要避免这种情况发生。

你不仅要将变量的定义推迟到必须使用它的时候，还要尽量推迟到可以为它提供一个初始化参数为止。这样做，不仅可以避免对不必要的对象进行构造和析构，还可以避免无意义的对缺省构造函数的调用。

#### 33: 明智地使用内联inline

编译器会优化掉无法被内联的函数，如果内联函数没被内联，每个调用内联函数的地方还是得承担函数调用的开销；如果是旧规则，还得忍受代码体积的增加，因为每个包含（或调用） f的被编译单元都有一份f的代码及其静态变量的拷贝！（更糟糕的是，每个f的拷贝以及每个f的静态变量的拷贝往往处于不同的虚拟内存页面，所以两个对f的不同拷贝进行调用有可能导致多个页面错误。）

调用内联函数的指针，会使编译器生成一个函数体

构造函数和析构函数常常不适合内联

如果函数中包含静态对象，通常要避免将它声明为内联函数

动态链接库中提供内联函数提高使用者的编译负担

实际编程时最初的原则是不要内联任何函数，除非函数确实很小很简单

#### 34: 将文件间的编译依赖性降至最低
分离的关键在于，"对类定义的依赖" 被 "对类声明的依赖" 取代了。所以，为了降低编译依赖性，我们只要知道这么一条就足够了：只要有可能，尽量让头文件不要依赖于别的文件；如果不可能，就借助于类的声明，不要依靠类的定义。其它一切方法都源于这一简单的设计思想。

#### 35: 使公有继承体现 "是一个" 的含义

#### 36: 区分接口继承和实现继承

#### 37: 决不要重新定义继承而来的非虚函数
非虚函数不是动态绑定的，继承时会带来调用混乱

#### 38: 决不要重新定义继承而来的缺省参数值
虚函数是动态绑定而缺省参数值是静态绑定的。

#### 39: 避免 "向下转换" 继承层次
如果一定要转换，使用dynamic_cast

#### 40: 通过分层来体现 "有一个" 或 "用...来实现"

#### 41: 区分继承和模板
"类的行为" 和 "类所操作的对象的类型"之间的关系
- 当对象的类型不影响类中函数的行为时，就要使用模板来生成这样一组类。
- 当对象的类型影响类中函数的行为时，就要使用继承来得到这样一组类。

#### 42: 明智地使用私有继承
如果两个类之间的继承关系为私有，编译器一般不会将派生类对象转换成基类对象。

从私有基类继承而来的成员都成为了派生类的私有成员，即使它们在基类中是protect或公有成员。

私有继承意味着 "用...来实现"。如果使类D私有继承于类B，这样做是因为你想利用类B中已经存在的某些代码，而不是因为类型B的对象和类型D的对象之间有什么概念上的关系。因而，私有继承纯粹是一种实现技术。用条款36引入的术语来说，私有继承意味着只是继承实现，接口会被忽略。如果D私有继承于B，就是说D对象在实现中用到了B对象，仅此而已。私有继承在软件 "设计" 过程中毫无意义，只是在软件 "实现" 时才有用。

尽可能地使用分层，必须时才使用私有继承。

#### 43: 明智地使用多继承
避免出现钻石结构
可以通过增加类来避免钻石结构

#### 47: 确保非局部静态对象在使用前被初始化

- 非局部动态变量
	1. 定义在全局或名字空间范围内（例如：theFileSystem和tempDir）
	2. 在一个类中被声明为static，或，
	3. 在一个文件范围被定义为static。

使用单例模式代替非局部静态对象，因为局部静态变量可以控制其初始化时机

#### 49：熟悉标准库
不要手工声明string（或标准库中其它任何部分）。相反，只用包含一个适当的头文件，如<string>。

- 容器
- 算法
- Iterator

#### 50：对C++的认识
- 和C的兼容性。很多很多C还存在，很多很多C程序员还存在。C++利用了这一基础，并建立在 ---- 我是指 "平衡在" ---- 这一基础之上。

- 效率。作为<C++>的设计者和第一个实现者，Bjarne Stroustrup从一开始就清楚地知道，要想把C程序员争取过来，就要避免转换语言会带来性能上的损失，否则他们不会对C++再看第二眼。结果，他确信<C++>在效率上可以和C匹敌 ---- 二者相差大约在5%之内。
- 和传统开发工具及环境的兼容性。各色不同的开发环境到处都是，编译器、链接器和编辑器则无处不在。从小型到大型的所有开发环境，<C++>都要轻松应对，所以带的包袱越轻越好。想移植C++？你实际上移植的只是一种语言，并利用了目标平台上现有的工具。（然而，往往也可能带来更好的实现，例如，如果链接器能被修改，使得它可以处理内联和模板在某些方面更高的要求）

- 解决真实问题的可应用性。C++没有被设计为一种完美的，纯粹的语言，不适于用它来教学生如何编程。它是设计为专业程序员的强大工具，用它来解决各种领域中的真实问题。真实世界都有些磕磕碰碰，因此，程序员们所依赖的工具如果偶尔出点问题，也不值得大惊小怪。