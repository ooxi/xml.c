#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./xml.h"





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
 * Behaves similar to `getElementsByTagName`, however returns just the first and
 * not all elements with a given tag name
 *
 * @param base Node in tree which should be the start of te recursive search
 * @param 0-terminated tag name, case sensitive
 *
 * @return First node below `base` iff found, otherwise 0
 * @warning Depth-First search!
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/API/Element/getElementsByTagName
 */
static struct xml_node* get_node_by_name(struct xml_node* base, uint8_t* name) {


	/* Check whether `base` already has the tag name we are searching for
	 */
	size_t const name_length = strlen(name);

	uint8_t* base_name = xml_easy_name(base);
	size_t const base_name_length = strlen(base_name);


	/* Length of `name` and `base_name` do match, we should now do a real
	 * comparison
	 */
	if (name_length == base_name_length) {
		int const rs = memcmp(name, base_name, name_length);

		/* Names match! We have found an element which fullfills our
		 * search criteria
		 */
		if (!rs) {
			free(base_name);
			return base;
		}
	}


	/* Unfortunately, `base` is not the element we are looking for :-(
	 */
	free(base_name);


	/* Let's take a look at the children of `base`
	 */
	size_t const number_of_children = xml_node_children(base);


	/* No children → No luck with base
	 */
	if (!number_of_children) {
		return 0;
	}


	/* Recursivly look through all children
	 */
	size_t child = 0; for (; child < number_of_children; ++child) {
		struct xml_node* child_node = xml_node_child(base, child);

		/* Maybe this child does contain the element we are looking for?
		 */
		struct xml_node* search_result = get_node_by_name(
			child_node, name
		);

		/* We are lucky!
		 */
		if (search_result) {
			return search_result;
		}
	}


	/* No luck :-(
	 */
	return 0;
}





int main(int argc, char** argv) {

	/* XML source, could be read from disk
	 */
	uint8_t* source = ""
		"<Root>"
			"<Hello>World</Hello>"

			"<Functions>"
				"<Function>"
					"<as>testas one</as>"
					"<os>testos</os>"
				"</Function>"

				"<Function>"
					"<is>testis</is>"
					"<us>testus</us>"
					"<ls>testls</ls>"
				"</Function>"

				"<Function>"
					"<mn>testmn</mn>"
					"<as>testas two</as>"
				"</Function>"
			"</Functions>"
		"</Root>"
	;

	struct xml_document* document = xml_parse_document(source, strlen(source));

	if (!document) {
		printf("Could parse document\n");
		exit(EXIT_FAILURE);
	}
	struct xml_node* root = xml_document_root(document);



	/* We expect to find Root / Functions / Function#1 / us
	 */
	struct xml_node* us = get_node_by_name(root, "us");
	assert_that(us, "Did not find element by tag name `us'");

	uint8_t* us_content = xml_easy_content(us);
	assert_that(us_content, "`us' should have content");
	assert_that(!strcmp(us_content, "testus"), "Unexpected content for node `us'");
	free(us_content);


	/* We expect to find Root / Functions / Function#0 / as
	 */
	struct xml_node* as = get_node_by_name(root, "as");
	assert_that(as, "Did not find element by tag name `as'");

	uint8_t* as_content = xml_easy_content(as);
	assert_that(as_content, "`as' should have content");
	assert_that(!strcmp(as_content, "testas one"), "Unexpected content for first `as' node");
	free(as_content);


	/* We do not expect do find a node with tag name `does_not_exist'
	 */
	struct xml_node* does_not_exist = get_node_by_name(root, "does_not_exist");
	assert_that(!does_not_exist, "Found node that should not exist");

	

	xml_document_free(document, false);
}

