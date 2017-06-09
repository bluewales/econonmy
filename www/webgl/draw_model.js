function createNewSegment(vertexPositions, vertexNormals, vertexCount, vertexIndecis, indecisCount) {
	var segment = {};
	
	segment.shipVertexPositionBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, segment.shipVertexPositionBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexPositions), gl.STATIC_DRAW);
	segment.shipVertexPositionBuffer.itemSize = 3;
	segment.shipVertexPositionBuffer.numItems = vertexCount;
	
	segment.shipVertexNormalBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, segment.shipVertexNormalBuffer);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexNormals), gl.STATIC_DRAW);
	segment.shipVertexNormalBuffer.itemSize = 3;
	segment.shipVertexNormalBuffer.numItems = vertexCount;
	
	segment.shipVertexIndexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, segment.shipVertexIndexBuffer);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(vertexIndecis), gl.STATIC_DRAW);
	segment.shipVertexIndexBuffer.itemSize = 1;
	segment.shipVertexIndexBuffer.numItems = indecisCount;
	
	segment.vertexPositions = vertexPositions;
	segment.vertexNormals = vertexNormals;
	segment.vertexIndecis = vertexIndecis;
	
	segment.vertexCount = vertexCount;
	segment.indecisCount = indecisCount;
	
	return segment;
}


function handleLoadedModelGrid(model_name, data) {
	

	var x_pivot = models[model_name].pivot.x;
	var y_pivot = models[model_name].pivot.y;	
	var z_pivot = models[model_name].pivot.z;
	
	models[model_name].segments = [];
	
	var planes = data.split("\r\n\r\n");
	for(var i = 0; i < planes.length; i++) {
		planes[i] = planes[i].split("\r\n");
		for(var j = 0; j < planes[i].length; j++) {
			planes[i][j] = planes[i][j].split("");
		}
	}
	
	var vertexPositions = [];
	var vertexNormals = [];
	var vertexCount = 0;
	var vertexIndecis = [];
	var indecisCount = 0;
	
	function decodify(c) {
		if(c == undefined) return 0;
		if(c == '1' || c == '2') {
			return c.charCodeAt(0)
		}
		return 0;
	}
	
	for(var i = 0; i < planes.length; i++) {
		for(var j = 0; j < planes[i].length; j++) {
			for(var k = 0; k < planes[i][j].length; k++) {
				if(decodify(planes[i][j][k]) > 0) {
					
					var x = j - x_pivot;
					var y = k - y_pivot;
					var z = i - z_pivot;
					
					
					var front_face = !(planes[i+1] && planes[i+1][j] && decodify(planes[i+1][j][k]) > 0);
					var back_face = !(planes[i-1] && planes[i-1][j] && decodify(planes[i-1][j][k]) > 0);
					var top_face = !(decodify(planes[i][j][k+1]) > 0);
					var bottom_face = !(decodify(planes[i][j][k-1]) > 0);
					var right_face = !(planes[i][j+1] && decodify(planes[i][j+1][k]) > 0);
					var left_face = !(planes[i][j-1] && decodify(planes[i][j-1][k]) > 0);
					/*
					var front_face = true;
					var back_face = true;
					var top_face = true;
					var bottom_face = true;
					var right_face = true;
					var left_face = true;
					*/
					
					if(front_face){
						vertexPositions = vertexPositions.concat(
							[0.0+x, 0.0+y, 1.0+z, 1.0+x, 0.0+y, 1.0+z, 1.0+x, 1.0+y, 1.0+z, 0.0+x, 1.0+y, 1.0+z]
						);
					}
					if(back_face){
						vertexPositions = vertexPositions.concat(
							[0.0+x, 0.0+y, 0.0+z, 0.0+x, 1.0+y, 0.0+z, 1.0+x, 1.0+y, 0.0+z, 1.0+x, 0.0+y, 0.0+z]
						);
					}
					if(top_face){
						vertexPositions = vertexPositions.concat(
							[0.0+x, 1.0+y, 0.0+z, 0.0+x, 1.0+y, 1.0+z, 1.0+x, 1.0+y, 1.0+z, 1.0+x, 1.0+y, 0.0+z]
						);
					}
					if(bottom_face){
						vertexPositions = vertexPositions.concat(
							[0.0+x, 0.0+y, 0.0+z, 1.0+x, 0.0+y, 0.0+z, 1.0+x, 0.0+y, 1.0+z, 0.0+x, 0.0+y, 1.0+z]
						);
					}
					if(right_face){
						vertexPositions = vertexPositions.concat(
							[1.0+x, 0.0+y, 0.0+z, 1.0+x, 1.0+y, 0.0+z, 1.0+x, 1.0+y, 1.0+z, 1.0+x, 0.0+y, 1.0+z]
						);
					}
					if(left_face){
						vertexPositions = vertexPositions.concat(
							[0.0+x, 0.0+y, 0.0+z, 0.0+x, 0.0+y, 1.0+z, 0.0+x, 1.0+y, 1.0+z, 0.0+x, 1.0+y, 0.0+z]
						);
					}
					
					if(front_face){
						vertexNormals = vertexNormals.concat(
							[0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0]
						);
					}
					if(back_face){
						vertexNormals = vertexNormals.concat(
							[0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0]
						);
					}
					if(top_face){
						vertexNormals = vertexNormals.concat(
							[0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0]
						);
					}
					if(bottom_face){
						vertexNormals = vertexNormals.concat(
							[0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0]
						);
					}
					if(right_face){
						vertexNormals = vertexNormals.concat(
							[1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0]
						);
					}
					if(left_face){
						vertexNormals = vertexNormals.concat(
							[-1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0]
						);
					}
					
					var new_vertecies = 0;
					var new_indecies = 0;
					var cubeVertexIndices = [];
					var faces = [front_face, back_face, top_face, bottom_face, right_face, left_face];
					for(var index in faces) {
						if(faces[index]) {
							
							cubeVertexIndices.push(new_vertecies + 0 + vertexCount);
							cubeVertexIndices.push(new_vertecies + 1 + vertexCount);
							cubeVertexIndices.push(new_vertecies + 2 + vertexCount);
							cubeVertexIndices.push(new_vertecies + 0 + vertexCount);
							cubeVertexIndices.push(new_vertecies + 2 + vertexCount);
							cubeVertexIndices.push(new_vertecies + 3 + vertexCount);
							new_vertecies += 4;
							new_indecies += 6;
						}
					}
					vertexIndecis = vertexIndecis.concat(cubeVertexIndices);
					
					vertexCount += new_vertecies;
					indecisCount += new_indecies;
					
					if(vertexCount + 24 >= 65536) {
						var new_segment = createNewSegment(vertexPositions, vertexNormals, vertexCount, vertexIndecis, indecisCount);
						models[model_name].segments.push(new_segment);
						
						vertexPositions = [];
						vertexNormals = [];
						vertexCount = 0;
						vertexIndecis = [];
						indecisCount = 0;
					}
				}
			}
		}
	}
	var new_segment = createNewSegment(vertexPositions, vertexNormals, vertexCount, vertexIndecis, indecisCount);
	models[model_name].segments.push(new_segment);
}


