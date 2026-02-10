#ifndef GLOBAL_H
#define GLOBAL_H
#include"QString"
#include"QThread"
extern QString my_host;
extern QString my_port;
extern QString my_email;
extern QThread* m_tcpThread;
enum ReqId
{
    ID_GET_VARIFY_CODE = 1001,//获取验证码
    ID_REG_USER = 1002, //注册用户
    ID_RESET_PWD = 1003, //重置密码
    ID_LOGIN_USER = 1004, //用户登录
    ID_CREATE_DIR = 1005,//创建文件夹
    ID_REMOVE = 1006, //删除
    ID_REFRESH = 1007, //循环
    ID_CREATE_FILE = 1008 ,//
    ID_DOWN_FILE = 1009 ,//下载文件
    ID_NEW_NAME = 1010 //重命名
};
enum Modules {
    REGISTERMOD = 0,//注册
    RESETMOD = 1,
    LOGINMOD = 2,//登录
};

//错误代码
enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1,//JSON解析失败
    ERR_NETWORK = 2,//网络错误
};


#endif // GLOBAL_H
