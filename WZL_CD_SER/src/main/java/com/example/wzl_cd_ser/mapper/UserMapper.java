package com.example.wzl_cd_ser.mapper;

import com.example.wzl_cd_ser.pojo.User;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface UserMapper {
    public int insertUser(User user);
    public int updateUseronline(User user);
}
