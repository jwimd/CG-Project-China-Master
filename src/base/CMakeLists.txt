# Add OpenCV headers location to your include paths

aux_source_directory(. BASE_SRCS) # 添加当前目录下所有的源文件

add_library(base ${BASE_SRCS}) # 当前目录下的文件生成一个链接库

