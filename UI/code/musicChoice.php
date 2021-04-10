<?php
    define("ROOTPATH", 'C:/Apache24/htdocs/dreamHacker');
    //include ROOTPATH . '/database/db.php';
    session_start();
?>
<html lang="en">
<head>
                            <!--        Meta Data       -->   
                                
                            <meta charset="utf-8">

<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Dream Hacker</title>    

<meta name="description" content="Music Choice page for Dream Hacker">

<meta name="author" content="Real Time Group">

							<!--        Styler          -->   
<link rel="stylesheet" type="text/css" href="css/main.css">						
<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.6.3/css/all.css" integrity="sha384-UHRtZLI+pbxtHCWp1t77Bi1L4ZtiqrqD80Kn4Z8NTSRyMA2Fd33n5dQ8lWUE00s/" crossorigin="anonymous">       
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css">
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-eOJMYsd53ii+scO/bJGFsiCZc+5NDVN2yr8+0RDqr0Ql0h+rP48ckxlpbzKgwra6" crossorigin="anonymous">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">    
</head>

<body>
<div class='container'>
        <h1>Dream Hacker <hr></h1>
        
    </div>
<div class="container">
        <div class = "row">
            <div class = "col">
                <h3>Select Music</Select></h3>
                <hr>
            </div>
            <div class = "col-xs-2">
                <a href='index.php'><h4 class="backButton w3-hover-red">Back<h4></a>
                <hr>
            </div>
        </div>
</div>
<div class="container">
<form>
<div class="form-group">
    <div class="row">
      <div class="col-md-6">
      <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
          <br>
          <small id="passwordHelpBlock" class="form-text text-muted"> Descirbe what this song does</small>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
      </div>
      <div class="col-md-6">
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="exampleRadios" id="exampleRadios1" value="option1" checked>
          <label class="form-check-label" for="exampleRadios2">
          Music Name <br>
          <audio controls> <source src="audio/Just_The_Two_Of_Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
      </div>
      </div>
      </div>
      <div class="container" align=right>
        <button type="input" class="btn btn-secondary">Cancel</button>
        <button type="submit" class="btn btn-primary">Save</button>
      </div>
  </form>
</div>
</body>
</html>