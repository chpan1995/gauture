/*
 Navicat Premium Data Transfer

 Source Server         : chpan
 Source Server Type    : MySQL
 Source Server Version : 80041
 Source Host           : 127.0.0.1:3306
 Source Schema         : GzLabel

 Target Server Type    : MySQL
 Target Server Version : 80041
 File Encoding         : 65001

 Date: 25/02/2025 15:17:36
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for graintags
-- ----------------------------
DROP TABLE IF EXISTS `graintags`;
CREATE TABLE `graintags` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `firstOrder` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL COMMENT '顶级命名',
  `chrildobj` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci,
  `deep` int NOT NULL COMMENT '标签属性 1 对应 一级标签，2 对应2级标签',
  `typeid` int NOT NULL COMMENT '对应graintypeid',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Table structure for graintype
-- ----------------------------
DROP TABLE IF EXISTS `graintype`;
CREATE TABLE `graintype` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Table structure for labtask
-- ----------------------------
DROP TABLE IF EXISTS `labtask`;
CREATE TABLE `labtask` (
  `taskid` int unsigned NOT NULL AUTO_INCREMENT COMMENT '任务id',
  `userid` int NOT NULL COMMENT '用户id',
  `imgcnt` int unsigned NOT NULL COMMENT '总的图片数量',
  `uploadcnt` int unsigned NOT NULL DEFAULT '0' COMMENT '已上传图片数量',
  `taskname` varchar(255) NOT NULL COMMENT '任务名称',
  `status` bit(1) NOT NULL DEFAULT b'0' COMMENT '完成状态',
  `createtime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '获取任务的时间',
  PRIMARY KEY (`taskid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Table structure for labtaskhistory
-- ----------------------------
DROP TABLE IF EXISTS `labtaskhistory`;
CREATE TABLE `labtaskhistory` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `taskid` int NOT NULL,
  `tags` varchar(1000) NOT NULL,
  `imgname` varchar(400) NOT NULL,
  `uploadtime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=45 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Table structure for labtaskinfo
-- ----------------------------
DROP TABLE IF EXISTS `labtaskinfo`;
CREATE TABLE `labtaskinfo` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `taskid` int NOT NULL,
  `tags` varchar(1000) NOT NULL DEFAULT '',
  `imgname` varchar(400) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`id`),
  KEY `imgname` (`imgname`)
) ENGINE=InnoDB AUTO_INCREMENT=5002 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL COMMENT '用户名',
  `password` varchar(255) NOT NULL COMMENT '用户密码',
  `nickname` varchar(255) NOT NULL COMMENT '中文名',
  `createtime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '创建时间',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

SET FOREIGN_KEY_CHECKS = 1;
