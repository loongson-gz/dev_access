/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50548
Source Host           : localhost:3306
Source Database       : results

Target Server Type    : MYSQL
Target Server Version : 50548
File Encoding         : 65001

Date: 2018-10-17 09:10:28
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for check_result
-- ----------------------------
DROP TABLE IF EXISTS `check_result`;
CREATE TABLE `check_result` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `PRODUCT_SN` varchar(64) NOT NULL,
  `CHECK_RESULT` varchar(32) NOT NULL,
  `CHECK_TIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=gbk COMMENT='PRODUCT_SN        产品条码\r\nCHECK_RESULT      检测结果  NG ：不合格\r\nCHECK_TIME         检测时间';

-- ----------------------------
-- Table structure for defect
-- ----------------------------
DROP TABLE IF EXISTS `defect`;
CREATE TABLE `defect` (
  `ID` char(38) NOT NULL,
  `TEST_ID` char(38) NOT NULL,
  `DEFECT_CODE` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `DEFECT_FK` (`TEST_ID`),
  CONSTRAINT `DEFECT_FK` FOREIGN KEY (`TEST_ID`) REFERENCES `test` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

-- ----------------------------
-- Table structure for measure
-- ----------------------------
DROP TABLE IF EXISTS `measure`;
CREATE TABLE `measure` (
  `ID` char(38) NOT NULL,
  `STEP_ID` char(38) NOT NULL,
  `MEASURE_NAME` varchar(255) DEFAULT NULL,
  `DATA` double DEFAULT NULL,
  `COMPARISON` varchar(30) DEFAULT NULL,
  `LOW_LIMIT` double DEFAULT NULL,
  `HIGH_LIMIT` double DEFAULT NULL,
  `STATUS` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `MEASURE_FK` (`STEP_ID`),
  CONSTRAINT `MEASURE_FK` FOREIGN KEY (`STEP_ID`) REFERENCES `step` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='    ID                      CHAR (38)  PRIMARY KEY, 对应测试数据搜索的识别号\r\n    STEP_ID                      CHAR (38)  NOT NULL, 测试数据所对应的单步测试表中搜索码，不能为空，用于追溯属于哪一步骤\r\n    MEASURE_NAME            VARCHAR (255), 测试数据的名称\r\n    DATA                DOUBLE, 测试数据结果\r\n    COMPARISON            VARCHAR (30),是否有比较判定，有几种方式，大于，小于，介于区间等\r\n    LOW_LIMIT            DOUBLE,  工艺参数下限\r\n    HIGH_LIMIT            DOUBLE, 工艺参数上限\r\n    STATUS                VARCHAR (50), 数据的状态（判定的状态）\r\n';

-- ----------------------------
-- Table structure for step
-- ----------------------------
DROP TABLE IF EXISTS `step`;
CREATE TABLE `step` (
  `ID` char(38) NOT NULL,
  `TEST_ID` char(38) NOT NULL,
  `EXEC_ORDER` int(11) DEFAULT NULL,
  `STEP_NAME` varchar(100) DEFAULT NULL,
  `STEP_TYPE` varchar(50) DEFAULT NULL,
  `EXEC_TIME` double DEFAULT NULL,
  `REPORT_TEXT` varchar(255) DEFAULT NULL,
  `STEP_ERROR` varchar(2500) DEFAULT NULL,
  `STEP_RESULT` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `STEP_FK` (`TEST_ID`),
  CONSTRAINT `STEP_FK` FOREIGN KEY (`TEST_ID`) REFERENCES `test` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='    ID                CHAR (38)  PRIMARY KEY, 对应step（单步测试）搜索的识别号\r\n    TEST_ID                CHAR (38)  NOT NULL, 单步测试所对应的Test表中搜索码，不能为空，用于追溯哪一个产品的测试\r\n    EXEC_ORDER            INT,测试序号，即第几步\r\n    STEP_NAME            VARCHAR (100),测试单步的名称\r\n    STEP_TYPE            VARCHAR (50),测试的类型\r\n    EXEC_TIME            DOUBLE, 单步测试所用时间\r\n    REPORT_TEXT            VARCHAR (255),报告文本\r\n    STEP_ERROR            VARCHAR (2500),单步测试中遇到的错误\r\n    STEP_RESULT            VARCHAR (50),单步测试的最终结果\r\n';

-- ----------------------------
-- Table structure for test
-- ----------------------------
DROP TABLE IF EXISTS `test`;
CREATE TABLE `test` (
  `ID` char(38) NOT NULL,
  `MODEL` varchar(50) DEFAULT NULL,
  `SERIAL_NUMBER` varchar(50) DEFAULT NULL,
  `STATION` varchar(50) DEFAULT NULL,
  `OPERATOR` varchar(50) DEFAULT NULL,
  `SEQUENCE` varchar(50) DEFAULT NULL,
  `START_DATE_TIME` datetime DEFAULT NULL,
  `TEST_TIME` double DEFAULT NULL,
  `TEST_RESULT` varchar(30) DEFAULT NULL,
  `TRANSFER` varchar(3) DEFAULT NULL,
  KEY `ID` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk COMMENT='ID               CHAR (38)  PRIMARY KEY,  Test表格对应的搜索识别号\r\nMODEL                VARCHAR (50), Model为被测产品型号代码\r\nSERIAL_NUMBER                    VARCHAR (50), 被测产品序列号\r\nSTATION                            VARCHAR (50), 检测台的工位或者名称\r\nOPERATOR                    VARCHAR (50), 作业员信息\r\nSEQUENCE            VARCHAR (50), 使用的测试程序（工艺流程）\r\nSTART_DATE_TIME            DATETIME, 测试日期\r\nTEST_TIME            DOUBLE, 测试总时间\r\nTEST_RESULT            VARCHAR (30),最终测试结果\r\nTRANSFER            VARCHAR (3)是否成功传送到外部数据库标识\r\n';
