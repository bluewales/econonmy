import math

f = open("inner_hull_panel.txt",'w')
l = 101




angle_width = 3
bolt_spacing = 4

string = ""

def simplify(string):
	lines = string.split("\r\n")

	string = ""
	for line in lines:
		while len(line) > 1 and line[len(line)-1] == ".":
			line = line[0:len(line)-1]
		string += line + "\r\n"
	return string


for i in range(0,l):
	for j in range(0,l):
	
		i_plate_bolt_row = (i == 2 or i == l-3) and (j >1 and j < l-2)
		i_bolt_spacing = i%bolt_spacing == 0
		
		j_plate_bolt_row = (j == 2 or j == l-3) and (i >1 and i < l-2)
		j_bolt_spacing = j%bolt_spacing == 0
		
		plate_bolt = i_plate_bolt_row and j_bolt_spacing or j_plate_bolt_row and i_bolt_spacing
	
		if (plate_bolt):
			string += "1"
		else:
			string += "."
	string+= "\r\n"
string+= "\r\n"


for i in range(0,l):
	for j in range(0,l):
		string += "1"
	string+= "\r\n"
string+= "\r\n"		


for i in range(0,l):
	for j in range(0,l):
	
		i_plate_bolt_row = (i == 2 or i == l-3) and (j >1 and j < l-2)
		i_bolt_spacing = i%bolt_spacing == 0
		
		j_plate_bolt_row = (j == 2 or j == l-3) and (i >1 and i < l-2)
		j_bolt_spacing = j%bolt_spacing == 0
		
		plate_bolt = i_plate_bolt_row and j_bolt_spacing or j_plate_bolt_row and i_bolt_spacing
	
		if (plate_bolt):
			string += "1"
		else:
			string += "."
	string+= "\r\n"
string+= "\r\n"

lines = string.split("\r\n")

string = ""
for line in lines:
	while len(line) > 1 and line[len(line)-1] == ".":
		line = line[0:len(line)-1]
	string += line + "\r\n"

f.write(string)
		
f.close()

r = 11
curve = [None]*(r+2)
for i in range(0,(r+2)) :
	curve[i] = [None]*(r+2)
	for j in range(0,(r+2)) :
		curve[i][j] = ["."]*l

for i in range (0,(r+1)):
	for j in range (0,(r+1)):
		for k in range (0,l):
			di = (r+0) - i
			dj = (r+0) - j
			d = math.sqrt(di*di + dj*dj)
			if d > r-math.sqrt(2) and d < r:
				curve[i][j][k] = 1

string = ""
for i in range (0,(r+2)):
	for j in range (0,(r+2)):
		for k in range (0,l):
			string += str(curve[i][j][k])
		string += "\r\n"
	string += "\r\n"
	
string = simplify(string)

f = open("inner_hull_edge.txt",'w')
f.write(string)
f.close()

curve = [None]*(r+2)
for i in range(0,(r+2)) :
	curve[i] = [None]*(r+2)
	for j in range(0,(r+2)) :
		curve[i][j] = ["."]*(r+2)

for i in range (0,(r+1)):
	for j in range (0,(r+1)):
		for k in range (0,(r+1)):
			di = (r+0) - i
			dj = (r+0) - j
			dk = (r+0) - k
			d = math.sqrt(di*di + dj*dj + dk*dk)
			if d > r-math.sqrt(2) and d < r:
				curve[i][j][k] = 1

string = ""
for i in range (0,(r+2)):
	for j in range (0,(r+2)):
		for k in range (0,(r+2)):
			string += str(curve[i][j][k])
		string += "\r\n"
	string += "\r\n"
	
string = simplify(string)

f = open("inner_hull_vertex.txt",'w')
f.write(string)
f.close()