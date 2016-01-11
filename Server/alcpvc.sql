/*
Navicat MySQL Data Transfer

Source Server         : alcp
Source Server Version : 50709
Source Host           : localhost:3306
Source Database       : alcpvc

Target Server Type    : MYSQL
Target Server Version : 50709
File Encoding         : 65001

Date: 2016-01-11 03:11:33
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for commands
-- ----------------------------
DROP TABLE IF EXISTS `commands`;
CREATE TABLE `commands` (
  `commandName` varchar(50) NOT NULL,
  `commandPower` int(11) NOT NULL,
  PRIMARY KEY (`commandName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of commands
-- ----------------------------
INSERT INTO `commands` VALUES ('ban', '49');
INSERT INTO `commands` VALUES ('chperm', '99');
INSERT INTO `commands` VALUES ('kick', '24');
INSERT INTO `commands` VALUES ('mute', '24');

-- ----------------------------
-- Table structure for permissions
-- ----------------------------
DROP TABLE IF EXISTS `permissions`;
CREATE TABLE `permissions` (
  `permissionid` int(10) unsigned NOT NULL,
  `permissionname` varchar(255) NOT NULL,
  `permissionpower` int(11) NOT NULL,
  `permissioncolor` varchar(25) NOT NULL,
  PRIMARY KEY (`permissionid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of permissions
-- ----------------------------
INSERT INTO `permissions` VALUES ('1', 'Admin', '100', 'red');
INSERT INTO `permissions` VALUES ('2', 'Moderator', '50', 'blue');
INSERT INTO `permissions` VALUES ('3', 'Superuser', '25', 'green');
INSERT INTO `permissions` VALUES ('4', 'User', '1', 'black');

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `username` varchar(25) NOT NULL,
  `password` varchar(64) NOT NULL,
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
INSERT INTO `users` VALUES ('Alkor', '4e4e35f5a6d3036da9d3771a83a91021dcc98388b8d0da93a3671ddab0943cea', '1', '0', null, '0', null);
INSERT INTO `users` VALUES ('mod', '7cd894b0b0a07d181f6301dbf7edd02d47f3fc00ba96b5ba2762734465c7a0e9', '2', '0', null, '0', null);
INSERT INTO `users` VALUES ('Shikyaro', 'fae8e6b611ecbcf042078627033534eab5772f17133976315a96880ad575bbbb', '2', '0', null, '0', null);
INSERT INTO `users` VALUES ('su', '92fd310a38b5cb3c63a119c48f5267c770e658ce128350898822e79b30c0fc59', '3', '0', null, '0', null);
INSERT INTO `users` VALUES ('user', '046e0dd12c7fc24c96174c64565d2716d42cfe3aba08a76ddc0c3f7419baeb55', '4', '0', null, '0', null);

-- ----------------------------
-- Procedure structure for getPower
-- ----------------------------
DROP PROCEDURE IF EXISTS `getPower`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `getPower`(IN uName VARCHAR(25), OUT uPower INT(11))
    SQL SECURITY INVOKER
    COMMENT 'Возвращает значение permissions.permissionpower, соответствующее user.permissions'
BEGIN
	SELECT permissionpower INTO uPower
	FROM permissions
	INNER JOIN users ON permissions.permissionid = users.permissions
	WHERE users.username = uName;
END
;;
DELIMITER ;
