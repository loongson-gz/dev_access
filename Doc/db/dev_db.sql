/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50548
Source Host           : localhost:3306
Source Database       : dev_db

Target Server Type    : MYSQL
Target Server Version : 50548
File Encoding         : 65001

Date: 2018-09-12 09:48:36
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_dev_info
-- ----------------------------
DROP TABLE IF EXISTS `t_dev_info`;
CREATE TABLE `t_dev_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) DEFAULT NULL,
  `dev_code` varchar(255) DEFAULT NULL,
  `dev_kind` int(11) NOT NULL DEFAULT '0',
  `host_type` varchar(255) NOT NULL,
  `ip_addr` varchar(255) DEFAULT NULL,
  `port` int(11) DEFAULT NULL,
  `username` varchar(255) DEFAULT NULL,
  `password` varchar(255) DEFAULT NULL,
  `slave_id` int(11) unsigned DEFAULT NULL,
  `dsn_name` varchar(255) DEFAULT NULL,
  `db_name` varchar(255) DEFAULT NULL,
  `poll_interval` int(11) DEFAULT '30',
  `line_number` int(11) DEFAULT NULL,
  `param` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of t_dev_info
-- ----------------------------
INSERT INTO `t_dev_info` VALUES ('1', '电气检测', 'SBXX000022', '0', 'XINJIE_XC3_32T_E', '192.168.104.199', '502', null, null, '1', null, null, '30', null, '');
INSERT INTO `t_dev_info` VALUES ('2', '生产线主控', 'SBXX000020', '0', 'MITSUBISHI_Q02UCCPU', '192.168.104.141', '3002', null, null, '0', null, null, '30', null, null);
INSERT INTO `t_dev_info` VALUES ('3', '生产线主控', 'SBXX000019', '0', 'MITSUBISHI_Q03UDVCPU', '192.168.103.82', '4806', null, null, '0', null, null, '30', null, null);
INSERT INTO `t_dev_info` VALUES ('4', '气密性检测', 'SBXX000018', '0', 'MITSUBISHI_FX3U_32M', '192.168.103.232', '502', null, null, '1', null, null, '30', null, null);
INSERT INTO `t_dev_info` VALUES ('5', '综合检测', 'SBXX000023', '1', 'MONDIAL', null, null, null, null, null, 'mondial_1', null, '30', null, null);
INSERT INTO `t_dev_info` VALUES ('6', '气密性检测', 'SBXX000021', '1', 'HUAXI', null, null, 'sa', 'lc000.net', null, 'huaxi_macrodb', 'macroDB', '30', null, null);
INSERT INTO `t_dev_info` VALUES ('7', '高压水检测', 'SBXX000025', '1', 'MICROPLAN', null, null, null, null, null, 'microplan_', null, '30', null, null);
INSERT INTO `t_dev_info` VALUES ('8', '固扫', 'SBXX000024', '2', 'SCANNER', null, '9999', null, null, null, null, null, '30', null, null);

-- ----------------------------
-- Table structure for t_mes_info
-- ----------------------------
DROP TABLE IF EXISTS `t_mes_info`;
CREATE TABLE `t_mes_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) DEFAULT NULL,
  `password` varchar(255) DEFAULT NULL,
  `db_name` varchar(255) DEFAULT NULL,
  `dsn_name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of t_mes_info
-- ----------------------------
INSERT INTO `t_mes_info` VALUES ('1', 'jmuser', 'qwer1234', 'wjlmesst', 'wjl_local_mes');

-- ----------------------------
-- Table structure for t_plc_var_addr
-- ----------------------------
DROP TABLE IF EXISTS `t_plc_var_addr`;
CREATE TABLE `t_plc_var_addr` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_id` int(11) NOT NULL,
  `addr` varchar(255) NOT NULL,
  `in_out` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `fk_dev_id` (`dev_id`),
  CONSTRAINT `fk_dev_id` FOREIGN KEY (`dev_id`) REFERENCES `t_dev_info` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=gbk COMMENT='in_out  读写控制，0：读 1：写';

-- ----------------------------
-- Records of t_plc_var_addr
-- ----------------------------
INSERT INTO `t_plc_var_addr` VALUES ('1', '1', '100', '0');
INSERT INTO `t_plc_var_addr` VALUES ('2', '1', '200', '0');
INSERT INTO `t_plc_var_addr` VALUES ('3', '1', '300', '0');
INSERT INTO `t_plc_var_addr` VALUES ('4', '1', '400', '0');
INSERT INTO `t_plc_var_addr` VALUES ('5', '1', '500', '0');
INSERT INTO `t_plc_var_addr` VALUES ('6', '2', 'Y181', '0');
INSERT INTO `t_plc_var_addr` VALUES ('7', '2', 'D3499', '0');
INSERT INTO `t_plc_var_addr` VALUES ('8', '2', 'D7001', '0');
INSERT INTO `t_plc_var_addr` VALUES ('9', '2', 'D3501', '0');
INSERT INTO `t_plc_var_addr` VALUES ('10', '2', 'D511', '1');
INSERT INTO `t_plc_var_addr` VALUES ('11', '2', 'D530', '1');
INSERT INTO `t_plc_var_addr` VALUES ('12', '3', 'D1200', '0');
INSERT INTO `t_plc_var_addr` VALUES ('13', '3', 'D1202', '0');
INSERT INTO `t_plc_var_addr` VALUES ('14', '3', 'D1204', '0');
INSERT INTO `t_plc_var_addr` VALUES ('15', '3', 'D1206', '0');
INSERT INTO `t_plc_var_addr` VALUES ('16', '3', 'D1208', '0');
INSERT INTO `t_plc_var_addr` VALUES ('17', '3', 'D1210', '0');
INSERT INTO `t_plc_var_addr` VALUES ('18', '3', 'D1212', '0');
INSERT INTO `t_plc_var_addr` VALUES ('19', '3', 'D1214', '0');
INSERT INTO `t_plc_var_addr` VALUES ('20', '3', 'D1220', '0');
INSERT INTO `t_plc_var_addr` VALUES ('21', '4', '2020', '0');
INSERT INTO `t_plc_var_addr` VALUES ('22', '4', '3000', '0');
INSERT INTO `t_plc_var_addr` VALUES ('23', '4', '3004', '0');
INSERT INTO `t_plc_var_addr` VALUES ('24', '4', '3002', '0');
INSERT INTO `t_plc_var_addr` VALUES ('25', '4', '2022', '0');
INSERT INTO `t_plc_var_addr` VALUES ('26', '4', '3100', '0');
INSERT INTO `t_plc_var_addr` VALUES ('27', '4', '3104', '0');
INSERT INTO `t_plc_var_addr` VALUES ('28', '4', '3102', '0');
