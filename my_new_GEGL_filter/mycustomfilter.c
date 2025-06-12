/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Credit to Øyvind Kolås (pippin) for major GEGL contributions
 * 2022 Beaver (Third party GEGL filter guide)
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/* This is where we list most parameters of filters and how far their settings are allowed to go, and rather the setting should be visible in Gimp's' GUI. Tagging the string below this text on the line under one of the properties will make the filter setting invisible to the GUI in Gimp. */

/*     ui_meta     ("role", "output-extent")  */



property_double (gaus, _("Gaussian Blur for both XY StD"), 0)
   description (_("Standard deviation for the XY axis"))
   value_range (0.0, 70.0)


property_double (hue, _("Hue from GEGL Hue Chroma"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)

property_double (rotate, _("Rotation's Property from GEGL Ripple"), 0.0)
    value_range (-180, 180)
    ui_meta     ("unit", "degree")
    ui_meta     ("direction", "ccw")

property_double (strength, _("Glow from GEGL Bloom"), 0.0)
    description (_("Glow strength"))
    value_range (0.0, G_MAXDOUBLE)
    ui_range    (0.0, 100.0)

property_double (x, _("Move Horizontal from GEGL Translate"), 0.0)
    description (_("Horizontal translation"))
    ui_range (-1000.0, 1000.0)
    ui_meta ("unit", "pixel-distance")
    ui_meta ("axis", "x")

property_double (y, _("Move Vertical from GEGL Translate"), 0.0)
    description (_("Vertical translation"))
    ui_range (-1000.0, 1000.0)
    ui_meta ("unit", "pixel-distance")
    ui_meta ("axis", "y")

property_color (value, _("Color fill from GEGL Color"), "#000000")
    description (_("The color to render (defaults to 'black')"))




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     mycustomfilter
#define GEGL_OP_C_SOURCE mycustomfilter.c

#include "gegl-op.h"


static void attach (GeglOperation *operation)
{

/* LIST ALL POTENTIAL NODES HERE '*/


/* This is where we list all the full filter operations that we are going to use in our GEGL Graph. You can list operations that you may choose not to use. Don't forget the asterisk * while entering these . You also get to define their names. The only part that requires GEGL syntax is the quotations after operation. These are not nodes and do not need to be in proper order. '*/



  GeglNode *gegl = operation->node;
  GeglNode *input, *hue, *gaussian, *rotate, *glow, *move, *fill, *behind, *output;

/* END OF LIST ALL POTENTIAL NODES '*/


/* LIST AND RENAME GEGL OPERATIONS*/

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  hue    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  rotate    = gegl_node_new_child (gegl,
                                  "operation", "gegl:rotate-on-center",
                                  NULL);

  glow   = gegl_node_new_child (gegl,
                                  "operation", "gegl:bloom",
                                  NULL);

  move   = gegl_node_new_child (gegl,
                                  "operation", "gegl:translate",
                                  NULL);

  behind   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over",
                                  NULL);

  fill   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);
 
 
 /* END OF LIST AND RENAME GEGL OPERATIONS*/
 

/* This is where we define actions for parts of full GEGL Operations.  What is in here will be used in our filter and it cannot contain errors and missing values like the potential node zone can. The ability to rearrange parts of existing GEGL filters in tons of different ways is what gives us such rich variety in custom filters */



  gegl_operation_meta_redirect (operation, "hue", hue, "hue");

  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-x");

  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-y");

  gegl_operation_meta_redirect (operation, "rotate", rotate, "degrees");

  gegl_operation_meta_redirect (operation, "strength", glow, "strength");

  gegl_operation_meta_redirect (operation, "x", move, "x");

  gegl_operation_meta_redirect (operation, "y", move, "y");

  gegl_operation_meta_redirect (operation, "value", fill, "value");












/* This is the GEGL Graph itself, in which GEGL operations can be chained together. The GEGL operations that we defined and set properties too are set out as nodes for the Graph. Filters are arranged from first to last with GEGL blend modes (ref and id) requiring their own gegl_node_link line with connections.  */

/* GEGL filters that modify the image by default will be auto applied to your custom filter without needing to define their properties above. You still need to list them in the potential nodes setting. Examples of filters that do not need properties at all are "invert". '*/

  gegl_node_link_many (input, gaussian, hue, rotate, glow, move, behind, output, NULL);

  gegl_node_link_many (input, fill);

  gegl_node_connect_from (behind, "aux", fill, "output");



}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
/* We title and describe the filter here. Filters must have different names but require gegl: in front of them. Making the category "hidden" will hide the filter from GEGL operations drop down list. This is useful for hidden gegl graphs that your filter needs to call. It is prefered to make a unique ID hash but not required. */
    "name",        "gegl:mynewfilter",
    "title",       _("My third party GEGL Filter"),
    "categories",  "Third Party Filters",
    "reference-hash", "331o1gabt4hk10fjf25sabax",
    "description", _("My new GEGL filter that works by chaining existing GEGL operations in different combinations "
                     ""),
    NULL);
}

/* gegl.org is where you can find and learn GEGL syntax. Make sure to practice in Gimp's GEGL Graph.' contrast_ on GimpChat aka Beaver is eager to help you! */

#endif
