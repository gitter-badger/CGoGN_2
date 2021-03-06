/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* Copyright (C) 2015, IGG Group, ICube, University of Strasbourg, France       *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#define CGOGN_RENDERING_DLL_EXPORT

#include <rendering/shaders/shader_vector_per_vertex.h>

#include <QOpenGLFunctions>
#include <iostream>

namespace cgogn
{

namespace rendering
{

const char* ShaderVectorPerVertex::vertex_shader_source_ =
	"#version 150\n"
	"in vec3 vertex_pos;\n"
	"in vec3 vertex_normal;\n"
	"out vec3 normal;\n"
	"void main() {\n"
	"   normal = vertex_normal;\n"
	"   gl_Position = vec4(vertex_pos,1.0);\n"
	"}\n";

const char* ShaderVectorPerVertex::geometry_shader_source_ =
	"#version 150\n"
	"layout(points) in;\n"
	"layout(line_strip,max_vertices=2) out;\n"
	"in vec3 normal[];\n"
	"uniform mat4 projection_matrix;\n"
	"uniform mat4 model_view_matrix;\n"
	"uniform float length;\n"
	"void main() {\n"
	"   gl_Position = projection_matrix * model_view_matrix * gl_in[0].gl_Position;\n"
	"	EmitVertex();\n"
	"   vec4 end_point = gl_in[0].gl_Position + vec4(length * normal[0], 0.0);\n"
	"   gl_Position = projection_matrix * model_view_matrix * end_point;\n"
	"	EmitVertex();\n"
	"	EndPrimitive();\n"
	"}\n";

const char* ShaderVectorPerVertex::fragment_shader_source_ =
	"#version 150\n"
	"uniform vec4 color;\n"
	"out vec4 fragColor;\n"
	"void main() {\n"
	"   fragColor = color;\n"
	"}\n";

ShaderVectorPerVertex::ShaderVectorPerVertex()
{
	prg_.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader_source_);
	prg_.addShaderFromSourceCode(QOpenGLShader::Geometry, geometry_shader_source_);
	prg_.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader_source_);
	prg_.bindAttributeLocation("vertex_pos", ATTRIB_POS);
	prg_.bindAttributeLocation("vertex_normal", ATTRIB_NORMAL);
	prg_.link();

	get_matrices_uniforms();

	unif_color_ = prg_.uniformLocation("color");
	unif_length_ = prg_.uniformLocation("length");
}

void ShaderVectorPerVertex::set_color(const QColor& rgb)
{
	prg_.setUniformValue(unif_color_, rgb);
}

void ShaderVectorPerVertex::set_length(float l)
{
	prg_.setUniformValue(unif_length_, l);
}

bool ShaderVectorPerVertex::set_vao(unsigned int i, VBO* vbo_pos, VBO* vbo_normal)
{
	if (i >= vaos_.size())
	{
		std::cerr << "VAO number " << i << " does not exist" << std::endl;
		return false;
	}

	QOpenGLFunctions *ogl = QOpenGLContext::currentContext()->functions();

	prg_.bind();
	vaos_[i]->bind();

	// position vbo
	vbo_pos->bind();
	ogl->glEnableVertexAttribArray(ATTRIB_POS);
	ogl->glVertexAttribPointer(ATTRIB_POS, vbo_pos->vector_dimension(), GL_FLOAT, GL_FALSE, 0, 0);
	vbo_pos->release();

	// normal vbo
	vbo_normal->bind();
	ogl->glEnableVertexAttribArray(ATTRIB_NORMAL);
	ogl->glVertexAttribPointer(ATTRIB_NORMAL, vbo_normal->vector_dimension(), GL_FLOAT, GL_FALSE, 0, 0);
	vbo_normal->release();

	vaos_[i]->release();
	prg_.release();

	return true;
}

} // namespace rendering

} // namespace cgogn
