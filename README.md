xml.c
=====

Similar to the [GLib Markup parser](http://developer.gnome.org/glib/2.34/glib-Simple-XML-Subset-Parser.html),
which also just parses an xml subset, [xml.c](https://github.com/ooxi/xml.c) is
a simple, small and self contained xml parser in one file. Ideal for embedding
into other projects without the need for big external dependencies.

[![Build Status](https://github.com/ooxi/xml.c/actions/workflows/ci.yaml/badge.svg)](https://github.com/ooxi/xml.c/actions)


Downloads
---------

All releases are based on master, so the preferred way of using xml.c is adding
the repository as [git submodule](http://git-scm.com/book/en/Git-Tools-Submodules).

If you prefer formal releases, check out the [release tags](https://github.com/ooxi/xml.c/tags).


Building xml.c
--------------

Since xml.c uses [CMake](http://www.cmake.org/), building the library is fairly
easy

    $ git clone https://github.com/ooxi/xml.c.git xml.c
    $ mkdir xml.c/build; cd xml.c/build
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make && make test

If you need a debug build, specify `CMAKE_BUILD_TYPE` as `Debug` and rebuild.


Usage
-----

This example is also [included in the repository](https://github.com/ooxi/xml.c/blob/master/test/example.c)
and will be build by default. Most of the code is C boilerplate, the important
functions are `xml_parse_document`, `xml_document_root`, `xml_node_name`,
`xml_node_content` and `xml_node_child` / `xml_node_children`.

```c
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
	 * xml.c, then use a debug build (cmake -DCMAKE_BUILD_TYPE=Debug) which
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
```

Another usage example can be found in the [unit case](https://github.com/ooxi/xml.c/blob/master/test/test-xml.c).


License
-------

[libpng/zlib](https://github.com/ooxi/xml.c/blob/master/LICENSE) (BSD)

