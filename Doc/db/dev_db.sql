/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50548
Source Host           : localhost:3306
Source Database       : dev_db

Target Server Type    : MYSQL
Target Server Version : 50548
File Encoding         : 65001

Date: 2018-09-10 10:23:36
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_dev_info
-- ----------------------------
DROP TABLE IF EXISTS `t_dev_info`;
CREATE TABLE `t_dev_info` (
  `id` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;
