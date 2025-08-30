//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas

#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>

//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

//Vertex Shader
//SHADERS PARA ACTIVIDAD 1
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//shaders nuevos se crearían acá
//SHADERS PARA ACTIVIDAD 2
static const char* fShaderAzul = "shaders/azul.frag";
static const char* vShaderAzul = "shaders/shaderAzul.vert";

static const char* fShaderCafe = "shaders/cafe.frag";
static const char* vShaderCafe = "shaders/shaderCafe.vert";

static const char* fShaderRojo = "shaders/rojo.frag";
static const char* vShaderRojo = "shaders/shaderRojo.vert";

static const char* fShaderVerdeClaro = "shaders/verdeClaro.frag";
static const char* vShaderVerdeClaro = "shaders/shaderVClaro.vert";

static const char* fShaderVerdeFuerte = "shaders/verdeFuerte.frag";
static const char* vShaderVerdeFuerte = "shaders/shaderVFuerte.vert";



float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067



//FUNCIONES PARA CREAR PIRAMIDES Y CUBOS. ACTIVIDAD 2

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}



// FUNCIÓN PARA CREAR LETRAS. ACTIVIDAD 1
void CrearLetrasyFiguras()
{
	
	//LETRA L
	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-0.95f,   0.8f,     0.0f, 			0.0f,	0.0f,	1.0f,
		-0.8f,    0.8f,     0.0f,		    0.0f,	0.0f,	1.0f,
		-0.8f,    0.2f,     0.0f,			0.0f,	0.0f,	1.0f,

		-0.95f,   0.8f,     0.0f,           0.0f,	0.0f,	1.0f,
		-0.8f,    0.2f,     0.0f,           0.0f,	0.0f,	1.0f,
		-0.95f,   0.2f,     0.0f,           0.0f,	0.0f,	1.0f,

		-0.95f,	  0.2f,     0.0f,			0.0f,	0.0f,	1.0f,
		-0.45f,   0.2f,     0.0f,			0.0f,	0.0f,	1.0f,
		-0.45f,   0.1f,     0.0f,			0.0f,	0.0f,	1.0f,

		-0.95f,   0.2f,     0.0f,			0.0f,	0.0f,	1.0f,
		-0.45f,   0.1f,     0.0f,			0.0f,	0.0f,	1.0f,
		-0.95f,   0.1f,     0.0f,			0.0f,	0.0f,	1.0f,

	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 72);
	meshColorList.push_back(triangulorojo);


	//LETRA C
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-0.35f,    0.8f,      0.0f,			1.0f,	0.0f,	0.0f,
		0.25f,     0.8f,      0.0f,			1.0f,	0.0f,	0.0f,
		-0.35f,    0.7f,      0.0f,			1.0f,	0.0f,	0.0f,

		-0.35f,    0.7f,      0.0f,			1.0f,	0.0f,	0.0f,
		0.25f,     0.8f,      0.0f,			1.0f,	0.0f,	0.0f,
		0.25f,     0.7f,      0.0f,			1.0f,	0.0f,	0.0f,

		-0.35f,    0.7f,      0.0f,			1.0f,	0.0f,	0.0f,
		-0.35f,    0.2f,      0.0f,			1.0f,	0.0f,	0.0f,
		-0.2f,     0.7f,      0.0f,			1.0f,	0.0f,	0.0f,

		-0.35f,    0.2f,      0.0f,			1.0f,	0.0f,	0.0f,
		-0.2f,     0.7f,      0.0f,			1.0f,	0.0f,	0.0f,
		-0.2f,     0.2f,      0.0f,			1.0f,	0.0f,	0.0f,

		-0.35f,    0.2f,      0.0f,			1.0f,	0.0f,	0.0f,
		0.25f,     0.2f,      0.0f,			1.0f,	0.0f,	0.0f,
		0.25f,     0.1f,      0.0f,			1.0f,	0.0f,	0.0f,

		-0.35f,    0.2f,      0.0f,			1.0f,	0.0f,	0.0f,
		0.25f,     0.1f,      0.0f,			1.0f,	0.0f,	0.0f,
		-0.35f,    0.1f,      0.0f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 108);
	meshColorList.push_back(trianguloazul);


	//LETRA G
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		0.35f,     0.8f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.95f,     0.8f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.95f,     0.7f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.35f,     0.8f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.95f,     0.7f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.35f,     0.7f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.35f,     0.7f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.5f,      0.7f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.35f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.5f,      0.7f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.35f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.5f,      0.2f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.35f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.35f,     0.1f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.95f,     0.1f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.35f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.95f,     0.1f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.95f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.95f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.85f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.85f,     0.5f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.95f,     0.2f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.85f,     0.5f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.95f,     0.5f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.85f,     0.5f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.85f,     0.4f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.7f,      0.5f,      0.0f,			0.0f,	1.0f,	0.0f,

		0.85f,     0.4f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.7f,      0.5f,      0.0f,			0.0f,	1.0f,	0.0f,
		0.7f,      0.4f,      0.0f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 180);
	meshColorList.push_back(trianguloverde);


}


void CreateShaders()
{
	//SHADERS USADOS EN ACTIVIDAD 1

	//INDICE 0
	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);




	//SHADERS USADOS EN ACTIVIDAD 2
	//INDICE 1
	Shader* azul = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	azul->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*azul);

	//INDICE 2
	Shader* verdeClaro = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	verdeClaro->CreateFromFiles(vShaderVerdeClaro, fShaderVerdeClaro);
	shaderList.push_back(*verdeClaro);

	//INDICE 3
	Shader* rojo = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	rojo->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*rojo);

	//INDICE 4
	Shader* cafe = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	cafe->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*cafe);

	//INDICE 5
	Shader* verdeFuerte = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	verdeFuerte->CreateFromFiles(vShaderVerdeFuerte, fShaderVerdeFuerte);
	shaderList.push_back(*verdeFuerte);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;



	////ACTIVIDAD 1
	////Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D (orthogonal) 
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	

	//o en 3D (perspectiva)
	//ACTIVIDAD 2
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);



	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();


		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		


		////ACTIVIDAD 1		
		//						
		////Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		//shaderList[0].useShader();
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();


		////Primer letra (L)
		////Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.05f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();


		////Segunda letra (C)
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[1]->RenderMeshColor();


		////Tercera letra (G)
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.05f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[2]->RenderMeshColor();



		//ACTIVIDAD 2
		
		//CUBO ROJO (casa)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.20f, -1.0f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.30f, 0.50f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//PIRAMIDE AZUL (techo)
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.30f, -0.9f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.70f, 0.40f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		//CUBO VERDE (ventana izquierda)
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.055f, -0.02f, -0.9f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.95f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//CUBO VERDE (ventana derecha)
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.055f, -0.02f, -0.9f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.95f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//CUBO VERDE (puerta)
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.235f, -0.9f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.85f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//CUBO CAFE (tronco izquierda)
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.50f, -0.50f, -1.0f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.04));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//PIRAMIDE VERDE (hojas izquierda)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.1;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.44f, -0.35f, -0.87));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.22f, 0.17f, 0.29f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		//CUBO CAFE (tronco derecha)
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.50f, -0.50f, -1.0f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.04));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//PIRAMIDE VERDE (hojas derecha)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.44f, -0.35f, -0.87));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTAAAR
		model = glm::scale(model, glm::vec3(0.22f, 0.17f, 0.29f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();




		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
