/*

*/

#include <RayTracing.h>

void RayTracing::init(){

	camera = std::make_shared<FPSCameraManager>();

	std::ifstream is;

	// Compute shader - Ray Tracing
	std::string src = "";
	try {
		std::string tmp;
		is.open(COMPUTE_SHADER_PATH);
		while (!(is.eof())) {
			std::getline(is, tmp);
			src.append(tmp);
			src.append("\n");
		}
		is.close();
	}
	catch (std::exception e) {
		is.close();
	}

	auto cs = std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER, src);
	tracer = std::make_shared<ge::gl::Program>(cs);
	// Compute shader - Ray Tracing


	// Vertex shader - display
	src = "";
	try {
		std::string tmp;
		is.open(VERTEX_SHADER_PATH);
		while (!(is.eof())) {
			std::getline(is, tmp);
			src.append(tmp);
			src.append("\n");
		}
		is.close();
	}
	catch (std::exception e) {
		is.close();
	}

	auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, src);
	// Vertex shader - display


	// Fragment shader - display
	src = "";
	try {
		std::string tmp;
		is.open(FRAGMENT_SHADER_PATH);
		while (!(is.eof())) {
			std::getline(is, tmp);
			src.append(tmp);
			src.append("\n");
		}
		is.close();
	}
	catch (std::exception e) {
		is.close();
	}
	auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, src);
	display = std::make_shared<ge::gl::Program>(vs, fs);
	// Fragment shader - display


	// SSBOs
	//geomBuff = std::make_shared<ge::gl::Buffer>(sizeof(Scene::gpu_triangle) * s.getGeometry().size());
	//geomBuff->realloc(sizeof(Scene::gpu_triangle) * s.getGeometry().size());
//	geomBuff->setData(s.getGeometry());
	//geomBuff = std::make_shared<ge::gl::Buffer>();
	//geomBuff->setData(s.getGeometry());
	//matBuff = std::make_shared<ge::gl::Buffer>();
	//matBuff->setData(s.getMaterials());
	//renderBuff = std::make_shared<ge::gl::Buffer>(sizeof(glm::vec4) * w.getWidth() * w.getHeight());*/
	geomBuff = std::make_shared<ge::gl::Buffer>(sizeof(Scene::gpu_triangle));
	matBuff = std::make_shared<ge::gl::Buffer>(sizeof(Scene::gpu_material));

	ge::gl::glGenTextures(1, &renderTexture);
	ge::gl::glActiveTexture(GL_TEXTURE0);
	ge::gl::glBindTexture(GL_TEXTURE_2D, renderTexture);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, win->getWidth(), win->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	ge::gl::glBindImageTexture(0, renderTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	// SSBOs


}

void RayTracing::setWindowObject(std::shared_ptr<Window> w){
	win = w;
}

void RayTracing::render(){

	if (drawFrame) {

		camera->camera_move(win->getWindow(), static_cast<float>(glfwGetTime()));

		if (win->isResized()) {
			ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, win->getWidth(), win->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
			printf("%d %d\n", win->getWidth(), win->getHeight());
		}
		
		ge::gl::glClearColor(0.1f, 0.9f, 0.4f, 1.0f);
		ge::gl::glClear(GL_COLOR_BUFFER_BIT);

		// Ray trace
		tracer->use();

		std::vector<glm::vec3> sp = camera->c.getScreenCoords();//c.getScreenCoords();
		glm::vec3 spx = sp[2] - sp[0];
		glm::vec3 spy = sp[1] - sp[0];
		tracer->set3f("screen_plane[0]", spx.x, spx.y, spx.z);
		tracer->set3f("screen_plane[1]", spy.x, spy.y, spy.z);
		tracer->set3f("screen_plane[2]", sp[0].x, sp[0].y, sp[0].z);
		tracer->set3f("view_pos", camera->c.getPosition().x, camera->c.getPosition().y, camera->c.getPosition().z);
		tracer->set1i("width", win->getWidth());
		tracer->set1i("height", win->getHeight());
	
		//tracer->set1i("shadowSamples", win.getUI().shadowSamples);
		//tracer->set1i("indirectSamples", w.getUI().indirectSamples);
		//tracer->set1i("dofSamples", w.getUI().dofSamples);
		//tracer->set1i("aoSamples", w.getUI().aoSamples);
		//tracer->set1i("triCount", s.getGeometry().size());
		geomBuff->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
		matBuff->bindBase(GL_SHADER_STORAGE_BUFFER, 2);
		GLint wgs[3];
		tracer->getComputeWorkGroupSize(wgs);
		ge::gl::glDispatchCompute(ceil(win->getWidth() / static_cast<float>(wgs[0])), ceil(win->getHeight() / static_cast<float>(wgs[1])), 1);
		ge::gl::glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		geomBuff->unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		matBuff->unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		// Ray trace

		// Draw frame
		display->use();
		ge::gl::glActiveTexture(GL_TEXTURE0);
		ge::gl::glBindTexture(GL_TEXTURE_2D, renderTexture);
		ge::gl::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		// Draw frame

	}

}

void RayTracing::updateScene(Scene & s){

	drawFrame = false;

	// ... load scene
	printf("geo %d\n", s.getGeometry().size());
	geomBuff->realloc(sizeof(Scene::gpu_triangle) * s.getGeometry().size());
	//geomBuff = std::make_shared<ge::gl::Buffer>(sizeof(Scene::gpu_triangle) * s.getGeometry().size());
	geomBuff->setData(s.getGeometry());
	
	printf("mat %d\n", s.getMaterials().size());
	matBuff->realloc(sizeof(Scene::gpu_material) * s.getMaterials().size());
	matBuff->setData(s.getMaterials());

	// BVH data

	drawFrame = true;

}
