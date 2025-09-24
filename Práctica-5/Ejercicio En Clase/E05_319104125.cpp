/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Goddard_M;
Model Mandibula_M;
Model PDD_M;
Model PDI_M;
Model PTD_M;
Model PTI_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	Goddard_M = Model();
	Goddard_M.LoadModel("Models/cuerpo.fbx");

	Mandibula_M = Model();
	Mandibula_M.LoadModel("Models/mandibula.fbx");

	PDD_M = Model();
	PDD_M.LoadModel("Models/pdd.fbx");

	PDI_M = Model();
	PDI_M.LoadModel("Models/pdi.fbx");

	PTD_M = Model();
	PTD_M.LoadModel("Models/ptd.fbx");

	PTI_M = Model();
	PTI_M.LoadModel("Models/pti.fbx");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Goddard
		color = glm::vec3(0.8f, 0.0f, 0.5f); //modelo de goddard de color negro
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.57f, 0.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		
		// Límite [-45°, 45°]
		float ang1 = mainWindow.getarticulacion1();   
		ang1 = glm::clamp(ang1, -30.0f, 30.0f);

		float ang2 = mainWindow.getarticulacion2();
		ang2 = glm::clamp(ang2, -30.0f, 30.0f);

		model = glm::translate(model, glm::vec3(3.13f, 0.9f, -0.06f));
		model = glm::rotate(model, glm::radians(ang1), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(ang2), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Mandibula_M.RenderModel();
		//En sesión se separara una parte del modelo de Goddard y se unirá por jeraquía al cuerpo
		////modelaux = model;
		//Cola
		


		//Siguientes modelos
		/* Ejercicio:
		1.- Separar las 4 patas de Goddard del modelo del cuerpo, unir por medio de jerarquía cada pata al cuerpo de Goddard
		2.- Hacer que al presionar una tecla cada pata pueda rotar un máximo de 45° "hacia adelante y hacia atrás"
		*/

		//pata delantera derecha
		
		float ang3 = mainWindow.getarticulacion3();
		ang3 = glm::clamp(ang3, -45.0f, 45.0f);

		float ang4 = mainWindow.getarticulacion4();
		ang4 = glm::clamp(ang4, -45.0f, 45.0f);

		model = modelaux;
		model = glm::translate(model, glm::vec3(1.2f, -0.44f, 0.6f));
		model = glm::rotate(model, glm::radians(ang3), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(ang4), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		PDD_M.RenderModel();

		// pata delantera izquierda
		
		float ang5 = mainWindow.getarticulacion5();
		ang5 = glm::clamp(ang5, -45.0f, 45.0f);

		float ang6 = mainWindow.getarticulacion6();
		ang6 = glm::clamp(ang6, -45.0f, 45.0f);

		model = modelaux;
		model = glm::translate(model, glm::vec3(1.2f, -0.44f, -0.6f));
		model = glm::rotate(model, glm::radians(ang5), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(ang6), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		PDI_M.RenderModel();
		
		
		//pata trasera derecha
		
		float ang7 = mainWindow.getarticulacion7();
		ang7 = glm::clamp(ang7, -45.0f, 45.0f);

		float ang8 = mainWindow.getarticulacion8();
		ang8 = glm::clamp(ang8, -45.0f, 45.0f);

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, -1.11f, 0.6f));
		model = glm::rotate(model, glm::radians(ang7), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(ang8), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.50f, 0.0f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		PTD_M.RenderModel();

		//pata trasera izquierda
		
		float ang9 = mainWindow.getarticulacion9();
		ang9 = glm::clamp(ang9, -45.0f, 45.0f);

		float ang10 = mainWindow.getarticulacion10();
		ang10 = glm::clamp(ang10, -45.0f, 45.0f);

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, -1.11f, -0.6f));
		model = glm::rotate(model, glm::radians(ang9), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(ang10), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		PTI_M.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
