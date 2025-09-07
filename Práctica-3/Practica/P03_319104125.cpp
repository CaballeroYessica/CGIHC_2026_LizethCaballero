//práctica 3: Modelado Geométrico y Cámara Sintética.
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}


// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		
	 -0.5f, -0.5f,0.0f,	//0
	0.5f,-0.5f,0.0f,	//1
	0.0f,0.5f, -0.15f,	//2
	0.0f,-0.5f,-0.5f,   // V3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CreateShaders();

	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera
		
		//  PYRAMINX: PIRÁMIDES 

		glm::vec3 C = glm::vec3(0.0f, 0.0f, -3.0f);
		glm::vec3 Sst = glm::vec3(0.45f, 0.45f, 0.76f);

		//// CARA 1 (ROJO) 
		color = glm::vec3(1.0f, 0.0f, 0.00f);

		// fila 0 (base) #1
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.02f, 0.0f, 0.18f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #2
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.30f, 0.07f, 0.18f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(27.5f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 0 (base) #3
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.595f, 0.0f, 0.18f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #4
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.89f, 0.07f, 0.18f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(27.5f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #5
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(1.17f, 0.0f, 0.18f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 1 (medio) #1
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.30f, 0.55f, 0.03f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila	1 (medio) #2
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.6f, 0.57f, 0.03f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(27.5f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 1 (medio) #3
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.89f, 0.55f, 0.03f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 1 (arriba) #1
		model = glm::mat4(1.0f);
		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.6f, 1.035f, -0.1f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();



		// CARA 2 (VERDE)
	
		color = glm::vec3(0.0f, 1.0f, 0.00f);



		// fila 0 (base) #1
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.750));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.02f, 0.0f, 0.0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #2
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.69));


		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.30f, 0.07f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 0 (base) #3
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.750));


		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.595f, 0.0f, 0.0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #4
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.69));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.89f, 0.07f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();
		
		// fila 0 (base) #5
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.750));


		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(1.17f, 0.0f, 0.0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 1 (medio) #1
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.75));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.30f, 0.5f, -0.19f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila	1 (medio) #2
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.7));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.6f, 0.57f, -0.19f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 2 (punta) #1
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.75));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.9f, 0.5f, -0.19f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 3 (punta) #1
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-3.87, 0.0, 4.5));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.65f, 1.1f, -0.15f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();




			// CARA 3 (AZUL) 
		color = glm::vec3(0.0f, 0.0f, 1.0f);



		// fila 0 (base) #1
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.72));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #2
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.76));


		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.37f, 0.07f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 0 (base) #3
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.76));


		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.66f, 0.0f, 0.0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #4
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.76));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.96f, 0.07f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 0 (base) #5
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.76));


		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(1.28f, 0.0f, 0.0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 1 (medio) #1
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.76));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.37f, 0.58f, -0.24f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila	1 (medio) #2
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.8));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.67f, 0.62f, -0.24f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// fila 1 (medio) #3
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.76));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.97f, 0.58f, -0.2f));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// fila 2 (punta) #1
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(1.95, 0.0, 5.76));

		model = glm::translate(model, C);
		model = glm::translate(model, glm::vec3(0.65f, 1.1f, -0.4));
		model = glm::scale(model, Sst);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();



			// CARA 4 (AMARILLO) 
			color = glm::vec3(1.0f, 0.95f, 0.15f);

			

			// fila 0 (base) #1
			model = glm::mat4(1.0f);

			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(0.10f, -0.10f, -0.08f));
			model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));

			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
			meshList[1]->RenderMesh();


			// fila 0 (base) #2
			model = glm::mat4(1.0f);

			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(0.42f, -0.10f, -0.08f));
			model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 1, 0));

			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();

			// fila 0 (base) #3
			model = glm::mat4(1.0f);

			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(0.70f, -0.10f, -0.08f));
			model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));

			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();


			// fila 0 (base) #4

			model = glm::mat4(1.0f);

			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(1.0f, -0.10f, -0.08f));
			model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 1, 0));

			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();

			// fila 0 (base) #5

			model = glm::mat4(1.0f);

			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(01.30f, -0.10f, -0.08f));
			model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0, 1, 0));

			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();


			// fila	2 (medio) #1
			model = glm::mat4(1.0f);
			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(0.30f, -0.10f, -0.35f));
			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();
			
			// fila	2 (medio) #2
			model = glm::mat4(1.0f);
			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(0.7f, -0.10f, -0.55f));
			model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 1, 0));
			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();

			// fila	2 (medio) #3
			model = glm::mat4(1.0f);
			model = glm::translate(model, C);

			//model = glm::rotate(model, glm::radians(76.0f), glm::vec3(1, 0, 0));
			model = glm::translate(model, glm::vec3(0.90f, -0.10f, -0.35f));
			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();

			// fila	3 (punta) #1
			model = glm::mat4(1.0f);
			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(0.6f, -0.10f, -0.9f));
			model = glm::scale(model, Sst);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();



			// PIRAMIDE NEGRA CENTRAL

			color = glm::vec3(0.0f, 0.0f, 0.0f);


			model = glm::mat4(1.0f);
			model = glm::translate(model, C);
			model = glm::translate(model, glm::vec3(0.59f, 0.55f, 0.2));
			model = glm::scale(model, glm::vec3(1.85f, 1.7f, 3.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[1]->RenderMesh();




		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		