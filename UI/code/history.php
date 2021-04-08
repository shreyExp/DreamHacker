<?php
    define("ROOTPATH", 'C:/Apache24/htdocs/dreamHacker');
    include ROOTPATH . '/database/db.php';
    require ROOTPATH . '/functions/paginator.php';
    session_start();

    $conn = mysqli_connect($host, $user, $pass, $db);
    $query = "SELECT * FROM records ORDER BY 'date'";

    //these variables are passed via URL
    $limit = ( isset( $_GET['limit'])) ? $_GET['limit'] : 10; // items per page
    $page = (isset ($_GET['page'])) ? $_GET['page'] : 1; //starting page
    $links = 10;

    $paginator = new Paginator ( $mysqli, $query); //__constructor is called
    $results = $paginator->getData( $limit, $page);
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
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">       
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">    
</head>

<body>
<div class='container'>
        <h1>Dream Hacker <hr></h1>
        
    </div>
<div class="container">
        <div class = "row">`
            <div class = "col-md-11">
                <h3>History</h3>
                <hr>
            </div>
            <div class = "col-md-1">
            <a href='index.php'><p class="backButton w3-hover-red">Back<p></a>
             </div>
        </div>
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
<table align=center>
    <tr>
        <th> Date</th>
        <th> Rem Sleep Detected</th>
        <th> Avg Heart Rate</th>
        <th> Hours Slept</th>
        <hr>
    </tr>
    <tr>
        <td> <?= $record['date'] ?></td>
        <td> <?= $record['remSleep'] ?></td>
        <td> <?= $record['heartRate'] ?></td>
        <td> <?= $record['hrsSlept'] ?></td>
    </tr>
</table>             
<?php endfor; ?>
        </div>
    
    <div align=center> 
        <?php 
            echo $paginator->createLinks($links, 'pagination pagination-sm');
        }
        ?>
    </div>
</body>
</html>