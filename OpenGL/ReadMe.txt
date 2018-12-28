HW5: OpenGL Rendering of HW2 Object
=============================

features implemented
- lightings are added in void init() function of main.cpp
- viewpoint is revised by 
  gluLookAt(0.5, 25.0, 1.0, 0.0, 5.0, -20.0, 0.0f,1.0f,0.0f); 
  in order to get a better view of the model.
- original color texture from HW2 is restored by adding color per vertex in drawCatchModel().
- speed of normal calculation is improved by c*O(ntris), 
  using vertices to store normals of adjacent triangles.

1. initialize all normal factor of vertices into 0
2. instead of going over all the triangles to find the adjacent triangle of a triangle i
   loop around all triangles and add its normal to its surrounding vertices ( that can be achieved from its index )
3. store back the sumed normals of vertices to its corresponding vertices of a triangle
4. normalize

---------pseudo code----------
for number of vertices :   
   vertex v.n = 0;
  
for each triangle t :
   vertex[ index1 of triangle t ].n0 += triangle t.n0;
   vertex[ index2 of triangle t ].n1 += triangle t.n1;
   vertex[ index3 of triangle t ].n2 += triangle t.n2;
       

for number of triangles :
  store back sumed vertex normals to the corresponding points in triangles ;
 
normalize(triangle's normal);



