# ipclientAI
桂电 linux/mac版 智能出校器

编译命令 g++ md5.cpp pack.cpp -o ipclient -lpthread

把带有出校器账号的文本以（账号+空格+密码）为一行保存为users.txt 放入和ipclient同级目录

用./ipclient 自动读取账号上网，以及切换账号
