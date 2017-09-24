INTERFACE [arm && 32bit]:

#include "types.h"

class Treemap;
class Space;

class Mapping_entry
{
public:
  enum { Alignment = 4 };
  union 
  {
    struct 
    {
      unsigned long _space:32;	///< Address-space number
/*      unsigned long _pad:1; */
      unsigned long address:20;	///< Virtual address in address space
    } data;
    Treemap *_submap;
  };
  Unsigned8 _depth;
  void set_space(Space *s) { data._space = (unsigned long)s; }
  Space *space() const { return (Space *)data._space; }
};

INTERFACE [arm && 64bit]:

#include "types.h"

class Treemap;
class Space;

class Mapping_entry
{
public:
  enum { Alignment = 8 };
  union
  {
    struct
    {
      unsigned long _space:64;	///< Address-space number
/*      unsigned long _pad:1; */
      unsigned long address:48;	///< Virtual address in address space
    } data;
    Treemap *_submap;
  };
  Unsigned8 _depth;
  void set_space(Space *s) { data._space = (unsigned long)s; }
  Space *space() const { return (Space *)data._space; }
};

