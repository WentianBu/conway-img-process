# conway-img-process
This is a simple image process program written with MFC and OpenCV.

#### Main features

- Open and display pictures in different format
- change the display scale
- change the picture size
- adjust the brightness and contrast of the picture
- mean blur, median blur, Gaussian blur
- USM sharpening, fast sharpening

This project is written for Conway Tan, and finished at 2019/06. The content below is a part of the document written for Conway.

#### Dependency and build instruction

You need OpenCV library to build this project. You should edit the linker configuration in the project configuration to specify the correct OpenCV library name (e.g. `opencv_world451.lib`).

## 项目文件结构

1. 大部分文件由同名的`.h`和`.cpp`文件构成。其中`.h`为头文件，主要内容是类的声明（包含类成员变量的声明和成员函数的声明）；`.cpp`文件则主要是类的定义（类的构造函数、析构函数、成员函数等的具体逻辑实现都在这里）。
2. `pch.h`和`pch.cpp`是由Visual Studio生成的预编译头文件，用于引入不常更改的包含文件。在编写和测试项目时往往需要反复调整代码并重新编译，将不常更改的头文件（例如第三方库）在这里引入，则这部分可以只编译一次就不再更改，从而有效地缩短了编译时间。
3. `OpenCVTest.h`和`OpenCVTest.cpp`是MFC应用程序的核心，由应用程序向导自动生成，我没有改也看不懂。
4. `OpenCVTestDoc.h`和`OpenCVTestDoc.cpp`顾名思义，负责文档的打开、保存、解析等相关内容。
5. `OpenCVTestView.h`和`OpenCVTestView.cpp`主要负责View（视图）的相关功能，说白了就是文档内容的呈现和刷新之类的功能。本程序中的显示比例的调节功能很大程度上依赖这个文件中的内容。
6. `MainFrm.h`和`MainFrm.cpp`是主框架`MainFrame`的相关内容。**功能区的按钮都属于主框架。**主框架还控制整个程序界面的显示。**显示比例相关的功能的实现代码位于这两个文件中，其他功能按钮在没打开文档时会被禁用，这个逻辑的实现代码也在这两个文件中。**
7. `ChildFrm.h`和`ChildFrm.cpp`控制子框架的内容，所谓子框架就是文档打开的标签。实际上这两个文件也是自动生成的，我没有改过。
8. 以下都是对话框类的定义和功能实现：
   1. `AdjustShape`：负责调整图像大小功能（这个是指调整图像的实际大小而非显示比例）
   2. `CDlgBlur`：均值模糊
   3. `CDlgGauss`：高斯模糊
   4. `CDlgMidBlur`：中值模糊（有效去除椒盐噪声）
   5. `CDlgUSM`：USM锐化对话框和快速锐化功能
   6. `CDlgBR`：调节亮度和对比度
9. 剩下的几个文件无需手动更改。
10. `res`文件夹是资源文件（resource），里面是一些图标图像，以及对话框的设计、功能区的设计等。用VS可以打开和编辑。

## 程序控制逻辑简介

Windows应用程序是由消息驱动的。用户的各种操作都会产生对应事件消息，程序内部初始化等也会产生消息，对话框、对象之间也可以互相发送消息。本程序为各类事件和消息编写处理函数，从而起到响应用户操作的功能。

## 程序功能实现原理简介

### 基本数据结构

基本数据结构定义在`OpenCVTestDoc.h`中。可以在其中找到如下代码：

```c++
public:
	Mat img; // 图像数据
	Mat timg; // 经过转换的图像数据
	Mat simg; // 要显示的图像数据（可能经过缩放）
```

