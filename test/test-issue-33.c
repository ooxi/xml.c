/**
 * Copyright (c) 2023 ooxi/xml.c
 *     https://github.com/ooxi/xml.c
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software in a
 *     product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 */
#include <alloca.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xml.h>





/**
 * @return true iff xml string equals the c string
 */
static bool string_equals(struct xml_string* a, char const* b) {
	size_t a_length = xml_string_length(a);
	size_t b_length = strlen(b);

	uint8_t* a_buffer = alloca((a_length + 1) * sizeof(uint8_t));
	xml_string_copy(a, a_buffer, a_length);
	a_buffer[a_length] = 0;

	if (a_length != b_length) {
		fprintf(stderr, "string_equals: %s#%i <> %s#%i\n", a_buffer, (int)a_length, b, (int)b_length);
		return false;
	}

	size_t i = 0; for (; i < a_length; ++i) {
		if (a_buffer[i] != b[i]) {
			fprintf(stderr, "string_equals: %s <> %s\n", a_buffer, b);
			return false;
		}
	}

	return true;
}





/**
 * Spaces in attribute values did not work at one time.
 *
 * @author nashidau
 * @author ooxi
 *
 * @see https://github.com/ooxi/xml.c/issues/33
 */
int main(int argc, char** argv) {
	uint8_t* source = "<a foo=\"Has Spaces\"></a>";

	struct xml_document* document = xml_parse_document(source, strlen(source));

	if (!document) {
		fprintf(stderr, "Could parse document\n");
		return EXIT_FAILURE;
	}
	struct xml_node* node_a = xml_document_root(document);
	struct xml_string* attribute_foo_name = xml_node_attribute_name(node_a, 0);
	struct xml_string* attribute_foo_content = xml_node_attribute_content(node_a, 0);

	if (!attribute_foo_name) {
		fprintf(stderr, "Cannot load name of attribute #0\n");
		return EXIT_FAILURE;
	}
	if (!attribute_foo_content) {
		fprintf(stderr, "Cannot load content of attribute #0\n");
		return EXIT_FAILURE;
	}

	if (!string_equals(attribute_foo_name, "foo")) {
		fprintf(stderr, "Name of attribute #0 must be `foo'\n");
		return EXIT_FAILURE;
	}
	if (!string_equals(attribute_foo_content, "Has Spaces")) {
		fprintf(stderr, "Content of attribute #0 must be `Has Spaces'\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

