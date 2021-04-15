<?php
    define("ROOTPATH", 'C:/Apache24/htdocs/dreamHacker');
    //include ROOTPATH . '/database/db.php';
    session_start();

    if($$_GET) {  
        $epoch = $_GET["epoch"]; 
        $bpm = $_GET["beats"]; 
        $sleep = $_GET["sleep"]; 

    }

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
    margin: 2%;
    padding: 2%;
    font-size: 36px;
    border-radius: 25px;
    border: 1px grey solid;
    
  }

  ul {
    list-style-type: none;
    text-align: center;
  }
</style>
  
</head>

<body>
    <div class='container'>
        <h1>Dream Hacker <hr></h1>
        
    </div>
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
    <div class="container">
        <table>
            <tr>
                <th>Epoch</th>
                <th>Beats</th>
                <th>Sleep</th>
            </tr>
            <tr>
                <td><?php echo $epoch;?></td>
                <td><?php echo $bpm;?></td>
                <td><?php echo $sleep;?></td>
            </tr>
        </table>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/js/bootstrap.bundle.min.js" integrity="sha384-JEW9xMcG8R+pH31jmWH6WWP0WintQrMb4s7ZOdauHnUtxwoG2vI5DkLtS3qm9Ekf" crossorigin="anonymous"></script>
</body>
</html>