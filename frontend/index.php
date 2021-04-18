<?php
   define("ROOTPATH", '/var/www/html/DreamHack/frontend');
    //include ROOTPATH . '/database/db.php';
    session_start();
?>

<html lang="en">  
<head>
                            <!--        Meta Data       -->   
                                
                            <meta charset="utf-8">

<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Dream Hacker</title>    

<meta name="description" content="Home page for Dream Hacker">

<meta name="author" content="Real Time Group">

							<!--        Styler          -->   
<link rel="stylesheet" type="text/css" href="css/main.css">						
<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.6.3/css/all.css" integrity="sha384-UHRtZLI+pbxtHCWp1t77Bi1L4ZtiqrqD80Kn4Z8NTSRyMA2Fd33n5dQ8lWUE00s/" crossorigin="anonymous">     
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css">
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6" crossorigin="anonymous">
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">  
<style>
    li {
    display: inline;
    margin: 1%;
    padding: 1%;
    font-size: 36px;
    border-radius: 20px;
    border: 1px grey solid;
    
  }

  ul {
    list-style-type: none;
    text-align: center;
  }
</style>
  
</head>

<body>
    <div class='container' align=center>
        <img src="pictures/dreamHacker.png" width="400" height="150"> 
        <hr>
    </div>

    <div class='centered'>
    <ul class='w3-ul w3-hoverable'>
            <a href="liveReading.php"><li> Live Reading</li></a>
            <a href='musicChoice.php'><li> Choose Audio</li></a>
            <a href='history.php'><li> Histroy</li></a>
    </ul>
    </div>
<footer>
    <div class="container">
    	<hr>
        <h3>What is Dream Hacker</h3>
        <p> Dream Hacker is program designed to help you sleep better. Based on numerous research on music and the effects it has on sleep our projects monitors our users heart rate and plays music. Once Dream Hacker detects the user is has entered REM sleep, when people are most susceptible to external stimuli, it will play calming music.</p>
        <hr>
    </div>
</footer>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.bundle.min.js" integrity="sha384-JEW9xMcG8R+pH31jmWH6WWP0WintQrMb4s7ZOdauHnUtxwoG2vI5DkLtS3qm9Ekf" crossorigin="anonymous"></script>
</body>
</html>