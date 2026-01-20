package com.example.backendgamedemo.service;

import com.example.backendgamedemo.dao.UserDao;
import com.example.backendgamedemo.entity.User;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.Date;

@Service
public class UserService {

    @Autowired
    private UserDao userDao;

    private static final String JWT_SECRET ="demo game";// 密钥 可使用更健硕的密钥
    //密码加密（使用 BCrypt）
    private final BCryptPasswordEncoder passwordEncoder = new BCryptPasswordEncoder();

    //注册用户
    public String registerUser(String username, String password, String email) {
        // 检查用户名是否已存在
        if (userDao.findUserByUsername(username) != null) {
            return "Username already exists";
        }

        // 密码加密
        String encodedPassword = passwordEncoder.encode(password);

        // 创建并保存用户
        User user = new User();
        user.setUsername(username);
        user.setPassword(encodedPassword);
        user.setEmail(email);

        userDao.save(user);
        return "Registration successful";
    }

    // 用户认证
    public String authenticateUser(String username, String password) {
        //从数据库查找用户
        User user=userDao.findUserByUsername(username);

        //验证用户密码
        if(user!=null &&user.getPassword().equals(password)){
            //生成 JWT Token
            return generateJwtToken(user);
        }
        return null;
    }
    // 生成 JWT Token
    private String generateJwtToken(User user) {
        Date now = new Date();
        Date expiryDate = new Date(now.getTime() + 86400000);  // 1天过期

        return Jwts.builder()
                .setSubject(user.getUserId().toString())  // 设置用户ID为 Token 的主题
                .setIssuedAt(now)  // 设置当前时间为创建时间
                .setExpiration(expiryDate)  // 设置过期时间
                .signWith(SignatureAlgorithm.HS512, JWT_SECRET)  // 使用密钥进行签名
                .compact();
    }

    // 验证 JWT Token
    public boolean validateToken(String token) {
        try {
            Jwts.parserBuilder()  // 使用 `parserBuilder()` 来解析 Token
                    .setSigningKey(JWT_SECRET)  // 设置签名密钥
                    .build()
                    .parseClaimsJws(token);  // 解析 Token
            return true;
        } catch (Exception e) {
            return false;  // 验证失败
        }
    }

    // 从 Token 中提取用户名
    public String getUsernameFromToken(String token) {
        Claims claims = Jwts.parserBuilder()  // 使用 `parserBuilder()`
                .setSigningKey(JWT_SECRET)  // 设置签名密钥
                .build()
                .parseClaimsJws(token)
                .getBody();  // 获取 Token 的内容（即 Claims）

        return claims.getSubject();  // 返回用户名
    }
}
