/*
 * (c) 2010 Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#include <l4/scout-gfx/user_state>


namespace Scout_gfx {

void User_state::_assign_mfocus(Widget *e, int force )
{
  /* return if mouse focus did not change */
  if (!force && e == _mfocus)
    return;

  /* tell old mouse focus to release focus */
  if (_mfocus)
    _mfocus->mfocus(0);

  /* assign new current mouse focus */
  _mfocus = e;

  /* notify new mouse focus */
  if (_mfocus)
    _mfocus->mfocus(1);

#if 0
  /* determine new current link destination */
  Widget *old_dst = _dst;
  Link_token *lt;
  if (_mfocus && (lt = dynamic_cast<Link_token*>(_mfocus)))
    {
      _dst = lt->dst();
    }
  else
    _dst = 0;
  /* nofify element tree about new link destination */
  if (_dst != old_dst)
    _root->curr_link_destination(_dst);
#endif
}

void User_state::handle_event(Event const &ev)
{
  Parent_widget::handle_event(ev);
  if (_active)
    {
      Event re = ev;
      re.m -= _am;
      _active->handle_event(re);
    }

  if (ev.type != 4)

    /* find element under the mouse cursor */
    _m = ev.m;

  Widget *e = find(_m);

  if (e == this)
    e = 0;

  switch (ev.type)
    {

    case Event::PRESS:
      if (ev.key_cnt != 1 || !e)
	break;

	{
	  Event re = ev;
	  _am = e->map_to_parent(Point(0,0));
	  re.m -= _am;

	  _active = e;
	  _active->handle_event(re);
	}

      _assign_mfocus(e, 1);

      break;

    case Event::RELEASE:
      if (ev.key_cnt == 0)
	{
	  _active = 0;
	  _assign_mfocus(e);
	}
      break;

    case Event::WHEEL:
	if (Widget *x = find_child(_m))
	  {
	    Event re = ev;
	    re.m = x->map_from_parent(ev.m);
	    x->handle_event(re);
	  }
	break;
    case Event::MOTION:
      if (!_active && e)
	{
	  Event re = ev;
	  re.m = e->map_from_parent(ev.m);
	  e->handle_event(re);
	}

      if (ev.key_cnt == 0)
	_assign_mfocus(e);
      break;

    default:

      break;
    }
}
}
