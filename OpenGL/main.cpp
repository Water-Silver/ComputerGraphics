#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <gl/glut.h>

using namespace std;

double con_rot = 0.0;

struct ModelTri
{
	double p0[3], p1[3], p2[3];
	float p0C[3]; // store colors
	int	pi[3];
	double n[3];
	double n0[3], n1[3], n2[3];
	double ver[3];
	
};

ModelTri *triangle;
ModelTri *vertex;
int ntris,nvertics;

inline void VmV(double Vr[3], const double V1[3], const double V2[3])
{
	Vr[0] = V1[0] - V2[0];
	Vr[1] = V1[1] - V2[1];
	Vr[2] = V1[2] - V2[2];
}

inline void VcrossV(double Vr[3], const double V1[3], const double V2[3])
{
	Vr[0] = V1[1]*V2[2] - V1[2]*V2[1];
	Vr[1] = V1[2]*V2[0] - V1[0]*V2[2];
	Vr[2] = V1[0]*V2[1] - V1[1]*V2[0];
}

inline void Vnormalize(double V[3])
{
	double d1=sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
	double d;
	if (d1==0)
	{
		d=0;
	}
	else
	{
		d = (double)1.0 / sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
	}

	V[0] *= d;
	V[1] *= d;
	V[2] *= d;
}

void ReadTriFile(const char *tris_file)
{
	FILE *fp;
	errno_t err = fopen_s(&fp, tris_file, "r");
	if (fp == NULL)
	{
		fprintf(stderr,"Model Constructor: Couldn't open %s\n",tris_file); 
		exit(-1); 
	}

	int i1,i2,i3;
	double am, bm, cm;
	char str[10];

	fscanf_s(fp,"%s",str, sizeof(str)); // OFF
	fscanf_s(fp,"%d",&nvertics); // num of vertices
	fscanf_s(fp,"%d",&ntris); // num of face

	
	triangle = new ModelTri[ntris];
	vertex = new ModelTri[nvertics];

	for (int i = 0; i < nvertics; i++)
	{
		fscanf_s(fp,"%lf %lf %lf",&am,&bm,&cm); //vertex 의 x y z
		vertex[i].ver[0]=am;
		vertex[i].ver[1]=bm;
		vertex[i].ver[2]=cm;
	}
	for (int i = 0; i < ntris; i++)
	{
		int temp;
		fscanf_s(fp,"%d %d %d", &i1,&i2,&i3);
		triangle[i].p0[0] = vertex[i1].ver[0]; 
		triangle[i].p0[1] = vertex[i1].ver[1]; 
		triangle[i].p0[2] = vertex[i1].ver[2]; 

		triangle[i].p1[0] = vertex[i2].ver[0]; 
		triangle[i].p1[1] = vertex[i2].ver[1]; 
		triangle[i].p1[2] = vertex[i2].ver[2]; 

		triangle[i].p2[0] = vertex[i3].ver[0]; 
		triangle[i].p2[1] = vertex[i3].ver[1]; 
		triangle[i].p2[2] = vertex[i3].ver[2]; 

		double a[3],b[3];
		VmV(a,triangle[i].p1,triangle[i].p0);
		VmV(b,triangle[i].p2,triangle[i].p0);
		VcrossV(triangle[i].n,a,b);
		
		Vnormalize(triangle[i].n);
	}
	fclose(fp);

}

