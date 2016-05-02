/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef XML_HPP_
#define XML_HPP_

#include <unistd.h>

#include "../Var/String.hpp"
#include "../Sys/File.hpp"

namespace Fmt {

/*! \brief XML Class
 * \details This class is used to read values from an XML file.  It can also be used to create
 * new XML files, but has limited ability to modify XML files.
 *
 * This example file is used in the code examples herein.
 *
 * \code
 *
 * <gpx
 * version="1.0"
 * creator="ExpertGPS 1.1 - http://www.topografix.com"
 * xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 * xmlns="http://www.topografix.com/GPX/1/0"
 * xsi:schemaLocation="http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd">
 * <time>2002-02-27T17:18:33Z</time>
 * <bounds minlat="42.401051" minlon="-71.126602" maxlat="42.468655" maxlon="-71.102973"/>
 * <wpt lat="42.438878" lon="-71.119277">
 *  <ele>44.586548</ele>
 *  <time>2001-11-28T21:05:28Z</time>
 *  <name>5066</name>
 *  <desc><![CDATA[5066]]></desc>
 *  <sym>Crossing</sym>
 *  <type><![CDATA[Crossing]]></type>
 * </wpt>
 * <wpt lat="42.439227" lon="-71.119689">
 *  <ele>57.607200</ele>
 *  <time>2001-06-02T03:26:55Z</time>
 *  <name>5067</name>
 *  <desc><![CDATA[5067]]></desc>
 *  <sym>Dot</sym>
 *  <type><![CDATA[Intersection]]></type>
 * </wpt>
 * </gpx>
 *
 * \endcode
 */
class Xml : public Sys::File {
public:

	/*! \brief Open an XML document
	 * \details Open or create an XML document.
	 *
	 * @param path XML file path
	 * @param mode (RDWR, WRONLY, RDONLY)
	 * @param perms File permissions if creating a new file (WRONLY)
	 *
	 */
	Xml(const char * path, int mode, int perms = 0666);

	Xml();

	int init(const char * path, int mode, int perms = 0666);

	inline int close(){
		file_size = 0;
		return File::close();
	}

	inline int exit(){ return close(); }

	/*! \brief Get the value of the XML element
	 * \details The gets the value of an XML element
	 * based on the string that describes it.
	 *
	 * The following XML (GPX file) is used as an example.
	 *
 	 *
 	 * The following \a str values will grab the associated strings from the XML file:
 	 *
 	 * - "gpx(version)" = 1.0
 	 * - "gpx.wpt.ele" = 44.586548
 	 * - "gpx.wpt[0].ele" = 44.586548
 	 * - "gpx.wpt[1].name" = 5067
 	 * - "gpx.wpt[1](lat)" = 42.439227
 	 * - "gpx.time" = 2002-02-27T17:18:33Z
	 *
	 *	@param dest The destination string to write the value of the specified attribute
	 *  @param str The string describing the element; 0 to get the value of the current element.
	 *  This values is relative to the current position.
	 *
	 *  \return Zero if str is successfully fetched
	 */
	int value(Var::String & dest, const char * key = 0) const;
	inline int value(Var::String * dest, const char * key = 0) const {
		return value(*dest, key);
	}

	int setvalue(const Var::String * src, const char * key) const;
	inline int setvalue(const Var::String & src, const char * key) const {
		return setvalue(&src, key);
	}


	/*! \brief Find the element described by str
	 * \details This method finds the element described by str.
	 * Subsequent calls to sibling(), child(), size(), and value() will only search the element
	 * specified.
	 *
	 * For example:
	 * \code
	 * StringMedium str;
	 * find("gpx.wpt[0]");
	 * value("ele", str);  //copies ele value of gpx.wpt[0] to str
	 * value("(lat)", str);  //copies the lat attibute of gpx.wpt[0] to str
	 *
	 * \endcode
	 *
	 *
	 * @param str This parameter describes the XML element to find
	 *
	 * \return Zero on success of -1 if the element was not found
	 *
	 */
	int find(const char * str);
	int find_next(const char * str);


	/*! \brief Returns the number of immediate children */
	int count();

	/*! \brief Get the next sibling
	 * \details This method gets the name of the next sibling
	 *
	 * @param name string reference for name
	 * @param value string pointer for the value
	 *
	 * Example:
	 *
	 * \code
	 * StringMedium str;
	 * StringMedium v;
	 *
	 * find("gpx.wpt[0]");
	 * sibling(str); //str is now "wpt" cursor points to time object
	 * sibling(str); //returns -1 because there are no more siblings
	 *
	 * \endcode
	 */
	int sibling(Var::String & dest, Var::String * value = 0);

