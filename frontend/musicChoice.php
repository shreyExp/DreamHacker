<?php
    define("ROOTPATH", '/var/www/html/DreamHack/frontend');
    include ROOTPATH . '/database/db.php';
    require ROOTPATH . '/functions/paginator.php';
    session_start();
    $conn = mysqli_connect($host, $user, $pass, $db);
    $query = "SELECT * FROM audio";

    //these variables are passed via URL
    $limit = ( isset( $_GET['limit'])) ? $_GET['limit'] : 1; // items per page
    $page = (isset ($_GET['page'])) ? $_GET['page'] : 1; //starting page
    $links = 10;

    $paginator = new Paginator ( $mysqli, $query); //__constructor is called
    $results = $paginator->getData( $limit, $page);

if ($_SERVER['REQUEST_METHOD'] == 'POST') 

{
    if (isset($_POST['save'])) { //user logging in



        require 'audioPlayer.php';

        

    }
}

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
<?php
    $y=0;
    if ($y == ($results->data)){
        echo'
            <div class="app-container">
                <div class="row">
                    <h1>No results found</h1>
                </div>
              <hr>
            </div>';
    } 
                else {
    for ($p = 0; $p < count($results->data); $p++): ?>
<?php
    $record = $results->data[$p];
    if ($results->num_rows = 0) {
        echo "No recrods Found";
    }
?>

<div class="container">
        <div class = "row">
            <div class = "col">
                <h3>Select Audio</Select></h3>
                <small class="form-text- text-muted"> Currently selected audio: <?= $record['song'] ?></small>
                <hr>
            </div>
            <div class = "col-xs-2">
                <a href='index.php'><h4 class="backButton w3-hover-red">Back<h4></a>
            </div>
        </div>
</div>
<?php endfor; }?>
<div class="container">
<form action="audioPlayer.php" method="post">
  <div class="form-group">
    <div class="row">
      <div class="col-md-6">
      <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song"  value="Just The Two Of Us" checked>
          <label class="form-check-label">
          Just The Two Of Us <br>
          <audio controls> <source src="audio/Just The Two Of Us.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song" value="Sunny" checked>
          <label class="form-check-label" for="exampleRadios2">
          Sunny <br>
          <audio controls> <source src="audio/Sunny.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song" value="Sunday Vibes" checked>
          <label class="form-check-label" for="exampleRadios2">
          Sunday Vibes <br>
          <audio controls> <source src="audio/Sunday Vibes.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song" value="Sleep Music" checked>
          <label class="form-check-label" for="exampleRadios2">
          Sleep Music <br>
          <audio controls> <source src="audio/Sleep Music.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
      </div>
      <div class="col-md-6">
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song" value="Viento Estelar" checked>
          <label class="form-check-label" for="exampleRadios2">
          Viento Estelar <br>
          <audio controls> <source src="audio/Viento Estelar.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song" value="In My Time" checked>
          <label class="form-check-label" for="exampleRadios2">
          In My Time <br>
          <audio controls> <source src="audio/In My Time.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song" value="Fracture" checked>
          <label class="form-check-label">
          Fracture <br>
          <audio controls> <source src="audio/Fracture.mp3" type="audio/mp3"></audio>
          </label>
        </div>
        <!-- Music Choice -->
        <div class="form-check">
          <input class="form-check-input" type="radio" name="song" value="Sunset at the Veranda" checked>
          <label class="form-check-label">
          Sunset at the Veranda <br>
          <audio controls> <source src="audio/Sunset at the Veranda.mp3" type="audio/mp3"></audio>
          </label>
        </div>
      </div>
      </div>
      </div>
      <div class="container" align=right>
        <button type="button" onclick="document.location='index.php'" class="btn btn-secondary">Cancel</button>
        <button type="submit" class="btn btn-primary" name="save">Save</button>
      </div>
  </form>
</div>
</body>
</html>