<?php
/* Database connection settings */
$host = 'localhost';
$user = 'dissertation'; /* insert your username */
$pass = '091164383707572306304pass'; /* insert your password */
$db = 'dreamHacker';
$mysqli = new mysqli($host,$user,$pass, $db) or die($mysqli->error);


/* Connect to MySQL
$conn = new mysqli($$host, $$user, $pass);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// If database is not exist create one
if (!mysqli_select_db($conn,$db)){
    $sql = "CREATE DATABASE ".$db;
    if ($conn->query($sql) === TRUE) {
        echo "Database created successfully";
    }else {
        echo "Error creating database: " . $conn->error;
    }
} */
?>

<html>
