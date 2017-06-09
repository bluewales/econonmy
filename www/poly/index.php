<!DOCTYPE>
<html>
<head>
</head>
<body>
<h1>n = <?php echo $_GET['n'];?></h1>
<canvas id="the_canvas">
</body>
<script>

var n = <?php echo $_GET['n'];?>;



var width = Math.max(800, n * 4);
var height = width;

var cell = Math.min(width, height) / n;

var polygon = [];


function circle(ctx, x, y, r) {
	ctx.beginPath();
	ctx.arc(x, y, r, 0, 2 * Math.PI, false);
	ctx.fillStyle = 'green';
	ctx.fill();
	ctx.lineWidth = cell / 10;
	ctx.strokeStyle = '#003300';
	ctx.stroke();
}

function c(i) {
	return cell * (i-1) + cell/2;
}

function draw_vertex(ctx, p) {
	circle(ctx, c(p[0]), c(p[1]), cell/4)
}

function draw_line(ctx, p1, p2) {
	
	
	ctx.beginPath();
	
	ctx.moveTo(c(polygon[0][0]),c(polygon[0][1]));
	
	for(var i = 1; i < n; i++) {
		ctx.lineTo(c(polygon[i][0]),c(polygon[i][1]));
	}
	ctx.lineTo(c(polygon[0][0]),c(polygon[0][1]));
	
	
	
	
	ctx.lineWidth = cell / 5;
	ctx.strokeStyle = '#003300';
	ctx.stroke();
	
}

function draw_grid(ctx) {
	
	ctx.lineWidth = 2;
	ctx.beginPath();
	
	for(var i = 0; i < n+1; i++) {
		ctx.moveTo(cell * i,0);
		ctx.lineTo(cell * i,cell * n);
		
		ctx.strokeStyle = '#999999';
	}
	
	for(var i = 0; i < n+1; i++) {
		ctx.moveTo(0,cell * i);
		ctx.lineTo(cell * n,cell * i);
		
	}
	ctx.stroke();
}

function generate_polygon() {
	
	var xs = [];
	var ys = [];
	for(var i = 0; i < n; i++){
		xs.push(0)
		ys.push(0)
	}
	
	var count = 0;
	
	for(var i = 0; i < n; i++){
		do {
			var r = Math.floor(Math.random()*n);
			
			count++
		} while(xs[r] != 0);
		xs[r] = i+1;
		
		do {
			var r = Math.floor(Math.random()*n);
		} while(ys[r] != 0);
		ys[r] = i+1;
		
	}
	
	for(var i = 0; i < n; i++){
		polygon[i] = [xs[i],ys[i]]
	}
	
}

function line_intersection(line1, line2) {
	var xdiff = [line1[0][0] - line1[1][0], line2[0][0] - line2[1][0]]
	var ydiff = [line1[0][1] - line1[1][1], line2[0][1] - line2[1][1]]

	function det(a, b) {
		return a[0] * b[1] - a[1] * b[0]
	}

	var div = det(xdiff, ydiff)
	if(div == 0)
		return false
	
	var d = [det(line1[0],line1[1]), det(line2[0],line2[1])]
	
	var x = det(d, xdiff) / div
	var y = det(d, ydiff) / div
	return [x, y]
}


function segments_intersect(line1, line2) {

	intersect = line_intersection(line1, line2)
	if(intersect === false) {
		return false;
	}
		
	if(intersect[0] < line1[0][0] && intersect[0] < line1[1][0])
		return false
	if(intersect[0] > line1[0][0] && intersect[0] > line1[1][0])
		return false
	if(intersect[1] < line1[0][1] && intersect[1] < line1[1][1])
		return false
	if(intersect[1] > line1[0][1] && intersect[1] > line1[1][1])
		return false
		
	if(intersect[0] < line2[0][0] && intersect[0] < line2[1][0])
		return false
	if(intersect[0] > line2[0][0] && intersect[0] > line2[1][0])
		return false
	if(intersect[1] < line2[0][1] && intersect[1] < line2[1][1])
		returnfalse
	if(intersect[1] > line2[0][1] && intersect[1] > line2[1][1])
		return false
	return true
}


function untangle() {
	for(var i = 0; i < n; i++) {
		for(var j = 0; j < n; j++) {
			if(i >= j || i+1 == j || (j == n-1 && i == 0)) {
				continue;
			}
		
			var pointA = [polygon[i][0],polygon[i][1]];
			var pointB = [polygon[(i+1)%n][0],polygon[(i+1)%n][1]];
			
			var pointC = [polygon[j][0],polygon[j][1]];
			var pointD = [polygon[(j+1)%n][0],polygon[(j+1)%n][1]];
			
			var line1 = [pointA, pointB];
			var line2 = [pointC, pointD];
			
			var is_intersect = segments_intersect(line1, line2);
			
			if(is_intersect) {
				
				for(var swap = 0; swap < ((j - i)/2); swap += 1) {
					var temp = polygon[i+1+swap];
					polygon[i+1+swap] = polygon[j-swap];
					polygon[j-swap] = temp;
				}
				return false;
			}
		}
	}
	return true;
}


function fix_slopes() {
	
	for(var i = 0; i < n; i++) {
		for(var j = 0; j < n; j++) {
			if(i >= j){
				continue;
			}
			i_slope = (polygon[(i+1)%n][1]-polygon[i][1]) / (polygon[(i+1)%n][0]-polygon[i][0])
			j_slope = (polygon[(j+1)%n][1]-polygon[j][1]) / (polygon[(j+1)%n][0]-polygon[j][0])
			
			
			if(i_slope == j_slope) {
			
				ps = [i,(i+1)%n,j,(j+1)%n]
				p = ps[Math.floor(Math.random()*4)]
				dir = Math.floor(Math.random()*2)
				axis = 1;//Math.floor(Math.random()*2)
				jump = Math.floor(Math.random()*(n/2))
				
				if(dir==0) {
					dir = -1
				}
				//dir *= jump
				
				if(polygon[p][axis]+dir >= n || polygon[p][axis]+dir <= 0) {
					dir *= -1
				}
				
				for(var k = 0; k < n; k++) {
					if(polygon[k][axis] == polygon[p][axis]+dir) {
						polygon[k][axis] = polygon[p][axis]
						polygon[p][axis] = polygon[p][axis]+dir
						break;
					}
				}
				return false
			}
		}
	}
	return true
}


function draw(ctx) {
	
	
	
	slope_problem = false

	if(untangle()) {
		if(fix_slopes()) {
			console.log(JSON.stringify(polygon))
		}
	}
	
	
	
	draw_grid(ctx)
	
	
	draw_line(ctx);
	
	for(var i = 0; i < n; i++) {
		draw_vertex(ctx, polygon[i]);
	}
}

function frame() {
	
	var c = document.getElementById("the_canvas");
	var ctx = c.getContext("2d");
	ctx.clearRect(0, 0, c.width, c.height);
	draw(ctx)
}

window.onload = function() {
	var c = document.getElementById("the_canvas");
	c.width = width;
	c.height = height;
	
	generate_polygon()
	
	var id = setInterval(frame, 1);
	frame()
	
	
}
</script>
</html>