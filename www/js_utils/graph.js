function createGraphToFit(data) {
	var graph = new Object();
	
	graph.keyMax = data[0].key;
	graph.keyMin = data[0].key;
	graph.valueMax = data[0].value;
	graph.valueMin = data[0].value;
	graph.dataPoints = data.length;
	graph.type = "line";
	
	for(var i = 0; i < graph.dataPoints; i++) {
		if(data[i].key > graph.keyMax)
			graph.keyMax = data[i].key;
		if(data[i].key < graph.keyMin)
			graph.keyMin = data[i].key;
		if(data[i].value > graph.valueMax)
			graph.valueMax = data[i].value;
		if(data[i].value < graph.valueMin)
			graph.valueMin = data[i].value;
	}
	graph.coordPadding = 30;
	graph.keyFormat = function(key) {
		return Math.round(key);
	};
	graph.valueFormat = function(value) {
		return Math.round(value);
	};
	graph.keyTicks = 10;
	graph.valueTicks = 10;
	graph.label_last_tick = true;
	graph.lineWidth = 1;
	graph.dataPointRadius = 0;
	graph.dataPointColor = "black";
	return graph;
}

function drawAxis(canvas, graph, color) {
	var margin = 10;
	var xMin = graph.coordPadding + margin;
	var xMax = canvas.width - margin;
	var yMin = margin;
	var yMax = canvas.height - (graph.coordPadding + margin);
	
	var ctx = canvas.getContext('2d');
	
	ctx.strokeStyle = color;
	//draw boundry of graph
	ctx.strokeRect(margin + graph.coordPadding,  
					margin, 
					canvas.width - (2 * margin + graph.coordPadding),
					canvas.height - (2 * margin + graph.coordPadding));
					
	// draw tick marks
	
	var steps = 10;
	
	ctx.fillStyle = "black";
	ctx.font = "bold 12px sans-serif"
	ctx.textAlign = "left";
	
	ctx.beginPath();
	for(var i = 0; i < graph.keyTicks; i++)
	{
		var xTick = xMin + ((xMax - xMin)*(i/graph.keyTicks));
		ctx.moveTo(xTick,yMax);
		ctx.lineTo(xTick,yMax + 10);
		
		var index = graph.keyFormat(graph.keyMin + ((graph.keyMax-graph.keyMin)/graph.keyTicks) * i);
		ctx.fillText(index, xTick + 2, yMax + 12);
	}
	ctx.textAlign = "right";
	if(graph.label_last_tick) {
		ctx.moveTo(xMax,yMax);
		ctx.lineTo(xMax,yMax + 10);
		ctx.fillText(graph.keyFormat(graph.keyMax), xMax - 2, yMax+12);
	}
	for(var i = 0; i < graph.valueTicks; i++)
	{
		var yTick = yMax - (yMax - yMin)*(i/graph.valueTicks);
		ctx.moveTo(xMin,yTick);
		ctx.lineTo(xMin - 10, yTick);
		
		var index = graph.valueFormat(graph.valueMin + ((graph.valueMax-graph.valueMin)/graph.valueTicks) * i);
		ctx.fillText(index, xMin - 2, yTick - 2);
	}
	ctx.fillText(graph.valueFormat(graph.valueMax), xMin - 2, yMin);
	ctx.stroke();
	
	// draw horizontal lines
	ctx.beginPath();
	ctx.globalAlpha = 0.5;
	for(var i = 0; i < (yMax-yMin); i += (yMax-yMin)/graph.valueTicks)
	{
		ctx.moveTo(xMin,yMax - i);
		ctx.lineTo(xMax, yMax - i);
	}
	ctx.stroke();
	ctx.globalAlpha = 1;
}

function drawGraph(canvas, graph, data, color) {
	
	var margin = 10;
	var xMin = graph.coordPadding + margin;
	var xMax = canvas.width - margin;
	var yMin = margin;
	var yMax = canvas.height - (graph.coordPadding + margin);
	
	var ctx = canvas.getContext('2d');
	ctx.strokeStyle = color;
	ctx.fillStyle = color;
	
	//ctx.lineTo(x,y);
	if(graph.type == "line") {
		
		ctx.beginPath();
		ctx.lineWidth=graph.lineWidth;
		
		if ( graph.lineWidth > 0) {
			for (var i = 0; i < data.length; i++) {
				var x = xMin + ((xMax - xMin)/(graph.keyMax - graph.keyMin)) * (data[i].key - graph.keyMin);
				var y = yMin + ((yMax - yMin)/(graph.valueMax - graph.valueMin)) * (data[i].value - graph.valueMin);
				
				if(y < yMin) y = yMin;
				if(y > yMax) y = yMax
				y = (canvas.height - graph.coordPadding) - y;
				if(x <= xMax && x >= xMin) {
					ctx.lineTo(x,y);
				}
			}
			ctx.stroke();
		}
		if (graph.dataPointRadius > 0) {
			for (var i = 0; i < data.length; i++) {
				var x = xMin + ((xMax - xMin)/(graph.keyMax - graph.keyMin)) * (data[i].key - graph.keyMin);
				var y = yMin + ((yMax - yMin)/(graph.valueMax - graph.valueMin)) * (data[i].value - graph.valueMin);
				
				if (y < yMin) y = yMin;
				if (y > yMax) y = yMax
				y = (canvas.height - graph.coordPadding) - y;
				if(x <= xMax && x >= xMin) {
					ctx.beginPath();
					ctx.arc(x, y, graph.dataPointRadius, 0, 2 * Math.PI, false);
					ctx.strokeStyle = graph.dataPointColor;
					ctx.stroke();
				}
			}
		}
	} else if(graph.type == "bar") {
		
		ctx.beginPath();
		ctx.lineWidth=graph.lineWidth;
		
		for (var i = 0; i+1 < data.length; i++) {
			var x = xMin + ((xMax - xMin)/(graph.keyMax - graph.keyMin)) * (data[i].key - graph.keyMin);
			var next_x = xMin + ((xMax - xMin)/(graph.keyMax - graph.keyMin)) * (data[i+1].key - graph.keyMin);
			var y = yMin + ((yMax - yMin)/(graph.valueMax - graph.valueMin)) * (data[i].value - graph.valueMin);
			
			if(y < yMin) y = yMin;
			if(y > yMax) y = yMax
			y = (canvas.height - graph.coordPadding) - y;
			var width = (next_x - x);
			width *= .9;
			ctx.rect(x,y,width,yMax - y);
			ctx.fill();
			
		}
	}
}