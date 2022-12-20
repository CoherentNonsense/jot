#ifndef SHADER_H
#define SHADER_H

unsigned int shader_create(const char* vertex_source, const char* fragment_source);
unsigned int shader_create_path(const char* vertex_path, const char* fragment_path);
void shader_delete(unsigned int shader);
void shader_use(unsigned int shader);

#endif