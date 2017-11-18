/*------------ -------------- -------- --- ----- ---   --       -            -
 *  wasora's mesh-related quadrangle element routines
 *
 *  Copyright (C) 2014--2017 C.P.Camusso.
 *
 *  This file is part of wasora.
 *
 *  wasora is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  wasora is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with wasora.  If not, see <http://www.gnu.org/licenses/>.
 *------------------- ------------  ----    --------  --     -       -         -
 */
#include <wasora.h>

#include <math.h>

// --------------------------------------------------------------
// cuadrangulo de cuatro nodos
// --------------------------------------------------------------
int mesh_nine_node_quadrangle_init(void) {
  
  element_type_t *element_type;
  gauss_t *gauss;
  
  element_type = &wasora_mesh.element_type[ELEMENT_TYPE_QUADRANGLE9];
  element_type->name = strdup("quadrangle9");
  element_type->id = ELEMENT_TYPE_QUADRANGLE9;
  element_type->dim = 2;
  element_type->order = 2;
  element_type->nodes = 9;
  element_type->faces = 4;
  element_type->nodes_per_face = 3;
  element_type->h = mesh_nine_node_quad_h;
  element_type->dhdr = mesh_nine_node_quad_dhdr;
  element_type->point_in_element = mesh_point_in_quadrangle;
  element_type->element_volume = mesh_quad_vol;

  // dos juegos de puntos de gauss
  element_type->gauss = calloc(2, sizeof(gauss_t));
  
  // el primero es el default
  // ---- cuatro puntos de Gauss ----  
    gauss = &element_type->gauss[GAUSS_POINTS_CANONICAL];
    mesh_alloc_gauss(gauss, element_type, 4);
  
    gauss->w[0] = 4 * 0.25;
    gauss->r[0][0] = -1.0/M_SQRT3;
    gauss->r[0][1] = -1.0/M_SQRT3;

    gauss->w[1] = 4 * 0.25;
    gauss->r[1][0] = +1.0/M_SQRT3;
    gauss->r[1][1] = -1.0/M_SQRT3;
 
    gauss->w[2] = 4 * 0.25;
    gauss->r[2][0] = +1.0/M_SQRT3;
    gauss->r[2][1] = +1.0/M_SQRT3;

    gauss->w[3] = 4 * 0.25;
    gauss->r[3][0] = -1.0/M_SQRT3;
    gauss->r[3][1] = +1.0/M_SQRT3;

    mesh_init_shape_at_gauss(gauss, element_type);
    
  // ---- un punto de Gauss  ----  
    gauss = &element_type->gauss[GAUSS_POINTS_SINGLE];
    mesh_alloc_gauss(gauss, element_type, 1);
  
    gauss->w[0] = 4 * 1.0;
    gauss->r[0][0] = 0.0;
    gauss->r[0][1] = 0.0;

    mesh_init_shape_at_gauss(gauss, element_type);  

  return WASORA_RUNTIME_OK;    
}

//Taken from https://www.code-aster.org/V2/doc/default/fr/man_r/r3/r3.01.01.pdf
//The aster node ordering of aster and gmsh are equal.
double mesh_nine_node_quad_h(int j, gsl_vector *gsl_r) {
  double r;
  double s;

  r = gsl_vector_get(gsl_r, 0);
  s = gsl_vector_get(gsl_r, 1);

  switch (j) {
    case 0:
      return r*s*(r-1)*(s-1)/4;
      break;
    case 1:
      return r*s*(r+1)*(s-1)/4;
      break;
    case 2:
      return r*s*(r+1)*(s+1)/4;
      break;
    case 3:
      return r*s*(r-1)*(s+1)/4;
      break;
    case 4:
      return (1-r*r)*s*(s-1)/2;
      break;
    case 5:
      return r*(r+1)*(1-s*s)/2;
      break;
    case 6:
      return (1-r*r)*s*(s+1)/2;
      break;
    case 7:
      return r*(r-1)*(1-s*s)/2;
      break;
    case 8:
      return (1-r*r)*(1-s*s);
      break;
  }

  return 0;

}

double mesh_nine_node_quad_dhdr(int j, int m, gsl_vector *gsl_r) {
  double r;
  double s;

  r = gsl_vector_get(gsl_r, 0);
  s = gsl_vector_get(gsl_r, 1);

  switch(j) {
    case 0:
      if (m == 0) {
        return (2*r-1)*s*(s-1)/4;
      } else {
        return r*(r-1)*(2*s-1)/4;
      }
      break;
    case 1:
      if (m == 0) {
        return (2*r+1)*s*(s-1)/4;
      } else {
        return r*(r+1)*(2*s-1)/4;
      }
      break;
    case 2:
      if (m == 0) {
        return (2*r+1)*s*(s+1)/4;
      } else {
        return r*(r+1)*(2*s+1)/4;
      }
      break;
    case 3:
      if (m == 0) {
        return (2*r-1)*s*(s+1)/4;
      } else {
        return r*(r-1)*(2*s+1)/4;
      }
      break;
    case 4:
      if (m == 0) {
        return -r*s*(s-1);
      } else {
        return (1-r*r)*(2*s-1)/2;
      }
      break;
    case 5:
      if (m == 0) {
        return (2*r+1)*(1-s*s)/2;
      } else {
        return -r*s*(r+1);
      }
      break;
    case 6:
      if (m == 0) {
        return -r*s*(s+1);
      } else {
        return (1-r*r)*(2*s+1)/2;
      }
      break;
    case 7:
      if (m == 0) {
        return (2*r-1)*(1-s*s)/2;
      } else {
        return -r*s*(r-1);
      }
      break;
    case 8:
      if (m == 0) {
        return -2*r*(1-s*s);
      } else {
        return -2*s*(1-r*r);
      }
      break;

  }

  return 0;


}