<?php
/* Database connection settings */
$host = 'localhost';
$user = ''; /* insert your username */
$pass = ''; /* insert your password */
$db = 'dreamHacker';
$mysqli = new mysqli($host,$user,$pass, $db) or die($mysqli->error);
?>

<html>
