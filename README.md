# single-source-shortest-path-with-dijkstra

This readme file is only for sequencetial_with_weight_dijkstra.cpp file.

The sample graph used in this code can be found in the link below:
https://indianinstituteofscience-my.sharepoint.com/:u:/g/personal/banwesha_iisc_ac_in/EYFCZdlppfFJtC4ABg4x8w4BKyAxyvsD-p8P5IEUjP4w5Q?e=Jt5y5Y

The algorithm used is dijkstra's method.
I have 4 functions in my code.
The function named extract_data is for extracting data from graph file.
For different style data set that function will be different.

FIrst we create a distance matrix with distance of source being 0 and all others being infinity.
We find the vertex with smallest distance among its neighbours given a source. Then we remove the source and make the new vertex as source.
Also we update the distance of all neighbours.

Then we keep finding using the temporary sources to update distance of its neightbours and keep then a list. 
Again delete the source and update the values if the new distance is less than the old.
We keep repeating the above algorithm.

The function sssp is used to keep removing source and assigning new source. 
It also changes the position of vertices in list of distance heirarchy.

For this we have the function search is used to find the new position of vertex.
And the function binary_search is used for finding old position of vertex.

As this graph had integer distances, I have used integer as distance type but for general case it should be float.
If distance type will be float, then the execution time will be even less because the number of collisions will be negligible.

<strong>I have written this code independent of sparsity in mind. So, it will work fairly efficiently for dense matrix as well.</strong>