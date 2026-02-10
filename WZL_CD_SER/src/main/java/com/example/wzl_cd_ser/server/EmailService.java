package com.example.wzl_cd_ser.server;

import jakarta.mail.MessagingException;
import jakarta.mail.internet.MimeMessage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.mail.javamail.MimeMessageHelper;
import org.springframework.stereotype.Service;

import java.util.Random;

@Service
public class EmailService {
    @Autowired
    private JavaMailSender mailSender;

    public String sendVerificationCode(String toEmail) throws MessagingException {
        // 生成6位数字验证码
        String verificationCode = String.format("%06d", new Random().nextInt(999999));

        // 创建邮件消息
        MimeMessage message = mailSender.createMimeMessage();
        MimeMessageHelper helper = new MimeMessageHelper(message, true);

        helper.setFrom("3537980074@qq.com");
        helper.setTo(toEmail);
        helper.setSubject("您的验证码");
        helper.setText("尊敬的用户，您的验证码是：" + verificationCode + "（5分钟内有效）", true);

        // 发送邮件
        mailSender.send(message);

        return verificationCode;
    }


}
