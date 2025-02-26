/*
 Navicat Premium Data Transfer

 Source Server         : 47.99.163.134
 Source Server Type    : MySQL
 Source Server Version : 80040
 Source Host           : 47.99.163.134:3316
 Source Schema         : GzLabel

 Target Server Type    : MySQL
 Target Server Version : 80040
 File Encoding         : 65001

 Date: 25/02/2025 16:19:24
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
-- Records of graintags
-- ----------------------------
BEGIN;
COMMIT;

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
-- Records of graintype
-- ----------------------------
BEGIN;
COMMIT;

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
-- Records of labtask
-- ----------------------------
BEGIN;
COMMIT;

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
-- Records of labtaskhistory
-- ----------------------------
BEGIN;
COMMIT;

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
-- Records of labtaskinfo
-- ----------------------------
BEGIN;
COMMIT;

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
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of user
-- ----------------------------
BEGIN;
INSERT INTO `user` VALUES (13, 'admin', '123456', '管理员', '2025-02-25 08:19:09');
COMMIT;

SET FOREIGN_KEY_CHECKS = 1;