function draw_model(model_name, x, y, z, yaw, pitch, roll) {
	
	var this_model = models[model_name];
	
	mvPushMatrix();
			
	mat4.translate(mvMatrix, [x, y, z]);
	mat4.rotate(mvMatrix, yaw, [0, 0, 1]);
	mat4.rotate(mvMatrix, pitch, [1, 0, 0]);
	mat4.rotate(mvMatrix, roll, [0, 1, 0]);
	
	if(this_model['basic']) {
		if(!this_model['loading'] && !this_model['loaded']) {
			models[model_name]['loading'] = true;
			
			var request = new XMLHttpRequest();
			request.open("GET", this_model['file_location']);
			request.onreadystatechange = function () {
				if (request.readyState == 4) {
					handleLoadedModelGrid(model_name, request.responseText)
					
					models[model_name]['loaded'] = true;
					console.log("Loaded " + model_name);
				}
			}
			request.send();
		}
		if(this_model['loaded']) {
			for(var i = 0; i < this_model.segments.length; i++) {
				var segment = this_model.segments[i];
				gl.bindBuffer(gl.ARRAY_BUFFER, segment.shipVertexPositionBuffer);
				gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, segment.shipVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);
				
				gl.bindBuffer(gl.ARRAY_BUFFER, segment.shipVertexNormalBuffer);
				gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, segment.shipVertexNormalBuffer.itemSize, gl.FLOAT, false, 0, 0);

				gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, segment.shipVertexIndexBuffer);
				setMatrixUniforms();
				gl.drawElements(gl.POINTS, segment.shipVertexIndexBuffer.numItems, gl.UNSIGNED_SHORT, 0);
			}
		}
		mvPopMatrix();
		return this_model['loaded'];
	} else {
		var all_loaded = true;
		for(var i = 0; i < this_model['pieces'].length; i++) {
			var piece = this_model['pieces'][i];
			var loaded = draw_model(piece['name'], piece['x'], piece['y'], piece['z'], piece['yaw'], piece['pitch'], piece['roll']);
			if(!loaded) {
				all_loaded = false;
			}
		}
		mvPopMatrix();
		return all_loaded;
	}
	
}