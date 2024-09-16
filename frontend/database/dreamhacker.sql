-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Apr 18, 2021 at 03:59 PM
-- Server version: 8.0.14
-- PHP Version: 7.3.2RC1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `dreamHacker`
--
CREATE DATABASE IF NOT EXISTS `dreamHacker` DEFAULT CHARACTER SET utf32 COLLATE utf32_general_ci;
USE `dreamHacker`;

-- --------------------------------------------------------
--
-- Table structure for table `audio`
--

CREATE TABLE IF NOT EXISTS `audio` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `song` varchar(100) DEFAULT 'default',
  `location` varchar(150) DEFAULT 'audio/Just_The_Two_Of_Us.mp3',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf32;

--
-- Dumping data for table `audio`
--

INSERT INTO `audio` (`id`, `song`, `location`) VALUES
(2, 'Sleep Music', 'audio/Sleep Music.mp3');

-- --------------------------------------------------------

--
-- Table structure for table `records`
--

CREATE TABLE IF NOT EXISTS `records` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `remSleep` int(11) NOT NULL DEFAULT '0',
  `heartRate` int(3) NOT NULL,
  `hrsSlept` int(3) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf32;

--
-- Dumping data for table `records`
--

INSERT INTO `records` (`id`, `remSleep`, `heartRate`, `hrsSlept`) VALUES
(1, 5, 82, 6);

-- --------------------------------------------------------

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
