//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#include "sgfx.hpp"
#include "draw.hpp"
#include "ui/ListItem.hpp"
#include "ui/Button.hpp"
#include "ui/Event.hpp"

using namespace ui;

ListItem::ListItem(const char * label, const sg_icon_t * icon, ElementLinked * parent, ElementLinked * child) : ElementLinked(parent, child){
	m_text_attr.assign(label);
	icon_attr().set_attr(icon, Pen(), 0);
	set_animation_type(AnimationAttr::PUSH_LEFT);
}


void ListItem::draw_to_scale(const DrawingScaledAttr & attr){
	sg_bounds_t bounds;
	sg_dim_t icon_dim;
	Dim d = attr.d();
	sg_point_t p = attr.p();
	char buffer[32];

	int height;
	sg_point_t icon_point;
	sg_size_t icon_height;

	//draw the label and the icon
	Dim padded;

	if( &(icon_attr().icon()) == 0 ){
		return;
	}

	icon_height = d.h()/2;

	Bitmap icon_bitmap(icon_height, icon_height);
	icon_bitmap.clear();

	Font * font;

	GfxMap map(icon_bitmap, icon_attr().pen(), icon_attr().rotation());

	Gfx::draw(icon_bitmap,
			icon_attr_const().icon(),
			map,
			&bounds);

	icon_dim = sg_draw_attr_dim(&bounds);

	padded.set_value(d.w() - icon_dim.w, d.h()*80/100 );

	if( m_text_attr.font_size() == 0 ){
		height = padded.h();
	} else {
		height = m_text_attr.font_size();
	}

	font = FontSystem::get_font(height, text_attr().font_bold());
	height = font->get_h();

	icon_point.x = p.x + d.w() - bounds.bottom_right.x;


	if( align_top() ){
		icon_point.y = p.y;
	} else if( align_bottom() ){
		p.y = d.h() - height;
		icon_point.y = d.h() - icon_bitmap.h();
	} else {
		icon_point.y = p.y + d.h()/2 - icon_bitmap.h()/2;
		p.y = p.y + d.h()/2 - height/2;
	}

	strcpy(buffer, text_attr().text());

	if( icon_dim.w > 0 ){
		if( m_text_attr.font_size() != 0 ){
			int len;
			len = font->calc_len(text_attr().text());
			if( len > (icon_point.x - p.x) ){
				memset(buffer, 0, 32);
				strncpy(buffer, text_attr().text(), 6);
				strcat(buffer, "...");
			}
		}
	}

	font->set_str(buffer, attr.bitmap(), p);

	//draw the icon -- on the right side
	if( icon_dim.w > 0 ){
		attr.bitmap().set_bitmap(icon_bitmap, icon_point);
	}
}


Element * ListItem::handle_event(const Event  & event, const DrawingAttr & attr){

	if( event.type() == Event::LIST_ITEM_ACTUATED ){
		if ( child() ){
			return child();
		}
		return parent();
	}

	if( event.type() == Event::LIST_ITEM_SELECTED ){
		return parent()->handle_event(event, attr);
	}

	return ElementLinked::handle_event(event, attr);
}

/*
ListItemToggle::ListItemToggle(const char * label, ElementLinked * parent) :
													//SettingInt(label, Setting::bool_attrs()),
													ListItem(key().name(), parent){
	set_sys_gfx(GfxAttr::TOTAL);
	set_thickness(0);
}
 */

void ListItemToggle::set_enabled(bool v){
	ListItem::set_enabled(v);
	if( v ){
		icon_attr().set_icon(m_toggle_enabled_icon);
	} else {
		icon_attr().set_icon(m_toggle_disabled_icon);
	}

	//save key to file
	//set_value( enabled() );

	//save();
}


Element * ListItemToggle::handle_event(const Event  & event, const DrawingAttr & attr){
	/*
	switch(event){
	case SETUP:
		//load();
		//set_enabled( value() );
		if( enabled() ){
			set_sys_gfx(IconAttr::OK);
		} else {
			set_sys_gfx(0);
		}
		return ListItem::handle_event(SETUP, attr);

	case RIGHT_HOLD:
	case CENTER_PRESS:
		set_enabled(!enabled());
		return parent();

	case LEFT_PRESS:
	case RIGHT_PRESS:
	case ENTER:
		App::bar().set_center( IconAttr::OK, 0 );
		return Element::handle_event(event, attr);
	}
	 */

	return ListItem::handle_event(event, attr);
}




ListItemBack::ListItemBack(const sg_icon_t * icon, ElementLinked * parent) : ListItem("Back", icon, parent){
	icon_attr().set_rotation(IconAttr::LEFT);
	if( parent ){
		set_child(parent->parent());
	}
}



ListItemExit::ListItemExit(const sg_icon_t * icon, ElementLinked * parent) : ListItemBack(icon, parent){
	text_attr().assign("Exit");
	icon_attr().set_rotation(IconAttr::DOWN);
}


ListItemCheck::ListItemCheck(const char * label, List * parent) :
												ListItem(label, 0, parent){
	set_enabled(false);
}


ListDir::ListDir(const char * path,
		const sg_icon_t * icon,
		ElementLinked * parent,
		ElementLinked * child) :
								List(parent),
								m_item("TBD", icon, this, child) {
	set_path(path);
}

ListDir::~ListDir(){
	m_dir.close();
}

ElementLinked * ListDir::at(list_attr_size_t i){
	m_dir.rewind();
	size_t j;
	j=0;

	if( i == 0 ){
		recount();
	}

	while( (m_dir.read() != 0) && (j < i) ){
		j++;
	}

	if( !is_suffix_visible() ){
		i32 k;
		for(k=strlen(m_dir.name())-1; k>=0; k--){
			if( m_dir.name()[k] == '.' ){
				m_dir.data()[k] = 0;
				break;
			}
		}
	}

	m_item.text_attr().assign(m_dir.name());

	return &m_item;
}


void ListDir::set_path(const char * path){
	m_path = path;
	m_dir.close();
	if( m_dir.open(path) < 0 ){

	}
	recount();

}

void ListDir::recount(void){
	int ret;
	ret = m_dir.count();
	if( ret > 0 ){
		m_total = ret;
	} else {
		m_total = 1;
		m_item.text_attr().assign("Empty");
	}
}

Element * ListDir::handle_event(const Event  & event, const DrawingAttr & attr){
	switch(event.type()){

	case Event::BUTTON_ACTUATION:
		if( event.button()->event_id() == Event::SELECT_BUTTON ){
			if( m_callback ){ m_callback(this); }
		}
		// no break
	default:
		return List::handle_event(event, attr);
	}

}
