use prodb_mgmt;
-- MySQL dump 10.13  Distrib 5.5.22, for Linux (x86_64)
--
-- Host: 10.36.55.57    Database: prodb_mgmt
-- ------------------------------------------------------
-- Server version	5.1.45-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `account_product_user`
--
-- WHERE:  role_id=1

INSERT INTO `account_product_user` VALUES (1,5085158,'prodbadmin',1,'2012-03-25 11:25:02','2012-03-25 11:25:02','孔越','kongyue@baidu.com',1,1,1);
INSERT INTO `account_product_user` VALUES (18,5360165,'prodbmanager',1,'2012-07-03 10:00:00','2012-07-03 10:00:00','孔越','kongyue@baidu.com',1,1,1);
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-08-22 11:06:29
-- MySQL dump 10.13  Distrib 5.5.22, for Linux (x86_64)
--
-- Host: 10.36.55.57    Database: prodb_mgmt
-- ------------------------------------------------------
-- Server version	5.1.45-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `product_line`
--

INSERT INTO `product_line` VALUES (1,'凤巢商品推广-闪投','com.baidu.adrc.pic.validation.prodline.validator.PAValidator','','2012-03-24 22:24:41','2012-03-24 22:24:41','606b45be-bf56-4d2b-a00e-45dbaf150a68',0,'|GetSetting|QueryProductByFeedID|QueryProductByUserID|QueryProductByBindingUserID|QueryProductIDByFeedID|QueryProductIDByUserID|QueryProductIDByBindingUserID|QueryProductByFeedIDU|QueryProductByUserIDU|QueryProductByBindingUserIDU|UploadLaunchSuggestion|QueryUserIDGroup|GetUAPIInfo|');
INSERT INTO `product_line` VALUES (2,'掘金（普通）','','','2012-03-24 22:24:41','2012-03-24 22:24:41','fb7d5244-b612-4138-815d-b17b7cafeb57',0,'|GetSetting|QueryProductByFeedID|QueryProductByUserID|QueryProductByBindingUserID|QueryProductIDByFeedID|QueryProductIDByUserID|QueryProductIDByBindingUserID|QueryProductByFeedIDU|QueryProductByUserIDU|QueryProductByBindingUserIDU|UploadLaunchSuggestion|QueryUserIDGroup|GetUAPIInfo|');
INSERT INTO `product_line` VALUES (3,'掘金自动化','','','2012-03-24 22:24:41','2012-03-24 22:24:41','e3ec9d72-4498-4a71-9afe-07a02bee0970',0,'|GetSetting|QueryProductByFeedID|QueryProductByUserID|QueryProductByBindingUserID|QueryProductIDByFeedID|QueryProductIDByUserID|QueryProductIDByBindingUserID|QueryProductByFeedIDU|QueryProductByUserIDU|QueryProductByBindingUserIDU|UploadLaunchSuggestion|QueryUserIDGroup|GetUAPIInfo|');
INSERT INTO `product_line` VALUES (4,'百度团购','','','2012-03-24 22:24:41','2012-03-24 22:24:41','4a830822-eb47-42f3-84d1-d0039ca0e6d5',0,'|GetSetting|QueryProductByFeedID|QueryProductByUserID|QueryProductByBindingUserID|QueryProductIDByFeedID|QueryProductIDByUserID|QueryProductIDByBindingUserID|QueryProductByFeedIDU|QueryProductByUserIDU|QueryProductByBindingUserIDU|UploadLaunchSuggestion|QueryUserIDGroup|GetUAPIInfo|');
INSERT INTO `product_line` VALUES (5,'网盟智能创意','com.baidu.adrc.pic.validation.prodline.validator.NetUnionValidator','','2012-05-28 15:44:12','2012-05-28 15:44:12','bc3457e3-2e99-4fba-902a-77090c9f6e9b',3,'|GetSetting|QueryProductByFeedID|QueryProductByUserID|QueryProductByBindingUserID|QueryProductIDByFeedID|QueryProductIDByUserID|QueryProductIDByBindingUserID|QueryProductByFeedIDU|QueryProductByUserIDU|QueryProductByBindingUserIDU|UploadLaunchSuggestion|QueryUserIDGroup|GetUAPIInfo|');
INSERT INTO `product_line` VALUES (6,'凤巢商品推广-先知','','','2012-06-15 15:00:00','2012-06-15 15:00:00','772dd7ea-5cff-4227-9806-c5eed4a044fd',3,'|GetSetting|QueryProductByFeedID|QueryProductByUserID|QueryProductByBindingUserID|QueryProductIDByFeedID|QueryProductIDByUserID|QueryProductIDByBindingUserID|QueryProductByFeedIDU|QueryProductByUserIDU|QueryProductByBindingUserIDU|UploadLaunchSuggestion|QueryUserIDGroup|GetUAPIInfo|');
INSERT INTO `product_line` VALUES (7,'品牌专区电商样式','','','2012-06-15 15:00:00','2012-06-15 15:00:00','140c0f90-ce06-4c07-af09-8813b3d07fb7',0,'|GetSetting|QueryProductByFeedID|QueryProductByUserID|QueryProductByBindingUserID|QueryProductIDByFeedID|QueryProductIDByUserID|QueryProductIDByBindingUserID|QueryProductByFeedIDU|QueryProductByUserIDU|QueryProductByBindingUserIDU|UploadLaunchSuggestion|QueryUserIDGroup|GetUAPIInfo|');

