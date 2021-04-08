<?php
    define("ROOTPATH", 'C:/Apache24/htdocs/dreamHacker');
    //include ROOTPATH . '/database/db.php';
    session_start();
?>

<html lang="en">
                            <!--        Meta Data       -->   
                                
<meta charset="utf-8">

<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Dream Hacker</title>    

<meta name="description" content="Home page for Dream Hacker">

<meta name="author" content="Real Time Group">

							<!--        Styler          -->   
<link rel="stylesheet" type="text/css" href="css/main.css">						
<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.6.3/css/all.css" integrity="sha384-UHRtZLI+pbxtHCWp1t77Bi1L4ZtiqrqD80Kn4Z8NTSRyMA2Fd33n5dQ8lWUE00s/" crossorigin="anonymous">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">       
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">    
<head>
    <div class='container'>
        <h1>Dream Hacker <hr></h1>
        
    </div>
</head>

<body>
    <div class="container">
        <h3>What is Dream Hacker</h3>
        <p> Here we will explain </p>
        <hr>
    </div>

    <div class='container'>
        <div class='row' align=center>
            <mainPoint id="action" class="w3-hover-green"> Start</mainPoint>
            <br><br>
        </div>
    </div>

    <div class='container'>
        <ul class='w3-ul w3-hoverable'>
            <a href='liveReading.php'><li> Live Reading</li></a>
            <a href='musicChoice.php'><li> Choose Music</li></a>
            <a href='history.php'><li> View Histroy</li></a>
        </ul>
    </div>
</body>
</html>