void ReadOffFile(const char *offs_file)
{
	FILE *fp;
	errno_t err = fopen_s(&fp, offs_file, "r");
	if (fp == NULL)
	{
		fprintf(stderr,"Model Constructor: Couldn't open %s\n",offs_file); 
		exit(-1); 
	}

	int a,i1,i2,i3;
	double am, bm, cm;
	double r, g, b, ab;
	char str[10];
	
	fscanf_s(fp,"%s",str, sizeof(str));
	fscanf_s(fp,"%d",&nvertics);
	fscanf_s(fp,"%d",&ntris);
	fscanf_s(fp,"%d",&a);
	triangle = new ModelTri[ntris];
	vertex = new ModelTri[nvertics];
	//printf("num of ntris : %d", ntris);
	//printf("num of nvertics : %d", nvertics);
	
	for (int i = 0; i < nvertics; i++)
	{
		fscanf_s(fp, "%lf %lf %lf %lf %lf %lf %lf", &am, &bm, &cm, &r, &g, &b, &ab); // store color information
		//fscanf_s(fp, "%lf %lf %lf", &am, &bm, &cm);
		vertex[i].ver[0] = am;
		vertex[i].ver[1] = bm;
		vertex[i].ver[2] = cm;
		vertex[i].p0C[0] = (float)(r)/ 255.0;
		vertex[i].p0C[1] = (float)(g)/ 255.0;
		vertex[i].p0C[2] = (float)(b)/ 255.0;

	}
	
		
  		for (int i = 0; i < ntris; i++)
  		{
  			int temp;
			fscanf_s(fp,"%d %d %d %d",&temp, &i1,&i2,&i3);
  			triangle[i].p0[0] = vertex[i1].ver[0]; 
 			triangle[i].p0[1] = vertex[i1].ver[1]; 
 			triangle[i].p0[2] = vertex[i1].ver[2]; 
			triangle[i].pi[0] = i1;

			


 			triangle[i].p1[0] = vertex[i2].ver[0]; 
 			triangle[i].p1[1] = vertex[i2].ver[1]; 
 			triangle[i].p1[2] = vertex[i2].ver[2]; 
			triangle[i].pi[1] = i2;

 
 			triangle[i].p2[0] = vertex[i3].ver[0]; 
 			triangle[i].p2[1] = vertex[i3].ver[1]; 
 			triangle[i].p2[2] = vertex[i3].ver[2]; 
			triangle[i].pi[2] = i3;
		

 			double a[3],b[3];
 			VmV(a,triangle[i].p1,triangle[i].p0);
 			VmV(b,triangle[i].p2,triangle[i].p0);
 			VcrossV(triangle[i].n,a,b);
 			Vnormalize(triangle[i].n);

			triangle[i].n0[0] = triangle[i].n[0]; //copy the normal to each vertex normal 
			triangle[i].n0[1] = triangle[i].n[1];
			triangle[i].n0[2] = triangle[i].n[2];

			triangle[i].n1[0] = triangle[i].n[0];
			triangle[i].n1[1] = triangle[i].n[1];
			triangle[i].n1[2] = triangle[i].n[2];

			triangle[i].n2[0] = triangle[i].n[0];
			triangle[i].n2[1] = triangle[i].n[1];
			triangle[i].n2[2] = triangle[i].n[2];
		}
		

		/////////////////////////////////////////////////////////////////////////   improvements

		for (int i = 0; i < nvertics; i++) { // initialize normal of each vertex to 0
			
				vertex[i].n0[0] = 0;
				vertex[i].n0[1] = 0;
				vertex[i].n0[2] = 0;
				vertex[i].n1[0] = 0;
				vertex[i].n1[1] = 0;
				vertex[i].n1[2] = 0;
				vertex[i].n2[0] = 0;
				vertex[i].n2[1] = 0;
				vertex[i].n2[2] = 0;
			
		}

		
		for (int i = 0; i < ntris; i++) { //for each triangle visit its vertices and add up its normal to vertex normal
			// vertex[ index ].n0[0] = 
			
			//vertex 1
			vertex[triangle[i].pi[0]].n0[0] += triangle[i].n0[0];
			vertex[triangle[i].pi[0]].n0[1] += triangle[i].n0[1];
			vertex[triangle[i].pi[0]].n0[2] += triangle[i].n0[2];

			//vertex2
			vertex[triangle[i].pi[1]].n1[0] += triangle[i].n1[0]; 
			vertex[triangle[i].pi[1]].n1[1] += triangle[i].n1[1];
			vertex[triangle[i].pi[1]].n1[2] += triangle[i].n1[2];

			//vertex3
			vertex[triangle[i].pi[2]].n2[0] += triangle[i].n2[0];
			vertex[triangle[i].pi[2]].n2[1] += triangle[i].n2[1];
			vertex[triangle[i].pi[2]].n2[2] += triangle[i].n2[2];
			

		}


		for (int i = 0; i < ntris; i++) {
			triangle[i].n0[0] = vertex[triangle[i].pi[0]].n0[0];  //copy the vertex normal to each corresponding triangle point's normal
			triangle[i].n0[1] = vertex[triangle[i].pi[0]].n0[1]; // triangle 의 첫번째 점의 normal에 그 점(vertex)의 normal을 넣어준다.
			triangle[i].n0[2] = vertex[triangle[i].pi[0]].n0[2];

			triangle[i].n1[0] = vertex[triangle[i].pi[1]].n1[0];
			triangle[i].n1[1] = vertex[triangle[i].pi[1]].n1[1];
			triangle[i].n1[2] = vertex[triangle[i].pi[1]].n1[2];

			triangle[i].n2[0] = vertex[triangle[i].pi[2]].n2[0];
			triangle[i].n2[1] = vertex[triangle[i].pi[2]].n2[1];
			triangle[i].n2[2] = vertex[triangle[i].pi[2]].n2[2];


			Vnormalize(triangle[i].n0);
			Vnormalize(triangle[i].n1);
			Vnormalize(triangle[i].n2);

		}
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////original code
		
		/*


		for (int i = 0; i < ntris; i++) //average adjacent triangle normals
		{
			int currIndex0 = triangle[i].pi[0];
			int currIndex1 = triangle[i].pi[1];
			int currIndex2 = triangle[i].pi[2];
			//printf("%d\t %d\t %d\t \n",triangle[i].pi[0], triangle[i].pi[1], triangle[i].pi[2]);
			
			for(int k = 0; k < ntris; k++) //brute force search 
			{
				if(triangle[k].pi[0] == currIndex0)
				{
					triangle[i].n0[0] += triangle[k].n[0];
					triangle[i].n0[1] += triangle[k].n[1];
					triangle[i].n0[2] += triangle[k].n[2];
				}
				if(triangle[k].pi[1] == currIndex1)
				{
					triangle[i].n1[0] += triangle[k].n[0];
					triangle[i].n1[1] += triangle[k].n[1];
					triangle[i].n1[2] += triangle[k].n[2];
				}
				if(triangle[k].pi[0] == currIndex2)
				{
					triangle[i].n2[0] += triangle[k].n[0];
					triangle[i].n2[1] += triangle[k].n[1];
					triangle[i].n2[2] += triangle[k].n[2];
				}
			}
			Vnormalize(triangle[i].n0);
			Vnormalize(triangle[i].n1);
			Vnormalize(triangle[i].n2);
			
		}
		*/
		////////////////////////////////////////////////////////////////////////////////////
		fclose(fp);
		 
}

