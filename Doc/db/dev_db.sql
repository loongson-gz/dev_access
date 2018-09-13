/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50548
Source Host           : localhost:3306
Source Database       : dev_db

Target Server Type    : MYSQL
Target Server Version : 50548
File Encoding         : 65001

Date: 2018-09-13 10:59:01
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for t_dev_info
-- ----------------------------
DROP TABLE IF EXISTS `t_dev_info`;
CREATE TABLE `t_dev_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) DEFAULT NULL,
  `dev_code` varchar(64) NOT NULL,
  `dev_kind` int(11) NOT NULL DEFAULT '0',
  `host_type` varchar(64) NOT NULL,
  `ip_addr` varchar(32) DEFAULT NULL,
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
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of t_dev_info
-- ----------------------------
INSERT INTO `t_dev_info` VALUES ('1', '电气检测', 'SBXX000022', '0', 'XINJIE_XC3_32T_E', '192.168.104.199', '502', null, null, '1', null, null, '30', '1', '');
INSERT INTO `t_dev_info` VALUES ('2', '生产线主控', 'SBXX000020', '0', 'MITSUBISHI_Q02UCCPU', '192.168.104.141', '3002', null, null, '0', null, null, '30', '9', null);
INSERT INTO `t_dev_info` VALUES ('3', '生产线主控', 'SBXX000019', '0', 'MITSUBISHI_Q03UDVCPU', '192.168.103.82', '4806', null, null, '0', null, null, '30', '0', null);
INSERT INTO `t_dev_info` VALUES ('4', '气密性检测', 'SBXX000018', '0', 'MITSUBISHI_FX3U_32M', '192.168.103.232', '502', null, null, '1', null, null, '30', '0', null);
INSERT INTO `t_dev_info` VALUES ('5', '综合检测1', 'SBXX000023', '1', 'MONDIAL', null, null, null, null, null, 'mondial_1', null, '30', '3', null);
INSERT INTO `t_dev_info` VALUES ('6', '气密性检测', 'SBXX000021', '1', 'HUAXI', null, null, 'sa', 'lc000.net', null, 'huaxi_macrodb', 'macroDB', '30', '7', null);
INSERT INTO `t_dev_info` VALUES ('7', '高压水检测', 'SBXX000027', '1', 'MICROPLAN', null, null, null, null, null, 'microplan_', null, '30', '2', null);
INSERT INTO `t_dev_info` VALUES ('8', '固扫1', 'SBXX000026', '2', 'SCANNER', null, '9999', null, null, null, null, null, '30', '6', null);
INSERT INTO `t_dev_info` VALUES ('9', '综合检测2', 'SBXX000024', '1', 'MONDIAL', null, null, null, null, null, 'mondial_2', null, '30', '4', null);
INSERT INTO `t_dev_info` VALUES ('10', '综合检测3', 'SBXX000025', '1', 'MONDIAL', null, null, null, null, null, 'mondial_3', null, '30', '5', null);
INSERT INTO `t_dev_info` VALUES ('11', '固扫2', 'SBXX000028', '2', 'SCANNER', null, '9998', null, null, null, null, null, '30', '8', null);

-- ----------------------------
-- Table structure for t_fx3u_32m
-- ----------------------------
DROP TABLE IF EXISTS `t_fx3u_32m`;
CREATE TABLE `t_fx3u_32m` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_code` varchar(64) NOT NULL,
  `node1_status` int(11) DEFAULT NULL,
  `node1_ok_amount` int(11) DEFAULT NULL,
  `node1_total_amount` int(11) DEFAULT NULL,
  `node1_ng_amount` int(11) DEFAULT NULL,
  `node2_status` int(11) DEFAULT NULL,
  `node2_ok_amount` int(11) DEFAULT NULL,
  `node2_total_amount` int(11) DEFAULT NULL,
  `node2_ng_amount` int(11) DEFAULT NULL,
  `check_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='三菱FX3U-32M 气密性检测\r\n\r\n`dev_code`                           设备编码\r\n`node1_status` 	              气密性1号工程状态\r\n`node1_ok_amount` 	气密性1号工程OK数量\r\n`node1_total_amount	气密性1号工程总测数量\r\n`node1_ng_amount` 	气密性1号工程NG数量\r\n`node2_status` 	              气密性2号工程状态\r\n`node2_ok_amount` 	气密性2号工程OK数量\r\n`node2_total_amount	气密性2号工程总测数量\r\n`node2_ng_amount` 	气密性2号工程NG数量\r\n`check_time`                        检测时间';

-- ----------------------------
-- Records of t_fx3u_32m
-- ----------------------------

-- ----------------------------
-- Table structure for t_huaxi
-- ----------------------------
DROP TABLE IF EXISTS `t_huaxi`;
CREATE TABLE `t_huaxi` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_code` varchar(64) NOT NULL,
  `product_barcode` varchar(64) NOT NULL,
  `leakage` varchar(32) DEFAULT NULL,
  `check_result` int(11) NOT NULL,
  `check_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='铧禧的气密性设备\r\n`dev_code`              设备编码\r\nproduct_barcode	 产品条码\r\nleakage	               泄漏量\r\ncheck_result	0 不合格， 1 合格\r\ncheck_time	检测时间';

-- ----------------------------
-- Records of t_huaxi
-- ----------------------------

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
-- Table structure for t_mondial
-- ----------------------------
DROP TABLE IF EXISTS `t_mondial`;
CREATE TABLE `t_mondial` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_code` varchar(64) NOT NULL,
  `test_time` varchar(32) DEFAULT NULL,
  `barcode` varchar(64) DEFAULT NULL,
  `time_used` int(11) DEFAULT NULL,
  `check_result` int(11) DEFAULT NULL,
  `check_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of t_mondial
-- ----------------------------

-- ----------------------------
-- Table structure for t_mondial_sub
-- ----------------------------
DROP TABLE IF EXISTS `t_mondial_sub`;
CREATE TABLE `t_mondial_sub` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `father` int(11) NOT NULL,
  `item` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_mondial_father` (`father`),
  CONSTRAINT `fk_mondial_father` FOREIGN KEY (`father`) REFERENCES `t_mondial` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of t_mondial_sub