`Mat`是OpenCV库中定义的数据结构，用于存储图像数据。读入的图像数据存入`img`中。由于可能读入单通道（灰度图）或者三通道（RGB）图像，为了方便处理，故在打开文档时统一转换为四通道图像（BGRA）再存入`timg`中。这部分代码可以在`OpenCVTestDoc.cpp`中的`OnOpenDocument`函数中找到。`simg`的作用是用于显示。说白了，图像的显示比例就是使用`resize`函数对`timg`进行缩放，并存入`simg`中，显示的也是`simg`的内容。

在这个文件中还有两个重要的成员变量定义：

```c++
protected:
	CSize m_docSize;
	int scale; // 图像当前的显示比例(%)
```

以及对应的访问函数：

```c++
CSize UpdateDocSize();
CSize GetDocSize() const;
int GetScale() const;
BOOL SetScale(int scl);
```

`scale`是图像当前的显示比例，存放一个整数如50或者200。当用户先对图像进行了调节显示比例，又进行了其他调整时，程序会先利用`timg`进行调整，然后根据`scale`放缩至刚才的显示比例，再显示到屏幕上。

`docSize`是`simg`的大小（宽和高）。引入这个变量的目的主要是滚动条功能需要。当图像较大或者显示比例较大使得当前屏幕显示不下时，程序会自动创建滚动条使用户能够滚动查看图像。但是由于滚动条必须在程序绘图之前就要指定参数，因此添加这个变量。这部分代码位于`COpenCVTestView::OnInitialUpdate()`和`CopenCVTestView::OnUpdate()`中实现。

### 基本功能逻辑

这部分将按照用户正常使用的顺序进行介绍。

> 用户启动程序，显示一个空背景，没有打开任何文档或者新建任何文档。

实际上，VS应用程序向导创建的程序在启动时默认会新建一个文档。为了禁止这一行为，在`OpenCVTestApp::InitInstance()`中（位于`OpenCVTest.cpp`）添加了如下一个语句：

```c++
cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
```

此时，用户可以点击左上角的主菜单按钮或者左上角的快速访问工具栏，执行打开、新建等操作。

> 用户选择打开一张图片，或者点击新建一张空白图。

打开对话框是应用程序向导自动创建的，这里只自定义了在右边文件类型的选择时显示的内容。这个是在资源文件中的`StringTable`中定义的，不再详述。

用户选定文件后，程序会自动调用

```c++
BOOL COpenCVTestDoc::OnOpenDocument(LPCTSTR lpszPathName)
```

并将文件路径传递给这个函数。接下来只需简单调用OpenCV库函数`imread`来读取文件数据至一个`Mat`对象。**注意：`imread`函数自动根据文件的内容来解析不同格式的图片，与文件名和扩展名都无关。因此这里只用把文件路径原封不动传入即可。**

读取文件之后会对文件进行转换，调用OpenCV库函数`cvtColor`将单通道和三通道的图片转换成BGRA四通道的格式，并存入`timg`中。然后利用`Mat`对象的`clone()`方法将`timg`克隆到`simg`。*注意：如果直接使用赋值运算符，则`timg`和`simg`会引用同一个对象，改变其中的一个就会改变另一个。*接下来计算`docSize`，并且将当前缩放比例`scale`设置为100。

打开文档后，需要将内容显示出来，这里利用了GDI+库进行绘图。Windows的系统绘图逻辑是：绘图先调用`OpenCVTestView::OnEraseBkgnd()`擦除背景（也就是先用一个背景橡皮擦把之前的内容擦掉），再调用 `OpenCVTest::OnDraw()`把新的内容画上去。默认的背景刷是纯白色，这可能导致新建的空文档和部分白底绘图和背景难以分清，因此重载`OnEraseBkgnd()`函数修改背景色为灰色(128,128,128)。然后在`OnDraw()`函数中添加GDI+绘图的相关代码，将`simg`绘制到屏幕窗口的指定区域。

对于新建操作，其实就是新建了一张1024*768的白色图片，然后执行和打开类似的操作：

```c++
img = Mat(1024, 768, CV_8UC3, Scalar(255, 255, 255));
```

