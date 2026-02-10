package com.example.wzl_cd_ser.pojo;

public class Verification {
    int error;
    String var;

    public Verification() {
    }

    public Verification(int error, String var) {
        this.error = error;
        this.var = var;
    }

    /**
     * 获取
     * @return error
     */
    public int getError() {
        return error;
    }

    /**
     * 设置
     * @param error
     */
    public void setError(int error) {
        this.error = error;
    }

    /**
     * 获取
     * @return var
     */
    public String getVar() {
        return var;
    }

    /**
     * 设置
     * @param var
     */
    public void setVar(String var) {
        this.var = var;
    }

    public String toString() {
        return "Verification{error = " + error + ", var = " + var + "}";
    }
}
