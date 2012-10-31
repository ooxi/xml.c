/**
 * Copyright (c) 2012 ooxi/xml.c
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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <xml.h>



int main(int argc, char** argv) {

	/* XML source, could be read from disk
	 */
	uint8_t* source = ""
		"<Root>"
			"<Hello>World</Hello>"
			"<This>"
				"<Is>:-)</Is>"
				"<An>:-O</An>"
				"<Example>:-D</Example>"
			"</This>"
		"</Root>"
	;


	/* Parse the document
	 *
	 * Watch out: Remember not to free the source until you have freed the
	 *     document itself. If you have to free the source before, supply a
	 *     copy to xml_parse_document which can be freed together with the
	 *     document (`free_buffer' argument to `xml_document_free')
	 */
	struct xml_document* document = xml_parse_document(source, strlen(source));

	/* You _have_ to check the result of `xml_parse_document', if it's 0
	 * then the source could not be parsed. If you think this is a bug in
	 * xml.c, than use a debug build (cmake -DCMAKE_BUILD_TYPE=Debug) which
	 * will verbosely tell you about the parsing process
	 */
	if (!document) {
		printf("Could parse document\n");
		exit(EXIT_FAILURE);
	}
	struct xml_node* root = xml_document_root(document);


	/* Say Hello World :-)
	 */
	struct xml_node* root_hello = xml_node_child(root, 0);
	struct xml_string* hello = xml_node_name(root_hello);
	struct xml_string* world = xml_node_content(root_hello);

	/* Watch out: `xml_string_copy' will not 0-terminate your buffers! (but
	 *     `calloc' will :-)
	 */
	uint8_t* hello_0 = calloc(xml_string_length(hello) + 1, sizeof(uint8_t));
	uint8_t* world_0 = calloc(xml_string_length(world) + 1, sizeof(uint8_t));
	xml_string_copy(hello, hello_0, xml_string_length(hello));
	xml_string_copy(world, world_0, xml_string_length(world));

	printf("%s %s\n", hello_0, world_0);
	free(hello_0);
	free(world_0);


	/* Extract amount of Root/This children
	 */
	struct xml_node* root_this = xml_node_child(root, 1);
	printf("Root/This has %lu children\n", (unsigned long)xml_node_children(root_this));


	/* Remember to free the document or you'll risk a memory leak
	 */
	xml_document_free(document, false);
}

