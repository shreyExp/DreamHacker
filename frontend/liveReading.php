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


    $dataPoints = array();
    $y = 5;
    for($i = 0; $i < 10; $i++){
	    $y += rand(-1, 1) * 0.1; 
	    array_push($dataPoints, array("x" => $i, "y" => $y));
    }
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
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.1.0/dygraph.min.js"></script>
<link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.1.0/dygraph.min.css" />

<script>
window.onload = function() {
 
var dataPoints = <?php echo json_encode($dataPoints, JSON_NUMERIC_CHECK); ?>;
 
var chart = new CanvasJS.Chart("chartContainer", {
	theme: "light2",
	title: {
		text: "Live Heart Rate Monitor"
	},
	axisX:{
		title: "Time in second"
	},
	axisY:{
		suffix: " bpm"
	},
	data: [{
		type: "line",
		yValueFormatString: "#,##0.0#",
		toolTipContent: "{y} Mbps",
		dataPoints: dataPoints
	}]
});
chart.render();
 
var updateInterval = 1500;
setInterval(function () { updateChart() }, updateInterval);
 
var xValue = dataPoints.length;
var yValue = dataPoints[dataPoints.length - 1].y;
 
function updateChart() {
	yValue += (Math.random() - 0.5) * 0.1;
	dataPoints.push({ x: xValue, y: yValue });
	xValue++;
	chart.render();
};
 
}
</script>
<head>
    <div class='container'>
        <h1>Dream Hacker <hr></h1>
    </div>
</head>
<body>
    <div class="container">
        <div class = "row">
            <div class = "col-md-11">
                <h3>Live Reading</h3>
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
?>
    <div class="container">
        <div class = "row">
           <h3><span id="beats">00</span> beats per minute</h3>
       </div>
   </div>
    <div class="container">
        <div class="conatiner">
            <div class = "row">
                <div class = "col-md-10 vl">
                    <div id="div_g" style="height: 370px; width: 100%;"></div>
                </div>
                <div class = "col-md-2" align=center>
                    <h4><strong>Song Choosen</strong></h4>
                    <p id='song'><?= $record['song'] ?></p>
                    <hr>
                </div>
            </div>
        </div>
    </div>
    <?php endfor; }?>
    <br />
    <br />

    <script type="text/javascript">
      // max samples for dygraph
      var maxSamples = 60;

      // relative path to the sensor data for get/post:
      var serverPath = "/sensor/:80";
      
      // callback when the Web page has been loaded
      jQuery(document).ready(function() {
          var data = [];
          var g = new Dygraph(document.getElementById("div_g"), data,
                  {
                      drawPoints: true,
                      labels: ['Time', 'BPM', 'Threshhold'],
                      legend: 'always',
                      title: 'Live Beats per minute monitor',
                      ylabel: 'Beats (BPM)',
                      xlabel: 'Time (Seconds)',
                      labelsDivStyles: {
                        'text-align': 'right',
                        'background': 'none'
                      },
                      strokeWidth: 2
                  });
          
          window.intervalId = setInterval(function() {
          // callback for interval timer for every second
          $.getJSON(serverPath,function(result){
              // callback after the php script has been called
              var utcSeconds = result.epoch;
              var d = new Date(0);
              d.setUTCSeconds(utcSeconds);
              var x = d;
              var y = result.beats;
              var threshHold = result.threshold
              document.getElementById("beats").innerHTML = Math.round(y * 100) / 100;
              if (data.length > maxSamples) {
              data.shift();
              }
              data.push([x, y, threshHold]);
              g.updateOptions( { 'file': data } );
          });
          }, 1000);
      });
    </script>
    <br />
    <br />
    <br />
    <br />
</body>

</html>