// This file is generated.
#pragma once

#include <stddef.h>

typedef void* (*array_alloc_t)(size_t);
typedef void* (*array_realloc_t)(void*, size_t);
typedef void* (*array_free_t)(void*);

void array_set_alloc(array_alloc_t, array_realloc_t, array_free_t);

struct array_uint32_t
{
	uint32_t* buffer;
	uint32_t size;
	uint32_t capacity;

	uint32_t& operator[](uint32_t idx);
};

void array_uint32_t_create(struct array_uint32_t* array);
void array_uint32_t_destroy(struct array_uint32_t* array);
void array_uint32_t_change_capacity(struct array_uint32_t* array, uint32_t new_size);
void array_uint32_t_resize(struct array_uint32_t* array, uint32_t new_size);
void array_uint32_t_push_back(struct array_uint32_t* array, uint32_t element);
void array_uint32_t_remove(struct array_uint32_t* array, uint32_t index);

struct array_uint16_t
{
	uint16_t* buffer;
	uint32_t size;
	uint32_t capacity;

	uint16_t& operator[](uint32_t idx);
};

void array_uint16_t_create(struct array_uint16_t* array);
void array_uint16_t_destroy(struct array_uint16_t* array);
void array_uint16_t_change_capacity(struct array_uint16_t* array, uint32_t new_size);
void array_uint16_t_resize(struct array_uint16_t* array, uint32_t new_size);
void array_uint16_t_push_back(struct array_uint16_t* array, uint16_t element);
void array_uint16_t_remove(struct array_uint16_t* array, uint32_t index);

struct array_uint8_t
{
	uint8_t* buffer;
	uint32_t size;
	uint32_t capacity;

	uint8_t& operator[](uint32_t idx);
};

void array_uint8_t_create(struct array_uint8_t* array);
void array_uint8_t_destroy(struct array_uint8_t* array);
void array_uint8_t_change_capacity(struct array_uint8_t* array, uint32_t new_size);
void array_uint8_t_resize(struct array_uint8_t* array, uint32_t new_size);
void array_uint8_t_push_back(struct array_uint8_t* array, uint8_t element);
void array_uint8_t_remove(struct array_uint8_t* array, uint32_t index);

struct array_int32_t
{
	int32_t* buffer;
	uint32_t size;
	uint32_t capacity;

	int32_t& operator[](uint32_t idx);
};

void array_int32_t_create(struct array_int32_t* array);
void array_int32_t_destroy(struct array_int32_t* array);
void array_int32_t_change_capacity(struct array_int32_t* array, uint32_t new_size);
void array_int32_t_resize(struct array_int32_t* array, uint32_t new_size);
void array_int32_t_push_back(struct array_int32_t* array, int32_t element);
void array_int32_t_remove(struct array_int32_t* array, uint32_t index);

struct array_int16_t
{
	int16_t* buffer;
	uint32_t size;
	uint32_t capacity;

	int16_t& operator[](uint32_t idx);
};

void array_int16_t_create(struct array_int16_t* array);
void array_int16_t_destroy(struct array_int16_t* array);
void array_int16_t_change_capacity(struct array_int16_t* array, uint32_t new_size);
void array_int16_t_resize(struct array_int16_t* array, uint32_t new_size);
void array_int16_t_push_back(struct array_int16_t* array, int16_t element);
void array_int16_t_remove(struct array_int16_t* array, uint32_t index);

struct array_int8_t
{
	int8_t* buffer;
	uint32_t size;
	uint32_t capacity;

	int8_t& operator[](uint32_t idx);
};

void array_int8_t_create(struct array_int8_t* array);
void array_int8_t_destroy(struct array_int8_t* array);
void array_int8_t_change_capacity(struct array_int8_t* array, uint32_t new_size);
void array_int8_t_resize(struct array_int8_t* array, uint32_t new_size);
void array_int8_t_push_back(struct array_int8_t* array, int8_t element);
void array_int8_t_remove(struct array_int8_t* array, uint32_t index);

struct array_double
{
	double* buffer;
	uint32_t size;
	uint32_t capacity;

	double& operator[](uint32_t idx);
};

void array_double_create(struct array_double* array);
void array_double_destroy(struct array_double* array);
void array_double_change_capacity(struct array_double* array, uint32_t new_size);
void array_double_resize(struct array_double* array, uint32_t new_size);
void array_double_push_back(struct array_double* array, double element);
void array_double_remove(struct array_double* array, uint32_t index);

struct array_float
{
	float* buffer;
	uint32_t size;
	uint32_t capacity;

	float& operator[](uint32_t idx);
};

void array_float_create(struct array_float* array);
void array_float_destroy(struct array_float* array);
void array_float_change_capacity(struct array_float* array, uint32_t new_size);
void array_float_resize(struct array_float* array, uint32_t new_size);
void array_float_push_back(struct array_float* array, float element);
void array_float_remove(struct array_float* array, uint32_t index);

struct array_node
{
	struct node* buffer;
	uint32_t size;
	uint32_t capacity;

	struct node& operator[](uint32_t idx);
};

void array_node_create(struct array_node* array);
void array_node_destroy(struct array_node* array);
void array_node_change_capacity(struct array_node* array, uint32_t new_size);
void array_node_resize(struct array_node* array, uint32_t new_size);
void array_node_push_back(struct array_node* array, struct node* element);
void array_node_remove(struct array_node* array, uint32_t index);

