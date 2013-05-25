
use prodb_product_info; 

INSERT INTO `index_helper_attribute` (`iftestdb`, `latesteffectivetime`, `preparedeffectivetime`) VALUES 
(1, NULL, NULL), 
(0, NULL, NULL); 
 
INSERT INTO `aw_updatetime_tracking` (`latesteffectivetime`, `istestdb`) VALUES  
(NULL, 1),  
(NULL, 0);  
 
INSERT INTO `uniqueid_creator` VALUES ('1', 'Product', '1');  
INSERT INTO `uniqueid_creator` VALUES ('2', 'Attribute', '1');  
INSERT INTO `uniqueid_creator` VALUES ('3', 'Image', '1');  
INSERT INTO `uniqueid_creator` VALUES ('4', 'TestProduct', '1');  
