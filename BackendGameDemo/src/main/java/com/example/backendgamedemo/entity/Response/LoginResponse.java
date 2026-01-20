package com.example.backendgamedemo.entity.Response;

public class LoginResponse {
    private boolean success;
    private String message;
    private String token;
    private String userId;

    // Constructor, getters, and setters
    public LoginResponse(boolean success, String message, String token, String userId) {
        this.success = success;
        this.message = message;
        this.token = token;
        this.userId = userId;
    }

    public boolean isSuccess() {
        return success;
    }

    public void setSuccess(boolean success) {
        this.success = success;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }
}
