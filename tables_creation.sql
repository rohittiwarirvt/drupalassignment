--lat long default
--trip_id default
use drupal;
SET foreign_key_checks = 0;
drop table routes;
drop table agencies;
drop table stop_times;
drop table trips;
drop table services;
drop table exceptions;
drop table route_fares;
SET foreign_key_checks = 1;

truncate routes;
truncate agencies;
trucate stops;  
truncate stop_times;
truncate trips_back;
truncate services;
truncate route_fares;

truncate exceptions;

CREATE TABLE IF NOT EXISTS `routes` (
  `route_id` VARCHAR(255) NOT NULL,
  `route_name` VARCHAR(255) NOT NULL,
  `route_type` INT(11) NOT NULL,
  `created` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `agency_id` INT(11) NOT NULL,
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;


CREATE TABLE IF NOT EXISTS `agencies` (
  `agency_id` VARCHAR(255) NOT NULL,
  `agency_name` VARCHAR(255) NOT NULL,
  `created` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;

CREATE TABLE IF NOT EXISTS `stops` (
  `stop_id` INT(11) NOT NULL,
  `stop_code` VARCHAR(100) NOT NULL,
  `stop_name` VARCHAR(255) NOT NULL,
  `latitude` DECIMAL(10,6) NOT NULL,
  `longitude` DECIMAL(10,6) NOT NULL,
  `created` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`stop_id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;


CREATE TABLE IF NOT EXISTS `stop_times` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `stop_id` INT(11) NOT NULL,
  `trip_id` INT(11) NOT NULL,
  `arrival_time` TIME NOT NULL,
  `stop_sequence` INT(11) NOT NULL,
  `created` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;


CREATE TABLE IF NOT EXISTS `trips_back` (
  `trip_id` VARCHAR(50) NOT NULL,
  `route_id` INT(11) NOT NULL,
  `service_id` INT(11) NOT NULL,
  `created` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `direction_id` INT(11) NULL,
  `direction_name` VARCHAR(50) NOT NULL,
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;


CREATE TABLE IF NOT EXISTS `services` (
  `service_id` VARCHAR(50) NOT NULL,
  `service_name` VARCHAR(255) NOT NULL,
  `is_available_monday` TINYINT(1) NULL DEFAULT NULL,
  `is_available_tuesday` TINYINT(1) NULL DEFAULT NULL,
  `is_available_wednesday` TINYINT(1) NULL DEFAULT NULL,
  `is_available_thursday` TINYINT(1) NULL DEFAULT NULL,
  `is_available_friday` TINYINT(1) NULL DEFAULT NULL,
  `is_available_saturday` TINYINT(1) NULL DEFAULT NULL,
  `is_available_sunday` TINYINT(1) NULL DEFAULT NULL,
  `start_date` TIMESTAMP NULL DEFAULT NULL,
  `end_date` TIMESTAMP NULL DEFAULT NULL,
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1;

CREATE TABLE IF NOT EXISTS `route_fares` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `origin_id` INT(11) NULL DEFAULT NULL,
  `destination_id` INT(11) NULL DEFAULT NULL,
  `created` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `amount` DECIMAL(5) NULL,
  `route_id` INT(11) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
