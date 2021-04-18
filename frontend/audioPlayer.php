<?php
/* Attempt MySQL server connection. Assuming you are running MySQL
server with default setting (user 'root' with no password) */
session_start();
$host = 'localhost';
$user = 'dissertation';
$pass = '091164383707572306304pass';
$db = 'dreamHacker';

$link = mysqli_connect($host,$user,$pass, $db);
 
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
 
// Escape user inputs for security
$song = mysqli_real_escape_string($link, $_REQUEST['song']);
//$location = mysqli_real_escape_string($link, $_REQUEST['exampleRadios']);
$location = 'audio/' . $song .'.mp3';


// Attempt insert query execution
//$sql = "INSERT INTO audio (song, location) VALUES ('$song','$location')";
$sql = "UPDATE audio SET song = '$song', location = '$location' WHERE id = 2";

    if(mysqli_query($link, $sql)){
        $_SESSION['message'] = "You have Succesfully changed the audio!";
        header("location: sucess.php");
    }
    else{
        $_SESSION['message'] = "ERROR: Could not able to execute: " . $sql. " . mysqli_error($link)";
        header("location: sucess.php");
    }
    
?>