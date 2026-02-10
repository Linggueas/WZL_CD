package com.example.wzl_cd_ser;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
@MapperScan("com.example.wzl_cd_ser.mapper")
public class WzlCdSerApplication {

    public static void main(String[] args) {
        SpringApplication.run(WzlCdSerApplication.class, args);
    }

}
