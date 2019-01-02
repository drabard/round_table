#include "scene.h"

#include <imgui.h>

#include "rendering.h"

void node_process_gui(struct node* node)
{
	ImGui::Text("%s: %f, %f", node->name.c_str(), node->position.x, node->position.y);
}

void scene_load_from_file(struct scene* scene, struct renderer* renderer, const char* path)
{
	std::string pstr{"data/images/panda.png"};
	texture_id tid = renderer_request_texture(renderer, &pstr);
	scene->nodes.push_back((node){(v2){}, tid, "Panda"});
}

void scene_process_gui(struct scene* scene, struct renderer* renderer)
{
	ImGui::Begin("Scene", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		if(ImGui::Button("Load from file"))
		{
			scene_load_from_file(scene, renderer, "");
		}

		for(uint32_t i = 0; i < scene->nodes.size(); ++i)
		{
			ImGui::PushID(i);
			node_process_gui(&scene->nodes[i]);
			ImGui::PopID();
		}
	}
	ImGui::End();
}

void scene_draw(struct scene* scene, struct renderer* renderer)
{
	for(uint32_t ni = 0; ni < scene->nodes.size(); ++ni)
	{
		renderer_draw_sprite(renderer, scene->nodes[ni].tex_id, scene->nodes[ni].position);
	}
}