-- ----------------------------

-- ----------------------------
-- Table structure for t_plc_var_addr
-- ----------------------------
DROP TABLE IF EXISTS `t_plc_var_addr`;
CREATE TABLE `t_plc_var_addr` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_id` int(11) NOT NULL,
  `addr` varchar(32) NOT NULL,
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

-- ----------------------------
-- Table structure for t_q02uccpu
-- ----------------------------
DROP TABLE IF EXISTS `t_q02uccpu`;
CREATE TABLE `t_q02uccpu` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_code` varchar(64) NOT NULL,
  `delay_stop_lock_time` float DEFAULT NULL,
  `product_amount` int(11) DEFAULT NULL,
  `product_beats` float DEFAULT NULL,
  `check_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=gbk COMMENT='三菱Q02UCCPU 生产线主控\r\n\r\n`dev_code` 		设备编码\r\n`delay_stop_lock_time` 	倍速链延时停止锁存时间\r\n`product_amount`		产品计数\r\n`product_beats` 		生产节拍\r\n`check_time`   		检测时间';

-- ----------------------------
-- Records of t_q02uccpu
-- ----------------------------
INSERT INTO `t_q02uccpu` VALUES ('1', 'ssxxxx', '25', '123', '30', '2018-09-12 23:06:05');
INSERT INTO `t_q02uccpu` VALUES ('2', 'ssxxxx', '25', '123', '30', '2018-09-12 23:07:11');
INSERT INTO `t_q02uccpu` VALUES ('3', 'ssxxxx', '25', '123', '30', '2018-09-12 23:07:25');

-- ----------------------------
-- Table structure for t_q03udvcpu
-- ----------------------------
DROP TABLE IF EXISTS `t_q03udvcpu`;
CREATE TABLE `t_q03udvcpu` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_code` varchar(64) NOT NULL,
  `input_amount` int(11) DEFAULT NULL,
  `finished_output_amount` int(11) DEFAULT NULL,
  `air_ng_amount` int(11) DEFAULT NULL,
  `repair_1_ok_amount` int(11) DEFAULT NULL,
  `test_fire_1_ng_amount` int(11) DEFAULT NULL,
  `repair_2_ok_amount` int(11) DEFAULT NULL,
  `test_fire_2_ng_amount` int(11) DEFAULT NULL,
  `repair_3_ok_amount` int(11) DEFAULT NULL,
  `product_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='三菱Q03UDVCPU 生产线主控\r\n\r\n`dev_code` 		设备编码\r\n`input_amount`		投入产量\r\n`finished_output_amount`	完成产量\r\n`air_ng_amount` 		气密检测 NG数量\r\n`repair_1_ok_amount` 	试火房1 NG数量\r\n`test_fire_1_ng_amount` 	试火房2 NG数量\r\n`repair_2_ok_amount` 	返修1 OK数量\r\n`test_fire_2_ng_amount` 	返修2 OK数量\r\n`repair_3_ok_amount` 	返修3 OK数量\r\n`product_time` 		生产时间';

-- ----------------------------
-- Records of t_q03udvcpu
-- ----------------------------

-- ----------------------------
-- Table structure for t_xc3_32t
-- ----------------------------
DROP TABLE IF EXISTS `t_xc3_32t`;
CREATE TABLE `t_xc3_32t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dev_code` varchar(64) NOT NULL,
  `barcode` varchar(64) DEFAULT NULL,
  `check_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='信捷 XC3-32T-E 电气检测\r\n\r\n`dev_code` 	设备编码\r\n`barcode`   	产品条码\r\n`check_time` 	检测时间';

-- ----------------------------
-- Records of t_xc3_32t
-- ----------------------------

-- ----------------------------
-- Table structure for t_xc3_32t_sub
-- ----------------------------
DROP TABLE IF EXISTS `t_xc3_32t_sub`;
CREATE TABLE `t_xc3_32t_sub` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `father` int(11) DEFAULT NULL,
  `item_name` varchar(32) DEFAULT NULL,
  `item_value_1` varchar(32) DEFAULT NULL,
  `item_value_2` varchar(32) DEFAULT NULL,
  `item_result` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_xc3_father` (`father`),
  CONSTRAINT `fk_xc3_father` FOREIGN KEY (`father`) REFERENCES `t_xc3_32t` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='信捷 XC3-32T-E 电气检测 细分检测项\r\n`father` 		父ID\r\n`item_name` 	检测项名称\r\n`item_value_1	检测值1\r\n`item_value_2         检测值2\r\n`item_result`          检测结果';

-- ----------------------------
-- Records of t_xc3_32t_sub
-- ----------------------------
