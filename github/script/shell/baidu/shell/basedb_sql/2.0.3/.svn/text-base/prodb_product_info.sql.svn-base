-- MySQL dump 10.13  Distrib 5.5.22, for Linux (x86_64)
--
-- Host: yx-hm-dmtest02.vm.baidu.com    Database: prodb_product_info
-- ------------------------------------------------------
-- Server version	5.5.21-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `prodb_product_info`
--

/*!40000 DROP DATABASE IF EXISTS `prodb_product_info`*/;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `prodb_product_info` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `prodb_product_info`;

--
-- Table structure for table `api_log`
--

DROP TABLE IF EXISTS `api_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `api_log` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'primary key',
  `apiid` bigint(20) DEFAULT NULL COMMENT 'id of api',
  `apiname` varchar(255) DEFAULT NULL COMMENT 'api name',
  `token` varchar(255) DEFAULT NULL COMMENT 'token of requester',
  `clientIP` varchar(255) DEFAULT NULL COMMENT 'ip of requester',
  `apiendtime` timestamp NULL DEFAULT NULL COMMENT 'the end time of api invoking',
  `params` varchar(1024) DEFAULT NULL COMMENT 'parameters of request',
  `servreturntype` varchar(255) DEFAULT NULL COMMENT 'return type of response',
  `servreturnstring` varchar(255) DEFAULT NULL COMMENT 'return information of response',
  `entertime` timestamp NULL DEFAULT NULL,
  `lefttime` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=445 DEFAULT CHARSET=utf8 COMMENT='record information about webservice api request and response';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `aw_updatetime_tracking`
--

DROP TABLE IF EXISTS `aw_updatetime_tracking`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `aw_updatetime_tracking` (
  `latesteffectivetime` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'the lastest finished write action time.',
  `istestdb` tinyint(4) DEFAULT NULL COMMENT 'is test db.'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='record max updatetime for all transaction';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `bw_updatetime_tracking`
--

DROP TABLE IF EXISTS `bw_updatetime_tracking`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bw_updatetime_tracking` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `latesteffectivetime` timestamp NULL DEFAULT NULL COMMENT 'the lastest successful write begining time.',
  `istestdb` tinyint(4) DEFAULT NULL COMMENT 'is test db',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=490146 DEFAULT CHARSET=utf8 COMMENT='track last update ation';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `coo8_change_product_task`
--

DROP TABLE IF EXISTS `coo8_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coo8_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `coo8_error_product`
--

DROP TABLE IF EXISTS `coo8_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coo8_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `coo8_product_attribute`
--

DROP TABLE IF EXISTS `coo8_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coo8_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `coo8_product_feed`
--

DROP TABLE IF EXISTS `coo8_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coo8_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `coo8_product_image`
--

DROP TABLE IF EXISTS `coo8_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coo8_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)),
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=762262 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `coo8_product_info`
--

DROP TABLE IF EXISTS `coo8_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coo8_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `unique_key` char(32) NOT NULL DEFAULT '',
  `feedid` bigint(20) unsigned NOT NULL,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL COMMENT 'the ad url of the product',
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `url` (`url`(255)),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `coo8_product_line_validation`
--

DROP TABLE IF EXISTS `coo8_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coo8_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(1024) DEFAULT NULL,
  `suggestion` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daoxila_change_product_task`
--

DROP TABLE IF EXISTS `daoxila_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daoxila_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daoxila_error_product`
--

DROP TABLE IF EXISTS `daoxila_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daoxila_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daoxila_product_attribute`
--

DROP TABLE IF EXISTS `daoxila_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daoxila_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daoxila_product_feed`
--

DROP TABLE IF EXISTS `daoxila_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daoxila_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daoxila_product_image`
--

DROP TABLE IF EXISTS `daoxila_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daoxila_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daoxila_product_info`
--

DROP TABLE IF EXISTS `daoxila_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daoxila_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daoxila_product_line_validation`
--

