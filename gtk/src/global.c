/* -*- c-file-style: "ruby" -*- */
/************************************************

  global.c -

  $Author: mutoh $
  $Date: 2002/05/19 15:48:28 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE
glist2ary(list)
    GList *list; 
{
    VALUE ary = rb_ary_new();

    while (list) {
	rb_ary_push(ary, get_value_from_gobject(GTK_OBJECT(list->data)));
	list = list->next;
    }

    return ary;
}

GList*
ary2glist(ary)
    VALUE ary;
{
    long i;
    GList *glist = NULL;

    Check_Type(ary, T_ARRAY);
    for (i=0; i<RARRAY(ary)->len; i++) {
	glist = g_list_prepend(glist,get_widget(RARRAY(ary)->ptr[i]));
    }

    return g_list_reverse(glist);
}

GSList*
ary2gslist(ary)
    VALUE ary;
{
    long i;
    GSList *glist = NULL;

    if (NIL_P(ary)) return NULL;
    Check_Type(ary, T_ARRAY);
    for (i=0; i<RARRAY(ary)->len; i++) {
	glist = g_slist_append(glist,get_widget(RARRAY(ary)->ptr[i]));
    }

    return glist;
}

VALUE
gslist2ary(list)
    GSList *list; 
{
    VALUE ary = rb_ary_new();

    while (list) {
	rb_ary_push(ary, get_value_from_gobject(GTK_OBJECT(list->data)));
	list = list->next;
    }

    return ary;
}

VALUE
arg_to_value(arg)
	 GtkArg *arg;
{
  switch (GTK_FUNDAMENTAL_TYPE(arg->type)) {
  case GTK_TYPE_CHAR:
	return INT2FIX(GTK_VALUE_CHAR(*arg));
	break;
	
  case GTK_TYPE_BOOL:
  case GTK_TYPE_INT:
  case GTK_TYPE_ENUM:
  case GTK_TYPE_FLAGS:
	return INT2NUM(GTK_VALUE_INT(*arg));
	break;

  case GTK_TYPE_UINT:
	return INT2NUM(GTK_VALUE_UINT(*arg));
	break;
  case GTK_TYPE_LONG:
	return INT2NUM(GTK_VALUE_LONG(*arg));
	break;
  case GTK_TYPE_ULONG:
	return INT2NUM(GTK_VALUE_ULONG(*arg));
	break;

  case GTK_TYPE_FLOAT:
	return rb_float_new(GTK_VALUE_FLOAT(*arg));
	break;

  case GTK_TYPE_STRING:
	return GTK_VALUE_STRING(*arg) ? rb_str_new2(GTK_VALUE_STRING(*arg)) : Qnil;
	break;

  case G_TYPE_OBJECT:
	return GTK_VALUE_OBJECT(*arg) ? get_value_from_gobject(GTK_VALUE_OBJECT(*arg)) : Qnil;
	break;

  case GTK_TYPE_BOXED:
	if (arg->type == GDK_TYPE_EVENT) {
	  return make_gdkevent(GTK_VALUE_BOXED(*arg));
	}
#ifdef GTK_TYPE_GDK_COLORMAP
	else if (arg->type == GTK_TYPE_GDK_COLORMAP) {
	  return make_gdkcmap(GTK_VALUE_BOXED(*arg));
	}
#endif
#ifdef GTK_TYPE_GDK_FONT
	else if (arg->type == GTK_TYPE_GDK_FONT) {
	  return make_gdkfont(GTK_VALUE_BOXED(*arg));
	}
#endif
#ifdef GTK_TYPE_GDK_PIXMAP
	else if (arg->type == GTK_TYPE_GDK_PIXMAP) {
	  return make_gdkpixmap(GTK_VALUE_BOXED(*arg));
	}
#endif
#ifdef GTK_TYPE_GDK_VISUAL
	else if (arg->type == GTK_TYPE_GDK_VISUAL) {
	  return make_gdkvisual(GTK_VALUE_BOXED(*arg));
	}
#endif
#ifdef GTK_TYPE_ACCELERATOR_TABLE
	else if (arg->type == GTK_TYPE_ACCELERATOR_TABLE) {
	  return make_gtkacceltbl(GTK_VALUE_BOXED(*arg));
	}
#endif
#ifdef GTK_TYPE_STYLE
	else if (arg->type == GTK_TYPE_STYLE) {
	  return make_gstyle(GTK_VALUE_BOXED(*arg));
	}
#endif
#ifdef GTK_TYPE_TOOLTIPS
	else if (arg->type == GTK_TYPE_TOOLTIPS) {
	  return make_gobject(gTooltips, GTK_OBJECT(GTK_VALUE_BOXED(*arg)));
	}
#endif
	else if (arg->type == GTK_TYPE_CTREE_NODE) {
	  return make_ctree_node(GTK_VALUE_BOXED(*arg));
	}
	else {
	  goto unsupported;
	}
	
  case GTK_TYPE_POINTER:
	return get_value_from_gobject(GTK_VALUE_OBJECT(*arg));
	break;
	
  case GTK_TYPE_INVALID:
  case GTK_TYPE_NONE:
  unsupported:
  default:
	rb_raise(rb_eTypeError, "unsupported arg type %s (fundamental type %s)",
			  gtk_type_name(arg->type),
			  gtk_type_name(GTK_FUNDAMENTAL_TYPE(arg->type)));
	break;
  }
}
