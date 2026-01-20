package com.example.backendgamedemo.dao;

import com.example.backendgamedemo.entity.User;
import org.springframework.data.jpa.repository.JpaRepository;

public interface UserDao extends JpaRepository<User,Long> {
    User findUserByUsername(String username);
}
