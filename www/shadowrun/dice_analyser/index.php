<!DOCTYPE HTML>
<html>
<head>
	<title>Shadowrun Dice Analyser</title>
	<script src="http://tradingseas.com/js_utils/graph.js"></script>
	<meta charset="UTF-8"/>
	<style>
		body{
			background-color:#dde;
		}
		.before {
			color: red;
		}
		.after {
			color: blue;
		}
		.none {
			color: green;
		}
		.legend td {
			text-align: center;
		}
		.legend th {
			padding:0px 20px;
		}
		table {
			padding: 20px;
			float:left;
			border-style: solid none none none;
			border-width: 1px;
		}
		.legend {
			border-style: solid none none solid;
			border-width: 1px;
		}
	</style> 
	<script>
	var natural = [];
	var before = [];
	var after = [];
	var number_of_dice = 17;
	var edge = 4;
	var limit_enabled = false;
	var limit = 0;
	var opposed_enabled = false;
	var opposed_dice = 5;
	
	var max = 0;
	
	function db() {
		return Math.floor(Math.random()*100000) % 2;
	}
	
	function d6() {
		var n = 0;
		
		while(!(n >= 1 && n <=6)) {
			n = db() + db()*2 + db()*4;
		}
		
		return n;
	}
	function natural_roll(dice, apply_limit) {
		var i = 0;
		var hits = 0;
		for(i = 0; i < dice; i++) {
			var d = d6();
			if(d == 5 || d == 6) {
				hits++;
			}
		}
		
		if(apply_limit && hits > limit) {
			hits = limit;
		}
		return hits;
	}
	function before_roll(dice, edge_dice) {
		dice += edge_dice;
		var i = 0;
		var hits = 0;
		var explosions = 0;
		for(i = 0; i < dice; i++) {
			var d = d6();
			if(d == 5 || d == 6) {
				hits++;
			}
			if(d == 6) {
				dice++;
				explosions++;
			}
		}
		return hits;
	}
	function after_roll(dice, apply_limit) {
		var i = 0;
		var hits = 0;
		for(i = 0; i < dice; i++) {
			var d = d6();
			if(d == 5 || d == 6) {
				hits++;
			} else {
				var d2 = d6();
				if(d2 == 5 || d2 == 6) {
					hits++;
				}
				
			}
		}
		if(apply_limit && hits > limit) {
			hits = limit;
		}
		return hits;
	}
	var samples = 0;
	function new_number(draw) {
		
		var canvas = document.getElementById("canvas");
		
		var j;
		
		for(j = 0; j < 1000; j++) {
			var n = natural_roll(number_of_dice, limit_enabled);
			var b = before_roll(number_of_dice, edge);
			var a = after_roll(number_of_dice, limit_enabled);
			
			if(opposed_enabled) {
				var no = natural_roll(opposed_dice, false);
				var bo = natural_roll(opposed_dice, false);
				var ao = natural_roll(opposed_dice, false);
				
				n = n - no;
				b = b - bo;
				a = a - ao;
			}
			
			var max = Math.max(Math.max(n,Math.max(a,b)),0);
			var min = Math.min(Math.min(n,Math.min(a,b),0));
			
			
			while(min < natural["min"]) {
				natural.unshift({"key":(natural["min"] - 1), "value":0});
				natural["min"] = natural["min"] - 1;
				before.unshift({"key":(before["min"] - 1), "value":0});
				before["min"] = before["min"] - 1;
				after.unshift({"key":(after["min"] - 1), "value":0});
				after["min"] = after["min"] - 1;
			}
			
			if(!((max - natural["min"]) in natural)) {
				var k;
				for(k = 0; k <= max; k++) {
					
					if(!((k - natural["min"]) in natural)) {
						natural[k - natural["min"]] = {"key":k, "value":0};
					}
					if(!((k - before["min"]) in before)) {
						before[k - before["min"]] = {"key":k, "value":0};
					}
					if(!((k - after["min"]) in after)) {
						after[k - after["min"]] = {"key":k, "value":0};
					}
				}
			}
			natural[n - natural["min"]]["value"]++;
			before[b - before["min"]]["value"]++;
			after[a - after["min"]]["value"]++;
			
			samples++;
		}
		max = n;
		
		var n_avg = 0;
		var n_pos = 0;
		var n_tot = 0;
		var b_avg = 0;
		var b_pos = 0;
		var b_tot = 0;
		var a_avg = 0;
		var a_pos = 0;
		var a_tot = 0;
		var max_value = 0;
		for(j = 0; j < natural.length; j++) {
			n_avg += natural[j]["key"] * natural[j]["value"];
			n_tot += natural[j]["value"];
			if(natural[j]["key"] > 0) n_pos += natural[j]["value"];
			if(natural[j]["value"] > max_value) max_value = natural[j]["value"];
			
			b_avg += before[j]["key"] * before[j]["value"];
			b_tot += before[j]["value"];
			if(before[j]["key"] > 0) b_pos += before[j]["value"];
			if(before[j]["value"] > max_value) max_value = before[j]["value"];
			
			a_avg += after[j]["key"] * after[j]["value"];
			a_tot += after[j]["value"];
			if(after[j]["key"] > 0) a_pos += after[j]["value"];
			if(after[j]["value"] > max_value) max_value = after[j]["value"];
		}
		
		n_avg = n_avg/n_tot;
		b_avg = b_avg/b_tot;
		a_avg = a_avg/a_tot;
		
		n_pos = n_pos/n_tot;
		b_pos = b_pos/b_tot;
		a_pos = a_pos/a_tot;

		
		n_avg_line = [
			{"key":n_avg, "value":0},
			{"key":n_avg, "value":0}//(natural[Math.floor(n_avg)].value * (1 - (n_avg - Math.floor(n_avg))) + natural[Math.ceil(n_avg)].value * (n_avg - Math.floor(n_avg)))}
		];
		b_avg_line = [
			{"key":b_avg, "value":0},
			{"key":b_avg, "value":0}//(before[Math.floor(b_avg)].value * (1 - (b_avg - Math.floor(b_avg))) + before[Math.ceil(b_avg)].value * (b_avg - Math.floor(b_avg)))}
		];
		a_avg_line = [
			{"key":a_avg, "value":0},
			{"key":a_avg, "value":0}//(after[Math.floor(a_avg)].value * (1 - (a_avg - Math.floor(a_avg))) + after[Math.ceil(a_avg)].value * (a_avg - Math.floor(a_avg)))}
		];
		
		n_avg = Math.round(n_avg * 100)/100;
		b_avg = Math.round(b_avg * 100)/100;
		a_avg = Math.round(a_avg * 100)/100;
		
		document.getElementById("none_avg").innerHTML = n_avg;
		document.getElementById("before_avg").innerHTML = b_avg;
		document.getElementById("after_avg").innerHTML = a_avg;
		
		n_pos = Math.round(n_pos * 100);
		b_pos = Math.round(b_pos * 100);
		a_pos = Math.round(a_pos * 100);
		
		if(opposed_enabled) {
			document.getElementById("net_hits_none").innerHTML = n_pos + "%";
			document.getElementById("net_hits_before").innerHTML = b_pos + "%";
			document.getElementById("net_hits_after").innerHTML = a_pos + "%";
		}
		
		
		if(draw) {
			var graph = createGraphToFit(natural);
			canvas.width = 800;
			canvas.height = 350;
			var ctx = canvas.getContext('2d');
			graph.valueMax = max_value;
			graph.keyMax = graph.keyMax;
			graph.coordPadding = 50;
			graph.dataPointRadius = 2;
			graph.lineWidth = 1;
			graph.valueMin = graph.keyMin;
			graph.keyTicks = natural.length-1;
			graph.label_last_tick = false;
			
			graph.valueFormat = function(value) {
				return Math.round(value/n_tot * 100) + "%";
			};
			
			drawAxis(canvas, graph, "black");
			graph.dataPointColor = "green";
			drawGraph(canvas, graph, natural, "green");
			graph.dataPointColor = "red";
			drawGraph(canvas, graph, before, "red");
			graph.dataPointColor = "blue";
			drawGraph(canvas, graph, after, "blue");
			
			graph.dataPointRadius = 0;
			
			drawGraph(canvas, graph, n_avg_line, "green");
			drawGraph(canvas, graph, b_avg_line, "red");
			drawGraph(canvas, graph, a_avg_line, "blue");
		} else {
			console.log("Don't draw");
		}
		//document.getElementById("debug").innerHTML = samples + " Rolls";
		
	}
	
	var interval = false;
	
	function restart() {
		number_of_dice = parseInt(document.getElementById("number_of_dice").value);
		edge = parseInt(document.getElementById("edge_points").value);
		limit_enabled = document.getElementById("limit_enabled").checked;
		limit = document.getElementById("limit").value; 
		opposed_enabled = document.getElementById("opposed_enabled").checked;
		opposed_dice = document.getElementById("opposed_dice").value;
		natural = [];
		natural["min"] = 0;
		before = [];
		before["min"] = 0;
		after = [];
		after["min"] = 0;
		
		if(opposed_enabled) {
			document.getElementById("graph_label").innerHTML = "Probability Distribution of Net Hits";
			
			document.getElementById("net_hits_header").innerHTML = "Chance of Possitive Net Hits";
			document.getElementById("net_hits_none").innerHTML = "";
			document.getElementById("net_hits_before").innerHTML = "";
			document.getElementById("net_hits_after").innerHTML = "";
		} else {
			document.getElementById("graph_label").innerHTML = "Probability Distribution of Hits";
			
			document.getElementById("net_hits_header").innerHTML = "";
			document.getElementById("net_hits_none").innerHTML = "";
			document.getElementById("net_hits_before").innerHTML = "";
			document.getElementById("net_hits_after").innerHTML = "";
		}
	}
	
	function checkbox_updated() {
		document.getElementById("limit").disabled = !document.getElementById("limit_enabled").checked;
		document.getElementById("opposed_dice").disabled = !document.getElementById("opposed_enabled").checked;
	}
	
	function run(draw) {
		var i;
		if (interval) window.clearInterval(interval);
		
		new_number(draw);
		
		if(draw) {

			interval = window.setInterval(function () {run(true)},1);
		}
	}
	
	window.onload = function () {
		
		restart();
		run(true);
	}
	</script>
