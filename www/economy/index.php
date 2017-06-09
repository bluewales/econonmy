<!DOCTYPE HTML>
<html>
<head>
	<title>Economy Simulator</title>
	<script src="http://tradingseas.com/js_utils/graph.js"></script>
	<meta charset="UTF-8"/>
	<style>
		body{
			background-color:#ddd;
		}
		canvas {
			float:left;
		}
		div{
			float:right;
			width:300px;
			border-radius: 2px;
			border-style:solid;
			border-width:2px;
			margin:10px;
		}
		div a{
			float:left;
			padding:0 20px 0 20px;
		}
	</style>
</head>
<body>
<header>
	<h1>Economic Simulator</h1>
</header>
	
		<?php
		echo "<script>";
		if(isset($_GET['good'])) {
			echo "var start_good = \"" .  $_GET['good'] . "\";";
		} else {
			echo "var start_good = \"wood\";";
		}
		if(isset($_GET['city'])) {
			echo "var all_data = [" .  substr(file_get_contents("city_" . $_GET['city'] . ".json"), 0, -1). "];";
		} else {
			if(file_exists("city_1.json")) {
				
				echo "var all_data = [" .  substr(file_get_contents("city_1.json"), 0, -1). "];";
			}
		}
		echo "</script>";
		echo "<p>Click on a city to view statistics.</p>";
		for($i = 0; $i < 100; $i++) {
			if(file_exists("city_$i.json")) {
				echo "<a href=\"?city=$i\" > City $i</a>";
				
			}
		}
		?>
	<script>
	var primary = "price";
	var good;
	var metrics = [];
	
	function set_primary(p) {
		primary = p;
		make_graph(good);
	}
	function make_graph(name) {
		good = name;
		var colors = ["red","blue","green","orange","purple","brown","black"];
		
		
		metrics = [];
		for (var i = 1; i < all_data.length; i++) {
			
			for (var x in all_data[i][name]) {
				
				if(!metrics[x]) {
					metrics[x] = [];
				}
				metrics[x][metrics[x].length] = {"key":i, "value":all_data[i][name][x]};
			}
		}
		
		
		for (var x in metrics) {
			metrics[x].checked = document.getElementById(x+'_box').checked;
		}
		
		
		document.getElementById("header").innerHTML = primary + " of " + name;
		
		var graph = createGraphToFit(metrics[primary]);
		var canvas = document.getElementById("myCanvas");
		canvas.width = 1000;
		canvas.height = 500;
		var ctx = canvas.getContext('2d');
		graph.valueMax += 1;
		graph.valueMin = 0;
		graph.keyMax = graph.keyMax+1; 
		graph.coordPadding = 50;
		graph.dataPointRadius = 0;
		if(graph.valueMax < graph.valueTicks)
			graph.valueTicks = graph.valueMax;
		graph.dataPointColor = "blue";
		drawAxis(canvas, graph, "black");
		var i = 0;
		for (var x in metrics) {
			if(metrics[x]["checked"])
				drawGraph(canvas, graph, metrics[x], colors[i++]);
			if(i > colors.length) i = 0;
		}
	}
	
	window.onload = function() {
		if(!all_data) return;
		for(var key in all_data[1]) {
			document.getElementById("keys").innerHTML += "<a href=\"javascript:make_graph('" 
			+ key + "')\">" + key + "</a>";
		}
		for (var x in all_data[1][start_good]) {
			document.getElementById("lines").innerHTML += 
				"<a href=\"javascript:set_primary('" + x + 
				"')\"><input checked=\"true\" type=\"checkbox\" id=\"" + x + "_box\">" + x + "</a>";
		}
		
		make_graph(start_good);
	}
	</script>
	<h2 id="header"></h2>
	<canvas id="myCanvas"></canvas >
	<p>Goods:</p>
	<div id="keys"></div>
	<p>Metrics:</p>
	<div id="lines"></div>
	<p id="debug"></p>
</body>
</html>
