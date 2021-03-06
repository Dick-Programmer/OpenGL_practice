#version 430
layout (location=0) in vec3 position;

uniform mat4 m_matrix=mat4(1.0);
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;
out vec4 varyingColor;


mat4 buildRotX(float rad) /*Declaration of matrix transformation utility functions*/
{
	mat4 trans=mat4(1.0, 0.0, 0.0, 0.0,
	            0.0, cos(rad), -sin(rad), 0.0,
	            0.0, sin(rad), cos(rad), 0.0,
	            0.0, 0.0, 0.0, 1.0);
	return trans;
}

mat4 buildRotY(float rad)   /*GLSL requires functions to be declared prior to invocation*/
{
	mat4 trans=mat4(cos(rad), 0.0, sin(rad), 0.0,
	            0.0, 1.0, 0.0, 0.0,
	            -sin(rad), 0.0, cos(rad), 0.0,
	            0.0, 0.0, 0.0, 1.0);
	return trans;
}

mat4 buildRotZ(float rad)
{
	mat4 trans=mat4(cos(rad), -sin(rad), 0.0, 0.0,
	            sin(rad), cos(rad), 0.0, 0.0,
	            0.0, 0.0, 1.0, 0.0,
	            0.0, 0.0, 0.0, 1.0);
	return trans;
}

mat4 buildTranslate(float x, float y, float z)
{
	mat4 trans=mat4(1.0, 0.0, 0.0, 0.0,
	                0.0, 1.0, 0.0, 0.0,
	                0.0, 0.0, 1.0, 0.0,
	                x,y,z,1.0);
	return trans;
}






void main(void)
{
	float i=gl_InstanceID+tf;
	float a = sin(203.0 * i/8000.0) * 403.0; 
	float b = cos(301.0 * i/4001.0) * 401.0; 
	float c = sin(400.0 * i/6003.0) * 405.0;



    /*build the rotation and translation matrices to be applied to this cube’s model matrix*/

 
	mat4 localRotX = buildRotX(1.75*i); 
	mat4 localRotY = buildRotY(1.75*i); 
	mat4 localRotZ = buildRotZ(1.75*i);
	mat4 localTrans = buildTranslate(a,b,c);


	/*build the model matrix and then the model view matrix*/
	mat4 newM_matrix=m_matrix*localTrans*localRotX*localRotY*localRotZ;
	mat4 mv_matrix=v_matrix*newM_matrix;

	gl_Position=proj_matrix*mv_matrix*vec4(position,1.0);
	varyingColor=vec4(position,1.0)*0.5+vec4(0.5,0.5,0.5,0.5);

}



