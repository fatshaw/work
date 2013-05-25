-- MySQL dump 10.13  Distrib 5.5.22, for Linux (x86_64)
--
-- Host: yx-hm-dmtest02.vm.baidu.com    Database: prodb_feed_testing
-- ------------------------------------------------------
-- Server version	5.5.21-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `prodb_feed_testing`
--

/*!40000 DROP DATABASE IF EXISTS `prodb_feed_testing`*/;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `prodb_feed_testing` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `prodb_feed_testing`;

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
  `frequency_type` smallint(5) DEFAULT NULL COMMENT '抓取频率，-1为手动上传，0–自动抓取',
  `feed_type` tinyint(3) DEFAULT '0' COMMENT 'feed类型，0非索引，1索引',
  PRIMARY KEY (`id`)
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
  CONSTRAINT `productline_feed_id` FOREIGN KEY (`feed_id`) REFERENCES `feed` (`id`)
);
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-07-16 17:57:46
