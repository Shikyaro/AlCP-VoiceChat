/*
Navicat MySQL Data Transfer

Source Server         : alcp
Source Server Version : 50709
Source Host           : localhost:3306
Source Database       : alcpvc

Target Server Type    : MYSQL
Target Server Version : 50709
File Encoding         : 65001

Date: 2015-11-19 01:12:24
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for permissions
-- ----------------------------
DROP TABLE IF EXISTS `permissions`;
CREATE TABLE `permissions` (
  `permissionid` int(10) unsigned NOT NULL,
  `permissionname` varchar(255) NOT NULL,
  `permissionpower` varchar(255) NOT NULL,
  PRIMARY KEY (`permissionid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of permissions
-- ----------------------------
INSERT INTO `permissions` VALUES ('1', 'Admin', '100');
INSERT INTO `permissions` VALUES ('2', 'Moderator', '50');
INSERT INTO `permissions` VALUES ('3', 'Superuser', '25');
INSERT INTO `permissions` VALUES ('4', 'User', '1');

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `username` varchar(25) NOT NULL,
  `password` varchar(25) NOT NULL,
  `permissions` int(11) unsigned NOT NULL,
  `muted` tinyint(1) unsigned zerofill NOT NULL DEFAULT '0',
  `muteend` datetime DEFAULT NULL,
  `banned` tinyint(1) unsigned zerofill NOT NULL DEFAULT '0',
  `banend` datetime DEFAULT NULL,
  PRIMARY KEY (`username`),
  KEY `perm` (`permissions`),
  CONSTRAINT `perm` FOREIGN KEY (`permissions`) REFERENCES `permissions` (`permissionid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of users
-- ----------------------------
INSERT INTO `users` VALUES ('Alkor', 'admin', '1', '0', null, '0', null);
