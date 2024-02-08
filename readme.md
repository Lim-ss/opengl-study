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

stbi_load()和glTexImage2D()的4字节对齐机制很奇怪，有时候匹配不上，如果发生越界可以加上glPixelStorei(GL_UNPACK_ALIGNMENT, 1)试试
