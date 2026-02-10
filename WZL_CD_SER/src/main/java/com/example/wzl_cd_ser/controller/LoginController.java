package com.example.wzl_cd_ser.controller;

import com.example.wzl_cd_ser.pojo.User;
import com.example.wzl_cd_ser.pojo.error;
import com.example.wzl_cd_ser.server.UserService;
import com.example.wzl_cd_ser.util.ErrorCodes;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class LoginController {
    @Autowired
    UserService userService;
    @PostMapping("/get_login")
    public error get_login(@RequestBody User user) {
        System.out.println("get_login");
        System.out.println(user);
        boolean _error = userService.updateUseronline(user);
        error __error = new error();
        if(_error){
            __error.setErrno(ErrorCodes.SUCCESS);
        }else{
            __error.setErrno(ErrorCodes.ERROR);
        }
        return __error;
    }
}
