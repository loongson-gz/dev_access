/*
Navicat SQL Server Data Transfer

Source Server         : 192.168.1.221
Source Server Version : 105000
Source Host           : 192.168.1.221:1433
Source Database       : macroDB
Source Schema         : dbo

Target Server Type    : SQL Server
Target Server Version : 105000
File Encoding         : 65001

Date: 2018-08-14 16:22:37
*/


-- ----------------------------
-- Table structure for ProductCheck
-- ----------------------------
DROP TABLE [dbo].[ProductCheck]
GO
CREATE TABLE [dbo].[ProductCheck] (
[ID] uniqueidentifier NULL ,
[CheckOrder] int NOT NULL IDENTITY(1,1) ,
[IP] nvarchar(50) NULL ,
[PCuser] nvarchar(255) NULL ,
[ComPort] nvarchar(50) NULL ,
[ProductBarcode] nvarchar(255) NULL ,
[ReturnResultStr] nvarchar(255) NULL ,
[CheckCode] nvarchar(255) NULL ,
[ResultFlag] nvarchar(255) NULL ,
[Leakage] nvarchar(255) NULL ,
[Checksum] nvarchar(255) NULL ,
[AutoCheckResult] bit NULL ,
[AutoCheckDate] datetime NULL ,
[UserCheckResult] bit NULL ,
[UserDate] datetime NULL ,
[UpFlag] bit NULL ,
[Second] int NULL 
)


GO
DBCC CHECKIDENT(N'[dbo].[ProductCheck]', RESEED, 8)
GO

-- ----------------------------
-- Records of ProductCheck
-- ----------------------------
SET IDENTITY_INSERT [dbo].[ProductCheck] ON
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'A24AC322-2EA5-47C7-BEAC-E2B97F33B307', N'1', N'192.168.1.221', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345612', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:08:15.523', N'0', null, N'0', N'32')
GO
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'D1761188-4774-402C-B21D-30EC9C42FD3D', N'2', N'192.168.1.221', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345612', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:08:17.520', N'0', null, N'0', N'34')
GO
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'C66181F6-29EF-4404-BC2B-22091137A97D', N'3', N'192.168.1.221', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345614', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:10:21.213', N'0', null, N'0', N'114')
GO
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'D9D23D4F-C25E-4577-80BF-D1C7EDA91EDB', N'4', N'192.168.1.221', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345614', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:10:23.210', N'0', null, N'0', N'116')
GO
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'F0AB5EC8-2C08-40D3-B038-7026F8F4B6AC', N'5', N'192.168.1.221', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345613', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:11:05.717', N'0', null, N'0', N'27')
GO
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'7185C275-AAA7-4499-BD36-F9F88B9E91EC', N'6', N'192.168.1.221', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345613', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:11:07.713', N'0', null, N'0', N'29')
GO
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'7EF76898-AD6E-4997-A23B-F27F8FF06AEA', N'7', N'192.168.1.221', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345615', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:38:58.693', N'0', null, N'0', N'34')
GO
GO
INSERT INTO [dbo].[ProductCheck] ([ID], [CheckOrder], [IP], [PCuser], [ComPort], [ProductBarcode], [ReturnResultStr], [CheckCode], [ResultFlag], [Leakage], [Checksum], [AutoCheckResult], [AutoCheckDate], [UserCheckResult], [UserDate], [UpFlag], [Second]) VALUES (N'DC13A6B9-7414-44DC-A8E3-3CDBCA50A794', N'8', N'DC13A6B9-7414-44DC-A8E3-3CDBCA50A794', N'LC-CTRL-002\LINGCHUANG', N'COM1', N'A12345615', N'#00 00 C +0999.:0C', N'#00_00', N'HH NG', N'+0999.', N'0C', N'0', N'2018-08-14 15:39:00.000', N'1', N'2018-08-14 15:40:24.750', N'0', N'36')
GO
GO
SET IDENTITY_INSERT [dbo].[ProductCheck] OFF
GO
