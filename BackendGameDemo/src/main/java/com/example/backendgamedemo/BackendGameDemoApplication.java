package com.example.backendgamedemo;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.data.jpa.repository.config.EnableJpaRepositories;

@SpringBootApplication(scanBasePackages = "com.example.backendgamedemo")  // 确保扫描正确的包
@EnableJpaRepositories(basePackages = "com.example.backendgamedemo.dao")  // 显式指定 JPA Repositories 的包
public class BackendGameDemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(BackendGameDemoApplication.class, args);
    }

}
