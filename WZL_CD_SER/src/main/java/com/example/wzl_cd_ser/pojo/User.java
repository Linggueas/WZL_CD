package com.example.wzl_cd_ser.pojo;

public class User {
    private String email;
    private String password;
    private Integer online;

    public User() {
    }

    public User(String email, String password, Integer  online) {
        this.email = email;
        this.password = password;
        this.online = online;
    }

    /**
     * 获取
     * @return email
     */
    public String getEmail() {
        return email;
    }

    /**
     * 设置
     * @param email
     */
    public void setEmail(String email) {
        this.email = email;
    }

    /**
     * 获取
     * @return password
     */
    public String getPassword() {
        return password;
    }

    /**
     * 设置
     * @param password
     */
    public void setPassword(String password) {
        this.password = password;
    }

    /**
     * 获取
     * @return online
     */
    public Integer  getOnline() {
        return online;
    }

    /**
     * 设置
     * @param online
     */
    public void setOnline(Integer  online) {
        this.online = online;
    }

    public String toString() {
        return "User{email = " + email + ", password = " + password + ", online = " + online + "}";
    }
}
