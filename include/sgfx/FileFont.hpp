/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SGFX_FILEFONT_HPP_
#define SGFX_FILEFONT_HPP_

#include "Font.hpp"
#include "../sys/File.hpp"

namespace sgfx {

/*! \brief File Font Class
 * \details The File Font class is used to access
 * fonts that are stored as files.
 *
 */
class FileFont : public Font {
public:
    FileFont();
    FileFont(const char * name, int offset = 0);
    ~FileFont();

	int set_file(const char * name, int offset = 0);

	sg_size_t get_height() const;
	sg_size_t get_width() const;

protected:
	void draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, sg_point_t point) const;
	int load_char(sg_font_char_t & ch, char c, bool ascii) const;
	int load_kerning(u16 first, u16 second) const;

private:
	mutable sys::File m_file;
	mutable Bitmap m_canvas;
	mutable u8 m_current_canvas;
	u32 m_canvas_start;
	u32 m_canvas_size;
	sg_font_kerning_pair_t * m_kerning_pairs;

};

}

#endif /* SGFX_FILEFONT_HPP_ */
