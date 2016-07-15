/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) Blender Foundation
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Lukas Toenne
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file GPU_strands.h
 *  \ingroup gpu
 */

#ifndef __GPU_STRANDS_H__
#define __GPU_STRANDS_H__

#ifdef __cplusplus
extern "C" {
#endif

struct GPUAttrib;
struct Strands;

/* Shader */

typedef struct GPUStrandsShader GPUStrandsShader;

typedef enum GPUStrands_ShaderModel {
	GPU_STRAND_SHADER_CLASSIC_BLENDER,
	GPU_STRAND_SHADER_KAJIYA,
	GPU_STRAND_SHADER_MARSCHNER,
} GPUStrands_ShaderModel;

typedef enum GPUStrands_Effects {
	GPU_STRAND_EFFECT_CLUMP      = (1 << 0),
	GPU_STRAND_EFFECT_CURL          = (1 << 1),
} GPUStrands_Effects;

typedef enum GPUStrands_FiberPrimitive {
	GPU_STRANDS_FIBER_LINE = 0,
	GPU_STRANDS_FIBER_RIBBON,
} GPUStrands_FiberPrimitive;

typedef struct GPUDrawStrandsParams {
	struct Strands *strands;
	struct BMEditStrands *edit;
	struct DerivedMesh *root_dm;
	int subdiv;
	GPUStrands_FiberPrimitive fiber_primitive;
	int effects;
	bool use_geomshader;
	GPUStrands_ShaderModel shader_model;
} GPUDrawStrandsParams;

GPUStrandsShader *GPU_strand_shader_get(struct GPUDrawStrandsParams *params);

void GPU_strand_shader_free(struct GPUStrandsShader *gpu_shader);

void GPU_strand_shader_bind(
        GPUStrandsShader *gpu_shader,
        float viewmat[4][4], float viewinv[4][4],
        float ribbon_width,
        float clump_thickness, float clump_shape,
        float curl_thickness, float curl_shape, float curl_radius, float curl_length,
        int debug_value);
void GPU_strand_shader_bind_uniforms(
        GPUStrandsShader *gpu_shader,
        float obmat[4][4], float viewmat[4][4]);
void GPU_strand_shader_unbind(GPUStrandsShader *gpu_shader);
bool GPU_strand_shader_bound(GPUStrandsShader *gpu_shader);

void GPU_strand_shader_get_fiber_attributes(struct GPUStrandsShader *gpu_shader,
                                            struct GPUAttrib **r_attrib, int *r_num);


/* Strand Buffers */

typedef enum GPUStrands_Component {
	GPU_STRANDS_COMPONENT_CONTROLS = (1 << 0),
	GPU_STRANDS_COMPONENT_FIBER_ATTRIBUTES = (1 << 1),
	GPU_STRANDS_COMPONENT_FIBERS = (1 << 2) | GPU_STRANDS_COMPONENT_FIBER_ATTRIBUTES,
	GPU_STRANDS_COMPONENT_ALL = ~0,
} GPUStrands_Component;

struct GPUDrawStrands *GPU_strands_buffer_create(struct GPUDrawStrandsParams *params);

void GPU_strands_setup_verts(struct GPUDrawStrands *gpu_buffer, struct GPUDrawStrandsParams *params);
void GPU_strands_setup_edges(struct GPUDrawStrands *gpu_buffer, struct GPUDrawStrandsParams *params);
void GPU_strands_setup_fibers(struct GPUDrawStrands *gpu_buffer, struct GPUDrawStrandsParams *params);
void GPU_strands_buffer_invalidate(struct GPUDrawStrands *gpu_buffer, GPUStrands_Component);

void GPU_strands_buffer_unbind(void);

void GPU_strands_buffer_free(struct GPUDrawStrands *gpu_buffer);


#ifdef __cplusplus
}
#endif

#endif /*__GPU_MATERIAL_H__*/
