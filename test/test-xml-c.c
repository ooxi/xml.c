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
#include <alloca.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <xml.h>





/**
 * Will halt the program iff assertion fails
 */
static void _assert_that(_Bool condition, char const* message, char const* func, char const* file, int line) {
	if (!condition) {
		fprintf(stderr, "Assertion failed: %s, in %s (%s:%i)\n", message, func, file, line);
		exit(EXIT_FAILURE);
	}
}

#define assert_that(condition, message)					\
	_assert_that(condition, message, __func__, __FILE__, __LINE__)	



/**
 * @return true iff xml string equals the c string
 */
static _Bool string_equals(struct xml_string* a, char const* b) {
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
 * Converts a static character array to an uint8_t data source
 */
#define SOURCE(source, content)						\
	uint8_t* source = calloc(strlen(content) + 1, sizeof(uint8_t));	\
	{	char const* content_string = content;			\
		memcpy(source, content_string, strlen(content) + 1);	\
	}



/**
 * Tries to parse a simple document containing only one tag
 */
static void test_xml_parse_document_0() {
	SOURCE(source, "<Hello>World</Hello>");
//	uint8_t* source = malloc((1 + strlen("<Hello>World</Hello>")) * sizeof(uint8_t));
//	{	char const* content_string = "<Hello>World</Hello>";
//		memcpy(source, content_string, strlen("<Hello>World</Hello>") + 1);
//	}

	struct xml_document* document = xml_parse_document(source, strlen(source));
	assert_that(document, "Could not parse document");

	struct xml_node* root = xml_document_root(document);
	assert_that(string_equals(xml_node_name(root), "Hello"), "root node name must be `Hello'");
	assert_that(string_equals(xml_node_content(root), "World"), "root node content must be `World'");

	xml_document_free(document, true);
}

/**
 * Tries to parse a document containing multiple tags
 */
static void test_xml_parse_document_1() {
	SOURCE(source, ""
		"<Parent>\n"
		"\t<Child>\n"
		"\t\tFirst content\n"
		"\t</Child>\n"
		"\t<Child>\n"
		"\t\tSecond content\n"
		"\t</Child>\n"
		"</Parent>\n"
	);
	struct xml_document* document = xml_parse_document(source, strlen(source));
	assert_that(document, "Could not parse document");

	struct xml_node* root = xml_document_root(document);
	assert_that(string_equals(xml_node_name(root), "Parent"), "root node name must be `Parent'");
	assert_that(2 == xml_node_children(root), "root must have two children");

	struct xml_node* first_child = xml_node_child(root, 0);
	struct xml_node* second_child = xml_node_child(root, 1);
	assert_that(first_child && second_child, "Failed retrieving the children of root");

	struct xml_node* third_child = xml_node_child(root, 2);
	assert_that(!third_child, "root has a third child where non should be");

	assert_that(string_equals(xml_node_name(first_child), "Child"), "first_child node name must be `Child'");
	assert_that(string_equals(xml_node_content(first_child), "First content"), "first_child node content must be `First content'");
	assert_that(string_equals(xml_node_name(second_child), "Child"), "second_child node name must be `Child'");
	assert_that(string_equals(xml_node_content(second_child), "Second content"), "second_child node content must be `tSecond content'");

	xml_document_free(document, true);
}



/**
 * Tests the eas functionality
 */
static void test_xml_parse_document_2() {
	SOURCE(source, ""
		"<Parent>\n"
		"\t<Child>\n"
		"\t\tFirst content\n"
		"\t</Child>\n"
		"\t<This><Is>\n"
			"<A><Test>Content A</Test></A>\n"
			"<B><Test>Content B</Test></B>\n"
		"\t</Is></This>\n"
		"\t<Child>\n"
		"\t\tSecond content\n"
		"\t</Child>\n"
		"</Parent>\n"
	);
	struct xml_document* document = xml_parse_document(source, strlen(source));
	assert_that(document, "Could not parse document");

	struct xml_node* root = xml_document_root(document);
	assert_that(string_equals(xml_node_name(root), "Parent"), "root node name must be `Parent'");
	assert_that(3 == xml_node_children(root), "root must have two children");

	struct xml_node* test_a = xml_easy_child(root, "This", "Is", "A", "Test", 0);
	assert_that(test_a, "Cannot find Parent/This/Is/A/Test");
	assert_that(string_equals(xml_node_content(test_a), "Content A"), "Content of Parent/This/Is/A/Test must be `Content A'");

	struct xml_node* test_b = xml_easy_child(root, "This", "Is", "B", "Test", 0);
	assert_that(test_b, "Cannot find Parent/This/Is/B/Test");
	assert_that(string_equals(xml_node_content(test_b), "Content B"), "Content of Parent/This/Is/B/Test must be `Content B'");

	struct xml_node* test_c = xml_easy_child(root, "This", "Is", "C", "Test", 0);
	assert_that(!test_c, "Must not find Parent/This/Is/C/Test because no such path exists");

	struct xml_node* must_be_null = xml_easy_child(root, "Child");
	assert_that(!must_be_null, "Parent/Child cannot be a valid expression, because there are two children named `Child' in `Parent'");

	uint8_t* name_is = xml_easy_name(xml_easy_child(root, "This", "Is", 0));
	assert_that(!strcmp(name_is, "Is"), "Name of Parent/This/Is must be `Is'");
	free(name_is);

	uint8_t* content_a = xml_easy_content(test_a);
	assert_that(!strcmp(content_a, "Content A"), "Content of Parent/This/Is/A/Test must be `Content A'");
	free(content_a);

	xml_document_free(document, true);
}



/**
 * Tests the xml_open_document functionality
 */
static void test_xml_parse_document_3() {
	#define FILE_NAME "test.xml"
	FILE* handle = fopen(FILE_NAME, "rb");
	assert_that(handle, "Cannot open " FILE_NAME);

	struct xml_document* document = xml_open_document(handle);
	assert_that(document, "Cannot parse " FILE_NAME);

	struct xml_node* element = xml_easy_child(
		xml_document_root(document), "Element", "With", 0
	);
	assert_that(element, "Cannot find Document/Element/With");
	assert_that(string_equals(xml_node_content(element), "Child"), "Content of Document/Element/With must be `Child'");

	xml_document_free(document, true);
	#undef FILE_NAME
}



/**
 * Test parsing of attributes
 *
 * @author Isty001
 * @see https://github.com/Isty001/
 */
static void test_xml_parse_attributes() {
	#define FILE_NAME "test-attributes.xml"
	FILE* handle = fopen(FILE_NAME, "rb");
	assert_that(handle, "Cannot open " FILE_NAME);

	struct xml_document* document = xml_open_document(handle);
	assert_that(document, "Cannot parse " FILE_NAME);

	struct xml_node* element = xml_easy_child(
		xml_document_root(document), 0
	);

	assert_that(element, "Cannot find Document/Element/With");
	assert_that(2 == xml_node_attributes(element), "Should have 2 attributes");

	assert_that(string_equals(xml_node_attribute_name(element, 0), "value"), "Content of Document/Element/With must be `Child'");
	assert_that(string_equals(xml_node_attribute_content(element, 0), "2"), "First attribute's content should be \"2\"");

	assert_that(string_equals(xml_node_attribute_name(element, 1), "value_2"), "Content of Document/Element/With must be `Child'");
	assert_that(string_equals(xml_node_attribute_content(element, 1), "Hello"), "Second attribute's content should be Hello");

	xml_document_free(document, true);
	#undef FILE_NAME
}



/**
 * Console interface
 */
int main(int argc, char** argv) {
	test_xml_parse_document_0();
	test_xml_parse_document_1();
	test_xml_parse_document_2();
	test_xml_parse_document_3();
	test_xml_parse_attributes();

	fprintf(stdout, "All tests passed :-)\n");
	exit(EXIT_SUCCESS);
}
