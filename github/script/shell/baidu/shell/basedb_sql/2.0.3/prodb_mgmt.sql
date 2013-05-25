-- MySQL dump 10.13  Distrib 5.5.22, for Linux (x86_64)
--
-- Host: yx-hm-dmtest02.vm.baidu.com    Database: prodb_mgmt
-- ------------------------------------------------------
-- Server version	5.5.21-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `prodb_mgmt`
--

/*!40000 DROP DATABASE IF EXISTS `prodb_mgmt`*/;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `prodb_mgmt` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `prodb_mgmt`;

--
-- Table structure for table `account_product_user`
--

DROP TABLE IF EXISTS `account_product_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_product_user` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `uc_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'ucid',
  `uc_name` varchar(255) DEFAULT '0' COMMENT 'ucname',
  `role_id` bigint(20) DEFAULT '0' COMMENT '角色id',
  `add_time` datetime NOT NULL COMMENT '记录添加时间',
  `update_time` datetime NOT NULL COMMENT '记录更新时间',
  `contact_name` varchar(255) DEFAULT '0' COMMENT 'UC账户联系人',
  `contact_email` varchar(255) DEFAULT '0' COMMENT '联系人邮件地址',
  `type_id` bigint(20) DEFAULT '0' COMMENT '账号类型id',
  `status` tinyint(3) DEFAULT '0' COMMENT '表示帐号状态，0为失效，1为生效',
  PRIMARY KEY (`uc_id`),
  UNIQUE KEY `uc_name` (`uc_name`),
  KEY `id` (`id`),
  KEY `user_role` (`role_id`),
  CONSTRAINT `user_role` FOREIGN KEY (`role_id`) REFERENCES `account_role` (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account_role`
--

