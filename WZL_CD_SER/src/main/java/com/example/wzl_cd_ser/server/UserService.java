package com.example.wzl_cd_ser.server;

import com.example.wzl_cd_ser.mapper.UserMapper;
import com.example.wzl_cd_ser.pojo.User;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class UserService {
    @Autowired
    UserMapper userMapper;
    public boolean insertUser(User user) {
        int num = userMapper.insertUser(user);
        return num > 0;
    }
    public boolean updateUseronline(User user){
        int num = userMapper.updateUseronline(user);
        return num > 0;
    }
}
