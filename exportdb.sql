-- MySQL dump 10.13  Distrib 5.5.53, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: drupal
-- ------------------------------------------------------
-- Server version	5.5.53-0ubuntu0.14.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `agencies`
--

DROP TABLE IF EXISTS `agencies`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agencies` (
  `agency_id` varchar(255) NOT NULL,
  `agency_name` varchar(255) NOT NULL,
  `agency_phone` varchar(64) DEFAULT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`agency_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `agencies`
--

LOCK TABLES `agencies` WRITE;
/*!40000 ALTER TABLE `agencies` DISABLE KEYS */;
set autocommit=0;
/*!40000 ALTER TABLE `agencies` ENABLE KEYS */;
UNLOCK TABLES;
commit;

--
-- Table structure for table `exceptions`
--

DROP TABLE IF EXISTS `exceptions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `exceptions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `service_id` varchar(50) NOT NULL,
  `exception_date` date NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `service_id` (`service_id`),
  CONSTRAINT `gotransit_gtfs_service_exceptions_ibfk_1` FOREIGN KEY (`service_id`) REFERENCES `services` (`service_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `exceptions`
--

LOCK TABLES `exceptions` WRITE;
/*!40000 ALTER TABLE `exceptions` DISABLE KEYS */;
set autocommit=0;
/*!40000 ALTER TABLE `exceptions` ENABLE KEYS */;
UNLOCK TABLES;
commit;

--
-- Table structure for table `routes`
--

DROP TABLE IF EXISTS `routes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `routes` (
  `route_id` varchar(255) NOT NULL,
  `agency_id` varchar(255) DEFAULT NULL,
  `route_name` varchar(255) NOT NULL,
  `route_type` int(11) NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`route_id`),
  KEY `agency_id` (`agency_id`),
  CONSTRAINT `fk_routes_1` FOREIGN KEY (`agency_id`) REFERENCES `agencies` (`agency_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `gotransit_gtfs_routes_ibfk_1` FOREIGN KEY (`agency_id`) REFERENCES `agencies` (`agency_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `routes`
--

LOCK TABLES `routes` WRITE;
/*!40000 ALTER TABLE `routes` DISABLE KEYS */;
set autocommit=0;
INSERT INTO `routes` VALUES ('BCC',NULL,'GoDurham - Duke / VA Hospitals-Rearch Drive-Golden Belt',1,'2017-01-02 09:19:45');
/*!40000 ALTER TABLE `routes` ENABLE KEYS */;
UNLOCK TABLES;
commit;

--
-- Table structure for table `services`
--

DROP TABLE IF EXISTS `services`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `services` (
  `service_id` varchar(50) NOT NULL,
  `service_name` varchar(255) NOT NULL,
  `is_available_monday` tinyint(1) DEFAULT NULL,
  `is_available_tuesday` tinyint(1) DEFAULT NULL,
  `is_available_wednesday` tinyint(1) DEFAULT NULL,
  `is_available_thursday` tinyint(1) DEFAULT NULL,
  `is_available_friday` tinyint(1) DEFAULT NULL,
  `is_available_saturday` tinyint(1) DEFAULT NULL,
  `is_available_sunday` tinyint(1) DEFAULT NULL,
  `start_date` timestamp NULL DEFAULT NULL,
  `end_date` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`service_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `services`
--

LOCK TABLES `services` WRITE;
/*!40000 ALTER TABLE `services` DISABLE KEYS */;
set autocommit=0;
INSERT INTO `services` VALUES ('2TSAT','2TSAT',1,1,1,1,1,1,1,'2016-08-05 18:30:00','2017-08-05 18:30:00');
/*!40000 ALTER TABLE `services` ENABLE KEYS */;
UNLOCK TABLES;
commit;

--
-- Table structure for table `stop_times`
--

DROP TABLE IF EXISTS `stop_times`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stop_times` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `stop_id` int(11) NOT NULL,
  `trip_id` varchar(255) DEFAULT NULL,
  `arrival_time` time NOT NULL,
  `stop_sequence` int(11) NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=136 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stop_times`
--

LOCK TABLES `stop_times` WRITE;
/*!40000 ALTER TABLE `stop_times` DISABLE KEYS */;
set autocommit=0;
INSERT INTO `stop_times` VALUES (1,6670,NULL,'10:03:00',1,'2017-01-02 10:53:44'),(2,5637,NULL,'10:12:00',1,'2017-01-02 10:53:44'),(3,6383,NULL,'10:22:00',1,'2017-01-02 10:53:45'),(4,6670,NULL,'10:18:00',2,'2017-01-02 10:53:45'),(5,5637,NULL,'10:27:00',2,'2017-01-02 10:53:45'),(6,6383,NULL,'10:37:00',2,'2017-01-02 10:53:45'),(7,6670,NULL,'10:33:00',3,'2017-01-02 10:53:45'),(8,5637,NULL,'10:42:00',3,'2017-01-02 10:53:45'),(9,6383,NULL,'10:52:00',3,'2017-01-02 10:53:45'),(10,6670,NULL,'10:48:00',4,'2017-01-02 10:53:45'),(11,5637,NULL,'10:57:00',4,'2017-01-02 10:53:45'),(12,6383,NULL,'11:07:00',4,'2017-01-02 10:53:45'),(13,6670,NULL,'11:03:00',5,'2017-01-02 10:53:45'),(14,5637,NULL,'11:12:00',5,'2017-01-02 10:53:45'),(15,6383,NULL,'11:22:00',5,'2017-01-02 10:53:45'),(16,6670,NULL,'11:18:00',6,'2017-01-02 10:53:45'),(17,5637,NULL,'11:27:00',6,'2017-01-02 10:53:45'),(18,6383,NULL,'11:37:00',6,'2017-01-02 10:53:45'),(19,6670,NULL,'11:33:00',7,'2017-01-02 10:53:45'),(20,5637,NULL,'11:42:00',7,'2017-01-02 10:53:45'),(21,6383,NULL,'11:52:00',7,'2017-01-02 10:53:45'),(22,6670,NULL,'11:48:00',8,'2017-01-02 10:53:45'),(23,5637,NULL,'11:57:00',8,'2017-01-02 10:53:45'),(24,6383,NULL,'12:07:00',8,'2017-01-02 10:53:45'),(25,6670,NULL,'12:03:00',9,'2017-01-02 10:53:45'),(26,5637,NULL,'12:12:00',9,'2017-01-02 10:53:45'),(27,6383,NULL,'12:22:00',9,'2017-01-02 10:53:45'),(28,6670,NULL,'12:18:00',10,'2017-01-02 10:53:45'),(29,5637,NULL,'12:27:00',10,'2017-01-02 10:53:45'),(30,6383,NULL,'12:37:00',10,'2017-01-02 10:53:45'),(31,6670,NULL,'12:33:00',11,'2017-01-02 10:53:45'),(32,5637,NULL,'12:42:00',11,'2017-01-02 10:53:45'),(33,6383,NULL,'12:52:00',11,'2017-01-02 10:53:46'),(34,6670,NULL,'12:48:00',12,'2017-01-02 10:53:46'),(35,5637,NULL,'12:57:00',12,'2017-01-02 10:53:46'),(36,6383,NULL,'13:07:00',12,'2017-01-02 10:53:46'),(37,6670,NULL,'13:03:00',13,'2017-01-02 10:53:46'),(38,5637,NULL,'13:12:00',13,'2017-01-02 10:53:46'),(39,6383,NULL,'13:22:00',13,'2017-01-02 10:53:46'),(40,6670,NULL,'13:18:00',14,'2017-01-02 10:53:46'),(41,5637,NULL,'13:27:00',14,'2017-01-02 10:53:46'),(42,6383,NULL,'13:37:00',14,'2017-01-02 10:53:46'),(43,6670,NULL,'13:33:00',15,'2017-01-02 10:53:46'),(44,5637,NULL,'13:42:00',15,'2017-01-02 10:53:46'),(45,6383,NULL,'13:52:00',15,'2017-01-02 10:53:46'),(46,6670,NULL,'13:48:00',16,'2017-01-02 10:53:46'),(47,5637,NULL,'13:57:00',16,'2017-01-02 10:53:46'),(48,6383,NULL,'14:07:00',16,'2017-01-02 10:53:46'),(49,6670,NULL,'14:03:00',17,'2017-01-02 10:53:46'),(50,5637,NULL,'14:12:00',17,'2017-01-02 10:53:46'),(51,6383,NULL,'14:22:00',17,'2017-01-02 10:53:46'),(52,6670,NULL,'14:18:00',18,'2017-01-02 10:53:46'),(53,5637,NULL,'14:27:00',18,'2017-01-02 10:53:46'),(54,6383,NULL,'14:37:00',18,'2017-01-02 10:53:46'),(55,6670,NULL,'14:33:00',19,'2017-01-02 10:53:46'),(56,5637,NULL,'14:42:00',19,'2017-01-02 10:53:46'),(57,6383,NULL,'14:52:00',19,'2017-01-02 10:53:46'),(58,6670,NULL,'14:48:00',20,'2017-01-02 10:53:46'),(59,5637,NULL,'14:57:00',20,'2017-01-02 10:53:46'),(60,6383,NULL,'15:07:00',20,'2017-01-02 10:53:46'),(61,6670,NULL,'15:03:00',21,'2017-01-02 10:53:46'),(62,5637,NULL,'15:12:00',21,'2017-01-02 10:53:46'),(63,6383,NULL,'15:22:00',21,'2017-01-02 10:53:46'),(64,6670,NULL,'15:18:00',22,'2017-01-02 10:53:46'),(65,5637,NULL,'15:27:00',22,'2017-01-02 10:53:46'),(66,6383,NULL,'15:37:00',22,'2017-01-02 10:53:46'),(67,6670,NULL,'15:33:00',23,'2017-01-02 10:53:46'),(68,5637,NULL,'15:42:00',23,'2017-01-02 10:53:46'),(69,6383,NULL,'15:52:00',23,'2017-01-02 10:53:46'),(70,6670,NULL,'15:48:00',24,'2017-01-02 10:53:47'),(71,5637,NULL,'15:57:00',24,'2017-01-02 10:53:47'),(72,6383,NULL,'16:07:00',24,'2017-01-02 10:53:47'),(73,6670,NULL,'16:03:00',25,'2017-01-02 10:53:47'),(74,5637,NULL,'16:12:00',25,'2017-01-02 10:53:47'),(75,6383,NULL,'16:22:00',25,'2017-01-02 10:53:47'),(76,6670,NULL,'16:18:00',26,'2017-01-02 10:53:47'),(77,5637,NULL,'16:27:00',26,'2017-01-02 10:53:47'),(78,6383,NULL,'16:37:00',26,'2017-01-02 10:53:47'),(79,6670,NULL,'16:33:00',27,'2017-01-02 10:53:47'),(80,5637,NULL,'16:42:00',27,'2017-01-02 10:53:47'),(81,6383,NULL,'16:52:00',27,'2017-01-02 10:53:47'),(82,6670,NULL,'16:48:00',28,'2017-01-02 10:53:47'),(83,5637,NULL,'16:57:00',28,'2017-01-02 10:53:47'),(84,6383,NULL,'17:07:00',28,'2017-01-02 10:53:47'),(85,6670,NULL,'17:03:00',29,'2017-01-02 10:53:47'),(86,5637,NULL,'17:12:00',29,'2017-01-02 10:53:47'),(87,6383,NULL,'17:22:00',29,'2017-01-02 10:53:47'),(88,6670,NULL,'17:18:00',30,'2017-01-02 10:53:47'),(89,5637,NULL,'17:27:00',30,'2017-01-02 10:53:47'),(90,6383,NULL,'17:37:00',30,'2017-01-02 10:53:47'),(91,6670,NULL,'17:33:00',31,'2017-01-02 10:53:47'),(92,5637,NULL,'17:42:00',31,'2017-01-02 10:53:47'),(93,6383,NULL,'17:52:00',31,'2017-01-02 10:53:47'),(94,6670,NULL,'17:48:00',32,'2017-01-02 10:53:47'),(95,5637,NULL,'17:57:00',32,'2017-01-02 10:53:47'),(96,6383,NULL,'18:07:00',32,'2017-01-02 10:53:47'),(97,6670,NULL,'18:03:00',33,'2017-01-02 10:53:47'),(98,5637,NULL,'18:12:00',33,'2017-01-02 10:53:47'),(99,6383,NULL,'18:22:00',33,'2017-01-02 10:53:47'),(100,6670,NULL,'18:18:00',34,'2017-01-02 10:53:47'),(101,5637,NULL,'18:27:00',34,'2017-01-02 10:53:47'),(102,6383,NULL,'18:37:00',34,'2017-01-02 10:53:47'),(103,6670,NULL,'18:33:00',35,'2017-01-02 10:53:47'),(104,5637,NULL,'18:42:00',35,'2017-01-02 10:53:47'),(105,6383,NULL,'18:52:00',35,'2017-01-02 10:53:47'),(106,6670,NULL,'18:48:00',36,'2017-01-02 10:53:47'),(107,5637,NULL,'18:57:00',36,'2017-01-02 10:53:47'),(108,6383,NULL,'19:07:00',36,'2017-01-02 10:53:47'),(109,6670,NULL,'19:03:00',37,'2017-01-02 10:53:47'),(110,5637,NULL,'19:12:00',37,'2017-01-02 10:53:47'),(111,6383,NULL,'19:22:00',37,'2017-01-02 10:53:48'),(112,6670,NULL,'19:18:00',38,'2017-01-02 10:53:48'),(113,5637,NULL,'19:27:00',38,'2017-01-02 10:53:48'),(114,6383,NULL,'19:37:00',38,'2017-01-02 10:53:48'),(115,6670,NULL,'19:33:00',39,'2017-01-02 10:53:48'),(116,5637,NULL,'19:42:00',39,'2017-01-02 10:53:48'),(117,6383,NULL,'19:52:00',39,'2017-01-02 10:53:48'),(118,6670,NULL,'19:48:00',40,'2017-01-02 10:53:48'),(119,5637,NULL,'19:57:00',40,'2017-01-02 10:53:48'),(120,6383,NULL,'20:07:00',40,'2017-01-02 10:53:48'),(121,6670,NULL,'20:03:00',41,'2017-01-02 10:53:48'),(122,5637,NULL,'20:12:00',41,'2017-01-02 10:53:48'),(123,6383,NULL,'20:22:00',41,'2017-01-02 10:53:48'),(124,6670,NULL,'20:18:00',42,'2017-01-02 10:53:48'),(125,5637,NULL,'20:27:00',42,'2017-01-02 10:53:48'),(126,6383,NULL,'20:37:00',42,'2017-01-02 10:53:48'),(127,6670,NULL,'20:33:00',43,'2017-01-02 10:53:48'),(128,5637,NULL,'20:42:00',43,'2017-01-02 10:53:48'),(129,6383,NULL,'20:52:00',43,'2017-01-02 10:53:48'),(130,6670,NULL,'20:48:00',44,'2017-01-02 10:53:48'),(131,5637,NULL,'20:57:00',44,'2017-01-02 10:53:48'),(132,6383,NULL,'21:07:00',44,'2017-01-02 10:53:48'),(133,6670,NULL,'21:03:00',45,'2017-01-02 10:53:48'),(134,5637,NULL,'21:12:00',45,'2017-01-02 10:53:48'),(135,6383,NULL,'21:22:00',45,'2017-01-02 10:53:48');
/*!40000 ALTER TABLE `stop_times` ENABLE KEYS */;
UNLOCK TABLES;
commit;

--
-- Table structure for table `stops`
--

DROP TABLE IF EXISTS `stops`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stops` (
  `stop_id` int(11) NOT NULL,
  `stop_code` varchar(100) NOT NULL,
  `stop_name` varchar(255) NOT NULL,
  `latitude` decimal(10,6) DEFAULT NULL,
  `longitude` decimal(10,6) DEFAULT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`stop_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stops`
--

LOCK TABLES `stops` WRITE;
/*!40000 ALTER TABLE `stops` DISABLE KEYS */;
set autocommit=0;
INSERT INTO `stops` VALUES (5637,'Main St at Gregson St','Main St at Gregson St',NULL,NULL,'2017-01-02 09:56:23'),(6383,'Main St at Golden Belt','Main St at Golden Belt',NULL,NULL,'2017-01-02 09:56:23'),(6670,'Research Dr at Circuit Dr','Research Dr at Circuit Dr',NULL,NULL,'2017-01-02 09:56:23');
/*!40000 ALTER TABLE `stops` ENABLE KEYS */;
UNLOCK TABLES;
commit;

--
-- Table structure for table `trips`
--

DROP TABLE IF EXISTS `trips`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `trips` (
  `trip_id` varchar(255) NOT NULL,
  `route_id` varchar(255) NOT NULL,
  `service_id` varchar(255) NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `driection_stop_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`trip_id`),
  KEY `route_id` (`route_id`),
  KEY `service_id` (`service_id`),
  CONSTRAINT `gotransit_gtfs_trips_ibfk_1` FOREIGN KEY (`route_id`) REFERENCES `routes` (`route_id`),
  CONSTRAINT `gotransit_gtfs_trips_ibfk_2` FOREIGN KEY (`service_id`) REFERENCES `services` (`service_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `trips`
--

LOCK TABLES `trips` WRITE;
/*!40000 ALTER TABLE `trips` DISABLE KEYS */;
set autocommit=0;
/*!40000 ALTER TABLE `trips` ENABLE KEYS */;
UNLOCK TABLES;
commit;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-01-03 12:43:26