--
-- Dumping data for table `account_role`
--

INSERT INTO `account_role` VALUES (1,'管理员','2012-03-25 11:25:02','2012-03-25 11:25:02');
INSERT INTO `account_role` VALUES (2,'普通用户','2012-03-25 11:25:02','2012-03-25 11:25:02');

--
-- Dumping data for table `account_type`
--

INSERT INTO `account_type` VALUES (1,'产品库账号','2012-03-25 11:03:53','2012-03-25 11:03:53');
INSERT INTO `account_type` VALUES (2,'锦囊','2012-03-25 11:05:33','2012-03-25 11:05:33');
INSERT INTO `account_type` VALUES (3,'凤巢','2012-03-30 18:33:29','2012-03-30 18:33:30');
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-08-22 11:06:29
-- MySQL dump 10.13  Distrib 5.5.22, for Linux (x86_64)
--
-- Host: 10.36.55.57    Database: prodb_mgmt
-- ------------------------------------------------------
-- Server version	5.1.45-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Dumping data for table `merchant`
--

INSERT INTO `merchant` VALUES (-1,'2012-02-23 17:21:04','2012-02-23 19:55:43','test','test',NULL,'test_product_info','test_product_attribute','test_error_product','test_product_image','test_change_product_task','test_product_feed','test_product_line_validation','TEST');
INSERT INTO `merchant` VALUES (1,'2012-03-25 11:25:02','2012-03-25 11:25:02','管理员商家','管理员商家',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'ADMIN');
INSERT INTO `merchant` VALUES (2,'2012-03-25 11:25:02','2012-03-25 11:25:02','dianping','dianping',NULL,'dianping_product_info','dianping_product_attribute','dianping_error_product','dianping_product_image','dianping_change_product_task','dianping_product_feed','dianping_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (3,'2012-03-30 17:50:02','2012-03-30 17:50:02','lashou','lashou',NULL,'lashou_product_info','lashou_product_attribute','lashou_error_product','lashou_product_image','lashou_change_product_task','lashou_product_feed','lashou_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (4,'2012-03-30 17:50:28','2012-03-30 17:50:29','daoxila','daoxila',NULL,'daoxila_product_info','daoxila_product_attribute','daoxila_error_product','daoxila_product_image','daoxila_change_product_task','daoxila_product_feed','daoxila_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (5,'2012-04-01 17:56:14','2012-04-01 17:56:15','kongyue','kongyue',NULL,'kongyue_product_info','kongyue_product_attribute','kongyue_error_product','kongyue_product_image','kongyue_change_product_task','kongyue_product_feed','kongyue_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (6,'2012-04-23 19:07:14','2012-04-23 19:07:15','wuba','wuba',NULL,'wuba_product_info','wuba_product_attribute','wuba_error_product','wuba_product_image','wuba_change_product_task','wuba_product_feed','wuba_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (7,'2012-05-02 14:51:21','2012-05-02 14:51:24','meituan','北京三快科技有限公司',NULL,'meituan_product_info','meituan_product_attribute','meituan_error_product','meituan_product_image','meituan_change_product_task','meituan_product_feed','meituan_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (8,'2012-05-02 14:51:21','2012-05-02 14:51:24','suoxian','suoxian',NULL,'suoxian_product_info','suoxian_product_attribute','suoxian_error_product','suoxian_product_image','suoxian_change_product_task','suoxian_product_feed','suoxian_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (9,'2012-05-04 17:31:25','2012-05-04 17:31:26','vancl','vancl',NULL,'vancl_product_info','vancl_product_attribute','vancl_error_product','vancl_product_image','vancl_change_product_task','vancl_product_feed','vancl_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (10,'2012-05-15 18:51:19','2012-05-15 18:51:20','meilele','meilele',NULL,'meilele_product_info','meilele_product_attribute','meilele_error_product','meilele_product_image','meilele_change_product_task','meilele_product_feed','meilele_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (11,'2012-05-16 18:21:52','2012-05-16 18:21:51','zouxiu','zouxiu',NULL,'zouxiu_product_info','zouxiu_product_attribute','zouxiu_error_product','zouxiu_product_image','zouxiu_change_product_task','zouxiu_product_feed','zouxiu_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (12,'2012-05-21 15:25:36','2012-05-21 15:25:37','weipin','weipin',NULL,'weipin_product_info','weipin_product_attribute','weipin_error_product','weipin_product_image','weipin_change_product_task','weipin_product_feed','weipin_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (13,'2012-06-07 14:51:21','2012-06-07 14:51:24','wowotuan','wowotuan',NULL,'wowotuan_product_info','wowotuan_product_attribute','wowotuan_error_product','wowotuan_product_image','wowotuan_change_product_task','wowotuan_product_feed','wowotuan_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (14,'2012-06-19 13:28:00','2012-06-19 13:28:00','coo8','coo8',NULL,'coo8_product_info','coo8_product_attribute','coo8_error_product','coo8_product_image','coo8_change_product_task','coo8_product_feed','coo8_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (15,'2012-07-13 17:54:08','2012-07-13 17:54:08','berries','berries',NULL,'berries_product_info','berries_product_attribute','berries_error_product','berries_product_image','berries_change_product_task','berries_product_feed','berries_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (16,'2012-07-20 17:26:54','2012-07-20 17:26:54','ctvonline','ctvonline',NULL,'ctvonline_product_info','ctvonline_product_attribute','ctvonline_error_product','ctvonline_product_image','ctvonline_change_product_task','ctvonline_product_feed','ctvonline_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (17,'2012-07-27 17:17:51','2012-07-27 17:17:51','elong','elong',NULL,'elong_product_info','elong_product_attribute','elong_error_product','elong_product_image','elong_change_product_task','elong_product_feed','elong_product_line_validation','NORMAL');
INSERT INTO `merchant` VALUES (23,'2012-08-20 18:03:18','2012-08-20 18:03:18','prodb_1381007','百度(中国)有限公司',NULL,'prodb_1381007_product_info','prodb_1381007_product_attribute','prodb_1381007_error_product','prodb_1381007_product_image','prodb_1381007_change_product_task','prodb_1381007_product_feed','prodb_1381007_product_line_validation','NORMAL');
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-08-22 11:06:29