DROP TABLE IF EXISTS `daoxila_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daoxila_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dianping_change_product_task`
--

DROP TABLE IF EXISTS `dianping_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dianping_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` longtext,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11576 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dianping_error_product`
--

DROP TABLE IF EXISTS `dianping_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dianping_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB AUTO_INCREMENT=232595 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dianping_product_attribute`
--

DROP TABLE IF EXISTS `dianping_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dianping_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dianping_product_feed`
--

DROP TABLE IF EXISTS `dianping_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dianping_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dianping_product_image`
--

DROP TABLE IF EXISTS `dianping_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dianping_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=790168 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dianping_product_info`
--

DROP TABLE IF EXISTS `dianping_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dianping_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `endtime` (`endtime`),
  KEY `inactivetime` (`inactivetime`),
  KEY `availability_active` (`availability`,`active`,`deleted`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dianping_product_line_validation`
--

DROP TABLE IF EXISTS `dianping_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dianping_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB AUTO_INCREMENT=55243 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inactive_product_attribute`
--

DROP TABLE IF EXISTS `inactive_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inactive_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL,
  `record_addtime` timestamp NULL DEFAULT NULL,
  `record_updatetime` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inactive_product_image`
--

DROP TABLE IF EXISTS `inactive_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inactive_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `recordaddtime` timestamp NULL DEFAULT NULL,
  `recordupdatetime` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inactive_product_info`
--

DROP TABLE IF EXISTS `inactive_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inactive_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NULL DEFAULT NULL,
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` bit(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` varchar(1000) DEFAULT NULL,
  `active` bit(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  `record_addtime` timestamp NULL DEFAULT NULL,
  `record_updatetime` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `index_helper`
--

DROP TABLE IF EXISTS `index_helper`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `index_helper` (
  `product_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'product id',
  `product_name` varchar(255) DEFAULT NULL,
  `feed_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'feed id',
  `merchant_id` bigint(11) NOT NULL DEFAULT '0' COMMENT 'merchant id',
  `product_outerid` varchar(50) DEFAULT NULL,
  `status` enum('error','warning','normal') DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NULL DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL,
  `active` tinyint(1) DEFAULT NULL,
  `uc_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  `deleted` tinyint(1) DEFAULT NULL COMMENT 'deleted flag',
  PRIMARY KEY (`product_id`,`feed_id`,`merchant_id`),
  KEY `feed_id` (`feed_id`),
  KEY `merchant_id` (`merchant_id`),
  KEY `status` (`status`),
  KEY `start_time` (`starttime`),
  KEY `end_time` (`endtime`),
  KEY `add_time` (`addtime`),
  KEY `updatetime_active_status` (`updatetime`,`active`,`status`),
  KEY `active_status_updatetime` (`active`,`status`,`updatetime`),
  KEY `productouterid_updatetime` (`product_outerid`,`updatetime`),
  KEY `productname_updatetime` (`product_name`,`updatetime`),
  KEY `productid_updatetime` (`product_id`,`updatetime`),
  KEY `uc_id` (`uc_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `index_helper_attribute`
--

DROP TABLE IF EXISTS `index_helper_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `index_helper_attribute` (
  `iftestdb` tinyint(4) DEFAULT NULL,
  `latesteffectivetime` timestamp NULL DEFAULT NULL,
  `preparedeffectivetime` timestamp NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `index_helper_testdb`
--

DROP TABLE IF EXISTS `index_helper_testdb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `index_helper_testdb` (
  `product_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'product id',
  `product_name` varchar(255) DEFAULT NULL,
  `feed_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'feed id',
  `merchant_id` bigint(11) NOT NULL DEFAULT '0' COMMENT 'merchant id',
  `product_outerid` varchar(50) DEFAULT NULL,
  `status` enum('error','warning','normal') DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NULL DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL,
  `active` tinyint(1) DEFAULT NULL,
  `uc_id` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  `deleted` tinyint(1) DEFAULT NULL COMMENT 'deleted flag',
  PRIMARY KEY (`product_id`,`feed_id`,`merchant_id`),
  KEY `product_name` (`product_name`),
  KEY `feed_id` (`feed_id`),
  KEY `merchant_id` (`merchant_id`),
  KEY `product_outerid` (`product_outerid`),
  KEY `status` (`status`),
  KEY `start_time` (`starttime`),
  KEY `end_time` (`endtime`),
  KEY `add_time` (`addtime`),
  KEY `update_time` (`updatetime`),
  KEY `product_id` (`product_id`),
  KEY `active` (`active`),
  KEY `updatetime_active_merchant_id_product_outerid` (`updatetime`,`active`,`merchant_id`,`product_outerid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kongyue_change_product_task`
--

DROP TABLE IF EXISTS `kongyue_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kongyue_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` longtext,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kongyue_error_product`
--

DROP TABLE IF EXISTS `kongyue_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kongyue_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kongyue_product_attribute`
--

DROP TABLE IF EXISTS `kongyue_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kongyue_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kongyue_product_feed`
--

DROP TABLE IF EXISTS `kongyue_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kongyue_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kongyue_product_image`
--

DROP TABLE IF EXISTS `kongyue_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kongyue_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kongyue_product_info`
--

DROP TABLE IF EXISTS `kongyue_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kongyue_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` varchar(1000) DEFAULT NULL,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `kongyue_product_line_validation`
--

DROP TABLE IF EXISTS `kongyue_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kongyue_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lashou_change_product_task`
--

DROP TABLE IF EXISTS `lashou_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lashou_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3776 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lashou_error_product`
--

DROP TABLE IF EXISTS `lashou_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lashou_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lashou_product_attribute`
--

DROP TABLE IF EXISTS `lashou_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lashou_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lashou_product_feed`
--

DROP TABLE IF EXISTS `lashou_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lashou_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lashou_product_image`
--

DROP TABLE IF EXISTS `lashou_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lashou_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=790846 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lashou_product_info`
--

DROP TABLE IF EXISTS `lashou_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lashou_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lashou_product_line_validation`
--

DROP TABLE IF EXISTS `lashou_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lashou_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB AUTO_INCREMENT=1207774 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meilele_change_product_task`
--

DROP TABLE IF EXISTS `meilele_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meilele_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meilele_error_product`
--

DROP TABLE IF EXISTS `meilele_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meilele_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meilele_product_attribute`
--

DROP TABLE IF EXISTS `meilele_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meilele_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meilele_product_feed`
--

DROP TABLE IF EXISTS `meilele_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meilele_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meilele_product_image`
--

DROP TABLE IF EXISTS `meilele_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meilele_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=789761 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meilele_product_info`
--

DROP TABLE IF EXISTS `meilele_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meilele_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meilele_product_line_validation`
--

DROP TABLE IF EXISTS `meilele_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meilele_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB AUTO_INCREMENT=20711 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meituan_change_product_task`
--

DROP TABLE IF EXISTS `meituan_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meituan_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meituan_error_product`
--

DROP TABLE IF EXISTS `meituan_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meituan_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meituan_product_attribute`
--

DROP TABLE IF EXISTS `meituan_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meituan_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meituan_product_feed`
--

DROP TABLE IF EXISTS `meituan_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meituan_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meituan_product_image`
--

DROP TABLE IF EXISTS `meituan_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meituan_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=790298 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meituan_product_info`
--

DROP TABLE IF EXISTS `meituan_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meituan_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `meituan_product_line_validation`
--

DROP TABLE IF EXISTS `meituan_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meituan_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `merchant`
--

DROP TABLE IF EXISTS `merchant`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `merchant` (
  `id` int(11) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL COMMENT 'merchant url',
  `product_table` varchar(255) DEFAULT NULL,
  `product_attribute_table` varchar(255) DEFAULT NULL,
  `error_product_table` varchar(255) DEFAULT NULL,
  `product_image` varchar(255) DEFAULT NULL,
  `change_product_task` varchar(255) DEFAULT NULL,
  `product_feed` varchar(255) DEFAULT NULL,
  `product_line_validate` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `key` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `merchant_product`
--

DROP TABLE IF EXISTS `merchant_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `merchant_product` (
  `productid` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `merchantid` int(11) NOT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`productid`)
) ENGINE=InnoDB AUTO_INCREMENT=1236720 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `searching_sessions`
--

DROP TABLE IF EXISTS `searching_sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `searching_sessions` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `sessionstr` varchar(2048) DEFAULT NULL,
  `latestactivetime` timestamp NULL DEFAULT NULL,
  `sessionfile` varchar(512) DEFAULT NULL,
  `number` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `sessionstr` (`sessionstr`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `suoxian_change_product_task`
--

DROP TABLE IF EXISTS `suoxian_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `suoxian_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `suoxian_error_product`
--

DROP TABLE IF EXISTS `suoxian_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `suoxian_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `suoxian_product_attribute`
--

DROP TABLE IF EXISTS `suoxian_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `suoxian_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `suoxian_product_feed`
--

DROP TABLE IF EXISTS `suoxian_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `suoxian_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `suoxian_product_image`
--

DROP TABLE IF EXISTS `suoxian_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `suoxian_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `suoxian_product_info`
--

DROP TABLE IF EXISTS `suoxian_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `suoxian_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `suoxian_product_line_validation`
--

DROP TABLE IF EXISTS `suoxian_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `suoxian_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_change_product_task`
--

DROP TABLE IF EXISTS `test_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `merchantid` int(11) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_error_product`
--

DROP TABLE IF EXISTS `test_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `merchantid` int(11) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_product_attribute`
--

DROP TABLE IF EXISTS `test_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_product_attribute` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB AUTO_INCREMENT=49120 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_product_feed`
--

DROP TABLE IF EXISTS `test_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `merchantid` int(11) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`,`merchantid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_product_image`
--

DROP TABLE IF EXISTS `test_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `merchantid` int(11) DEFAULT NULL,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=12037 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_product_info`
--

DROP TABLE IF EXISTS `test_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_product_info` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `merchantid` int(11) NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NULL DEFAULT NULL,
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `dataversion` bigint(20) DEFAULT NULL,
  `inactivetime` timestamp NULL DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT NULL COMMENT 'deleted in feed',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`,`merchantid`),
  KEY `updatetime` (`updatetime`),
  KEY `uelIndex` (`url`(255),`merchantid`)
) ENGINE=InnoDB AUTO_INCREMENT=9000001 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `test_product_line_validation`
--

DROP TABLE IF EXISTS `test_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `test_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `merchantid` int(11) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`merchantid`,`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `uniqueid_creator`
--

DROP TABLE IF EXISTS `uniqueid_creator`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `uniqueid_creator` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `idtype` varchar(20) NOT NULL,
  `idvalue` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vancl_change_product_task`
--

DROP TABLE IF EXISTS `vancl_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vancl_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vancl_error_product`
--

DROP TABLE IF EXISTS `vancl_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vancl_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vancl_product_attribute`
--

DROP TABLE IF EXISTS `vancl_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vancl_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vancl_product_feed`
--

DROP TABLE IF EXISTS `vancl_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vancl_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vancl_product_image`
--

DROP TABLE IF EXISTS `vancl_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vancl_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=790353 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vancl_product_info`
--

DROP TABLE IF EXISTS `vancl_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vancl_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `vancl_product_line_validation`
--

DROP TABLE IF EXISTS `vancl_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vancl_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB AUTO_INCREMENT=37729 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weipin_change_product_task`
--

DROP TABLE IF EXISTS `weipin_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weipin_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weipin_error_product`
--

DROP TABLE IF EXISTS `weipin_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weipin_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weipin_product_attribute`
--

DROP TABLE IF EXISTS `weipin_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weipin_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weipin_product_feed`
--

DROP TABLE IF EXISTS `weipin_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weipin_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weipin_product_image`
--

DROP TABLE IF EXISTS `weipin_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weipin_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=789720 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weipin_product_info`
--

DROP TABLE IF EXISTS `weipin_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weipin_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weipin_product_line_validation`
--

DROP TABLE IF EXISTS `weipin_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `weipin_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB AUTO_INCREMENT=107339 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wowotuan_change_product_task`
--

DROP TABLE IF EXISTS `wowotuan_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wowotuan_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wowotuan_error_product`
--

DROP TABLE IF EXISTS `wowotuan_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wowotuan_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wowotuan_product_attribute`
--

DROP TABLE IF EXISTS `wowotuan_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wowotuan_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wowotuan_product_feed`
--

DROP TABLE IF EXISTS `wowotuan_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wowotuan_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wowotuan_product_image`
--

DROP TABLE IF EXISTS `wowotuan_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wowotuan_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=789046 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wowotuan_product_info`
--

DROP TABLE IF EXISTS `wowotuan_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wowotuan_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wowotuan_product_line_validation`
--

DROP TABLE IF EXISTS `wowotuan_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wowotuan_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wuba_change_product_task`
--

DROP TABLE IF EXISTS `wuba_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wuba_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wuba_error_product`
--

DROP TABLE IF EXISTS `wuba_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wuba_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wuba_product_attribute`
--

DROP TABLE IF EXISTS `wuba_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wuba_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wuba_product_feed`
--

DROP TABLE IF EXISTS `wuba_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wuba_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wuba_product_image`
--

DROP TABLE IF EXISTS `wuba_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wuba_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=790308 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wuba_product_info`
--

DROP TABLE IF EXISTS `wuba_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wuba_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wuba_product_line_validation`
--

DROP TABLE IF EXISTS `wuba_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wuba_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zouxiu_change_product_task`
--

DROP TABLE IF EXISTS `zouxiu_change_product_task`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zouxiu_change_product_task` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `tasktype` enum('delete','update','add') DEFAULT NULL,
  `status` enum('fail','done','undo') DEFAULT NULL,
  `comment` varchar(255) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zouxiu_error_product`
--

DROP TABLE IF EXISTS `zouxiu_error_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zouxiu_error_product` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `product` text,
  `error_message` text,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `productid` (`productid`)
) ENGINE=InnoDB AUTO_INCREMENT=1070 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zouxiu_product_attribute`
--

DROP TABLE IF EXISTS `zouxiu_product_attribute`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zouxiu_product_attribute` (
  `id` bigint(20) NOT NULL,
  `productid` bigint(20) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `value` text,
  `sequence` int(11) DEFAULT '0',
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `key` (`productid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zouxiu_product_feed`
--

DROP TABLE IF EXISTS `zouxiu_product_feed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zouxiu_product_feed` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `feedid` bigint(20) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `title` varchar(1000) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productfeed` (`productid`,`feedid`),
  KEY `product` (`productid`),
  KEY `feed` (`feedid`)
) ENGINE=InnoDB AUTO_INCREMENT=5929 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zouxiu_product_image`
--

DROP TABLE IF EXISTS `zouxiu_product_image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zouxiu_product_image` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) DEFAULT NULL,
  `imageUrl` varchar(1024) DEFAULT NULL,
  `imageBCSUrl` varchar(1024) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `status` enum('done','downloading','error','undo') DEFAULT 'undo',
  `failtimes` smallint(6) DEFAULT '0',
  `errormessage` varchar(255) DEFAULT NULL,
  `threadid` smallint(6) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `imageurl` (`imageUrl`(255)) USING BTREE,
  KEY `threadid_status_failtimes` (`threadid`,`status`,`failtimes`)
) ENGINE=InnoDB AUTO_INCREMENT=766113 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zouxiu_product_info`
--

DROP TABLE IF EXISTS `zouxiu_product_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zouxiu_product_info` (
  `id` bigint(20) unsigned NOT NULL,
  `feedid` bigint(20) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `title` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `url` varchar(1024) DEFAULT NULL,
  `siteurl` varchar(1024) DEFAULT NULL,
  `outerid` varchar(50) DEFAULT NULL COMMENT 'the product id in merchant system',
  `logourl` varchar(1024) DEFAULT NULL,
  `brand` varchar(40) DEFAULT NULL,
  `targeturl` varchar(1024) DEFAULT NULL,
  `category` varchar(50) DEFAULT NULL,
  `subcategory` varchar(50) DEFAULT NULL,
  `thirdcategory` varchar(50) DEFAULT NULL,
  `fourthcategory` varchar(50) DEFAULT NULL,
  `starttime` timestamp NULL DEFAULT NULL,
  `endtime` timestamp NOT NULL DEFAULT '2037-12-31 15:59:59',
  `originalprice` decimal(10,2) DEFAULT NULL,
  `discount` decimal(3,2) DEFAULT NULL,
  `price_unit` char(3) DEFAULT 'RMB',
  `services` varchar(200) DEFAULT NULL COMMENT 'special services of the product',
  `stock` bigint(20) DEFAULT '0' COMMENT 'stock of the product, <=0 is no stock',
  `city` varchar(255) DEFAULT NULL,
  `sell_range` varchar(255) DEFAULT NULL COMMENT 'the product''s area range in the city',
  `imageurl` varchar(1024) DEFAULT NULL,
  `tags` varchar(255) DEFAULT NULL,
  `major` int(10) DEFAULT NULL COMMENT 'whether is major for some product line',
  `availability` tinyint(1) DEFAULT NULL COMMENT 'to under line the product:0 under line; 1, online',
  `address` varchar(200) DEFAULT NULL COMMENT 'shop address of product',
  `bought` bigint(20) DEFAULT '0' COMMENT 'number of buy the product',
  `preferred_item` varchar(255) DEFAULT NULL,
  `signature` char(32) NOT NULL,
  `validation_status` enum('Normal','Warning') NOT NULL DEFAULT 'Normal',
  `validation_message` longtext,
  `active` tinyint(1) DEFAULT NULL COMMENT 'only logic on active product',
  `inactivetime` timestamp NULL DEFAULT NULL,
  `dataversion` bigint(20) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `deleted` tinyint(1) DEFAULT '0' COMMENT '0, is not deleted; 1, is deleted, would be moved from the table',
  `seller_name` varchar(60) DEFAULT NULL COMMENT 'the merchant''s name',
  `unique_key` char(32) NOT NULL DEFAULT '',
  `ucid` bigint(20) NOT NULL DEFAULT '0' COMMENT 'uc user id',
  PRIMARY KEY (`id`),
  UNIQUE KEY `productkey` (`unique_key`),
  KEY `feed` (`feedid`),
  KEY `updatetime` (`updatetime`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `zouxiu_product_line_validation`
--

DROP TABLE IF EXISTS `zouxiu_product_line_validation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zouxiu_product_line_validation` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `productid` bigint(20) NOT NULL,
  `product_lineid` int(11) NOT NULL,
  `status` enum('error','warning','normal','unchecked') NOT NULL DEFAULT 'unchecked',
  `validate_info` varchar(256) DEFAULT NULL,
  `suggestion` varchar(256) DEFAULT NULL,
  `addtime` timestamp NULL DEFAULT NULL,
  `updatetime` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uniquevalidation` (`productid`,`product_lineid`),
  KEY `product` (`productid`),
  KEY `product_line` (`product_lineid`)
) ENGINE=InnoDB AUTO_INCREMENT=3351 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-07-16 17:57:52
