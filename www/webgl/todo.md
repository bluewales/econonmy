+ Don't display voxel faces that are hidden within model
* Combine voxel faces into larger rectangles
+ Make more models, vertex, edges, and doors for my room
* Components are organized in a hierarchy.  
	* Ship is the highest level component. 
	* Coponents come in 2 types, basic and composit
		* Basic components are made up of a voxel map
			* Either a custom voxel map (such as if it is damaged)
			* or a reference to a library component
		* Composite components are made up of other components
	* Sibling components are linked together by 'seams'
		* Components have joints, places where seams can be made
		* Unconnected joints can be declared by a parent component, then become a joint of that component
			* example: polysteal plate has 4 joints, the 4 sides of a square, a wall mgiht be a composite component
			made out of 4 plates.  Those plates would have seams connecting them to neighboring plates.  There would be
			eight unconnected joints. If the wall is going to connected to a floor with some of those joints, the joints
			would have to be declared within the wall
	* Components can split into two components because of damage, including top level components (ships)
		* so, pieces of your ship can be blown off and go flying to space
	