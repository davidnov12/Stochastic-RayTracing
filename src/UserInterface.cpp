#include <UserInterface.h>

#include <3rd_party/imgui/imgui.h>
#include <3rd_party/imgui/imgui_impl_glfw.h>
#include <3rd_party/imgui/imgui_impl_opengl3.h>
#include <3rd_party/imgui/ImGuiFileDialog.h>


UserInterface::~UserInterface() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UserInterface::initGUI() {

	IMGUI_CHECKVERSION();
	
	if(ImGui::CreateContext() == nullptr)
		throw std::exception("Cannot create GUI context\n");
	
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if(&io == nullptr)
		throw std::exception("Cannot create GUI\n");

	io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 12.0f);
	//io.Fonts->AddFontFromFileTTF("MyriadPro.ttf", 11.0f);

	if(!ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true))
		throw std::exception("Cannot create GUI\n");

	if(!ImGui_ImplOpenGL3_Init())
		throw std::exception("Cannot create GUI\n");

	//ImGui::StyleColorsLight();
	ImGui::StyleColorsDark();

}

void UserInterface::renderGUI() {

	if (!showGUI)
		return;

	std::string filePathName;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Stochastic Ray Tracing");

	ImGui::NewLine();
	ImGui::Text("Soft shadow samples");
	ImGui::SliderInt("S", &(data->shadowSamples), 0, 200);
	ImGui::NewLine();
	ImGui::Text("Indirect illumination samples");
	ImGui::SliderInt("I", &(data->indirectSamples), 0, 200);
	ImGui::NewLine();
	ImGui::Text("Depth of field samples");
	ImGui::SliderInt("D", &(data->dofSamples), 1, 100);
	ImGui::NewLine();
	ImGui::Text("Ambient occlusion samples");
	ImGui::SliderInt("A", &(data->aoSamples), 0, 200);
	ImGui::NewLine();
	ImGui::Text("Rendering mode");
	
	//ImGui::PlotLines("Chart", my_values, IM_ARRAYSIZE(my_values))

	if (ImGui::RadioButton("Standard", data->renderMode))
		data->renderMode = true;

	if (ImGui::RadioButton("Heatmap", !data->renderMode))
		data->renderMode = false;

	ImGui::NewLine();

	if (ImGui::Button("Load scene..."))
		showFileBrowser = true;
	ImGui::SameLine();
	if (ImGui::Button("Show profile"))
		showProfiler = true;
	//ImGui::SameLine();
	//ImGui::NewLine();
	if (ImGui::Button("Take picture"))
		;
	ImGui::SameLine();
	if (ImGui::Button("Show help"))
		showHelp = true;
	
	if (showFileBrowser && ImGuiFileDialog::Instance()->FileDialog("Choose Scene File", ".stl\0.obj\0.ply\0.dae\0\0", ".", "")) {
		if (ImGuiFileDialog::Instance()->IsOk == true){
			data->changeNotify = true;
			data->sceneFile = ImGuiFileDialog::Instance()->GetFilepathName();
			filePathName = ImGuiFileDialog::Instance()->GetFilepathName();
			showbvhChoose = true;
			//std::cout << filePathName << std::endl;
		}
		showFileBrowser = false;
	}
	
	
	if (showbvhChoose) {
		ImGui::Begin("Choose BVH type");

		ImGui::Text("Type of BVH");

		if (ImGui::RadioButton("CPU implementation", !data->bvhType)) {
			data->bvhType = 0;
		}
		if (ImGui::RadioButton("GPU implementation", data->bvhType)) {
			data->bvhType = 1;
		}
		ImGui::NewLine();
		if (ImGui::Button("Confirm")) {
			showbvhChoose = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			showbvhChoose = false;
		}

		ImGui::End();
	}

	/*
	ImGui::SliderInt("S", shadowSamples, 0, 200);
	ImGui::NewLine();
	ImGui::Text("Indirect illumination samples");
	ImGui::SliderInt("I", indirectSamples, 0, 200);
	ImGui::NewLine();
	ImGui::Text("Depth of field samples");
	ImGui::SliderInt("D", dofSamples, 1, 100);
	ImGui::NewLine();
	ImGui::Text("Ambient occlusion samples");
	ImGui::SliderInt("A", aoSamples, 0, 200);
	ImGui::NewLine();
	std::string rt = "Compute time: " + std::to_string(ui.renderTime) + " ms";
	ImGui::Text(rt.c_str());*/
	ImGui::NewLine();
	ImGui::End();

	
	


	if (showProfiler) {
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();
		ImGui::Begin("Profiler");
		const float my_values[] = { 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f, 0.2f, 0.1f, 10.0f, 0.5f, 0.9f, 2.2f };
		ImGui::PlotLines("Rendering times [ms]", my_values, IM_ARRAYSIZE(my_values), 0, "", 0.0f, 10.0f, ImVec2(250, 90));
		ImGui::Text("Average %.2f ms", 0.5f);
		ImGui::Text("Current %.2f ms", 0.5f);
		ImGui::NewLine();
		if (ImGui::Button("Close"))
			showProfiler = false;

		ImGui::End();
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	if (showHelp) {

		ImGui::Begin("Help");

		ImGui::Text("W, A. S, D - move");
		ImGui::Text("I, J, K, L, N, M - light move (x, y, z axis)");
		ImGui::Text("Mouse (Drag & Drop) - Camera looking, app controls");

		if (ImGui::Button("Close"))
			showHelp = false;

		ImGui::End();

	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}