// $Id: Tag.h,v 1.1 2003/01/31 07:22:50 thoma Exp $
/*  glade--: C++ frontend for glade (Gtk+ User Interface Builder)
 *  Copyright (C) 1998-2002  Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef TAG_HH
#define TAG_HH
//#include <ManuProCConfig.h>
#include <string>
#include <vector>
#include <algorithm> // for find
#include <stdexcept>

class Tag {
    	std::string type;
    	std::string value;
    	typedef std::vector <std::pair<std::string,std::string> > attvec_t;
    	attvec_t attributes;
    	std::vector <Tag> sub_specifications;

public: // nice to have for custom parsing
    	typedef std::vector<Tag>::difference_type difference_type;
    	template <class T>
    	 static T parse_value(const std::string &val) throw(std::out_of_range);
    	template <class T>
    	 static T parse_value_def(const std::string &val, const T &def)
    	{  try { return parse_value(val); } 
    	   catch (std::out_of_range &e) { return def; }
    	}
    	template <class T>
    	 static std::string create_value(const T &val);
    	 
public:
    	Tag(const std::string &t,const std::string &v="") throw()
		: type(t), value(v)
	{}
	Tag &push_back(const Tag t) throw()
	{  sub_specifications.push_back(t);
	   return *(sub_specifications.end()-1);
	}
	typedef std::vector<Tag>::iterator iterator;
	typedef std::vector<Tag>::const_iterator const_iterator;
	const_iterator begin() const throw()
	{  return sub_specifications.begin(); }
	const_iterator end() const throw()
	{  return sub_specifications.end(); }
	const Tag &back() const throw()
	{  return sub_specifications.back(); }
	std::vector<Tag>::size_type size() const throw()
	{  return sub_specifications.size(); }
	void reserve(std::vector<Tag>::size_type n) throw()
	{  return sub_specifications.reserve(n); }
	iterator begin() throw()
	{  return sub_specifications.begin(); }
	iterator end() throw()
	{  return sub_specifications.end(); }
	Tag &back() throw()
	{  return sub_specifications.back(); }
	
	const std::string Type() const throw()
	{  return type; }
	void Type(const std::string &val) throw()
	{  type=val; }
	const std::string Value() const throw()
	{  return value; }
	void Value(const std::string &val) throw()
	{  value=val; }

	// slow variants
	const Tag *find(const std::string &type) const;
	Tag *find(const std::string &type);
	// please prefer these finds, they are faster!
	const_iterator find(const_iterator it,const std::string &type) const
	{  return std::find(it,end(),type); }
	const_iterator find(const_iterator it,const std::string &type)
	{  return std::find(it,const_cast<const Tag*>(this)->end(),type); }
	iterator find(iterator it,const std::string &type)
	{  return std::find(it,end(),type); }

	// values of attributes
	typedef attvec_t::const_iterator const_attiterator;
	const_attiterator attbegin() const { return attributes.begin(); }
	attvec_t::iterator attend() { return attributes.end(); }
	const_attiterator attend() const { return attributes.end(); }
	attvec_t::iterator attfind(const std::string &name);
	const_attiterator attfind(const std::string &name) const;
	
	template <class T>
	 T getAttr(const std::string &name) const throw(std::out_of_range)
	{  const_attiterator t=attfind(name);
	   if (t==attend()) throw std::out_of_range(name);
	   return parse_value<T>(t->second);
	}
	template <class T>
	 T getAttr_def(const std::string &name, const T &def) const throw()
    	{  try { return getAttr<T>(name); } 
    	   catch (std::out_of_range &e) { return def; }
    	}
	bool hasAttr(const std::string &name) const throw();
	void setAttr(const std::string &name, const std::string &value);
	
	// values of substructures
	bool hasTag(const std::string &typ) const throw();
	void setValue(const std::string &tg,const std::string &value) throw();
	template <class T>
	 T getValue(const std::string &tg) const
	{  const_iterator t=find(begin(),tg);
	   if (t==end()) throw std::out_of_range(tg);
	   return parse_value<T>(t->Value());
	}
	template <class T>
	 T getValue_def(const std::string &tg, const T &def) const throw()
    	{  try { return getValue<T>(tg); } 
    	   catch (std::out_of_range &e) { return def; }
    	}
	
	bool operator==(const std::string &tp) const
	{  return type==tp; }
	bool operator!=(const std::string &tp) const
	{  return type!=tp; }
	
	void debug(int recursion_depth=2,int indent=0) const;

	// template specialization declarations
	
#ifndef TAG_OMIT_DEPRECATED
	const std::string getAttr(const std::string &name, const std::string &def="") const throw()
	{  return getAttr_def<std::string>(name,def); }
	bool getBoolAttr(const std::string &typ,bool def=false) const throw()
	{  return getAttr_def<bool>(typ,def); }
	int getIntAttr(const std::string &typ,int def=0) const throw()
	{  return getAttr_def<int>(typ,def); }
	float getFloatAttr(const std::string &typ,float def=0) const throw()
	{  return getAttr_def<float>(typ,def); }
	void setIntAttr(const std::string &name, int val);
	void setFloatAttr(const std::string &name, double val);
	void setBoolAttr(const std::string &name, bool val);
	// Spezialfall: nur setzen wenn !=""
	void setAttr_ne(const std::string &name, const std::string &value)
	{  if (!value.empty()) setAttr(name,value); }
	void setIntAttr_nn(const std::string &name, int val)
	{  if (val) setIntAttr(name,val); }

	const std::string getString(const std::string &typ,const std::string &def="") const throw()
	{  return getValue_def<std::string>(typ,def); }
	bool getBool(const std::string &typ,bool def=false) const throw()
	{  return getValue_def<bool>(typ,def); }
	int getInt(const std::string &typ,int def=0) const throw()
	{  return getValue_def<int>(typ,def); }
	float getFloat(const std::string &typ,float def=0) const throw()
	{  return getValue_def<float>(typ,def); }
	void mark(const std::string &tg,const std::string &value) throw()
	{  setValue(tg,value); }
	
#endif
};

// some nice macros

#define FOR_EACH_CONST_TAG(variable,parent) \
	for (Tag::const_iterator variable=(parent).begin(); \
		(variable)!=(parent).end(); ++(variable))

#define FOR_EACH_CONST_TAG_OF(variable,parent,type) \
	for (Tag::const_iterator variable= \
			(parent).find((parent).begin(),(type)); \
		(variable)!=(parent).end(); \
		variable=(parent).find((variable)+1,(type)))

#define FOR_EACH_CONST_TAG_OF_5(variable,parent,begin,end,type) \
	for (Tag::const_iterator variable= \
			(parent).find((begin),(type)); \
		(variable)!=(end); \
		variable=(parent).find((variable)+1,(type)))

#define FOR_EACH_TAG(variable,parent) \
	for (Tag::iterator variable=(parent).begin(); \
		(variable)!=(parent).end(); ++(variable))

#define FOR_EACH_TAG_OF(variable,parent,type) \
	for (Tag::iterator variable= \
			(parent).find((parent).begin(),(type)); \
		(variable)!=(parent).end(); \
		variable=(parent).find((variable)+1,(type)))

#define FOR_EACH_TAG_OF_5(variable,parent,begin,end,type) \
	for (Tag::iterator variable= \
			(parent).find((begin),(type)); \
		(variable)!=(end); \
		variable=(parent).find((variable)+1,(type)))

// you can skip these declarations
template <> std::string Tag::create_value<int>(const int &val);
template <> std::string Tag::create_value<double>(const double &val);
template <> std::string Tag::create_value<bool>(const bool &val);

inline void Tag::setIntAttr(const std::string &name, int val)
{  setAttr(name,create_value<int>(val)); }
inline void Tag::setFloatAttr(const std::string &name, double val)
{  setAttr(name,create_value<double>(val)); }
inline void Tag::setBoolAttr(const std::string &name, bool val)
{  setAttr(name,create_value<bool>(val)); }


#endif