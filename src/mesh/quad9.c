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
int mesh_quad9_init(void) {
  
  element_type_t *element_type;
  int j;
  
  element_type = &wasora_mesh.element_type[ELEMENT_TYPE_QUADRANGLE9];
  element_type->name = strdup("quad9");
  element_type->id = ELEMENT_TYPE_QUADRANGLE9;
  element_type->dim = 2;
  element_type->order = 2;
  element_type->nodes = 9;
  element_type->faces = 4;
  element_type->nodes_per_face = 3;
  element_type->h = mesh_quad9_h;
  element_type->dhdr = mesh_quad9_dhdr;
  element_type->point_in_element = mesh_point_in_quadrangle;
  element_type->element_volume = mesh_quad_vol;

  // coordenadas de los nodos
/*
 Quadrangle9:

 3-----6-----2
 |           |
 |           |
 7     8     5
 |           |
 |           |
 0-----4-----1   
*/ 
  
  element_type->node_coords = calloc(element_type->nodes, sizeof(double *));
  element_type->node_parents = calloc(element_type->nodes, sizeof(node_relative_t *));  
  for (j = 0; j < element_type->nodes; j++) {
    element_type->node_coords[j] = calloc(element_type->dim, sizeof(double));  
  }
  
  element_type->first_order_nodes++;
  element_type->node_coords[0][0] = -1;
  element_type->node_coords[0][1] = -1;
  
  element_type->first_order_nodes++;
  element_type->node_coords[1][0] = +1;  
  element_type->node_coords[1][1] = -1;
  
  element_type->first_order_nodes++;
  element_type->node_coords[2][0] = +1;  
  element_type->node_coords[2][1] = +1;

  element_type->first_order_nodes++;
  element_type->node_coords[3][0] = -1;
  element_type->node_coords[3][1] = +1;

 
  wasora_mesh_add_node_parent(&element_type->node_parents[4], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[4], 1);
  wasora_mesh_compute_coords_from_parent(element_type, 4);
  
  wasora_mesh_add_node_parent(&element_type->node_parents[5], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[5], 2);
  wasora_mesh_compute_coords_from_parent(element_type, 5); 
 
  wasora_mesh_add_node_parent(&element_type->node_parents[6], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[6], 3);
  wasora_mesh_compute_coords_from_parent(element_type, 6);   
 
  wasora_mesh_add_node_parent(&element_type->node_parents[7], 3);
  wasora_mesh_add_node_parent(&element_type->node_parents[7], 0);
  wasora_mesh_compute_coords_from_parent(element_type, 7);

  wasora_mesh_add_node_parent(&element_type->node_parents[8], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[8], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[8], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[8], 3);
  wasora_mesh_compute_coords_from_parent(element_type, 8);  
  
  mesh_quad_gauss9_init(element_type);

  return WASORA_RUNTIME_OK;    
}

void mesh_quad_gauss9_init(element_type_t *element_type) {
  double r[3];
  double a, w1, w2, w3;  
  int v;
  gauss_t *gauss;

  a = 0.774596669241483;
  w1 = 25.0/81.0;
  w2 = 40.0/81.0;
  w3 = 64.0/81.0;
  
  // dos juegos de puntos de gauss
  element_type->gauss = calloc(2, sizeof(gauss_t));
  
  // ---- nueve puntos de Gauss ----  
    gauss = &element_type->gauss[GAUSS_POINTS_FULL];
    mesh_alloc_gauss(gauss, element_type, 9);
  
    gauss->w[0] = w1;
    gauss->r[0][0] = -a;
    gauss->r[0][1] = -a;

    gauss->w[1] = w1;
    gauss->r[1][0] = +a;
    gauss->r[1][1] = -a;
 
    gauss->w[2] = w1;
    gauss->r[2][0] = +a;
    gauss->r[2][1] = +a;

    gauss->w[3] = w1;
    gauss->r[3][0] = -a;
    gauss->r[3][1] = +a;
    
    gauss->w[4] = w2;
    gauss->r[4][0] = 0;
    gauss->r[4][1] = -a;

    gauss->w[5] = w2;
    gauss->r[5][0] = +a;
    gauss->r[5][1] = 0;
 
    gauss->w[6] = w2;
    gauss->r[6][0] = 0;
    gauss->r[6][1] = +a;

    gauss->w[7] = w2;
    gauss->r[7][0] = -a;
    gauss->r[7][1] = 0;
    
    gauss->w[8] = w3;
    gauss->r[8][0] = 0;
    gauss->r[8][1] = 0;

    mesh_init_shape_at_gauss(gauss, element_type);
    
    // matriz de extrapolacion
    gauss->extrap = gsl_matrix_alloc(gauss->V, gauss->V);
    
    r[0] = -1/a;
    r[1] = -1/a;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 0, v, mesh_quad9_h(v, r));
    }  

    r[0] = +1/a;
    r[1] = -1/a;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 1, v, mesh_quad9_h(v, r));
    }  
    
    r[0] = +1/a;
    r[1] = +1/a;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 2, v, mesh_quad9_h(v, r));
    }  

    r[0] = -1/a;
    r[1] = +1/a;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 3, v, mesh_quad9_h(v, r));
    }

    r[0] = 0;
    r[1] = -1/a;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 4, v, mesh_quad9_h(v, r));
    }

    r[0] = +1/a;
    r[1] = 0;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 5, v, mesh_quad9_h(v, r));
    }

    r[0] = 0;
    r[1] = +1/a;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 6, v, mesh_quad9_h(v, r));
    }

    r[0] = -1/a;
    r[1] = 0;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 7, v, mesh_quad9_h(v, r));
    }
    
    r[0] = 0;
    r[1] = 0;
    for (v = 0; v < gauss->V; v++) {
      gsl_matrix_set(gauss->extrap, 8, v, mesh_quad9_h(v, r));
    }

  // ---- un punto de Gauss  ----  
    gauss = &element_type->gauss[GAUSS_POINTS_REDUCED];
    mesh_alloc_gauss(gauss, element_type, 1);
  
    gauss->w[0] = 4 * 1.0;
    gauss->r[0][0] = 0.0;
    gauss->r[0][1] = 0.0;

    mesh_init_shape_at_gauss(gauss, element_type);
    
  return;
}        

//Taken from https://www.code-aster.org/V2/doc/default/fr/man_r/r3/r3.01.01.pdf
//The node ordering of aster and gmsh are equal (yei!).
double mesh_quad9_h(int j, double *vec_r) {
  double r = vec_r[0];
  double s = vec_r[1];

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

double mesh_quad9_dhdr(int j, int m, double *vec_r) {
  double r = vec_r[0];
  double s = vec_r[1];

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
