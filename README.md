# ipclientAI
桂电 linux/mac版 智能出校器

编译命令 g++ md5.cpp pack.cpp -o ipclient -lpthread

把带有出校器账号的文本以（账号+空格+密码）或（账号+tab+密码）为一行保存为users.txt 放入和ipclient同级目录

用./ipclient 自动读取账号上网，以及切换账号

运行时会在当前目录生成两个配置文件：Start.ini表示读取的索引，保存上次上网账号在users.txt文件中的行数
，Month.ini表示存储月份，新的一个月Start.ini会清除，从第一行开始读
