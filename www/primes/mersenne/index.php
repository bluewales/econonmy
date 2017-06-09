<!DOCTYPE HTML>
<html>
<head>
	<title>Mersenne Primes</title>
	<script src="/primes/graph.js"></script>
	<meta charset="UTF-8"/>
	<style>
		body{
			background-color:#efefef;
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
	<h1>Mersenne Primes</h1>
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

$conn = new mysqli("localhost", "root", "rootpass", "primes");

$sql = "
  select 
	id, p, duration, prime, last_updated, started, progress
  from mersenne
  where
    started is not null
";

$result = submit_query($conn, $sql);
if(!$result) {
	echo $conn->error;
}

$data = array();
while($row = $result->fetch_assoc()) {
	$data[] = $row;
}

echo "var data=" . json_encode($data) . ";\n";

?>

function make_graph() {
	
	var max_time = 0;
	var max_prime = 0;
	
	for (var index in data) {
		
		var prime = data[index];
		prime['p'] = prime['p'] * 1;
		prime['duration'] = prime['duration'] * 1;
		
		if(prime['p'] > max_prime) max_prime = prime['p'];
		if(prime['duration'] > max_time) max_time = prime['duration'];
		
		data[index]['projected_duration'] = prime['duration'] * prime['p'] / prime['progress'];
		
		if(data[index]['projected_duration'] > max_time) max_time = data[index]['projected_duration'];
		
	}
	
	console.log(max_prime);
	
	//var max_time = 60 * 60 * 24 * 20;
	
	
	graph = createGraphToFit([{"key":0,"value":0}]);
	
	
	var canvas = document.getElementById("myCanvas");
	canvas.width = 1200;
	canvas.height = 600;
	var ctx = canvas.getContext('2d');
	
	graph.valueMax = max_time + max_time/20;
	graph.valueMin = 0;
	graph.keyMax = max_prime + max_prime/20; 
	graph.keyMin = 0; 
	
	
	graph.coordPadding = 100;
	
	graph.valueFormat = function(value) {
		if(value > (60 * 60 * 24 * 365)) {
			return Math.floor(Math.round(value) / (60 * 60 * 24 * 365)) + " y";
		} else if(value > (60 * 60 * 24 * 3)) {
			return Math.floor(Math.round(value) / (60 * 60 * 24)) + " d";
		} else if(value > (60 * 60)) {
			return Math.floor(Math.round(value) / (60 * 60)) + " h";
		} else if(value > 60) {
			return Math.floor(Math.round(value) / 60) + " m";
		}
		return Math.round(value) + " s";
	};
	
	graph.keyTicks = 16;
	
	drawAxis(canvas, graph, "black");
	for (var index in data) {
		var prime = data[index];
		var line = [{"key":prime['p'],"value":0},{"key":prime['p'],"value":prime['duration']}];
		
		if(prime['prime']) {
			if(prime['prime'] == 0) {
				graph.dataPointColor = "blue";
				graph.dataPointRadius = 1;
				graph.lineWidth = 1;
				drawGraph(canvas, graph, line, "blue");	
			} else {
				
			}
		} else {
			
			var projected = [{"key":prime['p'],"value":0},{"key":prime['p'],"value":prime['projected_duration']}];
			
			graph.dataPointColor = "grey";
			graph.dataPointRadius = 1;
			graph.lineWidth = 1;
			drawGraph(canvas, graph, projected, "grey");
			
			
			graph.dataPointColor = "red";
			graph.dataPointRadius = 0;
			graph.lineWidth = 3;
			drawGraph(canvas, graph, line, "red");
		}
	}
	for (var index in data) {
		var prime = data[index];
		var line = [{"key":prime['p'],"value":0},{"key":prime['p'],"value":prime['duration']}];
		
		if(prime['prime']) {
			if(prime['prime'] == 0) {
				
			} else {
				graph.dataPointColor = "green";
				graph.dataPointRadius = 2;
				graph.lineWidth = 2;
				drawGraph(canvas, graph, line, "green");	
			}
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