</head> 
<body id="main">
	<h1>Shadowrun Dice Analyser</h1> 
	<h2 id="graph_label">Probability Distribution of Hits</h2>
	<canvas id="canvas"></canvas>
	<br/>
	<table>
		<tr>
			<td></td>
			<td>Dice: </td><td><input type="text" id="number_of_dice" value="10"></td>
		</tr>
		<tr>
			<td></td>
			<td>Edge: </td><td><input type="text" id="edge_points" value="4"></td>
		</tr>
		<tr>
			<td><input type="checkbox" id="limit_enabled" onclick="checkbox_updated()"></td>
			<td>Limit: </td><td><input type="text" id="limit" value="5" disabled="disabled"></td>
		</tr>
		<tr>
			<td><input type="checkbox" id="opposed_enabled" onclick="checkbox_updated()"></td>
			<td>Opposed: </td><td><input type="text" id="opposed_dice" value="6" disabled="disabled"><td>
		</tr>
		<tr>
			<td></td>
			<td></td>
			<td><button type="button" onclick="restart()">Restart</button></td>
		</tr>
			
	</table>
	
	<table class="legend">
		<tr>
			<th></th><th>Average Hits</th><th id="net_hits_header"></th>
		</tr>
		<tr>
			<th class="none">Normal</th> <td id="none_avg" ></td><td id="net_hits_none"></td>
		</tr>
		<tr>
			<th class="before">Push the Limit</th> <td id="before_avg" ></td><td id="net_hits_before"></td>
		</tr>
		<tr>
			<th class="after">Second Chance</th> <td id="after_avg" ></td><td id="net_hits_after"></td>
		</tr>
	</table>

	<div id="debug"></div>
	<div id="new"></div>
</body>
</html>
