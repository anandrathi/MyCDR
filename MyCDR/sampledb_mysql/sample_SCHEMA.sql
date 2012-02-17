-- --------------------------------------------------------
-- Host:                         localhost
-- Server version:               5.5.20 - MySQL Community Server (GPL)
-- Server OS:                    Win32
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-02-16 20:34:06
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping database structure for mycdr
DROP DATABASE IF EXISTS `mycdr`;
CREATE DATABASE IF NOT EXISTS `mycdr` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `mycdr`;


-- Dumping structure for table mycdr.commonfields
DROP TABLE IF EXISTS `commonfields`;
CREATE TABLE IF NOT EXISTS `commonfields` (
  `NAME` varchar(50) NOT NULL,
  `TYPE` varchar(1) NOT NULL,
  `LEN` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.


-- Dumping structure for table mycdr.inelement
DROP TABLE IF EXISTS `inelement`;
CREATE TABLE IF NOT EXISTS `inelement` (
  `PATH` varchar(50) NOT NULL,
  `FILE_EXP` varchar(50) NOT NULL,
  `ELEMENT_NAME` varchar(50) NOT NULL,
  `TYPE` varchar(1) NOT NULL,
  `SCRIPT` varchar(512) NOT NULL,
  `LOGLEVEL` int(10) unsigned zerofill NOT NULL,
  `BUFFER_SIZE` int(10) unsigned zerofill NOT NULL,
  `FIELDSEPERATOR` int(10) unsigned zerofill NOT NULL,
  `LINE_SEPERATOR` int(10) unsigned zerofill NOT NULL,
  `FIELDS` int(10) unsigned zerofill NOT NULL,
  `MAX_FIELD_LEN` int(10) unsigned zerofill NOT NULL,
  `NUMBER_OF_FIELDS` int(10) unsigned zerofill NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.


-- Dumping structure for table mycdr.in_field_details
DROP TABLE IF EXISTS `in_field_details`;
CREATE TABLE IF NOT EXISTS `in_field_details` (
  `PATH` varchar(50) NOT NULL,
  `NAME` varchar(512) NOT NULL,
  `START` int(10) unsigned zerofill NOT NULL,
  `END` int(10) unsigned zerofill NOT NULL,
  `POS` int(10) unsigned zerofill NOT NULL,
  `LEN` int(10) unsigned zerofill NOT NULL,
  `seq` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
