#### 该仓库用于学习opengl

##### 学习资料：

learnopengl网站 https://learnopengl-cn.github.io/

The Cherno  Opengl 双语b站搬运 https://www.bilibili.com/video/BV1Ni4y1o7Au/

b站 halfpeople imgui https://www.bilibili.com/video/BV1ne4y1t7ED

##### 引入的库

glfw [glfw/glfw: A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input (github.com)](https://github.com/glfw/glfw)

glew [nigels-com/glew: The OpenGL Extension Wrangler Library (github.com)](https://github.com/nigels-com/glew)

stb_image [nothings/stb: stb single-file public domain libraries for C/C++ (github.com)](https://github.com/nothings/stb)

imgui [ocornut/imgui: Dear ImGui: Bloat-free Graphical User interface for C++ with minimal dependencies (github.com)](https://github.com/ocornut/imgui)

glm [g-truc/glm: OpenGL Mathematics (GLM) (github.com)](https://github.com/g-truc/glm)

##### 备注

glew使用静态链接时需要加上预编译宏__GLEW_STATIC__

glm中有一个glm.cppm文件编译通过不了，删掉即可

imgui默认用gl3w而不是glew，但是似乎imgui里面以及包含gl3w了（不确定），在其他地方照样用glew是没问题的

stbi_load()和glTexImage2D()的4字节对齐机制很奇怪，有时候匹配不上，如果发生越界可以加上glPixelStorei(GL_UNPACK_ALIGNMENT, 1)试试。另外，发现有些不标准jpg格式的图片加载失败，可以用python先转一下。

glfwSetCursorPosCallback(window, CursorPosCallback)这个用于设置光标位置回调函数的函数必须在imgui初始化之前调用，否则如果先初始化imgui再调用这个函数，就会覆盖掉imgui内部的鼠标处理逻辑，导致imgui以一定的形式失效，即使再调用glfwSetCursorPosCallback(window, nullptr)来取消掉自己的光标回调函数也不能修复。另外，一般在处理鼠标事件时，都应该先询问imgui是否需要处理鼠标逻辑，避免冲突，为了能够在不同的test生效时调用不同的回调函数，我在程序里又加了一层，由外层callback调用currentTest中的callback（用了动态绑定）。

~~~c++
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
    {
        currentTest->CursorPosCallback(window, xpos, ypos);
    }
}
~~~

先把所有纹理加载完再进行纹理槽的绑定，因为加载纹理的时候在调用glBindTexture(GL_TEXTURE_2D, ID)之前不会调用GLCall(glActiveTexture(GL_TEXTURE0 + slot))，所以会把上一个绑定的纹理顶掉。
