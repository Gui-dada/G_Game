package com.example.backendgamedemo.controller;

import com.example.backendgamedemo.entity.Request.LoginRequest;
import com.example.backendgamedemo.entity.Request.RegisterRequest;
import com.example.backendgamedemo.entity.Response.LoginResponse;
import com.example.backendgamedemo.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;


@RestController
@RequestMapping("/api/auth")
public class AuthController {
    @Autowired
    private UserService userService;

    @PostMapping("/login")
    public LoginResponse login(@RequestBody LoginRequest loginRequest) {
        String token = userService.authenticateUser(loginRequest.getUsername(), loginRequest.getPassword());

        if (token != null) {
            // 返回成功的响应，包含 JWT Token 和 userId
            return new LoginResponse(true, "Login successful", token, "userId123");  // 假设的 userId
        } else {
            return new LoginResponse(false, "Invalid username or password", null, null);
        }
    }

    // 注册功能
    @PostMapping("/register")
    public String register(@RequestBody RegisterRequest registerRequest) {
        String result = userService.registerUser(registerRequest.getUsername(), registerRequest.getPassword(), registerRequest.getEmail());
        return result;
    }
}