当然也可以弹出对话框让用户自行设置新建图片的参数，但是由于时间所限，没有实现这个功能。

> 用户调整显示比例。

程序调节显示比例的控件有三个按钮、一个滑动条和一个数字调节框。三个按钮用于快速调整至常用比例：50%，100%和200%。滑动条和编辑框用于在10%到400%之间进行调节，最小调整跨度为1%。注意：滑动条和编辑框的显示内容需要同步，并且当程序同时打开多个文档时，用户在文档间切换时，滑动条和编辑框也需要切换至不同图像的显示比例。

这部分逻辑主要在`MainFrm.cpp`中实现。首先获取当前活动文档的指针：

```c++
CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();	
```

无论调整到什么比例。都是先从`timg`（原图）克隆到`simg`，然后调用OpenCV库函数`resize`来调整`simg`的大小，然后更新`scale`的值。此外，还需要获取滑动条和编辑框控件的指针并设置它们的值：

```c++
CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
CMFCRibbonEdit* pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_SPIN1));
pSlider->SetPos(50);
pSpin->SetEditText(_T("50"));
```

完成上面这些更新内部数据的操作以后，还需要重绘视图以呈现出来：

```c++
// UpdateAllViews用于强制更新文档的所有视图，以重绘滚动条
pDoc->UpdateAllViews(NULL);
// InValidate指定当前视图已经过期失效，这样Windows会进行重绘。
pView->Invalidate();
```

> 用户点击一个功能按钮

程序中的功能按钮几乎都会弹出一个对话框Dialog与用户进行交互。对话框应该是模态的，也就是说在关闭对话框之前不能与主窗口进行交互。实现这一点，只需要创建一个对应的对话框类，然后在`MainFrm`类中为对应的按钮控件添加一个消息类型为`COMMAND`的事件处理函数即可。该函数的作用是创建对应的模态对话框：

```c++
void CMainFrame::OnGauss()
{
	// 创建高斯模糊的模态对话框
	CDlgGauss dialogGauss;
	dialogGauss.DoModal();
}
```

当然，为了防止用户在没有打开文档时使用按钮造成程序异常，需要在没有打开任何文档时禁用按钮控件。这一点可以为在`MainFrm`类中为对应的控件添加消息类型为`UPDATE_COMMAND_UI`的处理函数：

```c++
void CMainFrame::OnUpdateGauss(CCmdUI* pCmdUI)
{
	// 没有打开文档时禁用高斯模糊按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
    // 如果没有打开文档，pView就会是空指针nullptr
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}
```

> 对话框中的具体操作

尽管不同功能对话框的参数不同，但是其基本逻辑都差不多：获取参数->执行函数。参数通常采用编辑框Edit控件和数字微调Spin控件来获取。MFC可以将这两个控件绑定，并且绑定到对应类中的一个成员变量，之后只需要利用以下函数即可更新值：

```c++
UpdateData(TRUE); // 从编辑框中的值更新变量的值
// 或者
UpdateData(FALSE); // 从变量的值更新编辑框中的值
```

此外，Edit控件也可以设置数据的范围，从而自动起到有效性校验的作用。当数据无效时就会自动弹出对话框进行提醒。

部分对话框会在弹出时默认显示一些值，例如“调整大小”对话框弹出时会显示当前图片的大小。这个功能通过重载对话框类的`OnInitDialog()`方法实现。这个方法会在对话框初始化时自动调用。

当用户点击确定按钮时，就执行对应的操作。这部分的操作主要由OpenCV提供的库函数完成。只需要为确认按钮添加事件处理程序`OnOK()`，并将库函数写进去即可。**当点击确认按钮时，通过库函数对`timg`进行调整，然后克隆至`simg`，再根据设定的显示比例调用`resize`调整`simg`，最后重绘视图。**

