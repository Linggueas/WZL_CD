package com.example.wzl_cd_ser.util;

public class ReqId {
    public int ID_GET_VARIFY_CODE = 1001;//获取验证码
    public int ID_REG_USER = 1002; //注册用户
    public static int ID_RESET_PWD = 1003; //重置密码
    public static int ID_LOGIN_USER = 1004; //用户登录
    public static int ID_CHAT_LOGIN = 1005; //登陆聊天服务器
    public static int ID_CHAT_LOGIN_RSP = 1006; //登陆聊天服务器回包
    public static int ID_SEARCH_USER_REQ = 1007; //用户搜索请求
    public static int ID_SEARCH_USER_RSP = 1008; //搜索用户回包
    public static int ID_ADD_FRIEND_REQ = 1009; //申请添加好友请求
    public static int ID_ADD_FRIEND_RSP = 1010; //申请添加好友回复
    public static int ID_NOTIFY_ADD_FRIEND_REQ = 1011;  //通知用户添加好友申请
    public static int ID_AUTH_FRIEND_REQ = 1013;  //认证好友请求
    public static int ID_AUTH_FRIEND_RSP = 1014;  //认证好友回复
    public static int ID_NOTIFY_AUTH_FRIEND_REQ = 1015; //通知用户认证好友申请
    public static int ID_TEXT_CHAT_MSG_REQ = 1017; //文本聊天信息请求
    public static int ID_TEXT_CHAT_MSG_RSP = 1018; //文本聊天信息回复
    public static int ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019; //通知用户文本聊天信息
    public static int ID_HTTP_ADD = 1021;//Http
    public static int ID_HEART_BEAT_REQ = 1023;      //心跳请求
    public static int ID_HEARTBEAT_RSP = 1024;       //心跳回复
}
