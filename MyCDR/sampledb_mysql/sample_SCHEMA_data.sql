-- --------------------------------------------------------
-- Host:                         localhost
-- Server version:               5.5.20 - MySQL Community Server (GPL)
-- Server OS:                    Win32
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-02-16 20:32:09
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

-- Dumping data for table mycdr.commonfields: ~0 rows (approximately)
DELETE FROM `commonfields`;
/*!40000 ALTER TABLE `commonfields` DISABLE KEYS */;
/*!40000 ALTER TABLE `commonfields` ENABLE KEYS */;


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

-- Dumping data for table mycdr.inelement: ~2 rows (approximately)
DELETE FROM `inelement`;
/*!40000 ALTER TABLE `inelement` DISABLE KEYS */;
INSERT INTO `inelement` (`PATH`, `FILE_EXP`, `ELEMENT_NAME`, `TYPE`, `SCRIPT`, `LOGLEVEL`, `BUFFER_SIZE`, `FIELDSEPERATOR`, `LINE_SEPERATOR`, `FIELDS`, `MAX_FIELD_LEN`, `NUMBER_OF_FIELDS`) VALUES
	('FIX', 'NOR*', 'SEIMENS1', 'F', 'E:/ani/MyCDR/seimens1.nut', 0000000128, 0000004000, 0000000010, 0000000010, 0000000008, 0000000060, 0000000008),
	('CSV', 'NOR*', 'NORTEL1', 'C', 'E:/ani/MyCDR/nortel1.nut', 0000000128, 0000004000, 0000000044, 0000000010, 0000000008, 0000000060, 0000000008);
/*!40000 ALTER TABLE `inelement` ENABLE KEYS */;


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

-- Dumping data for table mycdr.in_field_details: ~16 rows (approximately)
DELETE FROM `in_field_details`;
/*!40000 ALTER TABLE `in_field_details` DISABLE KEYS */;
INSERT INTO `in_field_details` (`PATH`, `NAME`, `START`, `END`, `POS`, `LEN`, `seq`) VALUES
	('CSV', 'CALLER_NUMBER', 0000000000, 0000000000, 0000000001, 0000000020, 1),
	('CSV', 'CALLED_NUMBER', 0000000000, 0000000000, 0000000002, 0000000020, 2),
	('CSV', 'TIME', 0000000000, 0000000000, 0000000003, 0000000020, 3),
	('CSV', 'DURATION', 0000000000, 0000000000, 0000000004, 0000000020, 4),
	('CSV', 'CALLER_IMEI', 0000000000, 0000000000, 0000000005, 0000000020, 5),
	('CSV', 'CALLED_IMEI', 0000000000, 0000000000, 0000000006, 0000000020, 6),
	('CSV', 'CALLER_MSC', 0000000000, 0000000000, 0000000007, 0000000020, 7),
	('CSV', 'CALLED_MSC', 0000000000, 0000000000, 0000000008, 0000000020, 8),
	('FIX', 'CALLER_NUMBER', 0000000001, 0000000010, 0000000000, 0000000000, 1),
	('FIX', 'CALLED_NUMBER', 0000000011, 0000000020, 0000000000, 0000000000, 2),
	('FIX', 'TIME', 0000000021, 0000000030, 0000000000, 0000000000, 3),
	('FIX', 'DURATION', 0000000031, 0000000040, 0000000000, 0000000000, 4),
	('FIX', 'CALLER_IMEI', 0000000041, 0000000050, 0000000000, 0000000000, 5),
	('FIX', 'CALLED_IMEI', 0000000051, 0000000060, 0000000000, 0000000000, 6),
	('FIX', 'CALLER_MSC', 0000000061, 0000000070, 0000000000, 0000000000, 7),
	('FIX', 'CALLED_MSC', 0000000071, 0000000080, 0000000000, 0000000000, 8);
/*!40000 ALTER TABLE `in_field_details` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