void drawCatchModel()
{
	glClearColor(0.1,0.1,0.1,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glTranslatef(0,10,-20);
	glRotatef(90,-1,0,0);
	glRotatef(con_rot,0,0,1);

	

	GLfloat material_Ka[] = { 0.11, 0.11,0.11, 1.00 };
	GLfloat material_Ks[] = { 0.33, 0.33, 0.33, 1.00 };
	GLfloat material_Kc[] = { 0.00, 0.00, 0.00, 0.00 };
	GLfloat material_Sc[] = { 5 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_Kc);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_Kc);
	
 	glBegin(GL_TRIANGLES);  
	for (int i = 0; i < ntris; i++)
	{
		//p1 initializing per-vertex color information on diffuse element of material
		GLfloat material_Kd[] = { vertex[triangle[i].pi[0]].p0C[0],  vertex[triangle[i].pi[0]].p0C[1], vertex[triangle[i].pi[0]].p0C[2], 1.00 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
		
		
		glNormal3dv(triangle[i].n0);
		glVertex3dv(triangle[i].p0);


		//p2 initializing per-vertex color information on diffuse element of material
		material_Kd[0] = vertex[triangle[i].pi[1]].p0C[0];
		material_Kd[1] = vertex[triangle[i].pi[1]].p0C[1];
		material_Kd[2] = vertex[triangle[i].pi[1]].p0C[2];
		
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
		
	
		glNormal3dv(triangle[i].n1);
		glVertex3dv(triangle[i].p1);
		
		//p3 initializing per-vertex color information on diffuse element of material
		material_Kd[0] = vertex[triangle[i].pi[2]].p0C[0]; 
		material_Kd[1] = vertex[triangle[i].pi[2]].p0C[1];
		material_Kd[2] = vertex[triangle[i].pi[2]].p0C[2];
		
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
		

		glNormal3dv(triangle[i].n2);
		glVertex3dv(triangle[i].p2);
		
	}    

	


 	glEnd(); 
	glPopMatrix();
	glFlush();
}

static void Timer(int value)
{ 
	con_rot += 10.0;
	if(con_rot > 360.0)
		con_rot /= 360.0;
	glutPostRedisplay();
	glutTimerFunc(100, Timer, 0);
}



void myReshape(GLsizei w,GLsizei h)
{
	float ratio = 1.0* w / h; 
	glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100,(float)w/h,0.1,5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.5, 25.0, 1.0, 0.0, 5.0, -20.0, 0.0f,1.0f,0.0f); 	
}

void init()
{
	char filename[256];


	/////////////////////////////////////////////////////////////add lightings

	GLfloat light_position[]={0.5,0.5,1.0,1.0};
	GLfloat light_Ka[] = { 0.50, 0.50, 0.50, 1.00 };
	GLfloat light_Kd[] = { 1.00, 1.00, 1.00, 1.00 };
	GLfloat light_Ks[] = { 0.30, 0.30, 0.30, 1.00 };
	
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	printf("Enter the file name :  ");
	scanf_s( "%s", filename, sizeof(filename) ); 
	ReadOffFile(filename);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH |GLUT_SINGLE);

	glutInitWindowSize(800,600);
	glutInitWindowPosition(10,10);
	glutCreateWindow("123DCatchGLRendering");

	init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(drawCatchModel);
	Timer(100); // to rotate model 
	glutMainLoop();


	system("pause");

}