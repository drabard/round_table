// This file is generated.

#include "scene.h"

static array_alloc_t alloc;
static array_realloc_t realloc;
static array_free_t free;

void array_set_alloc(array_alloc_t a, array_realloc_t r, array_free_t f)
{
	alloc = a;
	realloc = r;
	free = f;
}


uint32_t& array_uint32_t::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_uint32_t_create(struct array_uint32_t* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_uint32_t_destroy(struct array_uint32_t* array)
{
	free(array->buffer);
	*array = (struct array_uint32_t){};
}

void array_uint32_t_change_capacity(struct array_uint32_t* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_uint32_t_resize(struct array_uint32_t* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_uint32_t_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_uint32_t_push_back(struct array_uint32_t* array, uint32_t element)
{
	if(array->size > array->capacity - 1)
	{
		array_uint32_t_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_uint32_t_remove(struct array_uint32_t array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

uint16_t& array_uint16_t::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_uint16_t_create(struct array_uint16_t* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_uint16_t_destroy(struct array_uint16_t* array)
{
	free(array->buffer);
	*array = (struct array_uint16_t){};
}

void array_uint16_t_change_capacity(struct array_uint16_t* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_uint16_t_resize(struct array_uint16_t* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_uint16_t_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_uint16_t_push_back(struct array_uint16_t* array, uint16_t element)
{
	if(array->size > array->capacity - 1)
	{
		array_uint16_t_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_uint16_t_remove(struct array_uint16_t array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

uint8_t& array_uint8_t::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_uint8_t_create(struct array_uint8_t* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_uint8_t_destroy(struct array_uint8_t* array)
{
	free(array->buffer);
	*array = (struct array_uint8_t){};
}

void array_uint8_t_change_capacity(struct array_uint8_t* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_uint8_t_resize(struct array_uint8_t* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_uint8_t_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_uint8_t_push_back(struct array_uint8_t* array, uint8_t element)
{
	if(array->size > array->capacity - 1)
	{
		array_uint8_t_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_uint8_t_remove(struct array_uint8_t array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

int32_t& array_int32_t::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_int32_t_create(struct array_int32_t* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_int32_t_destroy(struct array_int32_t* array)
{
	free(array->buffer);
	*array = (struct array_int32_t){};
}

void array_int32_t_change_capacity(struct array_int32_t* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_int32_t_resize(struct array_int32_t* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_int32_t_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_int32_t_push_back(struct array_int32_t* array, int32_t element)
{
	if(array->size > array->capacity - 1)
	{
		array_int32_t_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_int32_t_remove(struct array_int32_t array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

int16_t& array_int16_t::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_int16_t_create(struct array_int16_t* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_int16_t_destroy(struct array_int16_t* array)
{
	free(array->buffer);
	*array = (struct array_int16_t){};
}

void array_int16_t_change_capacity(struct array_int16_t* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_int16_t_resize(struct array_int16_t* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_int16_t_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_int16_t_push_back(struct array_int16_t* array, int16_t element)
{
	if(array->size > array->capacity - 1)
	{
		array_int16_t_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_int16_t_remove(struct array_int16_t array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

int8_t& array_int8_t::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_int8_t_create(struct array_int8_t* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_int8_t_destroy(struct array_int8_t* array)
{
	free(array->buffer);
	*array = (struct array_int8_t){};
}

void array_int8_t_change_capacity(struct array_int8_t* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_int8_t_resize(struct array_int8_t* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_int8_t_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_int8_t_push_back(struct array_int8_t* array, int8_t element)
{
	if(array->size > array->capacity - 1)
	{
		array_int8_t_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_int8_t_remove(struct array_int8_t array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

double& array_double::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_double_create(struct array_double* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_double_destroy(struct array_double* array)
{
	free(array->buffer);
	*array = (struct array_double){};
}

void array_double_change_capacity(struct array_double* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_double_resize(struct array_double* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_double_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_double_push_back(struct array_double* array, double element)
{
	if(array->size > array->capacity - 1)
	{
		array_double_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_double_remove(struct array_double array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

float& array_float::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_float_create(struct array_float* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_float_destroy(struct array_float* array)
{
	free(array->buffer);
	*array = (struct array_float){};
}

void array_float_change_capacity(struct array_float* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_float_resize(struct array_float* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_float_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_float_push_back(struct array_float* array, float element)
{
	if(array->size > array->capacity - 1)
	{
		array_float_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = element;
}

void array_float_remove(struct array_float array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

struct node& array_node::operator[](uint32_t idx)
{
	return buffer[idx];
}

void array_node_create(struct array_node* array)
{
	array->buffer = 0;
	array->size = 0;
	array->capacity = 0;
}

void array_node_destroy(struct array_node* array)
{
	free(array->buffer);
	*array = (struct array_node){};
}

void array_node_change_capacity(struct array_node* array, uint32_t new_capacity)
{
	realloc(array->buffer, new_capacity);
	array->capacity = new_capacity;
}


void array_node_resize(struct array_node* array, uint32_t new_size)
{
	if(new_size > array->capacity)
	{
		array_node_change_capacity(array, new_size);
	}

	array->size = new_size;
}

void array_node_push_back(struct array_node* array, struct node* element)
{
	if(array->size > array->capacity - 1)
	{
		array_node_change_capacity(array, array->capacity * 2);
	}

	array->buffer[array->size++] = *element;
}

void array_node_remove(struct array_node array, uint32_t index)
{
	array->buffer[index] = array->buffer[array->size - 1];
	array->size--;
}