“调整大小”对话框是一个特例。这个对话框添加了一个功能为“保持宽高比”，当用户勾选时，改变一个编辑框的值，当该编辑框失去焦点时就会自动按照比例计算另一个编辑框的值并且填充进去。这个功能主要用到了控件的`KILL_FOCUS`事件。控件失去焦点时会触发这个事件，从而调用对应的处理函数。

调整亮度和对比度对话框也是一个特例。这个对话框在用户拖动滑动条调整并松开后会自动更新视图供用户预览。如果用户满意，点击“确定”即可确认更改。若不满意，点击“取消”就能恢复原图。实现原理是先创建对象保存了原图，然后每次用户松开滑动条都会触发对应的事件从而调整`timg`和`simg`并重绘视图。如果用户不满意再拖动滑动条进行更改，则基于原图重新调整并显示。用户点击确定时，即确定更改；点击取消时，会从保存的原图对象中恢复`timg`和`simg`并重绘视图。

> 用户点击保存。

保存按钮和对话框也是VS创建的应用程序框架已经提供的。OpenCV提供了库函数`imwrite`可以写入图片，格式取决于用户提供的文件扩展名。本程序中这一点功能较为简单，无法详细选择保存类型，需要用户手动输入或者选择文件名和扩展名。

### 图像处理算法细节

这部分内容网上都能找得到具体的原理讲解和算法代码，因此只做简介。

##### 平滑

图像平滑又称图像滤波。大致原理就是根据每个像素周围一个区域的像素计算该点的值。例如，均值模糊是根据图像周围一定区域（如3*3区域）的值计算平均值，代替该点的色值，从而使图像变得模糊；高斯滤波则是根据高斯函数计算各点权重再加权平均，离得越远的点权重越低；中值滤波是取中位数来代替。当然，从数字图像处理的角度来看，这些平滑算法滤掉了图像中的高频部分，一定程度上消除了图像的噪声，也消除了图像的细节。例如对于带有椒盐噪声的图片（即图片上有很多分立的噪点），中值滤波效果极好，可以有效消除噪点。

OpenCV提供了各种各样的滤波函数，足以满足程序的需求。本程序中主要用到了`blur`函数（均值滤波）、`GaussianBlur`函数（高斯滤波）、`medianBlur`函数（中值滤波）。

##### 锐化

图像锐化可以强化图像的轮廓，使得图像看起来更加锐利。OpenCV没有直接提供锐化函数，但是可以利用已有的函数和原理很方便地写出来这个功能。

本程序中提供的USM锐化仿照PhotoShop进行。该功能需要三个参数：半径、阈值和数量。原理是先对原图进行高斯滤波，再根据阈值进行过滤，最后根据设定的数值调整锐化的强度。

快速锐化则是先构建了3*3的掩模版矩阵（锐化核）矩阵，中央为9.0，其他均为-1.0，然后利用OpenCV库函数`filter2D`对原图进行卷积，从而强化轮廓边缘。

##### 线性变换

图像的线性变换的原理其实就是$G_{i,j}=\alpha F_{i,j}+\beta$，对于运算后超出范围(0,255)的点都处理成0或者255。因此可以粗略地认为，$\beta$ 改变了图像的亮度（使图像所有像素点的色值整体增加或者减少），而$\alpha$ 改变了对比度（将图像的色值范围映射到一个更小或者更大的范围）。

本程序中，这两个参数均从-255到255进行调节，0代表未调节。显然，$\alpha$ 肯定不能是负值，因此在代码中将$- \alpha , \alpha>0$ 变成了$\frac{1}{\alpha}$ 作为真正的$\alpha$ 参与运算。此外，$\alpha$ 较小的变化就能引起图像巨大的改变，$\alpha >10  \space 或 \space  \alpha < -10$ 时图像几乎全灰或者全白，更大的值没有太大意义。因此为了增加调节的精度，程序先计算$\sqrt[4]{\alpha}$ 再将其作为$\alpha$ 代入运算，从而使调节效果更好。