DROP TABLE IF EXISTS `account_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_role` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `role_name` varchar(255) NOT NULL COMMENT '角色名称',
  `add_time` datetime NOT NULL COMMENT '记录添加时间',
  `update_time` datetime NOT NULL COMMENT '记录修改时间',
  PRIMARY KEY (`id`),
  UNIQUE KEY `role_name` (`role_name`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `account_type`
--

DROP TABLE IF EXISTS `account_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account_type` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `type_name` varchar(255) DEFAULT '0' COMMENT '账号类型名称',
  `add_time` datetime NOT NULL COMMENT '记录添加时间',
  `update_time` datetime NOT NULL COMMENT '记录修改时间',
  PRIMARY KEY (`id`),
  KEY `id` (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `alarm_mail`
--

DROP TABLE IF EXISTS `alarm_mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alarm_mail` (
  `id` bigint(20) NOT NULL,
  `subject` varchar(1024) DEFAULT NULL,
  `msg` longtext,
  `from` varchar(256) DEFAULT NULL,
  `tolist` varchar(512) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `sent` bit(1) DEFAULT NULL,
  `senttime` timestamp NULL DEFAULT NULL,
  `isneedgsm` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `base`
--

DROP TABLE IF EXISTS `base`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `base` (
  `productid` bigint(20) NOT NULL,
  `isold` tinyint(5) NOT NULL,
  `value` text NOT NULL,
  PRIMARY KEY (`productid`,`isold`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `db_log_201203`
--

DROP TABLE IF EXISTS `db_log_201203`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `db_log_201203` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `ucid` bigint(20) DEFAULT NULL COMMENT '当前进行操作的用户UCID',
  `table_name` varchar(50) NOT NULL DEFAULT '0' COMMENT '操作表格名称，含数据库前缀',
  `type` tinyint(4) NOT NULL COMMENT '0-insert, 1-update,2-delete',
  `primary_key` varchar(255) DEFAULT NULL COMMENT '记录的主键内容',
  `record_id` bigint(20) NOT NULL COMMENT '记录自增id',
  `record_content` tinytext NOT NULL COMMENT '记录最新内容，通常为对象的toString方法',
  `add_time` datetime NOT NULL COMMENT '日志添加时间，即表格操作时间',
  PRIMARY KEY (`id`),
  KEY `table_name` (`table_name`),
  KEY `add_time` (`add_time`),
  KEY `type` (`type`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `feed`
--

DROP TABLE IF EXISTS `feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `feed` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `feed_url` varchar(1024) NOT NULL COMMENT 'feed的url',
  `feed_name` varchar(256) NOT NULL COMMENT 'feed名称',
  `user_id` bigint(20) NOT NULL COMMENT '所属商家的ucid',
  `is_incremental` tinyint(4) DEFAULT NULL COMMENT '1-全量， 2-增量',
  `status` tinyint(4) DEFAULT NULL COMMENT '状态: 1-待处理，2-已暂停，3-导入成功，4-导入失败，5-处理中...',
  `error_msg` tinytext COMMENT 'Feed更新失败的错误提示信息，如xml格式错误，抓取不到xml等',
  `lastest_update_time` datetime DEFAULT '9999-12-31 23:59:59' COMMENT '最后一次更新时间',
  `successful_import_number` int(11) DEFAULT '0' COMMENT '更新结果：正常数',
  `total_import_number` int(11) DEFAULT '-1' COMMENT '更新结果：总数',
  `add_time` datetime NOT NULL COMMENT '记录添加时间',
  `update_time` datetime NOT NULL COMMENT '记录更新时间',
  `is_deleted` tinyint(2) DEFAULT '0' COMMENT '标记是否被删除',
  `feed_standard` tinyint(5) DEFAULT NULL COMMENT 'Feed规范标准，4-开放平台(商品), 3 -产品库标准，2 -掘金（普通）标准，1 -百团标准\r\n\r\n',
  `frequency_type` smallint(5) DEFAULT NULL COMMENT '抓取频率，-1为手动上传，否则0 – 每15min，1-每天，2-每周',
  `frequency_hour` int(11) DEFAULT NULL COMMENT '24小时计时，如17:00',
  `frequency_day` int(11) DEFAULT NULL COMMENT '1-7表示周一至周日',
  `feed_type` tinyint(3) DEFAULT '0' COMMENT 'feed类型，0非索引，1索引',
  PRIMARY KEY (`id`),
  KEY `feed_user` (`user_id`),
  KEY `status_index` (`status`),
  KEY `is_deleted_index` (`is_deleted`),
  KEY `frequency_type_index` (`frequency_type`),
  CONSTRAINT `feed_user` FOREIGN KEY (`user_id`) REFERENCES `account_product_user` (`uc_id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `feed_prodline`
--

DROP TABLE IF EXISTS `feed_prodline`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `feed_prodline` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `feed_id` bigint(20) DEFAULT '0' COMMENT 'feed_id',
  `product_line_id` bigint(20) DEFAULT '0' COMMENT 'product_line_id',
  `add_time` datetime NOT NULL,
  `update_time` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `productline_feed_id` (`feed_id`),
  KEY `feed_productline_id` (`product_line_id`),
  CONSTRAINT `feed_productline_id` FOREIGN KEY (`product_line_id`) REFERENCES `product_line` (`id`),
  CONSTRAINT `productline_feed_id` FOREIGN KEY (`feed_id`) REFERENCES `feed` (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `merchant`
--

DROP TABLE IF EXISTS `merchant`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `merchant` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `add_time` datetime NOT NULL COMMENT '记录添加时间',
  `update_time` datetime NOT NULL COMMENT '记录修改时间',
  `merchant_name` varchar(255) DEFAULT '0' COMMENT '广告主公司名称',
  PRIMARY KEY (`id`),
  KEY `id` (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `merchant_map`
--

DROP TABLE IF EXISTS `merchant_map`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `merchant_map` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `uc_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'ucid',
  `merchant_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'ucid属于的广告主',
  `add_time` datetime NOT NULL COMMENT '记录添加时间',
  `update_time` datetime NOT NULL COMMENT '记录修改时间',
  PRIMARY KEY (`id`),
  KEY `uc_id` (`uc_id`),
  KEY `merchant_id` (`merchant_id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `product_line`
--

DROP TABLE IF EXISTS `product_line`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `product_line` (
  `id` bigint(20) NOT NULL COMMENT '记录自增id',
  `product_line_name` varchar(255) NOT NULL DEFAULT '0' COMMENT '产品线名称',
  `validator` varchar(255) NOT NULL DEFAULT '0' COMMENT '产品线校验模块调用入口，Java反射回调',
  `asyn_validator` varchar(255) NOT NULL DEFAULT '' COMMENT 'validator for image url',
  `add_time` datetime NOT NULL,
  `update_time` datetime NOT NULL,
  `token` varchar(128) DEFAULT NULL,
  `host_addr` tinyint(5) DEFAULT '0' COMMENT '产品线bcs内外网地址设置 0：原始地址， 1：内网， 2：外网，3：内外网',
  `supported_webservice` longtext NOT NULL COMMENT '由|分隔的API列表，注意前后都要使用|',
  PRIMARY KEY (`id`),
  UNIQUE KEY `product_line_name` (`product_line_name`),
  KEY `token` (`token`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `push_tasks`
--

DROP TABLE IF EXISTS `push_tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `push_tasks` (
  `taskID` int(32) NOT NULL,
  `productlineID` int(20) NOT NULL COMMENT 'Product line ID',
  `pcAddress` varchar(255) NOT NULL COMMENT 'IP address or domain name of Pushservice Client',
  `maxUpdateTimeBoundary` timestamp NULL DEFAULT NULL COMMENT 'it''s to record the max FeedUpdateTime in products table',
  `latestbasechecktime` timestamp NULL DEFAULT '2012-06-09 16:00:00' COMMENT 'last basecheck time;',
  `registerTime` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'register Time for task',
  `taskOn` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'switch on/off this task',
  `PushDirectory` varchar(255) NOT NULL DEFAULT '' COMMENT 'record the remote client''s directory to receive the Push Files',
  `userName` varchar(255) DEFAULT NULL,
  `userPassword` varchar(255) DEFAULT NULL,
  `neededSections` varchar(1024) NOT NULL DEFAULT 'id',
  `proDBStatusFilter` set('Normal','Error','Warning','Unchecked') NOT NULL DEFAULT 'Normal,Warning' COMMENT 'Prodb level filter, it''s GE operation',
  `prolineStatusFilter` set('Normal','Error','Warning','Unchecked') NOT NULL DEFAULT 'Normal' COMMENT 'Productline level filter, it''s GE operation',
  `baselineCreateInterval` enum('daily','monthly','yearly','no') NOT NULL DEFAULT 'no',
  `indexNumber` bigint(20) NOT NULL DEFAULT '-1' COMMENT 'number used in one baseline, will be reset to 0 once new baseline is created',
  `taskName` varchar(50) NOT NULL DEFAULT 'anonymous_task',
  `ifPush` tinyint(4) NOT NULL DEFAULT '1' COMMENT 'if push or not, if no, the following pushdirectory, userName, userPassword are ignored',
  `isWOrking` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'indicate if the task is working or not',
  `minPushInterval` bigint(20) NOT NULL DEFAULT '0' COMMENT 'the minimal interval for one push task running',
  `lastpushtime` timestamp NULL DEFAULT NULL COMMENT 'last time at pushtask end',
  PRIMARY KEY (`taskID`),
  KEY `pcID` (`productlineID`),
  KEY `lastTimeBoundary` (`maxUpdateTimeBoundary`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `push_tasks_bak20120508`
--

DROP TABLE IF EXISTS `push_tasks_bak20120508`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `push_tasks_bak20120508` (
  `taskID` int(32) NOT NULL,
  `pcID` int(16) NOT NULL COMMENT 'Pushservice Client ID',
  `pcAddress` varchar(255) NOT NULL COMMENT 'IP address or domain name of Pushservice Client',
  `registerTime` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'register Time for task',
  `maxUpdateTimeBoundary` timestamp NULL DEFAULT NULL COMMENT 'it''s to record the max FeedUpdateTime in products table',
  `taskType` int(8) NOT NULL COMMENT '1 means to push once there are valid update. 2 means to push  once there are products reaching the offline condtion',
  `pushMode` int(8) NOT NULL DEFAULT '1' COMMENT '1. Push IDs file. 2. Push Products file. 3~ preserved ',
  `PushDirectory` varchar(255) NOT NULL DEFAULT '' COMMENT 'record the remote client''s directory to receive the Push Files',
  `userName` varchar(255) DEFAULT NULL,
  `userPassword` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`taskID`),
  KEY `pcID` (`pcID`),
  KEY `lastTimeBoundary` (`maxUpdateTimeBoundary`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sub_task`
--

DROP TABLE IF EXISTS `sub_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sub_task` (
  `id` bigint(20) unsigned NOT NULL,
  `task_id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `feed_name` varchar(1024) NOT NULL DEFAULT '',
  `feed_url` varchar(1024) NOT NULL DEFAULT '',
  `feed_type` enum('juejin','tuanbai','pic','hao123') NOT NULL DEFAULT 'pic',
  `start_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `end_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `feed_size` bigint(20) NOT NULL DEFAULT '0',
  `fetch_status` enum('undo','fetching','done') NOT NULL DEFAULT 'undo',
  `fetch_result` enum('success','unfetch','fail') NOT NULL DEFAULT 'unfetch',
  `file_name` varchar(255) NOT NULL DEFAULT '' COMMENT '下载后保存的feed的名称',
  `comment` longtext COMMENT 'when fetch fail, this is fetch fail reason',
  `addtime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `updatetime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `task`
--

DROP TABLE IF EXISTS `task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `feedname` varchar(1024) DEFAULT '',
  `feedurl` varchar(1024) DEFAULT '',
  `feedtype` enum('juejin','tuanbai','pic','hao123') DEFAULT NULL,
  `merchantid` int(11) NOT NULL,
  `product_line_ids` varchar(255) DEFAULT NULL,
  `spidername` varchar(255) DEFAULT NULL,
  `start_time` timestamp NULL DEFAULT NULL,
  `end_time` timestamp NULL DEFAULT NULL,
  `feed_size` bigint(20) DEFAULT NULL,
  `fetch_status` enum('undo','fetching','done') DEFAULT NULL,
  `fetch_result` enum('success','unfetch','fail') DEFAULT NULL,
  `file_name` varchar(255) DEFAULT NULL COMMENT '下载后保存的feed的名称',
  `is_test` bit(1) DEFAULT NULL COMMENT '是否是测试feed,1代表是test，0代表否',
  `isIncr` bit(1) DEFAULT NULL COMMENT '1-增量， 0-全量',
  `comment` longtext COMMENT 'when fetch fail, this is fetch fail reason',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL,
  `is_index_feed` tinyint(3) DEFAULT '0' COMMENT 'feed类型，0非索引，1索引',
  PRIMARY KEY (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-07-16 17:57:47
