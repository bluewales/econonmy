
<!DOCTYPE HTML>
<html>
<head>
	<title>Primes</title>
	<script src="graph.js"></script>
	<meta charset="UTF-8"/>
	<style>
		body{
			background-color:#effefe;
		}
		canvas {
			
		}
		p {
			margin:0px 100px;
		}
		.blue {
			color:blue;
			font-weight: bold;
		}
		.yellow {
			color:yellow;
			font-weight: bold;
		}
		.purple {
			color:purple;
			font-weight: bold;
		}
		.red {
			color:red;
			font-weight: bold;
		}
		
	</style>
</head>
<body>
<header>
	<h1>Primes</h1>
</header>
<script>

<?php

error_reporting(E_ALL);
ini_set('display_errors', 1);

date_default_timezone_set("America/Denver");

$script_start = microtime( true );
//printf( "%s --- Starting ...\n", date( 'r' ) );

function submit_query($conn, $sql) {
	//echo "<p>" . $sql . "</p>\n";

	$result = $conn->query($sql); 

	if($conn->error) {
		echo "<p>MYSQL ERROR</p>\n";
		echo "<p>" . $sql . "</p>\n";
		echo "<p>" . $conn->error . "</p>\n";
	} else {
		//echo "<p>no error</p>\n";
	}
	
	return $result;
}

$conn = new mysqli("localhost", "root", "rootpass", "test");


$sql = "
  select 
	rank,
	tweets,
	UNIX_TIMESTAMP(parse_date) as timestamp,
	mine
  from top_twitters
  where
	((tweets <= 1300000 and tweets >= 900000) or rank = 0) and UNIX_TIMESTAMP(parse_date) > 1441451520
";


$result = submit_query($conn, $sql);
if(!$result) {
	echo $conn->error;
}

$out_string = "";
	
$first = true;
$obj = array();
$obj['mine'] = array();
while($row = $result->fetch_assoc()) {
	
		
	
		if(!isset($obj[$row['rank']])) {
			$obj[$row['rank']] = array();
		}
		
		$key = (int)$row['timestamp'];
		$value = (int)$row['tweets'];
		//$value = floor(log((int)$row['tweets'],1.001));
		
		if($row['mine'] == "1") {
			$obj['mine'][] = array("key"=>$key,"value"=>$value);
		}
		
		$obj[$row['rank']][] = array("key"=>$key,"value"=>$value);
}

	
	

echo "var data=" . json_encode($obj) . ";\n";

//printf( "Done in %d seconds!\n", microtime( true ) - $script_start );

?>
function make_graph() {
	
	for (var rank in data) {
		var graph = createGraphToFit(data[rank]);
		break;
	}
	
	var real_value_max = graph.valueMax;
	var real_value_min = graph.valueMin;
	var real_key_max = graph.keyMax;
	var real_key_min = graph.keyMin;
	
	for (var rank in data) {
		graph = createGraphToFit(data[rank]);
		
		if(graph.valueMax > real_value_max)
			real_value_max = graph.valueMax;
		if(graph.valueMin < real_value_min)
			real_value_min = graph.valueMin;
		if(graph.keyMax > real_key_max)
			real_key_max = graph.keyMax;
		if(graph.keyMin < real_key_min)
			real_key_min = graph.keyMin;
	}
	
	
	var canvas = document.getElementById("myCanvas");
	canvas.width = 1200;
	canvas.height = 600;
	var ctx = canvas.getContext('2d');
	
	graph.valueMax = real_value_max + 1;
	graph.valueMin = real_value_min;
	graph.keyMax = real_key_max + 1; 
	graph.keyMin = real_key_min; 
	
	graph.dataPointRadius = 2;
	graph.coordPadding = 100;

	
	
	drawAxis(canvas, graph, "black");
	for (var rank in data) {
	
		if(rank == "0") {
			
		} else if(rank == "mine") {
			
		} else {
			
			
			graph.dataPointColor = "blue";
			graph.dataPointRadius = 2;
			graph.lineWidth = 2;
			drawGraph(canvas, graph, data[rank], "blue");
		}
	}
		for (var rank in data) {
	
		if(rank == "0") {
			graph.dataPointColor = "red";
			graph.dataPointRadius = 3;
			graph.lineWidth = 3;
			drawGraph(canvas, graph, data[rank], "red");
			
		} else if(rank == "mine") {
			graph.dataPointColor = "yellow";
			graph.dataPointRadius = 2;
			graph.lineWidth = 0;
			drawGraph(canvas, graph, data[rank], "yellow");
			
		} else {
			
		}
	}
	

}

window.onload = function() {
	make_graph();
}


</script>
	<canvas id="myCanvas"></canvas >
	<p id="debug"></p>
</body>
</html>

