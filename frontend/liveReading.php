<?php
    define("ROOTPATH", 'C:/Apache24/htdocs/dreamHacker');
    //include ROOTPATH . '/database/db.php';
    session_start();

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
        <div class = "row">`
            <div class = "col-md-11">
                <h3>Live Reading</h3>
                <hr>
            </div>
            <div class = "col-md-1">
            <a href='index.php'><p class="backButton w3-hover-red">Back<p></a>
             </div>
        </div>
    </div>
    <div class="container">
        <div class="conatiner">
            <div class = "row">
                <div class = "col-md-8 vl">
                    <div id="chartContainer" style="height: 370px; width: 100%;"></div>
                </div>
                <div class = "col-md-4" align=center>
                    <h4>Time In bed</h4>
                    <p id='bed'><</p>
                    <hr>
                    <h4>Song Choosen</h4>
                    <p id='song'>Vivaldi</p>
                    <hr>
                    <h4>Rem Sleep Detected</h4>
                    <p id='rem'>Yes</p>
                    <hr>
                    <p class='backButton w3-hover-red'> Stop Reading<p>
                    <p><?php echo $dataPoints ?></p>
                </div>
            </div>
        </div>
    </div>
    <script src="https://canvasjs.com/assets/script/canvasjs.min.js"></script>
</body>

</html>
