package com.example.wzl_cd_ser.controller;

import com.example.wzl_cd_ser.pojo.User;
import com.example.wzl_cd_ser.pojo.Verification;
import com.example.wzl_cd_ser.pojo.error;
import com.example.wzl_cd_ser.server.EmailService;
import com.example.wzl_cd_ser.server.UserService;
import com.example.wzl_cd_ser.util.ErrorCodes;
import jakarta.mail.MessagingException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class RegisterController {
    @Autowired
    EmailService emailService;
    @PostMapping("/get_varifycode")
    public Verification getVariableCode(@RequestBody User user) throws MessagingException {
        System.out.println("get_varifycode");
        System.out.println(user);
        String email = user.getEmail();
        String var = emailService.sendVerificationCode(email);
        Verification verification = new Verification();
        verification.setVar(var);
        verification.setError(ErrorCodes.SUCCESS);
        return verification;
    }
    @Autowired
    UserService userService;
    @PostMapping("get_registration")
    public error get_registration(@RequestBody User user){
        System.out.println("get_registration");
        System.out.println(user);
        user.setOnline(0);
        boolean _error =  userService.insertUser(user);
        error __error = new error();
        if(_error){
            __error.setErrno(ErrorCodes.SUCCESS);
        }else{
            __error.setErrno(ErrorCodes.ERROR);
        }

        return __error;
    }

}