	/*! \brief Get the next child
	 * \details This method gets the name of the next child
	 *
	 * @param name string reference for name
	 * @param value string pointer for the value
	 *
	 * Example:
	 *
	 * \code
	 * StringMedium str;
	 * StringMedium v;
	 *
	 * find("gpx");
	 * child(str); //str is not "time" cursor points to time object
	 * child(str); //str is now "wpt"  cursor points to first wpt object
	 * child(str); //str is now "wpt"  cursor points to second wpt object
	 *
	 * enter("gpx.wpt[0]"); //cursor points to first wpt object
	 * next_child(str, &v); //str is now ele, v is the value of gps.wpt[0].ele or "44.586548"
	 * \endcode
	 *
	 */
	int child(Var::String & name, Var::String * value = 0);
	inline int child(Var::String * name , Var::String * value = 0){
		return child(*name, value);
	}
	inline int child(Var::String & name , Var::String & value){
		return child(name, &value);
	}
	inline int child(Var::String * name , Var::String & value){
		return child(*name, &value);
	}



	/*! \brief Get the name of the next attribute
	 * \details This method gets the name and value of the next
	 * attribute.
	 *

	 *
	 * \code
	 * StringMedium name;
	 * StringMedium v;
	 * find("gpx.wpt");
	 * attr(name, &v); //name is "lat" and v is "42.438878"
	 * \endcode
	 *
	 * The value(String&, const char*) method can be used to
	 * get the value of an attribute directly by name.
	 *
	 * \code
	 * StringMedium a;
	 * find("gpx.wpt");
	 * value(a, "(lat)"); //a is now the value of gps.wpt(lat) or "42.438878"
	 * \endcode
	 *
	 */
	int attr(Var::String & name, Var::String * value = 0);

	/*! \brief Returns the size of the current object (including tags).
	 * \details This method returns the size of the current object.  It
	 * represents the number of bytes in the file
	 */
	inline int size() const { return content.size; }


	int write_start_tag(const char * name, const char * attrs = 0);
	int write_cdata(const char * str);
	int write_end_tag(const char * name);
	int write_empty_element_tag(const char * name, const char * attrs = 0);
	int write_element(const char * name, const char * data, const char * attrs = 0);

	/*
	 * Empty tag (no content) = < Name (optional attributes--zero or more)  />
	 * start tag = < Name (optional attributes--zero or more)  >
	 * content = element, reference, CDSect, PI, Comment
	 * end tag = </ Name S >
	 *
	 * An element is either an <empty tag /> or a <start tag>content</end tag>
	 * A reference is Entity ref or char ref
	 * Entity ref =&Name;
	 * PEReference = %Name;
	 *
	 * CDSect = '<![CDATA['	(Char* - (Char* ']]>' Char*)) CDEnd 	']]>'
	 *
	 * PI (processing instruction) = '<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>'
	 * Comment = '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
	 *
	 *
	 */

	typedef struct {
		ssize_t offset;
		ssize_t size;
		ssize_t start_tag_size;
		ssize_t end_tag_size;
		ssize_t cursor;
	} context_t;

	inline const context_t & context() const { return content; }
	inline void setcontext(const context_t & context) { content = context; }


	inline void reset(void){ reset_context(); }

private:

	context_t content;
	ssize_t file_size;

	int indent;

	void reset_context(void);

	int find_context(const char * str, const context_t & current, context_t & target) const;

	int check_string_for_open_bracket(Var::String * src, Var::String * cmp) const;

	int find_tag(const char * name,
			const context_t & context,
			const char * tag_style,
			ssize_t & tag_size) const;

	int next_tag_name(
			Var::String & name,
			const context_t & context,
			context_t target
			);

	int find_target_tag(Var::String & name, context_t & context, context_t & target) const;

	int read_context(int offset, Var::String & str, context_t & target) const;


	int find_attribute(Var::String & key, Var::String & value, context_t & target) const;

	bool is_empty_element_tag(context_t & target) const;

	static int parse_ref_array(Var::String & name, const char * str);
	static int parse_ref_attr(Var::String & name, Var::String & attr_name, const char * str);
	static int parse_ref(Var::String & name, Var::String & value, const char * str, const char * enclosing);

	static int check_chars(const char * src, const char * allowed);

	int load_start_tag(Var::String & tag, const context_t & target) const;
	void show_context(context_t & context);

	enum {
		FIND_STATE_OPENBRACKET,
		FIND_STATE_CLOSEBRACKET
	};

	int set_get_value(Var::String & dest, const char * key, bool set = false) const;


};

};

#endif /* XML_HPP_ */