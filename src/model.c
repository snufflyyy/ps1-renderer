#include "cglm/mat4.h"
#include <stdio.h>

#include <cglm/cglm.h>
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#include "model.h"
#include "texture.h"
#include "node.h"
#include "mesh.h"

static const cgltf_accessor* find_attribute(const cgltf_primitive* prim, cgltf_attribute_type type) {
 	for (cgltf_size i = 0; i < prim->attributes_count; i++) {
        if (prim->attributes[i].type == type) {
            return prim->attributes[i].data;
        }
    }

    return NULL;
}

static usize node_count_recursive(cgltf_node* node) {
	usize count = 1;
	for (usize i = 0; i < node->children_count; i++) {
		count += node_count_recursive(node->children[i]);
	}
	return count;
}

static void node_create_recursive(Node* nodes, usize nodes_length, usize* nodes_i, usize parent_i, cgltf_node* cgltf_node) {
	if (*nodes_i >= nodes_length) {
		return;
	}

	nodes[*nodes_i].meshes = NULL;
	nodes[*nodes_i].meshes_count = 0;

	if (cgltf_node->mesh) {
	 	nodes[*nodes_i].meshes_count = cgltf_node->mesh->primitives_count;
	  	nodes[*nodes_i].meshes = (Mesh*) malloc(sizeof(Mesh) * nodes[*nodes_i].meshes_count);
	   	if (!nodes[*nodes_i].meshes) {
	    	fprintf(stderr, "[ERROR] [MODEL] Failed to allocate memory for meshes!\n");
	  		return;
	    }

    	for (usize j = 0; j < nodes[*nodes_i].meshes_count; j++) {
     		cgltf_primitive* cgltf_primitive = &cgltf_node->mesh->primitives[j];

       		const cgltf_accessor* position_accessor = find_attribute(cgltf_primitive, cgltf_attribute_type_position);
			const cgltf_accessor* normal_accessor = find_attribute(cgltf_primitive, cgltf_attribute_type_normal);
			const cgltf_accessor* texture_coords_accessor = find_attribute(cgltf_primitive, cgltf_attribute_type_texcoord);

   			usize vertices_count = position_accessor->count;
     		Vertex* vertices = (Vertex*) malloc(sizeof(Vertex) * vertices_count);
       		if (!vertices) {
       			fprintf(stderr, "[ERROR] [MODEL] Failed to allocate memory for vertices!\n");
        		free(nodes[*nodes_i].meshes);
          		return;
        	}

     		for (usize v = 0; v < vertices_count; v++) {
       			Vertex vertex = {0};
       			cgltf_accessor_read_float(position_accessor, v, vertex.position, 3);
       			cgltf_accessor_read_float(normal_accessor, v, vertex.normal, 3);
       			cgltf_accessor_read_float(texture_coords_accessor, v, vertex.texture_coords, 2);
       			vertices[v] = vertex;
       		}

       		const cgltf_accessor* indices_accessor = cgltf_primitive->indices;

         	usize indices_count = indices_accessor->count;
         	GLuint* indices = (GLuint*) malloc(sizeof(GLuint) * indices_count);
          	if (!indices) {
         		fprintf(stderr, "[ERROR] [MODEL] Failed to allocate memory for indices!\n");
          		free(vertices);
           		free(nodes[*nodes_i].meshes);
            	return;
           	}

           for (usize k = 0; k < indices_count; k++) {
         		indices[k] = (GLuint) cgltf_accessor_read_index(indices_accessor, k);
           }

           nodes[*nodes_i].meshes[j] = mesh_create(vertices, vertices_count, indices, indices_count, 0);
     	}
	}

	mat4 local = GLM_MAT4_IDENTITY_INIT;

	if (cgltf_node->has_matrix) {
		memcpy(local, cgltf_node->matrix, sizeof(mat4));
   	} else {
   		vec3 transform = GLM_VEC3_ZERO_INIT;
    	vec3 scale = GLM_VEC3_ONE_INIT;
    	versor rotation = GLM_QUAT_IDENTITY_INIT;

     	if (cgltf_node->has_translation) { glm_vec3_copy(cgltf_node->translation, transform); }
        if (cgltf_node->has_scale) { glm_vec3_copy(cgltf_node->scale, scale); }
        if (cgltf_node->has_rotation) { glm_quat_copy(cgltf_node->rotation, rotation); }

        mat4 T, R, S;
        glm_translate_make(T, transform);
        glm_quat_mat4(rotation, R);
        glm_scale_make(S, scale);

        glm_mat4_mul(T, R, local);
        glm_mat4_mul(local, S, local);
    }

	if (parent_i != (usize) -1) {
		glm_mat4_mul(nodes[parent_i].transform, local, local);
	}

	glm_mat4_copy(local, nodes[*nodes_i].transform);

	usize current_i = *nodes_i;
	(*nodes_i)++;

	for (usize i = 0; i < cgltf_node->children_count; i++) {
		node_create_recursive(nodes, nodes_length, nodes_i, current_i, cgltf_node->children[i]);
	}
}

Model model_create(const char* model_path) {
    cgltf_options options = {0};
    cgltf_data* data = NULL;
    if (cgltf_parse_file(&options, model_path, &data) != cgltf_result_success) {
   		fprintf(stderr, "[ERROR] [MODEL] Failed to load gltf model: %s\n", model_path);
    	return (Model) {0};
    }

    if (cgltf_load_buffers(&options, data, model_path) != cgltf_result_success) {
  		fprintf(stderr, "[ERROR] [MODEL] Failed to load gltf model's buffers: %s\n", model_path);
   		cgltf_free(data);
     	return (Model) {0};
    }

    cgltf_scene* scene = data->scene;

    Model model = {0};

    usize nodes_total = 0;
    for (usize i = 0; i < scene->nodes_count; i++) {
   		nodes_total += node_count_recursive(scene->nodes[i]);
    }
    model.nodes_count = nodes_total;

    model.nodes = (Node*) malloc(sizeof(Node) * model.nodes_count);
    if (!model.nodes) {
   		fprintf(stderr, "[ERROR] [MODEL] Failed to allocate memory for nodes!\n");
    	cgltf_free(data);
    	return (Model) {0};
    }

    usize nodes_i = 0;
    for (usize i = 0; i < scene->nodes_count; i++) {
    	cgltf_node* node = scene->nodes[i];
    	node_create_recursive(model.nodes, model.nodes_count, &nodes_i, (usize) -1, node);
    }

   	cgltf_free(data);

    glm_mat4_identity(model.transform);

    return model;
}

void model_draw(Model* model, Shader shader) {
	for (usize i = 0; i < model->nodes_count; i++) {
		node_draw(&model->nodes[i], model->transform, shader);
	}
}

void model_destroy(Model* model) {
	if (model->nodes) {
		for (usize i = 0; i < model->nodes_count; i++) {
			node_destroy(&model->nodes[i]);
		}
	 	free(model->nodes);
	}
	if (model->textures) {
		for (usize i = 0; i < model->textures_count; i++) {
			texture_destroy(model->textures[i]);
		}
	 	free(model->textures);
	}
	model->textures_count = 0;
}
