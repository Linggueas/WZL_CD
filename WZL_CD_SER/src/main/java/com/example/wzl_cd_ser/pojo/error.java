package com.example.wzl_cd_ser.pojo;

public class error {
    int errno;

    public error() {
    }

    public error(int errno) {
        this.errno = errno;
    }

    /**
     * 获取
     * @return errno
     */
    public int getErrno() {
        return errno;
    }

    /**
     * 设置
     * @param errno
     */
    public void setErrno(int errno) {
        this.errno = errno;
    }

    public String toString() {
        return "error{errno = " + errno + "}";
    }
}
