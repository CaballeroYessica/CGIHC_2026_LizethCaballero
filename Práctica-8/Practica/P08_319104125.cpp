/*
Pr�ctica 7: Iluminaci�n 1
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;

Model Coche_M;
Model LlantaI_M;
Model LlantaD_M;
Model Blackhawk_M;
Model parabrisas_M = Model();
Model cofre_M = Model();
Model parrilla_M = Model();
Model lampara_M = Model();
Model piedraTrueno_M = Model();


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
SpotLight spotLights[MAX_SPOT_LIGHTS];
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

SpotLight spotLights2[MAX_SPOT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// Cara 1 (Frontal superior)
		0, 1, 2,
		// Cara 2 (Derecha superior)
		3, 4, 5,
		// Cara 3 (Trasera superior)
		6, 7, 8,
		// Cara 4 (Izquierda superior)
		9, 10, 11,
		// Cara 5 (Frontal inferior)
		12, 13, 14,
		// Cara 6 (Derecha inferior)
		15, 16, 17,
		// Cara 7 (Trasera inferior)
		18, 19, 20,
		// Cara 8 (Izquierda inferior)
		21, 22, 23
	};

	GLfloat cubo_vertices[] = {
		// Cara 5 - Trasera superior
	   0.0f,  0.0f, -0.5f,  0.97f,  0.76f,  0.0f,  -1.0f, 1.0f,
	  -0.5f,  0.0f,  0.0f,  0.75f,  0.99f,  0.0f,  -1.0f, 1.0f,
	   0.0f,  0.5f,  0.0f,  0.52f,  0.76f,  0.0f,  -1.0f, 1.0f,


	   // Cara 1 - Frontal superior
	  // x      y      z      S      T      NX     NY     NZ
	   0.0f,  0.0f,  0.5f,  0.27f,  0.51f,  0.0f,  -1.0f,  -1.0f,  // Derecha
	   0.5f,  0.0f,  0.0f,  0.72f,  0.51f,  0.0f,  -1.0f,  -1.0f,  // Izquierda
	   0.0f,  0.5f,  0.0f,  0.50f,  0.735f,  0.0f,  -1.0f,  -1.0f,  // Superior

	   // Cara 7 - Derecha superior
	   0.5f,  0.0f,  0.0f,  0.74f,  0.52f,  -1.0f,  -1.0f,  0.0f,
	   0.0f,  0.0f, -0.5f,  0.97f,  0.74f,  -1.0f,  -1.0f,  0.0f,
	   0.0f,  0.5f,  0.0f,  0.52f,  0.74f,  -1.0f,  -1.0f,  0.0f,


	   // Cara 3 - Izquierda superior
	   -0.5f,  0.0f,  0.0f,  0.02f,  0.74f, 1.0f,  -1.0f,  0.0f,
	   0.0f,  0.0f,  0.5f,  0.245f,  0.52f, 1.0f,  -1.0f,  0.0f,
	   0.0f,  0.5f,  0.0f,  0.48f,  0.74f, 1.0f,  -1.0f,  0.0f,

	   // Cara 4 - Frontal inferior
	   0.0f,  0.0f,  0.5f,  0.27f,  0.49f,  0.0f,  1.0f,  -1.0f,
	   0.0f, -0.5f,  0.0f,  0.495f,  0.26f,  0.0f,  1.0f,  -1.0f,
	   0.5f,  0.0f,  0.0f,  0.72f,  0.49f,  0.0f,  1.0f,  -1.0f,

	   // Cara 6 - Derecha inferior
	   0.5f,  0.0f,  0.0f,  0.75f,  0.49f,  -1.0f,  1.0f,  0.0f,
	   0.0f, -0.5f,  0.0f,  0.52f,  0.26f,  -1.0f,  1.0f,  0.0f,
	   0.0f,  0.0f, -0.5f,  0.96f,  0.26f,  -1.0f,  1.0f,  0.0f,

	   // Cara 8 - Trasera inferior
	   0.0f,  0.0f, -0.5f,  0.75f,  0.01f,  0.0f,  1.0f, 1.0f,
	   0.0f, -0.5f,  0.0f,  0.515f,  0.24f,  0.0f,  1.0f, 1.0f,
	   -0.5f,  0.0f,  0.0f,  0.96f,  0.24f,  0.0f,  1.0f, 1.0f,

	   // Cara 2 - Izquierda inferior
	   -0.5f,  0.0f,  0.0f, 0.02f,  0.26f, 1.0f,  1.0f,  0.0f,
	   0.0f, -0.5f,  0.0f,  0.47f,  0.26f, 1.0f,  1.0f,  0.0f,
	   0.0f,  0.0f,  0.5f,  0.24f,  0.48f, 1.0f,  1.0f,  0.0f,
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 24);
	meshList.push_back(dado);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado8.tga");
	dadoTexture.LoadTextureA();


	Coche_M = Model();
	Coche_M.LoadModel("Models/carro.obj");
	LlantaI_M = Model();
	LlantaI_M.LoadModel("Models/llantaI.obj");
	LlantaD_M = Model();
	LlantaD_M.LoadModel("Models/llantaD.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	parabrisas_M = Model();
	parabrisas_M.LoadModel("Models/parabrisas.obj");
	cofre_M = Model();
	cofre_M.LoadModel("Models/cofre.obj");
	parrilla_M = Model();
	parrilla_M.LoadModel("Models/parrilla.obj");
	lampara_M = Model();
	lampara_M.LoadModel("Models/lamPoke.obj");
	piedraTrueno_M = Model();
	piedraTrueno_M.LoadModel("Models/piedraTrueno.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);



	//DIRECCIONAL

	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);



	//PUNTUALES


	//contador de luces puntuales
	unsigned int pointLightCount = 0;



	glm::vec3 posLampara = glm::vec3(0.0f);

	//LUZ PUNTUAL BLANCA PARA LA LÁMPARA
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,                                 // Intensidades
		0.0f, 0.0f, 0.0f,                       // Posición
		0.1f, 0.1f, 0.1f);                         // Atenuación
	pointLightCount++;

	// Luz para la Piedra trueno
	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,    // Amarillo
		1.0f, 1.0f,                                 // Intensidad suave
		0.0f, 0.0f, 0.0f,                      
		0.1f, 0.2f, 0.3f);                         // Atenuación suave
	pointLightCount++;






	//SPOTLIGHTS


	unsigned int spotLightCount = 0;


	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,		//con, lin, exp
		5.0f);
	spotLightCount++;

	//LUZ COCHE
	spotLights[1] = SpotLight(0.0f, 0.0f, 0.8f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//Luz helicoptero
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		0.7f, 0.6f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;


	//Luz cofre
	spotLights[3] = SpotLight(1.0f, 0.5f, 0.0f,
		0.7f, 0.6f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;


	//Segundo arreglo de luces spotlight
	spotLights2[0] = spotLights[0];
	spotLights2[1] = SpotLight(0.0f, 0.0f, 0.8f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLights2[2] = spotLights[2];
	spotLights2[3] = spotLights[3];





	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();


		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		spotLights2[0].SetFlash(lowerLight, camera.getCameraDirection());



		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		//shaderList[0].SetPointLights(pointLights, pointLightCount); //Para mandar todas las luces puntuales



		if (mainWindow.getLamparaPrendida() && mainWindow.getObjetoPrendido())
		{
			// Estado 1: Ambas luces prendidas
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (!mainWindow.getLamparaPrendida() && !mainWindow.getObjetoPrendido())
		{
			// Estado 2: Ambas luces apagadas
			shaderList[0].SetPointLights(pointLights, 0);  // 0 luces
		}
		else if (mainWindow.getLamparaPrendida() && !mainWindow.getObjetoPrendido())
		{
			// Estado 3: Solo lámpara prendida
			PointLight tempLights[1] = { pointLights[0] };
			shaderList[0].SetPointLights(tempLights, 1);
		}
		else if (!mainWindow.getLamparaPrendida() && mainWindow.getObjetoPrendido())
		{
			// Estado 4: Solo piedra lunar prendida
			PointLight tempLights[1] = { pointLights[1] };
			shaderList[0].SetPointLights(tempLights, 1);
		}





		//Condicional para cambiar entre dos arreglos de luces puntuales

		if (mainWindow.getdireccion()) //Si la bandera es true
		{
			if (mainWindow.getspotlight() < 0.5f)		//Se manda la luz hacia enfrente
			{
				//Se envia al shader el arrelgo de luces puntuales
				shaderList[0].SetSpotLights(spotLights, spotLightCount); // Usar set 1
			}
			else
			{											//Se manda la luz hacia detras
				shaderList[0].SetSpotLights(spotLights2, spotLightCount); // Usar set 2
			}
		}





		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//Dado de Opengl
		//Ejercicio 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		dadoTexture.UseTexture(); //textura con transparencia o traslucidez
		meshList[4]->RenderMesh();
		glDisable(GL_BLEND);


		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.0f + mainWindow.getmuevex(), 1.25f, 0.0f));
		modelaux = model;
		spotLights[1].SetPos(glm::vec3(modelaux[3].x - 6.0f, modelaux[3].y - 0.7f, modelaux[3].z)); //posicion de la linterna con jerarquia
		spotLights2[1].SetPos(glm::vec3(modelaux[3].x + 6.0f, modelaux[3].y - 0.7f, modelaux[3].z)); //posicion de la linterna con jerarquia
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Coche_M.RenderModel();


		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -1.3f, -2.5f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaI_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.8f, -1.3f, -2.5f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaI_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.8f, -1.3f, 2.5f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaD_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.8f, -1.3f, 2.5f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaD_M.RenderModel();

		//Parabrisas
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.48f, 0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(1.645f, 1.645f, 1.645f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parabrisas_M.RenderModel();

		// Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.52f, 0.13f, -0.07));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		
		// Articulaciones
		float ang1 = glm::radians(mainWindow.getarticulacion1()); 
		float ang2 = glm::radians(mainWindow.getarticulacion2());

		model = glm::rotate(model, ang1, glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::rotate(model, ang2, glm::vec3(0.0f, 0.0f, -1.0f));

		model = glm::scale(model, glm::vec3(1.62f, 1.62f, 1.62f));

		// LUZ FIJA AL COFRE - dirección constante -Y relativa
		spotLights[3].SetPos(glm::vec3(modelaux[3].x - 4.5f, modelaux[3].y - 0.2f, modelaux[3].z));
		spotLights[3].SetDirection(glm::normalize(glm::vec3(model * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f))));

		spotLights2[3].SetPos(glm::vec3(model * glm::vec4(-0.2f, 0.15f, 0.0f, 1.0f)));
		spotLights2[3].SetDirection(glm::normalize(glm::vec3(model * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f))));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre_M.RenderModel();

		//Parrilla
		model = modelaux;
		model = glm::translate(model, glm::vec3(-7.1f, -1.06f, -0.03f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parrilla_M.RenderModel();


		//HELICOPTERO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevexHe(), 5.0f, 6.0f));
		spotLights[2].SetPos(glm::vec3(model[3].x, model[3].y - 1.0f, model[3].z));   //posicion de la luz del helicoptero
		spotLights2[2].SetPos(glm::vec3(model[3].x, model[3].y - 1.0f, model[3].z));   //posicion de la luz del helicoptero

		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Lampara alusiva a mi universo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -1.0f, -4.0f));
		pointLights[0].SetPos(glm::vec3(model[3].x, model[3].y + 8.0f, model[3].z)); //posicion de la luz de la lampara
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara_M.RenderModel();


		//PIEDRA TRUENO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-16.0f, 1.0f, -4.0f));  
		pointLights[1].SetPos(glm::vec3(model[3].x, model[3].y + 2.0f, model[3].z)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piedraTrueno_M.RenderModel();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AgaveTexture.UseTexture();



		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}